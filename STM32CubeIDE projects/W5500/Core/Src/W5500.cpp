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

//---------------------------------------------------------------------------

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

// читаем слово (2 байта) из регистров блока CRB
uint16_t W5500::readWordFromCRB(uint8_t address)
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = address;
	mosiBytes_w[2] = 0;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 5, 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	word_w5500 temp;
	temp.byte[0] = misoBytes_w[4];
	temp.byte[1] = misoBytes_w[3];
	return temp.word;
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
	mosiBytes_w[2] = 0b00000100;
	mosiBytes_w[3] = value;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi_w, mosiBytes_w, 4, 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
}

// записываем слово (2 байта) в регистры блока CRB
void W5500::writeWordToCRB(uint16_t value, uint8_t address)
{
	word_w5500 temp;
	temp.word = value;
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = address;
	mosiBytes_w[2] = 0b00000100;
	mosiBytes_w[3] = temp.byte[1];
	mosiBytes_w[4] = temp.byte[0];
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi_w, mosiBytes_w, 5, 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
}

// записываем массив байт в регистры блока CRB
void W5500::writeArrayToCRB(uint8_t* array, uint8_t sizeArray, uint8_t beginAddress)
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = beginAddress;
	mosiBytes_w[2] = 0b00000100;
	for(int i = 0; i < sizeArray; ++i)
	{
		mosiBytes_w[i+3] = array[i];
	}
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi_w, mosiBytes_w, (sizeArray + 3), 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
}

// читаем байт из регистра блока SRB
uint8_t W5500::readByteFromSRB(uint8_t socket, uint8_t address)
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = address;
	mosiBytes_w[2] = socket;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 4, 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return misoBytes_w[3];
}

// читаем слово (2 байта) из регистров блока SRB
uint16_t W5500::readWordFromSRB(uint8_t socket, uint8_t address)
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = address;
	mosiBytes_w[2] = socket;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, 5, 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	word_w5500 temp;
	temp.byte[0] = misoBytes_w[4];
	temp.byte[1] = misoBytes_w[3];
	return temp.word;
}

// читаем массив байт из регистров блока SRB
void W5500::readArrayFromSRB(uint8_t socket, uint8_t* destinationArray, uint8_t sizeArray, uint8_t beginAddress)
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = beginAddress;
	mosiBytes_w[2] = socket;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, (sizeArray + 3), 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	for(int i = 0; i < sizeArray; ++i)
	{
		destinationArray[i] = misoBytes_w[i+3];
	}
	return;
}

// записываем байт в регистр блока SRB
void W5500::writeByteToSRB(uint8_t socket, uint8_t value, uint8_t address)
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = address;
	mosiBytes_w[2] = (socket | 0b00000100);
	mosiBytes_w[3] = value;
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi_w, mosiBytes_w, 4, 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
}

// записываем слово (2 байта) в регистры блока SRB
void W5500::writeWordToSRB(uint8_t socket, uint16_t value, uint8_t address)
{
	word_w5500 temp;
	temp.word = value;
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = address;
	mosiBytes_w[2] = (socket | 0b00000100);
	mosiBytes_w[3] = temp.byte[1];
	mosiBytes_w[4] = temp.byte[0];
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi_w, mosiBytes_w, 5, 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
}

// записываем массив байт в регистры блока SRB
void W5500::writeArrayToSRB(uint8_t socket, uint8_t* array, uint8_t sizeArray, uint8_t beginAddress)
{
	mosiBytes_w[0] = 0;
	mosiBytes_w[1] = beginAddress;
	mosiBytes_w[2] = (socket | 0b00000100);
	for(int i = 0; i < sizeArray; ++i)
	{
		mosiBytes_w[i+3] = array[i];
	}
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi_w, mosiBytes_w, (sizeArray + 3), 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
}

//---------------------------------------------------------------------------

// читаем массив байт в буфер TX
void W5500::readArrayFromRXbuffer(uint8_t socket, uint8_t* destinationArray, uint16_t sizeArray, word_y beginAddress)
{
	mosiBytes_w[0] = beginAddress.byte[1];
	mosiBytes_w[1] = beginAddress.byte[0];
	mosiBytes_w[2] = (socket + 16);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_w, mosiBytes_w, misoBytes_w, (sizeArray + 3), 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	for(int i = 0; i < sizeArray; ++i)
	{
		destinationArray[i] = misoBytes_w[i+3];
	}
	return;
}


// записываем массив байт в буфер TX
void W5500::writeArrayToTXbuffer(uint8_t socket, uint8_t* array, uint16_t sizeArray, word_y beginAddress)
{
	mosiBytes_w[0] = beginAddress.byte[1];
	mosiBytes_w[1] = beginAddress.byte[0];
	mosiBytes_w[2] = ((socket + 8) | 0b00000100);
	for(int i = 0; i < sizeArray; ++i)
	{
		mosiBytes_w[i+3] = array[i];
	}
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi_w, mosiBytes_w, (sizeArray + 3), 1000);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port_w, W5500_CS_Pin_w, GPIO_PIN_SET);
	return;
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


// получаем данные по протоколу UDP
void W5500::receiveDataUDP(uint8_t socket, uint8_t* dataForReceive, uint16_t* sizeArray)
{
	// временные переменные для хранения значений регистров RX_RSR и RX_RD
	word_y valueRSR, valueRSRretry, valueRD;

	// читаем регистр RX_RSR (разница между значениями RX_WR и RX_RD)
	valueRSR.word = readWordFromSRB(socket, W5500_Sn_RX_RSR);
	// считываем еще раз
	valueRSRretry.word = readWordFromSRB(socket, W5500_Sn_RX_RSR);
	// сравниваем, тем самым исключая изменения в нём в момент считывания
	while (valueRSR.word != valueRSRretry.word)
	{
		valueRSR.word = valueRSRretry.word;
		valueRSRretry.word = readWordFromSRB(socket, W5500_Sn_RX_RSR);
	}

	// читаем регистр RX_RD (указатель на начало полученных данных)
	valueRD.word = readWordFromSRB(socket, W5500_Sn_RX_RD);

	// читаем буфер RX
	readArrayFromRXbuffer(socket, dataForReceive, valueRSR.word, valueRD);

	// увеличиваем значение указателя на полученные данные на число считанных байт
	valueRD.word += valueRSR.word;
	writeWordToSRB(socket, valueRD.word, W5500_Sn_RX_RD);

	// завершаем процесс чтения из буфера RX
	writeByteToSRB(socket, W5500_Sn_CR_RECV, W5500_Sn_CR);

	// записываем значение полученных байт
	*sizeArray = valueRSR.word;
	return;
}

// отправляем данные по протоколу UDP
void W5500::sendDataUDP(uint8_t socket, uint8_t* dataForSend, uint16_t sizeArray)
{
	// временные переменные для хранения значений регистров TX_FSR и TX_WR
	word_y valueFSR, valueWR;

	// читаем регистр TX_FSR (хранит значение свободного места в буфере TX
	valueFSR.word = readWordFromSRB(socket, W5500_Sn_TX_FSR);

	// проверяем, что размер данных для отправки не превышает количества свободного места в буфере
	if((uint16_t)sizeArray > valueFSR.word) return;

	// читаем регистр TX_WR (указатель на начало свободного места буфера)
	valueWR.word = readWordFromSRB(socket, W5500_Sn_TX_WR);

	// записываем данные для отправки в буфер TX
	writeArrayToTXbuffer(socket, dataForSend, sizeArray, valueWR);

	// увеличиваем значение указателя на начало свободного места
	valueWR.word += sizeArray;
	writeWordToSRB(socket, valueWR.word, W5500_Sn_TX_WR);

	// отправляем данные
	writeByteToSRB(socket, W5500_Sn_CR_SEND, W5500_Sn_CR);
	return;
}

