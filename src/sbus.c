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
 * @param	pInfo
 * @param	data
 * @param	rxError
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


/* @name
 *
 * @brief
 *
 * @param	pInfo
 *
 */
void sbusExtract(sbusINFO *pInfo){
	
	pInfo->channel[0U] =	((pInfo->rxBuffer[2] & 0b00000111) << 8U) |	((pInfo->rxBuffer[1] & 0b11111111) >> 0U);
	pInfo->channel[1U] =	((pInfo->rxBuffer[3] & 0b00111111) << 5U) | ((pInfo->rxBuffer[2] & 0b11111000) >> 3U);
	pInfo->channel[2U] =	((pInfo->rxBuffer[5] & 0b00000001) << 10U) | ((pInfo->rxBuffer[4] & 0b11111111) << 2U) | 
							((pInfo->rxBuffer[3] & 0b11000000) >> 6U);
	pInfo->channel[3U] =	((pInfo->rxBuffer[6] & 0b00001111) << 7U) | ((pInfo->rxBuffer[5] & 0b11111110) >> 1U);
	pInfo->channel[4U] =	((pInfo->rxBuffer[7] & 0b01111111) << 4U) | ((pInfo->rxBuffer[6] & 0b11110000) >> 4U);
	pInfo->channel[5U] =	((pInfo->rxBuffer[9] & 0b00000011) << 9U) | ((pInfo->rxBuffer[8] & 0b11111111) << 1U) | 
							((pInfo->rxBuffer[7] & 0b10000000) >> 7U);
	pInfo->channel[6U] =	((pInfo->rxBuffer[10] & 0b00011111) << 6U) | ((pInfo->rxBuffer[9] & 0b11111100) >> 2U);
	pInfo->channel[7U] =	((pInfo->rxBuffer[11] & 0b11111111) << 3U) | ((pInfo->rxBuffer[10] & 0b11100000) >> 5U);
	
	pInfo->channel[8U] =	((pInfo->rxBuffer[13] & 0b00000111) << 8U) | ((pInfo->rxBuffer[12] & 0b11111111) >> 0U);
	pInfo->channel[9U] =	((pInfo->rxBuffer[14] & 0b00111111) << 5U) | ((pInfo->rxBuffer[13] & 0b11111000) >> 3U);
	pInfo->channel[10U] =	((pInfo->rxBuffer[16] & 0b00000001) << 10U) | ((pInfo->rxBuffer[15] & 0b11111111) << 2U) | 
							((pInfo->rxBuffer[14] & 0b11000000) >> 6U);
	pInfo->channel[11U] =	((pInfo->rxBuffer[17] & 0b00001111) << 7U) | ((pInfo->rxBuffer[16] & 0b11111110) >> 1U);
	pInfo->channel[12U] =	((pInfo->rxBuffer[18] & 0b01111111) << 4U) | ((pInfo->rxBuffer[17] & 0b11110000) >> 4U);
	pInfo->channel[13U] =	((pInfo->rxBuffer[20] & 0b00000011) << 9U) | ((pInfo->rxBuffer[19] & 0b11111111) << 1U) |
							((pInfo->rxBuffer[18] & 0b10000000) >> 7U);
	pInfo->channel[14U] =	((pInfo->rxBuffer[21] & 0b00011111) << 6U) | ((pInfo->rxBuffer[20] & 0b11111100) >> 2U);
	pInfo->channel[15U] =	((pInfo->rxBuffer[22] & 0b11111111) << 3U) | ((pInfo->rxBuffer[21] & 0b11100000) >> 5U);
}