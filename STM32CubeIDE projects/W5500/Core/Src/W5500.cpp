//#pragma once
#include "W5500.h"

W5500::W5500(
		SPI_HandleTypeDef* hspi_m,
		GPIO_TypeDef* W5500_CS_GPIO_Port_m,
		uint16_t W5500_CS_Pin_m,
		GPIO_TypeDef* W5500_RST_GPIO_Port_m,
		uint16_t W5500_RST_Pin_m
		)
{
	hspi_w = hspi_m;
	W5500_CS_GPIO_Port_w = W5500_CS_GPIO_Port_m;
	W5500_CS_Pin_w = W5500_CS_Pin_m;
	W5500_RST_GPIO_Port_w = W5500_RST_GPIO_Port_m;
	W5500_RST_Pin_w = W5500_RST_Pin_m;
}

W5500::~W5500()
{
}

void W5500::reset()
{

}

uint8_t W5500::readVersion()
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_VERSIONR;
	mosiBytes_w[2] = 0;
	mosiBytes_w[4] = 0;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 4, 100);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return misoBytes_w[3];
}
