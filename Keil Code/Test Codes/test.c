#include "c8051F340.h"
#include "math.h"
#include "stdio.h"

void delay(unsigned int Ms);							//function for creating delay
void cmdwrt(unsigned char Command);					//function for sending commands to 16*2 lcd display
void datawrt(unsigned char Character);			//function for sending Data to 16*2 lcd display
void convert_display(unsigned char); 				//function for converting ADC value to temperature and display it on 16*2 lcd display
int binaryToDecimal(int n);

sbit LCD_RS = P2^5;			//Register Select(RS) pin of 16*2 lcd
sbit LCD_RW = P2^6;			//Read/Write(RW) pin of 16*2 lcd
sbit LCD_EN = P2^7;			//Enable(E) pin of 16*2 lcd


sbit wr_adc=P0^0; 			//Write(WR) pin of ADC0804
//Read(RD) pin of ADC0804 grounded
//Interrupt(INTR) pin of ADC0804 tied to WR for continous input

unsigned char i;
unsigned int x1,x2,x3,x4,a=1;
unsigned int b0,b1,b2,b3,b4,b5,b6,b7,dec;
unsigned char value=0, msg;


void main()
{
	//LCD Pins
	P2MDOUT = 0xE0;
	P3MDOUT = 0xFF;    // P2 output port
	
	//ADC Pins
	P0MDOUT = 0x01;	   // P0.0 output pins	
	P1MDIN 	= 0xFF;
	
	cmdwrt(0x0C);		// Display ON, Cursor OFF
	delay(50);
	cmdwrt(0x01);		// Clear Display Screen
	delay(50);
	cmdwrt(0x38);		// 2 Lines and 5x7 matrix
	delay(50);
	cmdwrt(0x80);		// Force cursor to beginning of 1st line
	delay(50);

			datawrt('T');
			delay(50);
			datawrt('E');
			delay(50);
			datawrt('M');
			delay(50);
			datawrt('P');
			delay(50);			
			datawrt('R');
			delay(50);
			datawrt('A');
			delay(50);
			datawrt('T');
			delay(50);
			datawrt('U');
			delay(50);
			datawrt('R');
			delay(50);
			datawrt('E');
			delay(50);
			datawrt(':');
			delay(50);
			
	while(a=1)	
	{	
		wr_adc=0; 	 //set WR pin LOW	
		delay(500);
		wr_adc=1;			//send LOW to HIGH pulse on WR pin
	
		b0= P0^0;
		b1= P0^1;
		b2= P0^2;
		b3= P0^3;
		b4= P0^4;
		b5= P0^5;
		b6= P0^6;
		b7= P0^7;
		
		dec = b0 + (b1*2) + (b2*4) + (b3*8)+ (b4*16) +(b5*32) + (b6*64) + (b7*128);
		
		msg=dec;
		
		convert_display(dec); 		//function call to convert ADC Data into temperature and display it on 16*2 lcd display
		delay(1000);  						//interval between every cycles

	/* display temperature on 16*2 lcd display	*/
		cmdwrt(0xC0);  					//command to force the cursor on 2nd line on 16*2 lcd
		datawrt(x1);
		delay(50);
		datawrt(x2);
		delay(50);
		datawrt(x3);
		delay(50);
		datawrt(x4);
		delay(50);
		datawrt('C');
		delay(50);
	}
	
}

/*--------FUNCTION DEFNITIONS--------*/

void delay(unsigned int Ms)
{
  unsigned int n,i;
	for(n=0;n<Ms;n++)  		//repeat for 'Ms' times
	{
		for(i=0;i<65;i++);	//repeat for '65' times
	}
}

void cmdwrt(unsigned char Command)
	{
    P3 = Command;   //send the command to Port 2 on which 16*2 lcd is connected
		LCD_RS = 0;     //make RS = 0 for Command
    LCD_RW = 0;			//make RW = 0 for write operation
    LCD_EN = 1;     //send a HIGH to LOW pulse on Enable(E) pin to start command write operation 
    delay(15);
    LCD_EN = 0;
	}

void datawrt(unsigned char Character)
	{
		P3 = Character;	//send the Data to Port 2 on which 16*2 lcd is connected
    LCD_RS = 1;     //make RS = 1 for Data
    LCD_RW = 0;			//make RW = 0 for write operation
    LCD_EN = 1; 	  //send a HIGH to LOW pulse on Enable(E) pin to start datawrite operation
    delay(15);
    LCD_EN = 0;
	}

void convert_display(unsigned char value)
{
	x1 = ((value/100)%10)+0x30;				  
	x2 = ((value%100)/10)+0x30;  			
	x3 = (value%10)+0x30;
	x4 = 0xDF;                		//ascii value of degree(°) symbol
}
