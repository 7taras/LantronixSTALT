/*
 * flashRW.cpp
 *
 *  Created on: May 13, 2023
 *      Author: 85tar
 */
#include "flashRW.h"
//#include "main.cpp"

extern struct CommonRegisterBlock crb;
extern struct SocketRegisterBlock srb0, srb1, srb2, srb3, srb4, srb5, srb6, srb7;

// Запись во FLASH
void writeFLASH()
{
	// Открываем доступ к FLASH (она закрыта от случайной записи)
	HAL_FLASH_Unlock();

	// Объявляем структуру, необходимую для функции стирания страницы
	FLASH_EraseInitTypeDef eraseInit;
	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES; // Стираем постранично
	//eraseInit.Banks = FLASH_BANK_1;
	eraseInit.PageAddress = CONFIG_FLASHPAGE; // Адрес страницы для стирания
	eraseInit.NbPages = 1; // Число страниц = 1

	// Объявляем переменную для сохранения результата стирания FLASH (функции HAL_FLASHEx_Erase()), д.б. 0xFFFFFFFF (не используем)
	uint32_t statusFLASHerase {0};

	// Очищаем страницу
	HAL_FLASHEx_Erase(&eraseInit, &statusFLASHerase);

	// Записываем блок Common Register
	writeBlockFLASH(CONFIG_FLASHPAGE, (uint64_t*) &crb, sizeof(crb)/8);

	// Записываем блок Socket 0 Register
	writeBlockFLASH(CONFIG_FLASHPAGE + 64, (uint64_t*) &srb0, sizeof(srb0)/8);

	// Записываем блок Socket 1 Register
	writeBlockFLASH(CONFIG_FLASHPAGE + 120, (uint64_t*) &srb1, sizeof(srb1)/8);

	// Записываем блок Socket 2 Register
	writeBlockFLASH(CONFIG_FLASHPAGE + 176, (uint64_t*) &srb2, sizeof(srb2)/8);

	// Записываем блок Socket 3 Register
	writeBlockFLASH(CONFIG_FLASHPAGE + 232, (uint64_t*) &srb3, sizeof(srb3)/8);

	// Записываем блок Socket 4 Register
	writeBlockFLASH(CONFIG_FLASHPAGE + 288, (uint64_t*) &srb4, sizeof(srb4)/8);

	// Записываем блок Socket 5 Register
	writeBlockFLASH(CONFIG_FLASHPAGE + 344, (uint64_t*) &srb5, sizeof(srb5)/8);

	// Записываем блок Socket 6 Register
	writeBlockFLASH(CONFIG_FLASHPAGE + 400, (uint64_t*) &srb6, sizeof(srb6)/8);

	// Записываем блок Socket 7 Register
	writeBlockFLASH(CONFIG_FLASHPAGE + 456, (uint64_t*) &srb7, sizeof(srb7)/8);

	// Закрываем доступ к FLASH, от случайной записи
	HAL_FLASH_Lock();
	return;
}

// Запись области во FLASH, по 64бита (doubleword)
void writeBlockFLASH(uint32_t address, uint64_t* pData64, unsigned int size)
{
	// Объявляем переменную для сохранения результата записи во FLASH (функции HAL_FLASH_Program())
	HAL_StatusTypeDef statusFLASHprogram;

	// Записываем нужную область памяти
	for (unsigned int i = 0; i < size; i++)
	{
		statusFLASHprogram = HAL_FLASH_Program (FLASH_TYPEPROGRAM_DOUBLEWORD, address + i*8, *(pData64++));
		if (statusFLASHprogram != HAL_OK) break; // Если что-то пошло не так - выходим из цикла
	}
	return;
}

// Чтение из FLASH
void readFLASH()
{
	// Читаем блок Common Register
	readBlockFlash(CONFIG_FLASHPAGE, (uint32_t*) &crb, sizeof(crb)/4);

	// Читаем блок Socket 0 Register
	readBlockFlash(CONFIG_FLASHPAGE + 64, (uint32_t*) &srb0, sizeof(srb0)/4);

	// Читаем блок Socket 1 Register
	readBlockFlash(CONFIG_FLASHPAGE + 120, (uint32_t*) &srb1, sizeof(srb1)/4);

	// Читаем блок Socket 2 Register
	readBlockFlash(CONFIG_FLASHPAGE + 176, (uint32_t*) &srb2, sizeof(srb2)/4);

	// Читаем блок Socket 3 Register
	readBlockFlash(CONFIG_FLASHPAGE + 232, (uint32_t*) &srb3, sizeof(srb3)/4);

	// Читаем блок Socket 4 Register
	readBlockFlash(CONFIG_FLASHPAGE + 288, (uint32_t*) &srb4, sizeof(srb4)/4);

	// Читаем блок Socket 5 Register
	readBlockFlash(CONFIG_FLASHPAGE + 344, (uint32_t*) &srb5, sizeof(srb5)/4);

	// Читаем блок Socket 6 Register
	readBlockFlash(CONFIG_FLASHPAGE + 400, (uint32_t*) &srb6, sizeof(srb6)/4);

	// Читаем блок Socket 7 Register
	readBlockFlash(CONFIG_FLASHPAGE + 456, (uint32_t*) &srb7, sizeof(srb7)/4);

	return;
}

// Чтение области из FLASH
void readBlockFlash(uint32_t address, uint32_t* pData32, unsigned int size)
{
	for (unsigned int i = 0; i < size; i++)
	{
		*(pData32 + i) = *((volatile uint32_t*)(address+i*4));
	}
	return;
}
