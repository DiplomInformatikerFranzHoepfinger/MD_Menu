/*
 * Menu_Enum.cpp
 *
 *  Created on: 18.10.2020
 *      Author: mail
 */

#include "Menu_Enum.h"


/*prefix  operator++*/ mnuId_e& operator++( mnuId_e &c )        {	return c = static_cast<mnuId_e>( static_cast<int>(c) + 1 );}
/*postfix operator++*/ mnuId_e& operator++( mnuId_e &c, int i ) {	return c = static_cast<mnuId_e>( static_cast<int>(c) + 1 );}
/*prefix  operator--*/ mnuId_e& operator--( mnuId_e &c )        {	return c = static_cast<mnuId_e>( static_cast<int>(c) - 1 );}
/*postfix operator--*/ mnuId_e& operator--( mnuId_e &c, int i ) {	return c = static_cast<mnuId_e>( static_cast<int>(c) - 1 );}


