/*
 * flashRW.h
 *
 *  Created on: May 13, 2023
 *      Author: 85tar
 */

// Последняя (128-я) страница (1 кбайт) FLASH-памяти используется для сохранения данных регистров чипа W5500
// Для одного чипа W5500 используется половина страницы 512 байт
// Карта памяти следуюшая:
// 0x0000 - 0x003F - блок Common Register
// 0x0040 - 0x0077 - блок Socket 0 Register
// 0x0078 - 0x00AF - блок Socket 1 Register
// 0x00B0 - 0x00E7 - блок Socket 2 Register
// 0x00E8 - 0x011F - блок Socket 3 Register
// 0x0120 - 0x0157 - блок Socket 4 Register
// 0x0158 - 0x018F - блок Socket 5 Register
// 0x0190 - 0x01C7 - блок Socket 6 Register
// 0x01C8 - 0x01FF - блок Socket 7 Register

#ifndef INC_FLASHRW_H_
#define INC_FLASHRW_H_

#include <main.h>

#define CONFIG_FLASHPAGE	0x0801FC00
#define FIRMWARE_PAGE_OFFSET 	0x0C00

extern struct CommonRegisterBlock crb;
extern struct SocketRegisterBlock srb0, srb1, srb2, srb3, srb4, srb5, srb6, srb7;

void writeFLASH(void);
void writeBlockFLASH(uint32_t address, uint64_t* pData64, unsigned int size);
void readFLASH(void);
void readBlockFlash(uint32_t address, uint32_t* pData32, unsigned int size);
void readCRBfromFlash(uint32_t* pData32);
void eraseFLASH();


#endif /* INC_FLASHRW_H_ */
