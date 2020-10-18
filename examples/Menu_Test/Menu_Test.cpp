// Example program for the MD_Menu library
//
// Test functionality of the library and demonstrate how a menu is structured.
// The code runs the menu and display debug information on the Serial Monitor.
//
// Different combinations of output display and navigation interfaces can be
// selected from the Menu_Test.h header file. Depending on the input and display
// options selected, one or more of the libraries listed below these may be
// required.
//
// The example UI callback routines cover the most common user input and display
// hardware:
//
// User Navigation - Menu_Test_Nav.cpp
// ---------------
// There are examples for user navigation for:
// - 3 separate momentary on switches for INC, DEC and ESC/SEL selections
// - Rotary encoder for INC/DEC (rotation) and momentary on switch for ESC/SEL
// - Analog 'resistor ladder' switches common on LCD shields for INC, DEC, ESC and SEL
//
// User Display - Menu_Test_Disp.cpp
// ------------
// - Serial Monitor output (useful for debugging)
// - LCD module display (LiquidCrystal Library)
// - Parola library and LED matrix modules
//
// External Dependencies
// ---------------------
// Depending on the input or display options selected, one or more of these may be required:
// - MD_UISwitch library for digital and analog switches at https://github.com/MajicDesigns/MD_UISwitch
// - MD_REncoder library for rotary encoder input at https://github.com/MajicDesigns/MD_REncoder
// - MD_Parola library for LED matrix display at https://github.com/MajicDesigns/MD_Parola
// - MD_MAX72XX library to support MD_Parola library at https://github.com/MajicDesigns/MD_MAX72XX
// - LiquidCrystal library for LCD module is a standard Arduino library.
//
#include "Menu_Test.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


static const char *TAG = "Menu_Test";


// Global menu data and definitions
uint8_t fruit = 2;
bool bValue = true;
int8_t  int8Value = 99;
int16_t int16Value = 999;
int32_t int32Value = 9999;
float floatValue = 999.99;
MD_Menu::value_t engValue = { 999900, 3 };
char _txt[] = "192.168.1.101";

MD_Menu::value_t vBuf;  // interface buffer for values

#include "Menu_Structure.h"

// bring it all together in the global menu object
MD_Menu M(navigation, display,        // user navigation and display
          mnuHdr, ARRAY_SIZE(mnuHdr), // menu header data
          mnuItm, ARRAY_SIZE(mnuItm), // menu item data
          mnuInp, ARRAY_SIZE(mnuInp));// menu input data

// Callback code for menu set/get input values
MD_Menu::value_t *mnuLValueRqst(MD_Menu::mnuId_t id, bool bGet)
// Value request callback for list selection variable
{
  if (id == id_MAIN_MENU)
  {
    if (bGet)
    {
      vBuf.value = fruit;
    }
    else
    {
      fruit = vBuf.value;
      ESP_LOGI(TAG, "List index changed to %d", fruit);

    }
  }
  return(&vBuf);
}

MD_Menu::value_t *mnuBValueRqst(MD_Menu::mnuId_t id, bool bGet)
// Value request callback for boolean variable
{
  MD_Menu::value_t *r = &vBuf;

  if (id == id_BOOLEAN)
  {
    if (bGet)
      vBuf.value = bValue;
    else
    {
      bValue = vBuf.value;
      ESP_LOGI(TAG, "Boolean changed to %d", bValue);
    }
  }
  else
    r = nullptr;
  
  return(r);
}

MD_Menu::value_t *mnuIValueRqst(MD_Menu::mnuId_t id, bool bGet)
// Value request callback for integers variables
{
  MD_Menu::value_t *r = &vBuf;

  switch (id)
  {
    case id_INTEGER_8:
      if (bGet)
        vBuf.value = int8Value;
      else
      {
        int8Value = vBuf.value;
        ESP_LOGI(TAG, "Int8 value changed to %d", int8Value);
      }
      break;

    case id_INTEGER_16:
      if (bGet)
        vBuf.value = int16Value;
      else
      {
        int16Value = vBuf.value;
        ESP_LOGI(TAG, "Int16 value changed to %d", int16Value);
      }
      break;

    case id_INTEGER_32:
      if (bGet)
        vBuf.value = int32Value;
      else
      {
        int32Value = vBuf.value;
        ESP_LOGI(TAG, "Int32 value changed to %d", int32Value);
      }
      break;

    default:
      r = nullptr;
      break;
  }

  return (r);
}

MD_Menu::value_t *mnuSerialValueRqst(MD_Menu::mnuId_t id, bool bGet)
// Value request callback for Serial parameters
{
  static uint8_t port = 0, speed = 0, parity = 0, stop = 0;
  MD_Menu::value_t *r = &vBuf;

  switch (id)
  {
    case 30:
      if (bGet)
        vBuf.value = port;
      else
      {
        port = vBuf.value;
        ESP_LOGI(TAG, "Port index=%d", port);
      }
      break;

    case 31:
      if (bGet)
        vBuf.value = speed;
      else
      {
        speed = vBuf.value;
        ESP_LOGI(TAG, "Speed index=%d", speed);
      }
      break;

    case 32:
      if (bGet)
        vBuf.value = parity;
      else
      {
        parity = vBuf.value;
        ESP_LOGI(TAG, "Parity index=%d", parity);
      }
      break;

    case 33:
      if (bGet)
        vBuf.value = stop;
      else
      {
        stop = vBuf.value;
        ESP_LOGI(TAG, "Stop index=%d", stop);
      }
      break;

    default:
      r = nullptr;
      break;
  }

  return (r);
}

MD_Menu::value_t *mnuFValueRqst(MD_Menu::mnuId_t id, bool bGet)
// Value request callback for floating value
{
  MD_Menu::value_t *r = &vBuf;

  if (id == 16)
  {
    if (bGet)
      vBuf.value = (uint32_t)(floatValue * 100.0);
    else
    {
      floatValue = (vBuf.value / 100.0);
      ESP_LOGI(TAG, "Float changed to %f", floatValue);
    }
  }
  else
    r = nullptr;

  return(r);
}

MD_Menu::value_t *mnuEValueRqst(MD_Menu::mnuId_t id, bool bGet)
// Value request callback for engineering value
{
  if (id == 17)
  {
    if (bGet)
      return(&engValue);
    else
    {
      float f = (engValue.value / 1000.0);
      ESP_LOGI(TAG, "Eng Unit changed to %f x10^ %d", f, engValue.power);
    }
  }

  return(nullptr);
}

MD_Menu::value_t *mnuFFValueRqst(MD_Menu::mnuId_t id, bool bGet)
// Value edit allowed request depends on another value
{
  static bool gateKeeper = false;
  MD_Menu::value_t *r = &vBuf;

  switch (id)
  {
    case 50:
      if (bGet)
      {
        if (gateKeeper)
        {
          ESP_LOGI(TAG, "FlipFlop value blocked");
          r = nullptr;
        }
        else
          vBuf.value = int8Value;
      }
      else
      {
        int8Value = vBuf.value;
        ESP_LOGI(TAG, "FlipFlop value changed to %d", int8Value);
        gateKeeper = !gateKeeper;
      }
      break;

    case 51:
      if (bGet)
      {
        if (!gateKeeper)    // reverse the logic of above
        {
          ESP_LOGI(TAG, "FlipFlop value blocked");
          r = nullptr;
        }
        else
          vBuf.value = int8Value;
      }
      else
      {
        int8Value = vBuf.value;
        ESP_LOGI(TAG, "FlipFlop value changed to %d", int8Value);
        gateKeeper = !gateKeeper;
      }
      break;

    default:
      r = nullptr;
      break;
  }

  return(r);
}

MD_Menu::value_t *mnuFBValueRqst(MD_Menu::mnuId_t id, bool bGet)
// Value request callback using real time feedback selection variable
{
  MD_Menu::value_t *r = &vBuf;

  switch (id)
  {
  case id_INTEGER_8:
    if (bGet)
      vBuf.value = int8Value;
    else
    {
      int8Value = vBuf.value;
      ESP_LOGI(TAG, "Uint8 value changed to %d", int8Value);
    }
    break;

  case id_FRUIT_LIST:
    if (bGet)
    {
      vBuf.value = fruit;
      return(&vBuf);
    }
    else
    {
      fruit = vBuf.value;
      ESP_LOGI(TAG, "Fruit index changed to %d", fruit);
    }
    break;

  default:
    r = nullptr;
    break;
  }

  return(r);
}

MD_Menu::value_t *myCode(MD_Menu::mnuId_t id, bool bGet)
// Value request callback for run code input
{
  switch (id)
  {
    case id_RESET_MENU:
      ESP_LOGI(TAG, "myCode called id= %d to %s", id, bGet ? "GET" : "SET - reset menu");

      if (!bGet) M.reset();
      break;
  
    case id_OUTPUT_TXT:
      if (bGet) return ((MD_Menu::value_t *)&_txt);
      break;

    default:
    	break;
  }
  return (nullptr);
}

MD_Menu::value_t *myLEDCode(MD_Menu::mnuId_t id, bool bGet)
// Value request callback for run code input
// Only use the index here
{
  ESP_LOGI(TAG, "Switching LED %s",id == 40 ? "off" : "on");
  //digitalWrite(LED_PIN, id == 40 ? LOW : HIGH);

  return(nullptr);
}

void Menu_Test_Task( void *pvParameters )
{
  //Serial.begin(BAUD_RATE);
  ESP_LOGI(TAG, "[Menu_Test Debug]");

  //pinMode(LED_PIN, OUTPUT);

  display(MD_Menu::DISP_INIT);
  setupNav();

  M.begin();
  M.setMenuWrap(true);
  M.setAutoStart(AUTO_START);
  M.setTimeout(MENU_TIMEOUT);


  for( ;; )
	{
	  static bool prevMenuRun = true;

	  // Detect if we need to initiate running normal user code
	  if (prevMenuRun && !M.isInMenu())
		ESP_LOGI(TAG, "SWITCHING TO USER'S NORMAL OPERATION\n");
	  prevMenuRun = M.isInMenu();

	  // If we are not running and not autostart
	  // check if there is a reason to start the menu
	  if (!M.isInMenu() && !AUTO_START)
	  {
		uint16_t dummy;

		if (navigation(dummy) == MD_Menu::NAV_SEL)
		  M.runMenu(true);
	  }

	  M.runMenu();   // just run the menu code
	  vTaskDelay(10 / portTICK_PERIOD_MS);

	}
}

void InitMenu(void)
{

	xTaskCreate(Menu_Test_Task, "Menu_Test_Task", 1024 * 4, nullptr, 8, nullptr);


}
