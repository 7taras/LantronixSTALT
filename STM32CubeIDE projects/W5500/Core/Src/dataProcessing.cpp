/*
 * flashRW.cpp
 * dataProcessing.cpp
 *
 *  Created on: Jun 21, 2023
 *      Author: 85tar
 */
#include "dataProcessing.h"

extern struct CommonRegisterBlock crb;
extern struct SocketRegisterBlock srb0, srb1, srb2, srb3, srb4, srb5, srb6, srb7;
extern char textConnected[];
extern char text0[];
extern char text1[];
extern char text2[];
extern char text3[];
extern char text4[];
extern char text5[];
extern char text6[];
extern char text7[];
extern char text8[];
extern char text9[];
extern char text10[];
extern char text11[];
extern char text12[];
extern char text13[];

extern uint32_t receivedDataTelnet[];

void int2letter(char& digit)
{
	  if (digit <= 9) digit += 48;
	  else digit += 55;
	  return;
}

void fillText()
{
	  textConnected[53] = crb.sipr0/100 + 48;
	  textConnected[54] = (crb.sipr0/10)%10 + 48;
	  textConnected[55] = crb.sipr0%10 + 48;
	  text0[27] = crb.sipr1/100 + 48;
	  text0[28] = (crb.sipr1/10)%10 + 48;
	  text0[29] = crb.sipr1%10 + 48;
	  text1[27] = crb.sipr2/100 + 48;
	  text1[28] = (crb.sipr2/10)%10 + 48;
	  text1[29] = crb.sipr2%10 + 48;
	  text2[27] = crb.sipr3/100 + 48;
	  text2[28] = (crb.sipr3/10)%10 + 48;
	  text2[29] = crb.sipr3%10 + 48;

	  text3[36] = crb.subr0/100 + 48;
	  text3[37] = (crb.subr0/10)%10 + 48;
	  text3[38] = crb.subr0%10 + 48;
	  text4[36] = crb.subr1/100 + 48;
	  text4[37] = (crb.subr1/10)%10 + 48;
	  text4[38] = crb.subr1%10 + 48;
	  text5[36] = crb.subr2/100 + 48;
	  text5[37] = (crb.subr2/10)%10 + 48;
	  text5[38] = crb.subr2%10 + 48;
	  text6[36] = crb.subr3/100 + 48;
	  text6[37] = (crb.subr3/10)%10 + 48;
	  text6[38] = crb.subr3%10 + 48;

	  text7[30] = crb.shar0 / 16;
	  int2letter(text7[30]);
	  text7[31] = crb.shar0 % 16;
	  int2letter(text7[31]);

	  text8[30] = crb.shar1 / 16;
	  int2letter(text8[30]);
	  text8[31] = crb.shar1 % 16;
	  int2letter(text8[31]);

	  text9[30] = crb.shar2 / 16;
	  int2letter(text9[30]);
	  text9[31] = crb.shar2 % 16;
	  int2letter(text9[31]);

	  text10[30] = crb.shar3 / 16;
	  int2letter(text10[30]);
	  text10[31] = crb.shar3 % 16;
	  int2letter(text10[31]);

	  text11[30] = crb.shar4 / 16;
	  int2letter(text11[30]);
	  text11[31] = crb.shar4 % 16;
	  int2letter(text11[31]);

	  text12[30] = crb.shar5 / 16;
	  int2letter(text12[30]);
	  text12[31] = crb.shar5 % 16;
	  int2letter(text12[31]);
	  return;
}

void fillText13()
{
	if (receivedDataTelnet[0] > 0 && receivedDataTelnet[0] < 256)
	{
		text13[35] = receivedDataTelnet[0] / 100 + 48;
		text13[36] = (receivedDataTelnet[0] / 10) % 10 + 48;
		text13[37] = receivedDataTelnet[0] % 10 + 48;
	}
	else
	{
		text13[35] = '0';
		text13[36] = '0';
		text13[37] = '0';
		receivedDataTelnet[0] = 0;
	}

	if (receivedDataTelnet[1] > 0 && receivedDataTelnet[1] < 256)
	{
		text13[39] = receivedDataTelnet[1] / 100 + 48;
		text13[40] = (receivedDataTelnet[1] / 10) % 10 + 48;
		text13[41] = receivedDataTelnet[1] % 10 + 48;
	}
	else
	{
		text13[39] = '0';
		text13[40] = '0';
		text13[41] = '0';
		receivedDataTelnet[1] = 0;
	}

	if (receivedDataTelnet[2] > 0 && receivedDataTelnet[2] < 256)
	{
		text13[43] = receivedDataTelnet[2] / 100 + 48;
		text13[44] = (receivedDataTelnet[2] / 10) % 10 + 48;
		text13[45] = receivedDataTelnet[2] % 10 + 48;
	}
	else
	{
		text13[43] = '0';
		text13[44] = '0';
		text13[45] = '0';
		receivedDataTelnet[2] = 0;
	}

	if (receivedDataTelnet[3] > 0 && receivedDataTelnet[3] < 256)
	{
		text13[47] = receivedDataTelnet[3] / 100 + 48;
		text13[48] = (receivedDataTelnet[3] / 10) % 10 + 48;
		text13[49] = receivedDataTelnet[3] % 10 + 48;
	}
	else
	{
		text13[47] = '0';
		text13[48] = '0';
		text13[49] = '0';
		receivedDataTelnet[3] = 0;
	}

	if (receivedDataTelnet[4] > 0 && receivedDataTelnet[4] < 256)
	{
		text13[73] = receivedDataTelnet[4] / 100 + 48;
		text13[74] = (receivedDataTelnet[4] / 10) % 10 + 48;
		text13[75] = receivedDataTelnet[4] % 10 + 48;
	}
	else
	{
		text13[77] = '0';
		text13[78] = '0';
		text13[79] = '0';
		receivedDataTelnet[4] = 0;
	}

	if (receivedDataTelnet[5] > 0 && receivedDataTelnet[5] < 256)
	{
		text13[77] = receivedDataTelnet[5] / 100 + 48;
		text13[78] = (receivedDataTelnet[5] / 10) % 10 + 48;
		text13[79] = receivedDataTelnet[5] % 10 + 48;
	}
	else
	{
		text13[77] = '0';
		text13[78] = '0';
		text13[79] = '0';
		receivedDataTelnet[5] = 0;
	}

	if (receivedDataTelnet[6] > 0 && receivedDataTelnet[6] < 256)
	{
		text13[81] = receivedDataTelnet[6] / 100 + 48;
		text13[82] = (receivedDataTelnet[6] / 10) % 10 + 48;
		text13[83] = receivedDataTelnet[6] % 10 + 48;
	}
	else
	{
		text13[81] = '0';
		text13[82] = '0';
		text13[83] = '0';
		receivedDataTelnet[6] = 0;
	}

	if (receivedDataTelnet[7] > 0 && receivedDataTelnet[7] < 256)
	{
		text13[85] = receivedDataTelnet[7] / 100 + 48;
		text13[86] = (receivedDataTelnet[7] / 10) % 10 + 48;
		text13[87] = receivedDataTelnet[7] % 10 + 48;
	}
	else
	{
		text13[85] = '0';
		text13[86] = '0';
		text13[87] = '0';
		receivedDataTelnet[7] = 0;
	}
}
