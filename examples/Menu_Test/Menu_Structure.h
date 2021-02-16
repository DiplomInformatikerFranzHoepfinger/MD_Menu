/*
 * Menu_Structure.h
 *
 *  Created on: 16.10.2020
 *      Author: mail
 */

#ifndef COMPONENTS_MD_MENU_MENU_STRUCTURE_H_
#define COMPONENTS_MD_MENU_MENU_STRUCTURE_H_



#include "Menu_Enum.h"


namespace {

const char *MAIN_MENU 		= "Main _Menu";
const char *INPUT_DATA 		= "Input Data";
const char *SERIAL_SETUP 	= "Serial Setup";
const char *LED_MENU 		= "LED Menu";
const char *FLIP_FLOP 		= "Flip-Flop";
const char *OUTPUT_TXT 		= "Output TXT";
const char *FRUIT_LIST 		= "Fruit List";
const char *BOOLEAN 		= "Boolean";
const char *INTEGER_8 		= "Integer 8";
const char *INTEGER_16 		= "Integer 16";
const char *INTEGER_32 		= "Integer 32";
const char *HEX_16 			= "Hex 16";
const char *FLOAT 			= "Float";
const char *ENG_UNITS 		= "Eng Units";
const char *RESET_MENU 		= "Reset Menu";
const char *COM_PORT 		= "COM Port";
const char *SPEED 			= "Speed";
const char *PARITY 			= "Parity";
const char *STOP_BITS 		= "Stop Bits";
const char *TURN_OFF 		= "Turn Off";
const char *TURN_ON 		= "Turn On";
const char *FLIP 			= "Flip";
const char *FLOP 			= "Flop";
const char *EMPTY 			= "";
// Input Items ---------
const char *listFruit 		= "Apple|Pear|Orange|Banana|Pineapple|Peach";
const char *listCOM 		= "COM1|COM2|COM3|COM4";
const char *listBaud 		= "9600|19200|57600|115200";
const char *listParity 		= "O|E|N";
const char *listStop 		= "0|1";
const char *engUnit 		= "Hz";
}


// Menu Headers --------
const MD_Menu::mnuHeader_t mnuHdr[] =
{
//  Identifier,    		Label	    	Start item	    End item	    Current
  { id_MAIN_MENU, 		MAIN_MENU, 		id_MAIN_MENU, 	id_FLIP_FLOP, 	id_NULL },
  { id_INPUT_DATA, 		INPUT_DATA, 	id_FRUIT_LIST, 	id_RESET_MENU, 	id_NULL },
  { id_SERIAL_SETUP,	SERIAL_SETUP, 	id_COM_PORT, 	id_STOP_BITS, 	id_NULL },
  { id_LED_MENU, 		LED_MENU, 		id_TURN_OFF, 	id_TURN_ON, 	id_NULL },
  { id_FLIP_FLOP, 		FLIP_FLOP,      id_FLIP, 		id_FLOP, 		id_NULL },
  { id_OUTPUT_TXT,  	OUTPUT_TXT, 	id_OUTPUT_TXT, 	id_OUTPUT_TXT, 	id_NULL },

};

// Menu Items ----------
const MD_Menu::mnuItem_t mnuItm[] =
{
  // Starting (Root) menu
//  Identifier,    		Label	    	action	    		Associated
  { id_MAIN_MENU, 		MAIN_MENU,  	MD_Menu::MNU_MENU, 	id_MAIN_MENU 	},
  { id_INPUT_DATA, 		INPUT_DATA,  	MD_Menu::MNU_MENU, 	id_INPUT_DATA 	},
  { id_SERIAL_SETUP, 	SERIAL_SETUP,   MD_Menu::MNU_MENU, 	id_SERIAL_SETUP },
  { id_LED_MENU, 		LED_MENU,       MD_Menu::MNU_MENU, 	id_LED_MENU 	},
  { id_FLIP_FLOP, 		FLIP_FLOP,		MD_Menu::MNU_MENU, 	id_FLIP_FLOP 	},

  // Input Data submenu
//  Identifier,    		Label	    	action	    		Associated
  { id_FRUIT_LIST, 		FRUIT_LIST, 	MD_Menu::MNU_INPUT, id_FRUIT_LIST 	},
  { id_BOOLEAN, 		BOOLEAN,		MD_Menu::MNU_INPUT, id_BOOLEAN 		},
  { id_INTEGER_8, 		INTEGER_8, 		MD_Menu::MNU_INPUT,	id_INTEGER_8 	},
  { id_INTEGER_16, 		INTEGER_16, 	MD_Menu::MNU_INPUT, id_INTEGER_16 	},
  { id_INTEGER_32,		INTEGER_32, 	MD_Menu::MNU_INPUT, id_INTEGER_32 	},
  { id_HEX_16, 			HEX_16,			MD_Menu::MNU_INPUT, id_HEX_16 		},
  { id_FLOAT, 			FLOAT, 			MD_Menu::MNU_INPUT, id_FLOAT 		},
  { id_ENG_UNITS, 		ENG_UNITS, 		MD_Menu::MNU_INPUT, id_ENG_UNITS 	},
  { id_RESET_MENU, 		RESET_MENU,		MD_Menu::MNU_INPUT, id_RESET_MENU 	},

  // Serial Setup
//  Identifier,    		Label	    	action	    		Associated
  { id_COM_PORT, 		COM_PORT, 		MD_Menu::MNU_INPUT, id_COM_PORT 	},
  { id_SPEED, 			SPEED,			MD_Menu::MNU_INPUT, id_SPEED 		},
  { id_PARITY, 			PARITY, 		MD_Menu::MNU_INPUT, id_PARITY 		},
  { id_STOP_BITS, 		STOP_BITS,		MD_Menu::MNU_INPUT, id_STOP_BITS 	},
  // LED
//  Identifier,    		Label	    	action	    		Associated
  { id_TURN_OFF, 		TURN_OFF, 		MD_Menu::MNU_INPUT, id_TURN_OFF 	},
  { id_TURN_ON, 		TURN_ON,		MD_Menu::MNU_INPUT, id_TURN_ON 		},
  // Flip-flop - boolean controls variable edit
//  Identifier,    		Label	    	action	    		Associated
  { id_FLIP, 			FLIP, 			MD_Menu::MNU_INPUT, id_FLIP 		},
  { id_FLOP, 			FLOP, 			MD_Menu::MNU_INPUT, id_FLOP 		},
  // Output Data submenu
//  Identifier,    		Label	    	action	    		Associated
  { id_OUTPUT_TXT, 		OUTPUT_TXT, 	MD_Menu::MNU_INPUT, id_OUTPUT_TXT 	},
};



const MD_Menu::mnuInput_t mnuInp[] =
{
//  Identifier,    	Label		action	    		Callback 		Width  min			max 		base  list			delta
  { id_FRUIT_LIST, 	EMPTY, 		MD_Menu::INP_LIST,  mnuLValueRqst, 		6,  0, 		0,  0, 		0,   0,   listFruit 	,1}, // shorter and longer list labels
  { id_BOOLEAN, 	EMPTY,    	MD_Menu::INP_BOOL,  mnuBValueRqst, 		1,  0, 		0,  0, 		0,   0,   nullptr 		,1},
  { id_INTEGER_8, 	EMPTY,  	MD_Menu::INP_INT,   mnuIValueRqst, 		4, -128, 	0,  127, 	0,  10,   nullptr 		,1},
  { id_INTEGER_16, 	EMPTY, 		MD_Menu::INP_INT,   mnuIValueRqst, 		4, -32768, 	0,  32767, 	0,  10,   nullptr 		,1},  // test field too small
  { id_INTEGER_32, 	EMPTY,		MD_Menu::INP_INT,   mnuIValueRqst, 		6, -66636, 	0,  65535, 	0,  10,   nullptr		,1},
  { id_HEX_16, 		EMPTY,    	MD_Menu::INP_INT,   mnuIValueRqst, 		4,  0x0000, 0,  0xffff, 0,  16,   nullptr 		,1},  // test hex display
  { id_FLOAT, 		EMPTY, 		MD_Menu::INP_FLOAT, mnuFValueRqst, 		7, -10000, 	0,	99950, 	0,  10,   nullptr 		,1},  // test float number
  { id_ENG_UNITS, 	ENG_UNITS, 	MD_Menu::INP_ENGU,  mnuEValueRqst, 		7,  0, 		0,  999000, 3,  50,   engUnit 		,1},  // test engineering units number
  { id_COM_PORT, 	COM_PORT,   MD_Menu::INP_LIST,  mnuSerialValueRqst, 4,  0, 		0,  0, 		0,   0,   listCOM 		,1},
  { id_SPEED, 		SPEED,   	MD_Menu::INP_LIST,  mnuSerialValueRqst, 6,  0, 		0,  0, 		0,   0,   listBaud 		,1},
  { id_PARITY,		PARITY, 	MD_Menu::INP_LIST,  mnuSerialValueRqst, 1,  0, 		0,	0, 		0,   0,   listParity 	,1},
  { id_STOP_BITS, 	STOP_BITS, 	MD_Menu::INP_LIST,  mnuSerialValueRqst, 1,  0, 		0,  0, 		0,   0,   listStop 		,1},
  { id_TURN_OFF, 	TURN_OFF, 	MD_Menu::INP_RUN,   myLEDCode, 			0,  0, 		0,  0, 		0,   0,   nullptr 		,1},  // test using index in run code
  { id_TURN_ON, 	TURN_ON, 	MD_Menu::INP_RUN,   myLEDCode, 			0,  0, 		0,  0, 		0,   0,   nullptr 		,1},
  { id_FLIP, 		FLIP, 		MD_Menu::INP_INT,   mnuFFValueRqst, 	4, -128, 	0,  127,	0,  10,   nullptr 		,1},
  { id_FLOP, 		FLOP, 		MD_Menu::INP_INT,   mnuFFValueRqst, 	4, -128, 	0,  127, 	0,  16,   nullptr 		,1},
  { id_OUTPUT_TXT, 	OUTPUT_TXT, MD_Menu::INP_RUN,   myCode,        		0,  0, 		0,  0, 		0,  10,   nullptr 		,1},  // test output TXT
};


#endif /* COMPONENTS_MD_MENU_MENU_STRUCTURE_H_ */
