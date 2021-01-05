#include "Menu_Test.h"

// Routines for specific user navigation hardware, defining
// - Appropriate header files
// - Hardware definitions
// - Global library objects
// - Navigation callback function 

#if INPUT_SWITCH
// Implemented as momentary on 3 switches 
// one switch each for INC, DEC
// one switch for SEL (click) or ESC (long press)

#include <MD_UISwitch.h>

const uint8_t INC_PIN = 3;
const uint8_t DEC_PIN = 4;
const uint8_t CTL_PIN = 5;
uint8_t pins[] = { INC_PIN, DEC_PIN, CTL_PIN };

MD_UISwitch_Digital swNav(pins, ARRAY_SIZE(pins), LOW);

void setupNav(void)
{
  swNav.begin();
  swNav.enableRepeat(false);
}

MD_Menu::userNavAction_t navigation(uint16_t &incDelta)
{
  MD_Menu::userNavAction_t nav = MD_Menu::NAV_NULL;

  switch (swNav.read())
  {
    case MD_UISwitch::KEY_PRESS:
    {
      ESP_LOGI(TAG, swNav.getKey());
      switch (swNav.getKey())
      {
      case INC_PIN: nav = MD_Menu::NAV_INC; break;
      case DEC_PIN: nav = MD_Menu::NAV_DEC; break;
      case CTL_PIN: nav = MD_Menu::NAV_SEL; break;
      }
    }
    break;

    case MD_UISwitch::KEY_LONGPRESS:
    {
      if (swNav.getKey() == 2)
        nav = MD_Menu::NAV_ESC;
    }
    break;
  }

  incDelta = 1;
  
  return(nav);
}
#endif

#if INPUT_LCDSWITCH
// Using switches found on a LCD shield
// up and down map to INC and DEC
// Right and Left map to ESC
// Select maps to SEL

#include <MD_UISwitch.h>

#define LCD_KEYS_PIN A0

// These key values work for most LCD shields
MD_UISwitch_Analog::uiAnalogKeys_t kt[] =
{
  {  10, 10, 'R' },  // Right
  { 140, 15, 'U' },  // Up
  { 315, 15, 'D' },  // Down
  { 490, 15, 'L' },  // Left
  { 725, 15, 'S' },  // Select
};

MD_UISwitch_Analog lcdKeys(LCD_KEYS_PIN, kt, ARRAY_SIZE(kt));

void setupNav(void)
{
  lcdKeys.begin();
}

MD_Menu::userNavAction_t navigation(uint16_t &incDelta)
{
  incDelta = 1;
  if (lcdKeys.read() == MD_UISwitch::KEY_PRESS)
  {
    switch (lcdKeys.getKey())
    {
    case 'D': return(MD_Menu::NAV_DEC);
    case 'U': return(MD_Menu::NAV_INC);
    case 'S': return(MD_Menu::NAV_SEL);
    case 'R':
    case 'L': return(MD_Menu::NAV_ESC);
    }
  }

  return(MD_Menu::NAV_NULL);
}
#endif

#if INPUT_RENCODER
// Implemented as a rotary encoder with integrated push switch.
// Rotate encoder clockwise for INC, anti-clockwise for DEC
// Switch for SEL (press) or ESC (long press)
//
// If a value is being edited, the rotary encoder will also apply 
// a factor for how fast the RE is being turned to increase the rate 
// of change of values. This will be applied by the menu library to 
// numeric input only.

#include <MD_REncoder.h>
#include <MD_UISwitch.h>

extern MD_Menu M;

const uint8_t RE_A_PIN = 2;
const uint8_t RE_B_PIN = 3;
const uint8_t CTL_PIN = 4;

MD_REncoder  RE(RE_A_PIN, RE_B_PIN);
MD_UISwitch_Digital swCtl(CTL_PIN);

void setupNav(void)
{
  RE.begin();
  swCtl.begin();
  swCtl.enableRepeat(false);
}

MD_Menu::userNavAction_t navigation(uint16_t &incDelta)
{
  uint8_t re = RE.read();

  if (re != DIR_NONE)
  {
    incDelta = (M.isInEdit() ? (1 << (abs(RE.speed() >> 2))) : 1);
    return(re == DIR_CCW ? MD_Menu::NAV_DEC : MD_Menu::NAV_INC);
  }

  switch (swCtl.read())
  {
  case MD_UISwitch::KEY_PRESS:     return(MD_Menu::NAV_SEL);
  case MD_UISwitch::KEY_LONGPRESS: return(MD_Menu::NAV_ESC);
  default: return(MD_Menu::NAV_NULL);
  }

  return(MD_Menu::NAV_NULL);
}
#endif

#if INPUT_SERIAL
// Using Serial as input
// 'U' and 'D' map to INC and DEC
// 'R' and 'L' map to ESC
// 'S' maps to SEL

void setupNav(void)
{
}

MD_Menu::userNavAction_t navigation(uint16_t &incDelta)
{
  char c = 0;
  //if(Serial.available()>0)
  //  c = Serial.read();

  incDelta = 1;
  switch (c)
  {
  case 'D':  return(MD_Menu::NAV_DEC);
  case 'U':  return(MD_Menu::NAV_INC);
  case 'S':  return(MD_Menu::NAV_SEL);
  case 'R':
  case 'L':  return(MD_Menu::NAV_ESC);
  }

  return(MD_Menu::NAV_NULL);
}
#endif




#if INPUT_GPIO
// Using GPIO as input
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

static const gpio_num_t GPIO_INPUT_NAV_ESC  =   GPIO_NUM_36; //36_ESC
static const gpio_num_t GPIO_INPUT_NAV_DEC  =   GPIO_NUM_38; //38_DOWN
static const gpio_num_t GPIO_INPUT_NAV_INC  =   GPIO_NUM_17; //17_UP
static const gpio_num_t GPIO_INPUT_NAV_SEL  =   GPIO_NUM_2;  // 2_OK
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_NAV_ESC) | (1ULL<<GPIO_INPUT_NAV_DEC) | (1ULL<<GPIO_INPUT_NAV_INC) | (1ULL<<GPIO_INPUT_NAV_SEL))
#define ESP_INTR_FLAG_DEFAULT 0

static xQueueHandle gpio_evt_queue = NULL;


static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

void setupNav(void)
{
	   gpio_config_t io_conf;

	    //interrupt of rising edge
	    io_conf.intr_type = GPIO_INTR_ANYEDGE;
	    //bit mask of the pins, use GPIO4/5 here
	    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
	    //set as input mode
	    io_conf.mode = GPIO_MODE_INPUT;
	    //disable pull-down mode
	    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	    //enable pull-up mode
	    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
	    gpio_config(&io_conf);

	    //create a queue to handle gpio event from isr
	    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
	    //start gpio task
	    //xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

	    //install gpio isr service
	    //Additionally, `gpio_install_isr_service()` must have been called to initialize the GPIO ISR handler service.
	    //hook isr handler for specific gpio pin
	    gpio_isr_handler_add(GPIO_INPUT_NAV_ESC, gpio_isr_handler, (void*) GPIO_INPUT_NAV_ESC);
	    //hook isr handler for specific gpio pin
	    gpio_isr_handler_add(GPIO_INPUT_NAV_DEC, gpio_isr_handler, (void*) GPIO_INPUT_NAV_DEC);
	    //hook isr handler for specific gpio pin
	    gpio_isr_handler_add(GPIO_INPUT_NAV_INC, gpio_isr_handler, (void*) GPIO_INPUT_NAV_INC);
	    //hook isr handler for specific gpio pin
	    gpio_isr_handler_add(GPIO_INPUT_NAV_SEL, gpio_isr_handler, (void*) GPIO_INPUT_NAV_SEL);
}

MD_Menu::userNavAction_t navigation(uint16_t &incDelta) {
	gpio_num_t io_num;
	MD_Menu::userNavAction_t result = MD_Menu::NAV_NULL;
	if (xQueueReceive(gpio_evt_queue, &io_num, 1000 / portTICK_RATE_MS)) //1s wait if no Input there !
	{
		if (0 == gpio_get_level(io_num)) {
			switch (io_num){
			case GPIO_INPUT_NAV_ESC:
				result = MD_Menu::NAV_ESC;
				break;
			case GPIO_INPUT_NAV_DEC:
				result = MD_Menu::NAV_DEC;
				break;
			case GPIO_INPUT_NAV_INC:
				result = MD_Menu::NAV_INC;
				break;
			case GPIO_INPUT_NAV_SEL:
				result = MD_Menu::NAV_SEL;
				break;
			default:
				break;
			}
		}
		//here do NOT use 1 Command with 30ms, but 3 with 10, as every Receive consumes 1 Bounce.
		//only if we got something.
		xQueueReceive(gpio_evt_queue, &io_num, 10 / portTICK_RATE_MS); //Debounce 10ms
		xQueueReceive(gpio_evt_queue, &io_num, 10 / portTICK_RATE_MS); //Debounce 10ms
		xQueueReceive(gpio_evt_queue, &io_num, 10 / portTICK_RATE_MS); //Debounce 10ms
	}
	return (result);
}
#endif
