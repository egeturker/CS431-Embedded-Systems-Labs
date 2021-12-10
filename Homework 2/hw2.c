#include "reg51.h"
#include "stdio.h"
#include "inttypes.h"
#include <string.h>

//Keypad bits
sbit col2 = P0^6;
sbit col1 = P0^5;
sbit col0 = P0^4;
sbit row3 = P0^3;
sbit row2 = P0^2;
sbit row1 = P0^1;
sbit row0 = P0^0;

//LCD bits
sbit DB7 = P1^7;
sbit DB6 = P1^6;
sbit DB5 = P1^5;
sbit DB4 = P1^4;
sbit RS = P1^3;
sbit E = P1^2;

sbit clear = P2^4;
sbit ret = P2^5;				  
sbit left = P2^6;
sbit right = P2^7;

char lcd_string[4] = "";
char lcd_string2[16] = "";
int flag = 0;

void delay10ms(){
	TMOD = 0x01; // timer 0 mode 1
	TH0 = 0xB8; //  TH0 AND TL0 creates an overflow every 10 milliseconds
	TL0 = 0x01;
	TR0 = 1; // start timer 0
	
	while(TF0 == 0); // wait until overflow
	TF0 = 0; // clear overflow 
	TR0 = 0; // stop timer 0
}

/* loop delay10ms() function (100*input) times to 
achieve (input) seconds of delay */
void delay_custom(uint8_t delay_seconds){
	uint8_t loop_count = delay_seconds ;
	uint8_t i = 0;
	while(i < loop_count * 100){
		delay10ms();
		i++;
	}
}

/* decodes keypad input three times with 20ms delay
after each press, concats the key in a string */
void decode(){	

		char key = '-'; // dummy key that's not present on keypad
			
		col0 = 1;
		col1 = 1;
		col2 = 1;
		row0 = 1;
		row1 = 1;
		row2 = 1;
		row3 = 1;
	
		row0 = 0;
		if(col0 == 0)
			key = '#';
		else if(col1 == 0)		
			key = '0';
		else if(col2 == 0)
			key = '*';
		
		row0 = 1;
		row1 = 0;
		if(col0 == 0)
			key = '9';
		else if(col1 == 0)		
			key = '8';
		else if(col2 == 0)
			key = '7';

		row1 = 1;
		row2 = 0;
		if(col0 == 0)
			key = '6';
		else if(col1 == 0)		
			key = '5';
		else if(col2 == 0)
			key = '4';		
		
		row2 = 1;
		row3 = 0;
		if(col0 == 0)
			key = '3';
		else if(col1 == 0)		
			key = '2';
		else if(col2 == 0)
			key = '1';
		
		row3 = 1;
		if(key != '-'){
			strcat(lcd_string, &key);	
			delay10ms();
			delay10ms();
		}


}

// gets uart input and writes it to a string char by char
void uart(){
	
		uint8_t uart_input;
		
		if(RI == 1){
			uart_input = SBUF;
			strcat(lcd_string2, &uart_input);	
			RI = 0;
			SBUF = uart_input;
			TI = 0;
		}
}

// LCD functions are used from a library 
// https://github.com/jithinjohnjose/Edsim51_CandHex/blob/master/Display.C

bit getBit(char c, char bitNumber) {
	return (c >> bitNumber) & 1;
}

void returnHome(void) {
	RS = 0;
	DB7 = 0;
	DB6 = 0;
	DB5 = 0;
	DB4 = 0;
	E = 1;
	E = 0;
	DB5 = 1;
	E = 1;
	E = 0;
	delay10ms();
}	

void entryModeSet(bit id, bit s) {
	RS = 0;
	DB7 = 0;
	DB6 = 0;
	DB5 = 0;
	DB4 = 0;
	E = 1;
	E = 0;
	DB6 = 1;
	DB5 = id;
	DB4 = s;
	E = 1;
	E = 0;
	delay10ms();
}

void displayOnOffControl(bit display, bit cursor, bit blinking) {
	DB7 = 0;
	DB6 = 0;
	DB5 = 0;
	DB4 = 0;
	E = 1;
	E = 0;
	DB7 = 1;
	DB6 = display;
	DB5 = cursor;
	DB4 = blinking;
	E = 1;
	E = 0;
	delay10ms();
}

void cursorOrDisplayShift(bit sc, bit rl) {
	RS = 0;
	DB7 = 0;
	DB6 = 0;
	DB5 = 0;
	DB4 = 1;
	E = 1;
	E = 0;
	DB7 = sc;
	DB6 = rl;
	E = 1;
	E = 0;
	delay10ms();
}

void functionSet(void) {
	DB7 = 0;
	DB6 = 0;
	DB5 = 1;
	DB4 = 0;
	RS = 0;
	E = 1;
	E = 0;
	delay10ms();
	E = 1;
	E = 0;
	DB7 = 1;
	E = 1;
	E = 0;
	delay10ms();
}

void sendChar(char c) {
	DB7 = getBit(c, 7);
	DB6 = getBit(c, 6);
	DB5 = getBit(c, 5);
	DB4 = getBit(c, 4);
	RS = 1;
	E = 1;
	E = 0;
	DB7 = getBit(c, 3);
	DB6 = getBit(c, 2);
	DB5 = getBit(c, 1);
	DB4 = getBit(c, 0);
	E = 1;
	E = 0;
	delay10ms();
}

void sendString(char* str) {
	int index = 0;
	while (str[index] != 0) {
		sendChar(str[index]);
		index++;
	}
}

void setDdRamAddress(char address) {
	RS = 0;
	DB7 = 1;
	DB6 = getBit(address, 6);
	DB5 = getBit(address, 5);
	DB4 = getBit(address, 4);
	E = 1;
	E = 0;
	DB7 = getBit(address, 3);
	DB6 = getBit(address, 2);
	DB5 = getBit(address, 1);
	DB4 = getBit(address, 0);
	E = 1;
	E = 0;
	delay10ms();
}

void main (void)
{
	functionSet();
	entryModeSet(1, 0); // increment and no shift
	displayOnOffControl(1, 0, 0); // display on, cursor on and blinking on
	
	SCON  = 0x50;                   // UART mode 1  
	TMOD |= 0x20;                   // timer 1 mode 2     
	TH1   = 0xfd;                   // 19200 baud  22.118 mhz clock      
	TR1   = 1;                      // timer 1 run                        
	TI    = 1;                      // UART receive first character

while(1)
  {
		uart();		
		decode();
		
		if(strlen(lcd_string) == 3 && flag == 0){
			sendString(lcd_string);
			flag = 1;
			
			setDdRamAddress(0x40); // set address to start of second line
			sendString(lcd_string2);
		}
  }
}
