// Date: 2023-04-06 
// Author: https://github.com/7taras/
// W5500 API's.

#pragma once
#include <main.h>
#include "W5500_Memory.h"
#include <string.h>

union word_w5500
{
	uint16_t word;
	uint8_t byte[2];
};

class W5500
{
public:
	W5500(
			SPI_HandleTypeDef* hspi_m,
			GPIO_TypeDef* W5500_CS_GPIO_Port_m,
			uint16_t W5500_CS_Pin_m,
			GPIO_TypeDef* W5500_RST_GPIO_Port_m,
			uint16_t W5500_RST_Pin_m
			);
	~W5500();

	void reset();
	void softwareReset();
	void softwareResetPHY();
	void switchOn();
	void shutDown();

	uint8_t readByteFromCRB(uint8_t address);
	uint16_t readWordFromCRB(uint8_t address);
	void readArrayFromCRB(uint8_t* destinationArray, uint8_t sizeArray, uint8_t beginAddress);
	void writeByteToCRB(uint8_t value, uint8_t address);
	void writeWordToCRB(uint16_t value, uint8_t address);
	void writeArrayToCRB(uint8_t* array, uint8_t sizeArray, uint8_t beginAddress);

	uint8_t readByteFromSRB(uint8_t socket, uint8_t address);
	uint16_t readWordFromSRB(uint8_t socket, uint8_t address);
	void readArrayFromSRB(uint8_t socket, uint8_t* destinationArray, uint8_t sizeArray, uint8_t beginAddress);
	void writeByteToSRB(uint8_t socket, uint8_t value, uint8_t address);
	void writeWordToSRB(uint8_t socket, uint16_t value, uint8_t address);
	void writeArrayToSRB(uint8_t socket, uint8_t* array, uint8_t sizeArray, uint8_t beginAddress);

	void readArrayFromRXbuffer(uint8_t socket, uint8_t* destinationArray, uint16_t sizeArray, word_y beginAddress);
	void writeArrayToTXbuffer(uint8_t socket, uint8_t* array, uint16_t sizeArray, word_y beginAddress);

	void receivePacket(uint8_t socket, uint8_t* dataForReceive, uint16_t* sizeArray);
	void sendPacket(uint8_t socket, uint8_t* dataForSend, uint16_t sizeArray);
	void sendString(uint8_t socket, char* stringForSend);

private:
	SPI_HandleTypeDef* hspi_w;
	GPIO_TypeDef* W5500_CS_GPIO_Port_w;
	GPIO_TypeDef* W5500_RST_GPIO_Port_w;
	uint16_t W5500_CS_Pin_w;
	uint16_t W5500_RST_Pin_w;

	uint8_t mosiBytes_w[256] {0};
	uint8_t misoBytes_w[256] {0};
};
