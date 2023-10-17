/*
 * main.c
 *
 *  Created on: Aug 20, 2023
 *      Author: ACS
 */
#include "lcd.h"
#include "gpio.h"
#include <avr/delay.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"
uint8 start = 4;
uint8 size = 10;
/*** The binary for the characters ***/
uint8 CharacterOfPerson[] = {
  0b00001110,
  0b00001110,
  0b00000100,
  0b00001110,
  0b00010101,
  0b00000100,
  0b00001110,
  0b00010001
};
/*** The binary of the gun for first character ***/
uint8 Hamda_gun[] = {
	0b00000000,
	0b00000000,
	0b00001111,
	0b00001100,
	0b00010000,
	0b00010000,
	0b00000000,
	0b00000000
};
/*** The binary of gun for the second character ***/
uint8 Hamocksha_gun[] = {
   0b00000000,
   0b00000000,
   0b00011110,
   0b00000110,
   0b00000001,
   0b00000001,
   0b00000000,
   0b00000000
};
/*** The prototype of function that use to display the data in start***/
void Intro_function(void);
/*** The prototype of function that use to display motion of shoot ***/
void shoot(void);
/*** The prototype of function that use to display who is winner ***/
void Display_winner(uint8 *name);
/*** The prototype of function that use to display motion of HAMADA_character at death ***/
void HAMADA_DIE(void);
/*** The prototype of function that use to display motion of HAMADA_character at death ***/
void HAMOCKSHA_DIE(void);

uint8 main(void)
{
	sint8 flag = 0;
	uint8 hamada_score = 0;
	uint8 hamocksha_score = 0;
	/*** This push for Hamad's gun ***/
	GPIO_setupPinDirection(PORTA_ID,PIN0,PIN_INPUT);
	/*** To active the internal pull up resistor ***/
	GPIO_writePin(PORTA_ID,PIN0,LOGIC_HIGH);
	/*** This push for Hamoksha's motion ***/
	GPIO_setupPinDirection(PORTD_ID,PIN1,PIN_INPUT);
	/*** To active the internal pull up resistor ***/
	GPIO_writePin(PORTD_ID,PIN7,LOGIC_HIGH);
	LCD_init();
	/**** introduction for game ***/
	Intro_function();
	/**** Hamada's Character ***/
	LCD_secpecialCharacter(CharacterOfPerson,0,0,1);
	while(1)
	{
		/*** Hamocsha's Character ***/
		LCD_secpecialCharacter(CharacterOfPerson,5,0,10);
		if(GPIO_readPin(PORTA_ID,PIN0) == 0)
		{
			LCD_secpecialCharacter(Hamda_gun,1,0,2);
			_delay_ms(30);
			flag = 1;
			start = 4;
			size = 10;
			shoot();

		}
		/*** Hamocsha's Character ***/
		if(GPIO_readPin(PORTD_ID,PIN7) == 0)
		{
			LCD_moveCursor(0,10);
			LCD_displayCharacter('_');
			if(flag == 1)
			{
				start = 11;
				size = 14;
				shoot();
				/*** Set score **/
				hamocksha_score ++;
				flag = 0;
			}
			LCD_moveCursor(1,10);
			/*** Display score ***/
			LCD_displayIntgerNumber(hamocksha_score);
			 if(hamocksha_score == 3)
			 {
				 HAMADA_DIE();
				 Display_winer("hamocksha");
				 return 1;
			 }
		}
		else if(flag == 1)
		{
			/*** Set score **/
			 hamada_score ++;
			 /*** Display score ***/
			 LCD_moveCursor(1,1);
			LCD_displayIntgerNumber(hamada_score);
			 flag = 0;
			 if(hamada_score == 3)
			 {
				for(flag = 1; flag <= 3; flag ++)
				{
					HAMOCKSHA_DIE();
				}
				Display_winer("hamada");
				return 0;
			 }
		}



	}
}
/*** The implementation of function that use to display who is winner ***/
void Display_winner(uint8 *name)
{
	 LCD_clearScreen();
	 LCD_moveCursor(0,2);
	 LCD_displayString("The winner is :");
	 LCD_moveCursor(1,4);
	 LCD_displayString(name);


}
/*** The implementation of function that use to display motion of HAMOCKSHA_character at death ***/
void HAMOCKSHA_DIE(void)
{
	LCD_moveCursor(0,11);
	LCD_displayCharacter(' ');
	LCD_secpecialCharacter(CharacterOfPerson,5,0,10);
	_delay_ms(500);
	LCD_moveCursor(0,10);
	LCD_displayCharacter('_');
	LCD_displayCharacter('_');
	_delay_ms(500);
}
/*** The implementation of function that use to display motion of HAMADA_character at death ***/
void HAMADA_DIE(void)
{
	uint8 count;
	LCD_moveCursor(0,2);
	LCD_displayCharacter(' ');
	for(count= 9; count >= 3; count --)
	{
		LCD_secpecialCharacter(CharacterOfPerson,5,0,count);
		LCD_secpecialCharacter(Hamocksha_gun,6,0,count - 1);
		_delay_ms(100);
		LCD_moveCursor(0,count);
		LCD_displayCharacter(' ');

	}
	LCD_secpecialCharacter(CharacterOfPerson,5,0,count + 1);
	_delay_ms(1000);
}
/*** The implementation of function that use to display motion of shoot ***/
void shoot(void)
{
	uint8 counter;
	for(counter = start; counter < size;counter++ )
	{

		LCD_moveCursor(0,counter);
		LCD_displayCharacter('-');
		_delay_ms(100);
		LCD_moveCursor(0,counter);
		LCD_displayCharacter(' ');

	}

}
/*** The implementation of function that use to display the data in start***/
void Intro_function(void)
{
	/*** introduction of game ***/
	 LCD_moveCursor(0,0);
	 LCD_displayString("This match :");
	 LCD_moveCursor(1,0);
	 LCD_displayString("of 3 points:");
	 _delay_ms(2000);
	 LCD_clearScreen();
	 LCD_moveCursor(0,0);
	 LCD_displayString("between Hamada ");
	 LCD_moveCursor(1,0);
	 LCD_displayString("And Hamocksha: ");
	 _delay_ms(2000);
	 LCD_clearScreen();
	 LCD_moveCursor(0,1);
	 LCD_displayString("The Winner will :");
	 _delay_ms(2000);
	 LCD_clearScreen();
	 LCD_moveCursor(0,0);
	 LCD_displayString("win a Beans");
	 LCD_moveCursor(1,0);
	 LCD_displayString("or Koshary dish: ");
	 _delay_ms(2000);
	 LCD_clearScreen();
	 LCD_moveCursor(0,0);
	 LCD_displayString("According to  ");
	 LCD_moveCursor(1,0);
	 LCD_displayString("The winner , ");
	 _delay_ms(2000);
	 LCD_clearScreen();
	 LCD_moveCursor(0,0);
	 LCD_displayString("What does");
	 LCD_moveCursor(1,0);
	 LCD_displayString("He like ? ");
	 _delay_ms(2000);
	 LCD_clearScreen();
}
