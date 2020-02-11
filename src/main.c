/*
 * @file	main.c
 * @author	GClown25
 * @date	02-02-2020
 * @brief	SBUS receiver interface 
 */


#define F_CPU 20000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <util/delay.h>

#include "sbus.h"


void SystemInit();
void UartConsoleInit();
uint8_t UartConsoleSend(uint8_t *data, uint16_t len, uint16_t timeout);
int USARTconsolePrintCHAR(char character, FILE *stream);
void _ErrorHandler(char *file, int line);

void UartSBUSInit();


#define USART_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5)
#define ErrorHandler() _ErrorHandler(__FILE__, __LINE__)

FILE USARTconsoleStream = FDEV_SETUP_STREAM(USARTconsolePrintCHAR, NULL, _FDEV_SETUP_WRITE);


sbusINFO sbusInfo;

int main(void){
	
	SystemInit();
	
	PORTA.DIRSET = PIN3_bm | PIN2_bm;
	
	stdout = &USARTconsoleStream;
	UartConsoleInit();	
	
	UartSBUSInit();
	sei();
	
	while(1){

		//printf("Hello, World!\n");
		_delay_ms(1000U);
	}
	
}



void SystemInit(){
	//Disable Clk Prescaler (F_CPU = 20MHz)
	CCP = 0xd8;
	CLKCTRL.MCLKCTRLB = 0x00;
	
}


void UartConsoleInit(){
	PORTF.DIR |= PIN0_bm;
	PORTF.OUTSET |= PIN0_bm;
	
	USART2.BAUD = (uint16_t)USART_BAUD_RATE(115200);
	USART2.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;
}

void UartSBUSInit(){
	
	PORTMUX.USARTROUTEA = PORTMUX_USART1_ALT1_gc;
	
	USART1.BAUD = (uint16_t)USART_BAUD_RATE(100000);
	USART1.CTRLA = USART_RXCIE_bm;
	USART1.CTRLB = USART_RXEN_bm;
	USART1.CTRLC = USART_PMODE_EVEN_gc | USART_SBMODE_2BIT_gc | USART_CHSIZE_8BIT_gc;
}

ISR(USART1_RXC_vect){
	uint8_t dataL, dataH;
	bool rxError, newFrame;
	
	dataH = USART1.RXDATAH;	// rx error flags
	dataL = USART1.RXDATAL;	// data byte
	
	rxError = (dataH & (USART_BUFOVF_bm | USART_FERR_bm | USART_PERR_bm)) ? true : false;
	
	PORTA.OUTSET = PIN3_bm;
	newFrame = sbusCallback(&sbusInfo, dataL, rxError);
	PORTA.OUTCLR = PIN3_bm;	
	
	if(newFrame){
		PORTA.OUTSET = PIN2_bm;
		sbusExtract(&sbusInfo);
		PORTA.OUTCLR = PIN2_bm;	
		printf("R:%i P:%i T:%i Y:%i arm:%i buz:%i aux:%i lq:%i\n", 
				sbusInfo.channel[0], sbusInfo.channel[1], sbusInfo.channel[2], sbusInfo.channel[3],
				sbusInfo.channel[4], sbusInfo.channel[5], sbusInfo.channel[6], sbusInfo.channel[7]);
	}
	
}

uint8_t UartConsoleSend(uint8_t *data, uint16_t len, uint16_t timeout){
	
	for(int i = 0; i < len; i++){
		while (!(USART2.STATUS & USART_DREIF_bm))
		;	//TODO possible endless loop
		
		USART2.TXDATAL = data[i];
	}
	
	return 0;
}

int USARTconsolePrintCHAR(char character, FILE *stream){
	
	UartConsoleSend((uint8_t*)&character, 1U, 100U);
	
	return 0;
}


void _ErrorHandler(char *file, int line){
	
	printf("ERROR: '%s' LINE: '%i'\n", file, line);
	
}