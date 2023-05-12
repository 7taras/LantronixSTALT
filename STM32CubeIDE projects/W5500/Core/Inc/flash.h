/*
 * FLASH.h
 *
 *  Created on: May 12, 2023
 *      Author: 85tar
 */

#pragma once

#include "main.h"

#define CONFIG_FLASHPAGE	0x0801FC00
#define FIRMWARE_PAGE_OFFSET 	0x0C00

void writeFLASH(void);
void writeBlockFLASH(uint32_t address, uint64_t* pData64, unsigned int size);
void readFLASH(void);
void readBlockFlash(uint32_t address, uint32_t* pData32, unsigned int size);
