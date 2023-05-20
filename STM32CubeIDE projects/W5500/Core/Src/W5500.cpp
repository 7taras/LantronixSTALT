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
	HAL_Delay(1);
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
	HAL_Delay(1);
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

// читаем байт из регистра блока CRB
uint8_t W5500::readByteFromCRB(uint8_t address)
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = address;
	mosiBytes_w[2] = 0;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 4, 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return misoBytes_w[3];
}

// читаем массив байт из регистров блока CRB
void W5500::readArrayFromCRB(uint8_t* destinationArray, uint8_t sizeArray, uint8_t beginAddress)
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = beginAddress;
	mosiBytes_w[2] = 0;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, (sizeArray + 3), 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	for(int i = 0; i < sizeArray; ++i)
	{
		destinationArray[i] = misoBytes_w[i+3];
	}
	return;
}

// записываем байт в регистр блока CRB
void W5500::writeByteToCRB(uint8_t value, uint8_t address)
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = address;
	mosiBytes_w[2] = 0;
	mosiBytes_w[3] = value;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi_w, mosiBytes_w, 4, 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
}

// записываем массив байт в регистры блока CRB
void W5500::writeArrayToCRB(uint8_t* array, uint8_t sizeArray, uint8_t beginAddress)
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = beginAddress;
	mosiBytes_w[2] = 0;
	for(int i = 0; i < sizeArray; ++i)
	{
		mosiBytes_w[i+3] = array[i];
	}
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi_w, mosiBytes_w, (sizeArray + 3), 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
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

void W5500::initSocket0()
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_Sn_PORT;
	mosiBytes_w[2] = 0b00001100;
	mosiBytes_w[3] = 0x1D; // port 7500
	mosiBytes_w[4] = 0x4C;
	mosiBytes_w[5] = 0x50; // 50-EB-F6-4D-BA-12
	mosiBytes_w[6] = 0xEB;
	mosiBytes_w[7] = 0xF6;
	mosiBytes_w[8] = 0x4D;
	mosiBytes_w[9] = 0xBA;
	mosiBytes_w[10] = 0x12;
	mosiBytes_w[11] = 0xC0; // 192.168.1.7
	mosiBytes_w[12] = 0xA8;
	mosiBytes_w[13] = 0x01;
	mosiBytes_w[14] = 0x07;
	mosiBytes_w[15] = 0x19; // port 6500
	mosiBytes_w[16] = 0x64;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 17, 100);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
}

void W5500::setUDPmodeSocket0()
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_Sn_MR;
	mosiBytes_w[2] = 0b00001100;
	mosiBytes_w[3] = 0b00000010; // UDP mode
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 4, 100);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
}

void W5500::openSocket0()
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_Sn_CR;
	mosiBytes_w[2] = 0b00001100;
	mosiBytes_w[3] = 0b00000001; // open
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 4, 100);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
}

uint8_t W5500::getStatusSocket0()
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_Sn_SR;
	mosiBytes_w[2] = 0b00001000;
	mosiBytes_w[3] = 0;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 4, 100);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return misoBytes_w[3];
}

uint8_t W5500::readRXbufferSocket0()
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = 0;
	mosiBytes_w[2] = 0b00010000;
	mosiBytes_w[3] = 0;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 4, 100);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return misoBytes_w[3];
}

// читаем флаги прерываний, общих для чипа
uint8_t W5500::readIR()
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_IR;
	mosiBytes_w[2] = 0;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 4, 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return misoBytes_w[3];
}

// сбрасываем флаги прерываний, общих для чипа
void W5500::clearIR()
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_SIR;
	mosiBytes_w[2] = 0b00000100;
	mosiBytes_w[3] = 0xFF;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	//HAL_SPI_Transmit(hspi_w, mosiBytes_w, 4, 1000);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 4, 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
}

// читаем флаги прерываний от сокетов
uint8_t W5500::readSIR()
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_SIR;
	mosiBytes_w[2] = 0;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 4, 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return misoBytes_w[3];
}

// сбрасываем флаги прерываний от сокетов
void W5500::clearSIR()
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_SIR;
	mosiBytes_w[2] = 0b00000100;
	mosiBytes_w[3] = 0;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	//HAL_SPI_Transmit(hspi_w, mosiBytes_w, 4, 1000);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 4, 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
}

uint8_t W5500::readSn_IR()
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_Sn_IR;
	mosiBytes_w[2] = 0b00001000;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 4, 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return misoBytes_w[3];
}

void W5500::clearSn_IR()
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_Sn_IR;
	mosiBytes_w[2] = 0b00001100;
	mosiBytes_w[3] = 0xFF;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 4, 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
}

void W5500::readSocketTXRX(uint8_t* regTXRX)
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = W5500_Sn_TX_FSR;
	mosiBytes_w[2] = 0b00001000;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 15, 100);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	for(int i = 0; i < 12; ++i)
	{
		regTXRX[i] = misoBytes_w[i+3];
	}
	return;
}
