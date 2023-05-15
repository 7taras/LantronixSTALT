// Date: 2023-04-06 
// Author: https://github.com/7taras/
// W5500 API's.

#pragma once
#include <main.h>
#include "W5500_Memory.h"
#include "W5500_SPI.h"

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
	uint8_t readVersion();
	void readSHA(uint8_t* regSHA);
	void writeSHA();
	void readCRB(uint8_t* regCRB);

private:
	SPI_HandleTypeDef* hspi_w;
	GPIO_TypeDef* W5500_CS_GPIO_Port_w;
	GPIO_TypeDef* W5500_RST_GPIO_Port_w;
	uint16_t W5500_CS_Pin_w;
	uint16_t W5500_RST_Pin_w;

	uint8_t mosiBytes_w[64] {0};
	uint8_t misoBytes_w[64] {0};
};
