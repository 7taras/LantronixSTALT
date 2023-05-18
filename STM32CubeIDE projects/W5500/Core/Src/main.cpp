/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "W5500.h"
#include "flashRW.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

CommonRegisterBlock crb {
	0, // uint8_t mr {0};		// offset 0x01
	0, // uint8_t gar0 {0};		// offset 0x01
	0, // uint8_t gar1 {0};		// offset 0x02
	0, // uint8_t gar2 {0};		// offset 0x03
	0, // uint8_t gar3 {0};		// offset 0x04
	255, // uint8_t subr0 {0};		// offset 0x05
	255, // uint8_t subr1 {0};		// offset 0x06
	255, // uint8_t subr2 {0};		// offset 0x07
	0, // uint8_t subr3 {0};		// offset 0x08
	0x89, // uint8_t shar0 {0};		// offset 0x09
	0xAB, // uint8_t shar1 {0};		// offset 0x0A
	0xCD, // uint8_t shar2 {0};		// offset 0x0B
	0xEF, // uint8_t shar3 {0};		// offset 0x0C
	0, // uint8_t shar4 {0};		// offset 0x0D
	0x01, // uint8_t shar5 {0};		// offset 0x0E
	192, // uint8_t sipr0 {0};		// offset 0x0F
	168, // uint8_t sipr1 {0};		// offset 0x10
	1, // uint8_t sipr2 {0};		// offset 0x11
	15, // uint8_t sipr3 {0};		// offset 0x12
	0, // uint8_t intlevel0 {0};	// offset 0x13
	0, // uint8_t intlevel1 {0};	// offset 0x14
	0, // uint8_t ir {0};			// offset 0x15
	0b11000000, // uint8_t imr {0};		// offset 0x16
	0, // uint8_t sir {0};		// offset 0x17
	0x01, // uint8_t simr {0};		// offset 0x18
	0x07, // uint8_t rtr0 {0x07};	// offset 0x19
	0xD0, // uint8_t rtr1 {0xD0};	// offset 0x1A
	0x08, // uint8_t rcr {0x08};		// offset 0x1B
	0x28, // uint8_t ptimer {0x28};	// offset 0x1C
	0, // uint8_t pmagic {0};		// offset 0x1D
	0, // uint8_t phar0 {0};		// offset 0x1E
	0, // uint8_t phar1 {0};		// offset 0x1F
	0, // uint8_t phar2 {0};		// offset 0x20
	0, // uint8_t phar3 {0};		// offset 0x21
	0, // uint8_t phar4 {0};		// offset 0x22
	0, // uint8_t phar5 {0};		// offset 0x23
	0, // uint8_t psid0 {0};		// offset 0x24
	0, // uint8_t psid1 {0};		// offset 0x25
	0xFF, // uint8_t pmru0 {0xFF};	// offset 0x26
	0xFF, // uint8_t pmru1 {0xFF};	// offset 0x27
	0, // uint8_t uipr0 {0};		// offset 0x28
	0, // uint8_t uipr1 {0};		// offset 0x29
	0, // uint8_t uipr2 {0};		// offset 0x2A
	0, // uint8_t uipr3 {0};		// offset 0x2B
	0, // uint8_t uportr0 {0};	// offset 0x2C
	0, // uint8_t uportr1 {0};	// offset 0x2D
	0b10111000, // uint8_t phycfgr {0b10111000};	// offset 0x2E
	0, // uint8_t reserved {0};	// offset 0x2F
};

SocketRegisterBlock srb0, srb1, srb2, srb3, srb4, srb5, srb6, srb7 {
	0, // uint8_t sNmr {0};			// offset 0x00
	0, // uint8_t sNcr {0};			// offset 0x01
	0, // uint8_t sNir {0};			// offset 0x02
	0, // uint8_t sNsr {0};			// offset 0x03
	0, // uint8_t sNport0 {0};		// offset 0x04
	0, // uint8_t sNport1 {0};		// offset 0x05
	0xFF, // uint8_t sNdhar0 {0xFF};		// offset 0x06
	0xFF, // uint8_t sNdhar1 {0xFF};		// offset 0x07
	0xFF, // uint8_t sNdhar2 {0xFF};		// offset 0x08
	0xFF, // uint8_t sNdhar3 {0xFF};		// offset 0x09
	0xFF, // uint8_t sNdhar4 {0xFF};		// offset 0x0A
	0xFF, // uint8_t sNdhar5 {0xFF};		// offset 0x0B
	0, // uint8_t sNdipr0 {0};		// offset 0x0C
	0, // uint8_t sNdipr1 {0};		// offset 0x0D
	0, // uint8_t sNdipr2 {0};		// offset 0x0E
	0, // uint8_t sNdipr3 {0};		// offset 0x0F
	0, // uint8_t sNdport0 {0};		// offset 0x10
	0, // uint8_t sNdport1 {0};		// offset 0x11
	0, // uint8_t sNmssr0 {0};		// offset 0x12
	0, // uint8_t sNmssr1 {0};		// offset 0x13
	0, // uint8_t reserved14 {0};		// offset 0x14
	0, // uint8_t sNtos {0};			// offset 0x15
	0x80, // uint8_t sNttl {0x80};		// offset 0x16
	0, // uint8_t reserved17 {0};		// offset 0x17
	0, // uint8_t reserved18 {0};		// offset 0x18
	0, // uint8_t reserved19 {0};		// offset 0x19
	0, // uint8_t reserved1A {0};		// offset 0x1A
	0, // uint8_t reserved1B {0};		// offset 0x1B
	0, // uint8_t reserved1C {0};		// offset 0x1C
	0, // uint8_t reserved1D {0};		// offset 0x1D
	0x02, // uint8_t sNrxbufSize {0x02};	// offset 0x1E
	0x02, // uint8_t sNtxbufSize {0x02};	// offset 0x1F
	0x08, // uint8_t sNtxFsr0 {0x08};	// offset 0x20
	0, // uint8_t sNtxFsr1 {0};		// offset 0x21
	0, // uint8_t sNtxRd0 {0};		// offset 0x22
	0, // uint8_t sNtxRd1 {0};		// offset 0x23
	0, // uint8_t sNtxWr0 {0};		// offset 0x24
	0, // uint8_t sNtxWr1 {0};		// offset 0x25
	0, // uint8_t sNrxRsr0 {0};		// offset 0x26
	0, // uint8_t sNrxRsr1 {0};		// offset 0x27
	0, // uint8_t sNrxRd0 {0};		// offset 0x28
	0, // uint8_t sNrxRd1 {0};		// offset 0x29
	0, // uint8_t sNrxWr0 {0};		// offset 0x2A
	0, // uint8_t sNrxWr1 {0};		// offset 0x2B
	0xFF, // uint8_t sNimr {0xFF};		// offset 0x2C
	0x40, // uint8_t sNfrag0  {0x40};	// offset 0x2D
	0, // uint8_t sNfrag1 {0};		// offset 0x2E
	0, // uint8_t sNkpalvtr {0};		// offset 0x2F
};

uint8_t rxByte {0};
uint8_t txByte {0};
uint8_t rxHello[20] = "\nHello everyone!!!!";
uint8_t mosiBytes[10] {0, 0x19, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t misoBytes[10] {0};
uint8_t rxBytes[128] {0};
uint8_t rxCounter {0};
uint8_t rxBytesToParse[128] {0};
uint8_t rxCounterToParse {0};
uint8_t irq4[6] = "IRQ  ";
bool rxDataIsReadyToParse {false};
//uint8_t txByte[6] {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Cоздаем интерфейc c чипом W5500
W5500 port1(&hspi1, W5500_CS_GPIO_Port, W5500_CS_Pin, W5500_RST_GPIO_Port, W5500_RST_Pin);
//RingBuffer buf1;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */



  //HAL_UART_Transmit_IT(&huart1, rxHello, 20 );

  //writeFLASH();
  /*
  crb.gar1 = 50;
  crb.gar2 = 100;
  crb.gar3 = 200;
  crb.subr0 = 1;
  crb.subr1 = 2;
  crb.subr2 = 3;
  crb.subr3 = 4;
  srb0.sNdipr1 = 1;
  srb0.sNdipr2 = 2;
  srb1.sNdipr1 = 1;
  srb2.sNdipr2 = 2;
  */
  //readFLASH();



  // Включаем чип W5500

  port1.reset();
  port1.writeCRB(&crb.mr);
  port1.initSocket0();
  port1.setUDPmodeSocket0();
  HAL_Delay(10);
  port1.openSocket0();
  HAL_Delay(10);

  txByte = port1.getStatusSocket0();
  HAL_GPIO_WritePin(LED_TX_GPIO_Port, LED_TX_Pin, GPIO_PIN_RESET);
  HAL_UART_Transmit_IT(&huart1, &txByte, 1 );




  // Разрешаем прием по UART
  HAL_UART_Receive_IT(&huart1, &rxByte, 1);

  CommonRegisterBlock test;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (rxDataIsReadyToParse)
	  {
		  if(rxBytesToParse[0] == 'A' && rxBytesToParse[1] == 'T')
		  {
			  switch(rxBytesToParse[2])
			  {
			  case '0':
				  port1.writeSHA();
				  break;
			  case '1':
				  port1.switchOn();
				  break;
			  default:
				  break;
			  }


		  }
		  rxDataIsReadyToParse = false;
	  }

	  HAL_Delay(1000);
	  //HAL_GPIO_TogglePin(LED_TX_GPIO_Port, LED_TX_Pin);
	  //HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);
	  //HAL_SPI_TransmitReceive(&hspi1, mosiBytes, misoBytes, 10, 100);
	  //HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET);
	  //txByte = port1.readVersion();
	  //HAL_UART_Transmit_IT(&huart1, &txByte, 1 );
	  //port1.readCRB(&test.mr);

	  //HAL_GPIO_WritePin(LED_TX_GPIO_Port, LED_TX_Pin, GPIO_PIN_RESET);
	  //HAL_UART_Transmit_IT(&huart1, &test.mr, 48 );

	  //HAL_Delay(1000);
	  //port1.writeSHA();
	  //port1.readSHA(rxHello);
	  //HAL_GPIO_WritePin(LED_TX_GPIO_Port, LED_TX_Pin, GPIO_PIN_RESET);
	  //HAL_UART_Transmit_IT(&huart1, rxHello, 6 );

	  txByte = port1.readRXbufferSocket0();
	  HAL_GPIO_WritePin(LED_TX_GPIO_Port, LED_TX_Pin, GPIO_PIN_RESET);
	  HAL_UART_Transmit_IT(&huart1, &txByte, 1 );

	  //HAL_UART_Transmit(&huart1, rxHello, 20, 1000 );
	  //HAL_GPIO_WritePin(LED_RX_GPIO_Port, LED_RX_Pin, GPIO_PIN_RESET);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 11;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 20000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 3;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 12000;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_TX_Pin|LED_RX_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : W5500_INT_Pin */
  GPIO_InitStruct.Pin = W5500_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(W5500_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : W5500_RST_Pin W5500_CS_Pin */
  GPIO_InitStruct.Pin = W5500_RST_Pin|W5500_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_TX_Pin */
  GPIO_InitStruct.Pin = LED_TX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_TX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_RX_Pin */
  GPIO_InitStruct.Pin = LED_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_RX_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart1)
  {
	  HAL_GPIO_WritePin(LED_RX_GPIO_Port, LED_RX_Pin, GPIO_PIN_RESET);
	  HAL_TIM_Base_Start_IT(&htim4);
	  //HAL_TIM_Base_Stop(&htim3);
	  __HAL_TIM_SET_COUNTER(&htim3, 0);
	  HAL_TIM_Base_Start_IT(&htim3);
	  rxBytes[rxCounter] = rxByte;
	  //buf1.put(rxByte);
	  rxCounter++;
	  HAL_UART_Receive_IT(&huart1, &rxByte, 1);
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
  if(huart == &huart1)
  {
	  // гаcим cветодиод "TX"
	  HAL_GPIO_WritePin(LED_TX_GPIO_Port, LED_TX_Pin, GPIO_PIN_SET);
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3) //check if the interrupt comes from TIM4
	{
		HAL_TIM_Base_Stop(&htim3);
		//HAL_GPIO_WritePin(LED_TX_GPIO_Port, LED_TX_Pin, GPIO_PIN_RESET);
		//HAL_UART_Transmit_IT(&huart1, rxBytes, rxCounter);
		for (int i = 0; i < rxCounter; ++i)
		{
			rxBytesToParse[i] = rxBytes[i];
		}
		rxCounterToParse = rxCounter;
		rxDataIsReadyToParse = true;
		rxCounter = 0;

		//HAL_GPIO_WritePin(LED_RX_GPIO_Port, LED_RX_Pin, GPIO_PIN_SET);
	}
	if(htim->Instance == TIM4) //check if the interrupt comes from TIM4
	{
		HAL_TIM_Base_Stop(&htim4);
		HAL_GPIO_WritePin(LED_RX_GPIO_Port, LED_RX_Pin, GPIO_PIN_SET);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == W5500_INT_Pin)
	{
		irq4[4] = port1.readSIR();
		irq4[5] = port1.readSn_IR();
		if(irq4[4]) port1.clearSIR();

		HAL_GPIO_WritePin(LED_TX_GPIO_Port, LED_TX_Pin, GPIO_PIN_RESET);
		HAL_UART_Transmit_IT(&huart1, irq4, 6 );
	}
}








/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
