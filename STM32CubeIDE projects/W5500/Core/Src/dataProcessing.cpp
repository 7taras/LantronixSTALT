/*
 * flashRW.cpp
 * dataProcessing.cpp
 *
 *  Created on: Jun 21, 2023
 *      Author: 85tar
 */
#include "dataProcessing.h"
#include "flashRW.h"

extern struct CommonRegisterBlock crb;
extern struct SocketRegisterBlock srb0, srb1, srb2, srb3, srb4, srb5, srb6, srb7;

extern uint8_t receivedIPAddress[4];
extern uint8_t receivedSubnetMask[4];
extern uint8_t receivedMACAddress[6];

char text0[] {"Setting ACTA.468353.020\n\n\rType IP address (1st bit) [\0      "};
char textValue [4] {0};
char textEnd[] {"]: "};
char text1[] {"Type IP Address (2nd bit) [\0      "};
char text2[] {"Type IP Address (3rd bit) [\0      "};
char text3[] {"Type IP Address (4th bit) [\0      "};
char text4[] {"Type Subnet Mask Address (1st bit) [\0      "};
char text5[] {"Type Subnet Mask Address (2nd bit) [\0      "};
char text6[] {"Type Subnet Mask Address (3rd bit) [\0      "};
char text7[] {"Type Subnet Mask Address (4th bit) [\0      "};
char text8[] {"Type MAC Address (1st octet) [\0      "};
char text9[] {"Type MAC Address (2nd octet) [\0      "};
char text10[] {"Type MAC Address (3rd octet) [\0      "};
char text11[] {"Type MAC Address (4th octet) [\0      "};
char text12[] {"Type MAC Address (5th octet) [\0      "};
char text13[] {"Type MAC Address (6th octet) [\0      "};
//char text14[] {"\n\rSo, your setting is:\n\rIP Address xxx.xxx.xxx.xxx\n\rSubnet Mask Address: xxx.xxx.xxx.xxx\n\rMAC Address xx:xx:xx:xx:xx:xx\n\n\rDo you want to save setting?[y/n]: "};

char text14[] {"\n\rSo, your setting is:\n\rIP Address: "};
char text15[16] {0};
char text16[] {"\n\rSubnet Mask Address: "};
char text17[16] {0};
char text18[] {"\n\rMAC Address "};
char text19[18] {0};
char text20[] {"\n\n\rDo you want to save setting?[y/n]: "};
char textFinal[256] {0};

char* arrText[] {text1, text2, text3, text4, text5, text6, text7, text8, text9, text10, text11, text12, text13, textFinal};
char* arrFinalText[] {text14, text15, text16, text17, text18, text19, text20};
char textError[] {"ERROR"};
char textApply[] {"Your setting will be saved and device will be reboot"};
char textNotApply[] {"Your setting is NOT saved\n\n\rPress [ENTER] to disconnect "};

void hex2char(char& digit)
{
	if (digit <= 9) digit += 48;
	else digit += 55;
	return;
}

void hex2letter(char& digit)
{
	  if (digit <= 9) digit += 48;
	  else digit += 55;
	  return;
}

// четыре байта в строку
void adr_int2string(const uint8_t* adr, char* text)
{
	int index{ 0 };
	char num, num2;
	for (int i = 0; i < 4; ++i)
	{
		num = adr[i] / 100;
		if (num)
		{
			text[index] = num + 48;
			++index;
		}
		num2 = (adr[i] / 10) % 10;
		if (num2 || num)
		{
			text[index] = num2 + 48;
			++index;
		}
		text[index] = adr[i] % 10 + 48;
		++index;
		if (i < 3)
		{
			text[index] = '.';
			++index;
		}
		else
		{
			text[index] = '\0';
		}
	}
	return;
}

// шесть октетов MAC-адреса в строку
void mac_int2string(const uint8_t* adr, char* text)
{
	int index{ 0 };
	for (int i = 0; i < 6; ++i)
	{
		text[index] = adr[i] / 16;
		hex2char(text[index]);
		++index;
		text[index] = adr[i] % 16;
		hex2char(text[index]);
		++index;
		if (i < 5)
		{
			text[index] = ':';
			++index;
		}
		else
		{
			text[index] = '\0';
		}
	}
	return;
}

// байт адреса в строку
void byte_int2string(const uint8_t& byte, char* text)
{
	int index{ 0 };
	char num, num2;

	num = byte / 100;
	if (num)
	{
		text[index] = num + 48;
		++index;
	}
	num2 = (byte / 10) % 10;
	if (num2 || num)
	{
		text[index] = num2 + 48;
		++index;
	}
	text[index] = byte % 10 + 48;
	++index;

	text[index] = '\0';

	return;
}

// октет MAC-адреса в строку
void macOctet2string(const uint8_t& byte, char* text)
{
	text[0] = byte / 16;
	hex2letter(text[0]);
	text[1] = byte % 16;
	hex2letter(text[1]);
	text[2] = '\0';
}

// склеивание трех строк
void concatenate3strings(char* moderated, const char* second, const char* third)
{
	int index {0};
	int index2 {0};
	while (moderated[index] != '\0')
	{
		++index;
	}
	while (second[index2] != '\0')
	{
		moderated[index] = second[index2];
		++index;
		++index2;
	}
	index2 = 0;
	while (third[index2] != '\0')
	{
		moderated[index] = third[index2];
		++index;
		++index2;
	}
	moderated[index] = '\0';
	return;
}

void concatenate7strings(char* newString, char** array)
{
	int index {0};
	for (int i = 0; i < 7; ++i)
	{
		int index2 {0};
		while (array[i][index2] != '\0')
		{
			newString[index] = array[i][index2];
			++index;
			++index2;
		}
	}
	newString[index] = '\0';
	return;
}



uint8_t letter2int(const char& digit)
{
	if (digit >= '0' && digit <= '9')
	{
		return digit - 48;
	}
	else if (digit >= 'A' && digit <= 'F')
	{
		return digit - 55;
	}
	else if (digit >= 'a' && digit <= 'f')
	{
		return digit - 87;
	}
	else return 0;
}

void fillText()
{
	byte_int2string(crb.sipr0, textValue);
	concatenate3strings(text0, textValue, textEnd);
	byte_int2string(crb.sipr1, textValue);
	concatenate3strings(text1, textValue, textEnd);
	byte_int2string(crb.sipr2, textValue);
	concatenate3strings(text2, textValue, textEnd);
	byte_int2string(crb.sipr3, textValue);
	concatenate3strings(text3, textValue, textEnd);

	byte_int2string(crb.subr0, textValue);
	concatenate3strings(text4, textValue, textEnd);
	byte_int2string(crb.subr1, textValue);
	concatenate3strings(text5, textValue, textEnd);
	byte_int2string(crb.subr2, textValue);
	concatenate3strings(text6, textValue, textEnd);
	byte_int2string(crb.subr3, textValue);
	concatenate3strings(text7, textValue, textEnd);

	macOctet2string(crb.shar0, textValue);
	concatenate3strings(text8, textValue, textEnd);
	macOctet2string(crb.shar1, textValue);
	concatenate3strings(text9, textValue, textEnd);
	macOctet2string(crb.shar2, textValue);
	concatenate3strings(text10, textValue, textEnd);
	macOctet2string(crb.shar3, textValue);
	concatenate3strings(text11, textValue, textEnd);
	macOctet2string(crb.shar4, textValue);
	concatenate3strings(text12, textValue, textEnd);
	macOctet2string(crb.shar5, textValue);
	concatenate3strings(text13, textValue, textEnd);

	return;
}

void fillFinalText()
{
	adr_int2string(receivedIPAddress, text15);
	adr_int2string(receivedSubnetMask, text17);
	mac_int2string(receivedMACAddress, text19);
	concatenate7strings(textFinal, arrFinalText);
	return;
}

void saveSettings()
{
	crb.sipr0 = receivedIPAddress[0];
	crb.sipr1 = receivedIPAddress[1];
	crb.sipr2 = receivedIPAddress[2];
	crb.sipr3 = receivedIPAddress[3];
	crb.subr0 = receivedSubnetMask[0];
	crb.subr1 = receivedSubnetMask[1];
	crb.subr2 = receivedSubnetMask[2];
	crb.subr3 = receivedSubnetMask[3];
	crb.shar0 = receivedMACAddress[0];
	crb.shar1 = receivedMACAddress[1];
	crb.shar2 = receivedMACAddress[2];
	crb.shar3 = receivedMACAddress[3];
	crb.shar4 = receivedMACAddress[4];
	crb.shar5 = receivedMACAddress[5];

	writeFLASH();

	NVIC_SystemReset();

	return;
}
