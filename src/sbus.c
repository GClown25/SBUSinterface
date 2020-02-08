/*
 * @file	sbus.c
 * @author	GClown25
 * @date	02-02-2020
 * @brief	SBUS receiver interface 
 */

#include "sbus.h"


void sbusInit(sbusINFO *pInfo){
	
}



/* @name
 *
 * @brief
 *
 * @param
 * @param
 * @param
 *
 * @return	bool	returns true, if a new frame got received
 *
 */
bool sbusCallback(sbusINFO *pInfo, uint8_t data, bool rxError){
	
	bool newFrame = false;
	
	bool headerError = (pInfo->rxIndex == 0U) && (data != 0x0f) ? true : false; 
	bool footerError = (pInfo->rxIndex == 24U) && (data != 0x00) ? true : false;
	
	
	if(rxError || headerError || footerError){
		pInfo->rxIndex = 0U;
	}else{
		pInfo->rxBuffer[pInfo->rxIndex] = data;
		pInfo->rxIndex++;
		
		if(pInfo->rxIndex > 24U){
			newFrame = true;
			pInfo->rxIndex = 0U;
		}
	}
	
	
	return newFrame;
	
}



void sbusExtract(sbusINFO *pInfo){
	
	pInfo->channel[0U] = ((pInfo->rxBuffer[2] & 0b00000111) << 8U) | ((pInfo->rxBuffer[1] & 0b11111111) >> 0U);
	pInfo->channel[1U] = ((pInfo->rxBuffer[3] & 0b00111111) << 5U) | ((pInfo->rxBuffer[2] & 0b11111000) >> 3U);
	pInfo->channel[2U] = ((pInfo->rxBuffer[5] & 0b00000001) << 10U) | ((pInfo->rxBuffer[4] & 0b11111111) << 2U) 
						 | ((pInfo->rxBuffer[3] & 0b11000000) >> 6U);
	pInfo->channel[3U] = ((pInfo->rxBuffer[6] & 0b00001111) << 7U) | ((pInfo->rxBuffer[5] & 0b11111110) >> 1U);
}