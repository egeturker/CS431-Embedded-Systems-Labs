#include "reg51.h"
#include "inttypes.h"
#include <stdio.h>

#define BUFFER_SIZE 10
#define ENTER 0x10

struct buffer{
	uint8_t buff[BUFFER_SIZE];
	uint8_t current_size ;
	uint8_t current_index ;
	
}b;

sbit dac_wr = P0^7;
sbit serial_interrupt = IE^4;
sbit interrupts = IE^7;

uint8_t numbers[10]  = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
uint8_t out_flag = 0;
void dot(){
	int i;
	dac_wr = 0;
	P1 = 0x34;
	for(i = 0; i <20; i++){}
	P1 = 0x00;
	dac_wr = 1;
}

void dash(){
	int i;
	dac_wr = 0;
	P1 = 0x9a;
	for(i = 0; i <20; i++){}
	P1 = 0x00;
	dac_wr = 1;
}

void decode_morse(uint8_t number){
	switch(number){
		case 0x31:
			dot(); 			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			break;
		case 0x32:
			dot();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			break;
		case 0x33:
			dot();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			break;
		case 0x34:
			dot();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			break;
		case 0x35:
			dot();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			break;
		case 0x36:
			dash();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			break;
		case 0x37:
			dash();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			break;
		case 0x38:
			dash();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			break;
		case 0x39:
			dash();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			dot();			if(out_flag == 0) return;
			break;
		case 0x30:
			dash();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;	
			dash();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			dash();			if(out_flag == 0) return;
			break;		
	}
}

void uart_in(struct buffer* b){
	uint8_t i;
		
		if(RI == 1){
			if(ENTER == SBUF){
				out_flag = 1;
				RI = 0;	
				TI = 0;
				return;
			}
			for(i = 0; i < 10; i++){
				if(numbers[i] == SBUF){
					b->buff[b->current_size] = SBUF;
					RI = 0;
					TI = 0;
					b->current_size++;
					break;
				}
			}
		}
}

void uart_out(struct buffer* b){
		if(out_flag == 1){
				putchar(b->buff[b->current_index]);
				b->current_index++;
			if(b->current_index == b->current_size){
					out_flag = 0;
					b->current_size = 0;
					b->current_index = 0;
			}
		}
}

void uart_echo(){
		uint8_t uart_input;
		if(RI == 1){
			uart_input = SBUF;
			RI = 0;
			SBUF = uart_input;
			TI = 0;
		}
}

void flash_next(struct buffer* b){
		if(out_flag == 1){
			decode_morse(b->buff[b->current_index]);
			if(b->current_index == b->current_size){
					out_flag = 0;
					b->current_size = 0;
					b->current_index = 0;
			}
		}
		if(out_flag == 1){
				b->current_index++;
		}
}


void uart_isr(void) interrupt 4 {
	if(out_flag == 1){
		out_flag = 0;
		b.current_size = 0;
		b.current_index = 0;
	}
	uart_in(&b);

}



void main (void)
{
	b.current_size = 0;
	b.current_index = 0;
	
	SCON  = 0x50;                   // UART mode 1  
	TMOD |= 0x20;                   // timer 1 mode 2     
	TH1   = 0xfd;                   // 19200 baud  22.118 mhz clock      
	TR1   = 1;                      // timer 1 run                        
	TI    = 1;                      // UART receive first character	
	
	interrupts = 1;
	serial_interrupt = 1;
	
//Demo to be made in update freq = 20
while(1)
  {
		//uart_in(&b);	
		flash_next(&b);		
		//uart_out(&b);
		//uart_echo();
		//decode_morse(0x31);
  }
}
