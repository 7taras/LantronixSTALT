// 20230621.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
char text13[]{ "\n\rSo, your setting is:\n\rIP Address xxx.xxx.xxx.xxx\n\rSubnet Mask Address: xxx.xxx.xxx.xxx\n\rMAC Address xx:xx:xx:xx:xx:xx\n\rDo you want to save setting[y/n]: " };
uint32_t receivedDataTelnet[]{ 198, 255, 178, 179, 181, 182, 183, 184, 185 };

uint8_t adr[]{ 1, 0, 0, 100 };
uint8_t mac[]{ 0xAB, 0x15, 0x87, 0xCD , 0xEF, 0x66 };
char adrText[16]{ 0 };
char macText[18]{ 0 };

void hex2char(char& digit)
{
	if (digit <= 9) digit += 48;
	else digit += 55;
	return;
}

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

int main()
{
	adr_int2string(adr, adrText);

	std::cout << adrText << '\n';

	mac_int2string(mac, macText);

	std::cout << macText << '\n';




	//if (receivedDataTelnet[0] > 0 && receivedDataTelnet[0] < 256)
	//{
	//	text13[35] = receivedDataTelnet[0] / 100 + 48;
	//	text13[36] = (receivedDataTelnet[0] / 10) % 10 + 48;
	//	text13[37] = receivedDataTelnet[0] % 10 + 48;
	//}
	//else
	//{
	//	text13[35] = '0';
	//	text13[36] = '0';
	//	text13[37] = '0';
	//	receivedDataTelnet[0] = 0;
	//}

	//if (receivedDataTelnet[1] > 0 && receivedDataTelnet[1] < 256)
	//{
	//	text13[39] = receivedDataTelnet[1] / 100 + 48;
	//	text13[40] = (receivedDataTelnet[1] / 10) % 10 + 48;
	//	text13[41] = receivedDataTelnet[1] % 10 + 48;
	//}
	//else
	//{
	//	text13[39] = '0';
	//	text13[40] = '0';
	//	text13[41] = '0';
	//	receivedDataTelnet[1] = 0;
	//}

	//if (receivedDataTelnet[2] > 0 && receivedDataTelnet[2] < 256)
	//{
	//	text13[43] = receivedDataTelnet[2] / 100 + 48;
	//	text13[44] = (receivedDataTelnet[2] / 10) % 10 + 48;
	//	text13[45] = receivedDataTelnet[2] % 10 + 48;
	//}
	//else
	//{
	//	text13[43] = '0';
	//	text13[44] = '0';
	//	text13[45] = '0';
	//	receivedDataTelnet[2] = 0;
	//}

	//if (receivedDataTelnet[3] > 0 && receivedDataTelnet[3] < 256)
	//{
	//	text13[47] = receivedDataTelnet[3] / 100 + 48;
	//	text13[48] = (receivedDataTelnet[3] / 10) % 10 + 48;
	//	text13[49] = receivedDataTelnet[3] % 10 + 48;
	//}
	//else
	//{
	//	text13[47] = '0';
	//	text13[48] = '0';
	//	text13[49] = '0';
	//	receivedDataTelnet[3] = 0;
	//}

	//if (receivedDataTelnet[4] > 0 && receivedDataTelnet[4] < 256)
	//{
	//	text13[73] = receivedDataTelnet[4] / 100 + 48;
	//	text13[74] = (receivedDataTelnet[4] / 10) % 10 + 48;
	//	text13[75] = receivedDataTelnet[4] % 10 + 48;
	//}
	//else
	//{
	//	text13[77] = '0';
	//	text13[78] = '0';
	//	text13[79] = '0';
	//	receivedDataTelnet[4] = 0;
	//}

	//if (receivedDataTelnet[5] > 0 && receivedDataTelnet[5] < 256)
	//{
	//	text13[77] = receivedDataTelnet[5] / 100 + 48;
	//	text13[78] = (receivedDataTelnet[5] / 10) % 10 + 48;
	//	text13[79] = receivedDataTelnet[5] % 10 + 48;
	//}
	//else
	//{
	//	text13[77] = '0';
	//	text13[78] = '0';
	//	text13[79] = '0';
	//	receivedDataTelnet[5] = 0;
	//}

	//if (receivedDataTelnet[6] > 0 && receivedDataTelnet[6] < 256)
	//{
	//	text13[81] = receivedDataTelnet[6] / 100 + 48;
	//	text13[82] = (receivedDataTelnet[6] / 10) % 10 + 48;
	//	text13[83] = receivedDataTelnet[6] % 10 + 48;
	//}
	//else
	//{
	//	text13[81] = '0';
	//	text13[82] = '0';
	//	text13[83] = '0';
	//	receivedDataTelnet[6] = 0;
	//}

	//if (receivedDataTelnet[7] > 0 && receivedDataTelnet[7] < 256)
	//{
	//	text13[85] = receivedDataTelnet[7] / 100 + 48;
	//	text13[86] = (receivedDataTelnet[7] / 10) % 10 + 48;
	//	text13[87] = receivedDataTelnet[7] % 10 + 48;
	//}
	//else
	//{
	//	text13[85] = '0';
	//	text13[86] = '0';
	//	text13[87] = '0';
	//	receivedDataTelnet[7] = 0;
	//}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
