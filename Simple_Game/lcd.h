/*
 * lcd.h
 *
 *  Created on: Apr 28, 2023
 *      Author: ACS
 */

#ifndef LCD_H_
#define LCD_H_
#include"STD_TYPES.h"

#define NUMBER_OF_ROW_OF_LCD 				2
#define LCD_DATA_MODE 						8

#if( NUMBER_OF_ROW_OF_LCD == 0 )
#error "Number of row not equal Zero"
#endif

#if((LCD_DATA_MODE != 4) & (LCD_DATA_MODE != 8))
#error "Number of Data bits should be equal to 4 or 8"
#endif

#define LCD_DATA_PORT_ID                     PORTC_ID

#define LCD_RS_PORT_ID                       PORTD_ID
#define LCD_RS_PIN                           PIN4

#define LCD_EN_PORT_ID                       PORTD_ID
#define LCD_EN_PIN                           PIN5
#if LCD_DATA_MODE == 4
#define LCD_DATA_PIN4_ID                     PIN3
#define LCD_DATA_PIN5_ID                     PIN4
#define LCD_DATA_PIN6_ID                     PIN5
#define LCD_DATA_PIN7_ID                     PIN6
#endif

#define LCD_CLEAR_COMMAND                    0x01
#define LCD_GO_TO_HOME                       0x02
#define LCD_TWO_LINES_EIGHT_BITS_MODE        0x38
#define LCD_TWO_LINES_FOUR_BITS_MODE         0x28
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1   0x33
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2   0x32
#define LCD_CURSOR_OFF                       0x0C
#define LCD_CURSOR_ON                        0x0E
#define LCD_SET_CURSOR_LOCATION              0x80
#define SET_CURSOR_TO_CGRAM 				 0x40
void LCD_sendCommand(uint8 com);

void LCD_displayCharacter(uint8 data);

void LCD_init(void);

void LCD_displayString(const uint8 *str);

void LCD_moveCursor(uint8 row,uint8 col);

void LCD_displayStringRowColumn(uint8 row,uint8 col,const uint8 *str);

void LCD_displayIntgerNumber(uint32 num);
/*** Sepecial character function ***/
void LCD_secpecialCharacter(uint8 *pattern,uint8 Location_number,uint8 row,uint8 column);







void LCD_clearScreen(void);
#endif /* LCD_H_ */
