/*
 * Menu_Enum.h
 *
 *  Created on: 18.10.2020
 *      Author: mail
 */

#ifndef COMPONENTS_MD_MENU_MENU_ENUM_H_
#define COMPONENTS_MD_MENU_MENU_ENUM_H_


enum mnuId_e
{
  //none
  no_id 			=  -1,
  //NULL
  id_NULL			=   0,
  // Starting (Root) menu
  id_MAIN_MENU		=  10,
  id_INPUT_DATA		=  11,
  id_SERIAL_SETUP	=  12,
  id_LED_MENU		=  13,
  id_FLIP_FLOP		=  14,
  // Input Data submenu
  id_FRUIT_LIST		=  20,
  id_BOOLEAN		=  21,
  id_INTEGER_8		=  22,
  id_INTEGER_16		=  23,
  id_INTEGER_32		=  24,
  id_HEX_16			=  25,
  id_FLOAT			=  26,
  id_ENG_UNITS		=  27,
  id_RESET_MENU		=  28,
  // Serial Setup
  id_COM_PORT		=  30,
  id_SPEED			=  31,
  id_PARITY			=  32,
  id_STOP_BITS		=  33,
  // LED
  id_TURN_OFF		=  40,
  id_TURN_ON		=  41,
  // Flip-flop - boolean controls variable edit
  id_FLIP			=  50,
  id_FLOP			=  51,
  // Output Data submenu
  id_OUTPUT_TXT		=  70,
  //Last item
  id_LAST_ITEM
};


/*prefix  operator++*/ mnuId_e& operator++( mnuId_e &c );
/*postfix operator++*/ mnuId_e& operator++( mnuId_e &c, int i );
/*prefix  operator--*/ mnuId_e& operator--( mnuId_e &c );
/*postfix operator--*/ mnuId_e& operator--( mnuId_e &c, int i );






#endif /* COMPONENTS_MD_MENU_MENU_ENUM_H_ */
