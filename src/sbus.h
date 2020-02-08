/*
 * @file	sbus.h
 * @author	GClown25
 * @date	02-02-2020
 * @brief	SBUS receiver interface (8 channel)
 */

#ifndef SBUS_H_
#define SBUS_H_

#include <stdbool.h>
#include <inttypes.h>

typedef struct {
	uint8_t rxBuffer[25U];
	uint8_t rxIndex;
	
	uint16_t channel[16U];
	bool frameLost;
	bool failsafeFlag;
}sbusINFO;


void sbusInit(sbusINFO *pInfo);
bool sbusCallback(sbusINFO *pInfo, uint8_t data, bool rxError);
void sbusExtract(sbusINFO *pInfo);






#endif //SBUS_H_