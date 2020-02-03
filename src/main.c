/*
 * @file	main.c
 * @author	GClown25
 * @date	02-02-2020
 * @brief	SBUS receiver interface 
 */


#define F_CPU 20000000UL

#include <avr/io.h>
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


#define USART_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5)
#define ErrorHandler() _ErrorHandler(__FILE__, __LINE__)

FILE USARTconsoleStream = FDEV_SETUP_STREAM(USARTconsolePrintCHAR, NULL, _FDEV_SETUP_WRITE);


int main(void){
	
	SystemInit();
	
	stdout = &USARTconsoleStream;
	UartConsoleInit();	
	
	
	while(1){
		printf("Hello, World!\n");
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