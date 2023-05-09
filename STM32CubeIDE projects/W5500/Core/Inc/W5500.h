// Date: 2023-04-06 
// Author: https://github.com/7taras/
// W5500 API's.

#pragma once
#include "W5500_Memory.h"
#include "W5500_SPI.h"
#include "main.h"

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
	void switchOn();
	void shutDown();
	uint8_t readVersion();

private:
	SPI_HandleTypeDef* hspi_w;
	GPIO_TypeDef* W5500_CS_GPIO_Port_w;
	GPIO_TypeDef* W5500_RST_GPIO_Port_w;
	uint16_t W5500_CS_Pin_w;
	uint16_t W5500_RST_Pin_w;

	uint8_t mosiBytes_w[10] {0};
	uint8_t misoBytes_w[10] {0};
};
