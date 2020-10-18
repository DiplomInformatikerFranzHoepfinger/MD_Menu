#include "Menu_Test.h"
#include "esp_log.h"


static const char *TAG = "Menu_Test_Disp";

// Routines for specific display hardware, defining
// - Appropriate header files
// - Hardware definitions
// - Global library objects
// - Display callback function 

#if DISPLAY_SERIAL
// Display the output to the Serial Monitor
// This is useful for debugging and troubleshooting the structure of the 
// menu definitions without using the final output device.

bool display(MD_Menu::userDisplayAction_t action, char *msg)
{
  switch (action)
  {
  case MD_Menu::DISP_INIT:
    //Serial.begin(BAUD_RATE);
    break;
    
  case MD_Menu::DISP_CLEAR:
    ESP_LOGI(TAG, "-> CLS");
    break;

  case MD_Menu::DISP_L0:
    ESP_LOGI(TAG, "0> %s", msg);
    break;

  case MD_Menu::DISP_L1:
    ESP_LOGI(TAG, "1> %s", msg);
    break;
  }

  return(true);
}
#endif

#if DISPLAY_SSD1306

#include "ssd1306.h"
#include "ssd1306_draw.h"
#include "ssd1306_font.h"
#include "ssd1306_default_if.h"


#define USE_I2C_DISPLAY

static const int I2CDisplayAddress = 0x3C;
static const int I2CDisplayWidth = 128;
static const int I2CDisplayHeight = 64;
static const int I2CResetPin = 16; // Board WiFi LoRa 32 (V2) https://heltec.org/project/wifi-lora-32/


struct SSD1306_Device I2CDisplay;

bool DefaultBusInit(void) {

	assert(SSD1306_I2CMasterInitDefault( ) == true);
	assert(SSD1306_I2CMasterAttachDisplayDefault( &I2CDisplay, I2CDisplayWidth, I2CDisplayHeight, I2CDisplayAddress, I2CResetPin ) == true);
	SSD1306_SetHFlip( &I2CDisplay, true );
	SSD1306_SetVFlip( &I2CDisplay, true );
	return true;
}

void DrawL0(const char *msg) {
	SSD1306_SetFont(&I2CDisplay, &Font_droid_sans_mono_13x24);
	SSD1306_FontDrawAnchoredString(&I2CDisplay, TextAnchor_NorthWest, msg,	SSD_COLOR_WHITE);
	SSD1306_Update(&I2CDisplay);
	ESP_LOGI(TAG, "0> %s", msg);
}

void DrawL1(const char *msg) {
	SSD1306_SetFont(&I2CDisplay, &Font_droid_sans_mono_7x13);
	SSD1306_FontDrawAnchoredString(&I2CDisplay, TextAnchor_SouthWest, msg,	SSD_COLOR_WHITE);
	SSD1306_Update(&I2CDisplay);
	ESP_LOGI(TAG, "1> %s", msg);
}

void Clear() {
	ESP_LOGI(TAG, "-> CLS");
	SSD1306_Clear(&I2CDisplay, SSD_COLOR_BLACK);
}

bool display(MD_Menu::userDisplayAction_t action, const char *msg) {
	static const char *L0msg; //Buffer L0
	switch (action) {
	case MD_Menu::DISP_INIT:
		assert(DefaultBusInit());
		printf("BUS Init lookin good...\n");
		printf("Drawing.\n");
		break;

	case MD_Menu::DISP_CLEAR:
		Clear();
		break;

	case MD_Menu::DISP_L0:
		L0msg = msg;
		DrawL0(L0msg);
		break;

	case MD_Menu::DISP_L1:
		Clear();
		DrawL0(L0msg);
		DrawL1(msg);
		break;
	}

	return (true);
}
#endif

#if DISPLAY_LCDSHIELD
// Output display to a one of 2 line LCD display. 
// For a one line display, comment out the L0 handling code.
// The output display line is cleared with spaces before the
// requested message is shown.
#include <LiquidCrystal.h>

// LCD display definitions
#define  LCD_ROWS  2
#define  LCD_COLS  16

// LCD pin definitions
#define  LCD_RS    8
#define  LCD_ENA   9
#define  LCD_D4    4
#define  LCD_D5    LCD_D4+1
#define  LCD_D6    LCD_D4+2
#define  LCD_D7    LCD_D4+3

static LiquidCrystal lcd(LCD_RS, LCD_ENA, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

bool display(MD_Menu::userDisplayAction_t action, char *msg)
{
  static char szLine[LCD_COLS + 1] = { '\0' };

  switch (action)
  {
  case MD_Menu::DISP_INIT:
    lcd.begin(LCD_COLS, LCD_ROWS);
    lcd.clear();
    lcd.noCursor();
    memset(szLine, ' ', LCD_COLS);
    break;
  
  case MD_Menu::DISP_CLEAR:
    lcd.clear();
    break;

  case MD_Menu::DISP_L0:
    lcd.setCursor(0, 0);
    lcd.print(szLine);
    lcd.setCursor(0, 0);
    lcd.print(msg);
    break;

  case MD_Menu::DISP_L1:
    lcd.setCursor(0, 1);
    lcd.print(szLine);
    lcd.setCursor(0, 1);
    lcd.print(msg);
    break;
  }

  return(true);
}
#endif

#if DISPLAY_PAROLA
// Simple output to a one line LED Matrix display managed 
// by the Parola library.

#include <MD_MAX72XX.h>
#include <MD_Parola.h>
#include <SPI.h>

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define MAX_DEVICES 8
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

// Hardware SPI connection
MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);
// Arbitrary output pins
// MD_Parola P = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

bool display(MD_Menu::userDisplayAction_t action, char *msg)
{
  switch (action)
  {
  case MD_Menu::DISP_INIT:
    P.begin();
    break;

  case MD_Menu::DISP_CLEAR:
    P.displayClear();
    break;

  case MD_Menu::DISP_L0:
    // P.print(msg);    // only one zone, no line 0
    break;

  case MD_Menu::DISP_L1:
    P.print(msg);
    break;
  }

  return(true);
}
#endif

