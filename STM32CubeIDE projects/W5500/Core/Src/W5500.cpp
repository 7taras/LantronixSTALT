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

// аппаратный сброс чипа
void W5500::reset()
{
	// для аппаратного сброса "тянем" вход RSTn к "0" на 1 мс
	HAL_GPIO_WritePin(W5500_RST_GPIO_Port_w, W5500_RST_Pin_w, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(W5500_RST_GPIO_Port_w, W5500_RST_Pin_w, GPIO_PIN_SET);
}

// программный сброс чипа
void W5500::softwareReset()
{
	// для программного сброса устанавливаем бит RST регистра MR в "1"
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_MR;
	mosiBytes_w[2] = 0b00000100;
	mosiBytes_w[3] = 0b10000000;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi_w, mosiBytes_w, 4, 100);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	mosiBytes_w[2] = 0;
	misoBytes_w[3] = 0b11111111;
	// ждём подтверждения сброса - бит RST регистра MR должен установиться в "0"
	while(misoBytes_w[3] & 0b10000000)
	{
		HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 4, 100);
	}
	return;
}

// программный сброс PHY
void W5500::softwareResetPHY()
{
	// для программного сброса устанавливаем бит RST регистра PHYCFGR в "0"
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_PHYCFGR;
	mosiBytes_w[2] = 0b00000100;
	mosiBytes_w[3] = 0b00111000;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi_w, mosiBytes_w, 4, 100);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	mosiBytes_w[2] = 0;
	misoBytes_w[3] = 0;
	// ждём подтверждения сброса - бит RST регистра PHYCFGR должен установиться в "1"
	while(!(misoBytes_w[3] & 0b10000000))
	{
		HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 4, 100);
	}
	return;
}

// включение чипа
void W5500::switchOn()
{
	HAL_GPIO_WritePin(W5500_RST_GPIO_Port_w, W5500_RST_Pin_w, GPIO_PIN_SET);
	HAL_Delay(1);
}

// выключение чипа
void W5500::shutDown()
{
	HAL_GPIO_WritePin(W5500_RST_GPIO_Port_w, W5500_RST_Pin_w, GPIO_PIN_RESET);
}

uint8_t W5500::readVersion()
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_VERSIONR;
	mosiBytes_w[2] = 0;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 4, 100);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return misoBytes_w[3];
}

void W5500::readSHA(uint8_t* regSHA)
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_SHAR;
	mosiBytes_w[2] = 0;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 9, 100);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	regSHA[0] = misoBytes_w[3];
	regSHA[1] = misoBytes_w[4];
	regSHA[2] = misoBytes_w[5];
	regSHA[3] = misoBytes_w[6];
	regSHA[4] = misoBytes_w[7];
	regSHA[5] = misoBytes_w[8];
	return;
}

void W5500::writeSHA()
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_SHAR;
	mosiBytes_w[2] = 0b00000100;
	mosiBytes_w[3] = 0;
	mosiBytes_w[4] = 0x08;
	mosiBytes_w[5] = 0xDC;
	mosiBytes_w[6] = 0xAB;
	mosiBytes_w[7] = 0xCD;
	mosiBytes_w[8] = 0x01;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 9, 100);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
}

void W5500::readCRB(uint8_t* regCRB)
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_MR;
	mosiBytes_w[2] = 0;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 50, 100);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	for(int i = 0; i < 47; ++i)
	{
		regCRB[i] = misoBytes_w[i+3];
	}
	return;
}

void W5500::writeCRB(uint8_t* regCRB)
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_MR;
	mosiBytes_w[2] = 0b00000100;
	for(int i = 0; i < 47; ++i)
	{
		mosiBytes_w[i+3] = regCRB[i];
	}
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 50, 100);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
}
