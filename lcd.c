// lcd.c
// All definitions of functions for text LCD
// for 16x2 LCD	
#include "config.h"
//#include "delay.h"
//#include "lcd.h"


#define	LCD_DATA_BUS	IO0PIN
   
// LCD connections
// RS = P0.13
// EN = P0.16
// D4 = P0.17
// D5 = P0.18
// D6 = P0.19
// D7 = P0.20
#define MASK_EN		0x00010000
#define MASK_EN_NEG	0xFFFEFFFF

#define MASK_RS		0x00002000
#define MASK_RS_NEG 0xDFFF

#define MASK_LCD_DATA 		0x001E0000
#define MASK_LCD_DATA_NEG 	0xFFE1FFFF

#define MASK_EINT1 		0x00000008
#define MASK_EINT1_NEG 	0xFFFFFFF8



void lcd_init(void)
{
	IO0DIR = IO0DIR | ( MASK_LCD_DATA |  MASK_EN  | MASK_RS );

	IO0CLR = MASK_EN;

	delay_5ms();				// 20 ms Power on Delay
	delay_5ms();
	delay_5ms();
	delay_5ms();
 	delay_5ms();				// 20 ms Power on Delay
	delay_5ms();
	delay_5ms();
	delay_5ms();
	delay_5ms();
	delay_5ms();
	delay_5ms();

//2, 8, 2, 8, 0, 6, 0, E, 0, 1
	
	// lcd_command(0x28);			
	// // Set interface data length (DL)=4 bit , 
	// // number of display line (N)=2 
	// // and character font(F)=5x7 dots.	

	// lcd_command(0x28);			

	// lcd_command(0x28);			// 16x2 Chars., 5x7 matrix, 8-bit data		

	// lcd_command(0x06);			// Shift Display	
	// // Set Cursor move direction (I/D)= Auto-increment, 
	// // Shift the display (S) = No.

	// lcd_command(0x14);			// Shift Display
	// // Sets cursor-move or display-shift (S/C), 
	// // shift direction (R/L).
	// lcd_command(0x0C);			// Display on/off	
	
	// lcd_command(0x01);			// Entry Mode
	
	// // LCD Initialization over

	//Above code doesnt seems to work
	//bottom one works

	lcd_command (0x02);	/* 4bit mode */
	lcd_command (0x28);	/* Initialization of 16X2 LCD in 4bit mode */
	lcd_command (0x0C);	/* Display ON Cursor OFF */
	lcd_command (0x06);	/* Auto Increment cursor */
	lcd_command (0x01);	/* Clear display */
	lcd_command (0x80);	/* Cursor at home position */

}

void lcd_command(unsigned char command)
{
	IO0CLR = MASK_RS; 

	delay_200us();
	//LCD_RW = 0;   // already grounded
	// load higher nibble first
	LCD_DATA_BUS = IO0SET & (MASK_LCD_DATA_NEG) |(((unsigned int)command & 0xF0) << 13);

	IO0SET = MASK_EN;
	delay_5us();
	IO0CLR = MASK_EN;
	delay_5us();

	// then load lower nibble
	LCD_DATA_BUS = IO0SET & (MASK_LCD_DATA_NEG) |(((unsigned int)command & 0x0F) << 17);
	
	IO0SET = MASK_EN;
	delay_5us();
	IO0CLR = MASK_EN;

	delay_5ms();	
}

void lcd_data(unsigned char d)
{
	IO0SET = MASK_RS;
	delay_200us();
	//LCD_RW = 0;   // already grounded
	LCD_DATA_BUS = IO0SET & (MASK_LCD_DATA_NEG) |(((unsigned int)d & 0xF0) << 13);

	IO0SET = MASK_EN;
	delay_5us();
	IO0CLR = MASK_EN;
	delay_5us();

	LCD_DATA_BUS = IO0SET & (MASK_LCD_DATA_NEG) |(((unsigned int)d & 0x0F) << 17);

	IO0SET = MASK_EN;
	delay_5us();
	IO0CLR = MASK_EN;
	delay_200us();	
}


void  LCD_SendData(char *str)
{  
	while(1)
   {  
   		if( *str == '\0' ) 
		{break;}
//		if( *str == '*' )
//		{ 
//		  lcd_command (0x01); 
//		}
		else
		{ lcd_data(*str++); }	    			// 
   }   
} 
