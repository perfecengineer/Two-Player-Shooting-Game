#include"lcd.h"
#include"gpio.h"
#include<util/delay.h>

#include "BIT_MATH.h"
#include "STD_TYPES.h"
void LCD_init(void)
{
	GPIO_setupPinDirection(LCD_RS_PORT_ID,LCD_RS_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_EN_PORT_ID,LCD_EN_PIN,PIN_OUTPUT);
#if LCD_DATA_MODE == 8
	GPIO_setupPortDirection(LCD_DATA_PORT_ID,PORT_OUTPUT);
	_delay_ms(20);
	// 8 bits mode
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE);
#elif LCD_DATA_MODE == 4
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DATA_PIN4_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DATA_PIN5_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DATA_PIN6_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DATA_PIN7_ID,PIN_OUTPUT);
	_delay_ms(20);
	/* Send for 4 bit initialization of LCD  */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);
	// 4 bits mode
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE);
#endif
	_delay_ms(1);
	//disable cursor
	LCD_sendCommand(LCD_CURSOR_OFF);
	_delay_ms(1);
	// clear lcd
	LCD_sendCommand(LCD_CLEAR_COMMAND);

}
void LCD_sendCommand(uint8 com)
{
	GPIO_writePin(LCD_RS_PORT_ID,LCD_RS_PIN,LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(LCD_EN_PORT_ID,LCD_EN_PIN,LOGIC_HIGH);
	_delay_ms(1);
#if LCD_DATA_MODE == 8
	GPIO_writePort(LCD_DATA_PORT_ID,com);
	_delay_ms(1);
	GPIO_writePin(LCD_EN_PORT_ID,LCD_EN_PIN,LOGIC_LOW);
	_delay_ms(1);
#elif LCD_DATA_MODE == 4
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DATA_PIN4_ID,READ_BIT(com,4));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DATA_PIN5_ID,READ_BIT(com,5));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DATA_PIN6_ID,READ_BIT(com,6));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DATA_PIN7_ID,READ_BIT(com,7));
	_delay_ms(1);
	GPIO_writePin(LCD_EN_PORT_ID,LCD_EN_PIN,LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(LCD_EN_PORT_ID,LCD_EN_PIN,LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DATA_PIN4_ID,READ_BIT(com,0));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DATA_PIN5_ID,READ_BIT(com,1));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DATA_PIN6_ID,READ_BIT(com,2));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DATA_PIN7_ID,READ_BIT(com,3));
	_delay_ms(1);
	GPIO_writePin(LCD_EN_PORT_ID,LCD_EN_PIN,LOGIC_LOW);
	_delay_ms(1);
#endif
}
void LCD_displayCharacter(uint8 data)
{
	GPIO_writePin(LCD_RS_PORT_ID,LCD_RS_PIN,LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePin(LCD_EN_PORT_ID,LCD_EN_PIN,LOGIC_HIGH);
	_delay_ms(1);
#if LCD_DATA_MODE == 8
	GPIO_writePort(LCD_DATA_PORT_ID,data);
	_delay_ms(1);
	GPIO_writePin(LCD_EN_PORT_ID,LCD_EN_PIN,LOGIC_LOW);
	_delay_ms(1);
#elif LCD_DATA_MODE == 4
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DATA_PIN4_ID,READ_BIT(data,4));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DATA_PIN5_ID,READ_BIT(data,5));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DATA_PIN6_ID,READ_BIT(data,6));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DATA_PIN7_ID,READ_BIT(data,7));
	_delay_ms(1);
	GPIO_writePin(LCD_EN_PORT_ID,LCD_EN_PIN,LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(LCD_EN_PORT_ID,LCD_EN_PIN,LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DATA_PIN4_ID,READ_BIT(data,0));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DATA_PIN5_ID,READ_BIT(data,1));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DATA_PIN6_ID,READ_BIT(data,2));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DATA_PIN7_ID,READ_BIT(data,3));
	_delay_ms(1);
	GPIO_writePin(LCD_EN_PORT_ID,LCD_EN_PIN,LOGIC_LOW);
	_delay_ms(1);
#endif
}
void LCD_displayString(const uint8 *str)
{
	while(*str != '\0')
	{
		LCD_displayCharacter((*str));
		str++;
	}
}
void LCD_moveCursor(uint8 row,uint8 col)
{
	uint8 shift = 0;
	switch(row)
	{
#if NUMBER_OF_ROW_OF_LCD == 1
		case 0:shift = 0x00 + col;break;
#elif NUMBER_OF_ROW_OF_LCD == 2
		case 0:shift = 0x00 + col;break;
		case 1:shift = 0x40 + col;break;
#elif NUMBER_OF_ROW_OF_LCD == 4
		case 0:shift = 0x00 + col;break;
		case 1:shift = 0x40 + col;break;
		case 2:shift = 0x10 + col;break;
		case 3:shift = 0x50 + col;break;
#endif
	}
	LCD_sendCommand(LCD_SET_CURSOR_LOCATION | shift);
}
void LCD_displayStringRowColumn(uint8 row,uint8 col,const uint8 *str)
{
	LCD_moveCursor(row,col);
	LCD_displayString(str);
}
void LCD_displayIntgerNumber(uint32 num)
{
	/************* Array to store the digit ************/
	uint8 digit[16] = {0};
	/******* counter use for count the number of digits ***/
	sint8 count = 0;
	/**** Special case for zero ******/

	if(num == 0)
	{
		digit[count] = '0';
	}
	else
	{
		/**** This looping use for slicing the input number to digits***/
		while(num != 0)
		{
			digit[count] = (num % 10) + '0';
			num /= 10;
			count++;
		}
	}
	/************** This loop using for display on LCD ****/
	while(count >= 0)
	{
		LCD_displayCharacter(digit[count]);
		count--;
	}
}

void LCD_clearScreen(void)
{
	LCD_sendCommand(LCD_CLEAR_COMMAND);
}
/*
 * void LCD_intgerToString(uint32 data)
{
	uint8 buff[16];
	itoa(data,buff,10);
	LCD_displayString(buff);
}*/

void LCD_secpecialCharacter(uint8 *pattern,uint8 Location_number,uint8 row,uint8 column)
{

	uint8 counter = 0;
	LCD_sendCommand(SET_CURSOR_TO_CGRAM + (Location_number * 8));
	_delay_ms(5);

	 for(counter = 0; counter < 8; counter ++)
	 {
		 LCD_displayCharacter(pattern[counter]);
	 }
	 LCD_moveCursor(row,column);
	 LCD_displayCharacter(Location_number);
}
