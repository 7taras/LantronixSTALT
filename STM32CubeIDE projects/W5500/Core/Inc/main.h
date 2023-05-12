/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define W5500_INT_Pin GPIO_PIN_4
#define W5500_INT_GPIO_Port GPIOA
#define W5500_INT_EXTI_IRQn EXTI4_IRQn
#define W5500_RST_Pin GPIO_PIN_4
#define W5500_RST_GPIO_Port GPIOC
#define W5500_CS_Pin GPIO_PIN_5
#define W5500_CS_GPIO_Port GPIOC
#define LED_TX_Pin GPIO_PIN_8
#define LED_TX_GPIO_Port GPIOB
#define LED_RX_Pin GPIO_PIN_9
#define LED_RX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

typedef struct CommonRegisterBlock
{
	uint8_t mr {0};			// offset 0x00
	uint8_t gar0 {0};		// offset 0x01
	uint8_t gar1 {0};		// offset 0x02
	uint8_t gar2 {0};		// offset 0x03
	uint8_t gar3 {0};		// offset 0x04
	uint8_t subr0 {0};		// offset 0x05
	uint8_t subr1 {0};		// offset 0x06
	uint8_t subr2 {0};		// offset 0x07
	uint8_t subr3 {0};		// offset 0x08
	uint8_t shar0 {0};		// offset 0x09
	uint8_t shar1 {0};		// offset 0x0A
	uint8_t shar2 {0};		// offset 0x0B
	uint8_t shar3 {0};		// offset 0x0C
	uint8_t shar4 {0};		// offset 0x0D
	uint8_t shar5 {0};		// offset 0x0E
	uint8_t sipr0 {0};		// offset 0x0F
	uint8_t sipr1 {0};		// offset 0x10
	uint8_t sipr2 {0};		// offset 0x11
	uint8_t sipr3 {0};		// offset 0x12
	uint8_t intlevel0 {0};	// offset 0x13
	uint8_t intlevel1 {0};	// offset 0x14
	uint8_t ir {0};			// offset 0x15
	uint8_t imr {0};		// offset 0x16
	uint8_t sir {0};		// offset 0x17
	uint8_t simr {0};		// offset 0x18
	uint8_t rtr0 {0x07};	// offset 0x19
	uint8_t rtr1 {0xD0};	// offset 0x1A
	uint8_t rcr {0x08};		// offset 0x1B
	uint8_t ptimer {0x28};	// offset 0x1C
	uint8_t pmagic {0};		// offset 0x1D
	uint8_t phar0 {0};		// offset 0x1E
	uint8_t phar1 {0};		// offset 0x1F
	uint8_t phar2 {0};		// offset 0x20
	uint8_t phar3 {0};		// offset 0x21
	uint8_t phar4 {0};		// offset 0x22
	uint8_t phar5 {0};		// offset 0x23
	uint8_t psid0 {0};		// offset 0x24
	uint8_t psid1 {0};		// offset 0x25
	uint8_t pmru0 {0xFF};	// offset 0x26
	uint8_t pmru1 {0xFF};	// offset 0x27
	uint8_t uipr0 {0};		// offset 0x28
	uint8_t uipr1 {0};		// offset 0x29
	uint8_t uipr2 {0};		// offset 0x2A
	uint8_t uipr3 {0};		// offset 0x2B
	uint8_t uportr0 {0};	// offset 0x2C
	uint8_t uportr1 {0};	// offset 0x2D
	uint8_t phycfgr {0b10111000};	// offset 0x2E
	uint8_t reserved {0};	// offset 0x2F
} CommonRegisterBlock;


// создаем регистры
typedef struct SocketRegisterBlock
{
	uint8_t sNmr {0};			// offset 0x00
	uint8_t sNcr {0};			// offset 0x01
	uint8_t sNir {0};			// offset 0x02
	uint8_t sNsr {0};			// offset 0x03
	uint8_t sNport0 {0};		// offset 0x04
	uint8_t sNport1 {0};		// offset 0x05
	uint8_t sNdhar0 {0xFF};		// offset 0x06
	uint8_t sNdhar1 {0xFF};		// offset 0x07
	uint8_t sNdhar2 {0xFF};		// offset 0x08
	uint8_t sNdhar3 {0xFF};		// offset 0x09
	uint8_t sNdhar4 {0xFF};		// offset 0x0A
	uint8_t sNdhar5 {0xFF};		// offset 0x0B
	uint8_t sNdipr0 {0};		// offset 0x0C
	uint8_t sNdipr1 {0};		// offset 0x0D
	uint8_t sNdipr2 {0};		// offset 0x0E
	uint8_t sNdipr3 {0};		// offset 0x0F
	uint8_t sNdport0 {0};		// offset 0x10
	uint8_t sNdport1 {0};		// offset 0x11
	uint8_t sNmssr0;		// offset 0x12
	uint8_t sNmssr1;		// offset 0x13
	uint8_t reserved14;		// offset 0x14
	uint8_t sNtos;			// offset 0x15
	uint8_t sNttl;		// offset 0x16
	uint8_t reserved17;		// offset 0x17
	uint8_t reserved18;		// offset 0x18
	uint8_t reserved19;		// offset 0x19
	uint8_t reserved1A;		// offset 0x1A
	uint8_t reserved1B;		// offset 0x1B
	uint8_t reserved1C;		// offset 0x1C
	uint8_t reserved1D;		// offset 0x1D
	uint8_t sNrxbufSize;	// offset 0x1E
	uint8_t sNtxbufSize;	// offset 0x1F
	uint8_t sNtxFsr0;	// offset 0x20
	uint8_t sNtxFsr1;		// offset 0x21
	uint8_t sNtxRd0;		// offset 0x22
	uint8_t sNtxRd1;		// offset 0x23
	uint8_t sNtxWr0;		// offset 0x24
	uint8_t sNtxWr1;		// offset 0x25
	uint8_t sNrxRsr0;		// offset 0x26
	uint8_t sNrxRsr1;		// offset 0x27
	uint8_t sNrxRd0;		// offset 0x28
	uint8_t sNrxRd1;		// offset 0x29
	uint8_t sNrxWr0;		// offset 0x2A
	uint8_t sNrxWr1;		// offset 0x2B
	uint8_t sNimr;		// offset 0x2C
	uint8_t sNfrag0;	// offset 0x2D
	uint8_t sNfrag1;		// offset 0x2E
	uint8_t sNkpalvtr;		// offset 0x2F
} SocketRegisterBlock;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
