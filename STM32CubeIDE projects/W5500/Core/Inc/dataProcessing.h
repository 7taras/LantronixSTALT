/*
 * dataProcessing.h
 *
 *  Created on: Jun 21, 2023
 *      Author: 85tar
 */

#ifndef INC_DATAPROCESSING_H_
#define INC_DATAPROCESSING_H_

#include <main.h>

extern struct CommonRegisterBlock crb;
extern struct SocketRegisterBlock srb0, srb1, srb2, srb3, srb4, srb5, srb6, srb7;


void fillText(void);
void fillText13();
void saveSettings();

#endif /* INC_DATAPROCESSING_H_ */
