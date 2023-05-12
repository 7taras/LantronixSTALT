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
#include "ringBuffer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CONFIG_FLASHPAGE	0x0801FC00
#define FIRMWARE_PAGE_OFFSET 	0x0C00
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
typedef struct
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
typedef struct
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
	uint8_t sNmssr0 {0};		// offset 0x12
	uint8_t sNmssr1 {0};		// offset 0x13
	uint8_t reserved14 {0};		// offset 0x14
	uint8_t sNtos {0};			// offset 0x15
	uint8_t sNttl {0x80};		// offset 0x16
	uint8_t reserved17 {0};		// offset 0x17
	uint8_t reserved18 {0};		// offset 0x18
	uint8_t reserved19 {0};		// offset 0x19
	uint8_t reserved1A {0};		// offset 0x1A
	uint8_t reserved1B {0};		// offset 0x1B
	uint8_t reserved1C {0};		// offset 0x1C
	uint8_t reserved1D {0};		// offset 0x1D
	uint8_t sNrxbufSize {0x02};	// offset 0x1E
	uint8_t sNtxbufSize {0x02};	// offset 0x1F
	uint8_t sNtxFsr0 {0x08};	// offset 0x20
	uint8_t sNtxFsr1 {0};		// offset 0x21
	uint8_t sNtxRd0 {0};		// offset 0x22
	uint8_t sNtxRd1 {0};		// offset 0x23
	uint8_t sNtxWr0 {0};		// offset 0x24
	uint8_t sNtxWr1 {0};		// offset 0x25
	uint8_t sNrxRsr0 {0};		// offset 0x26
	uint8_t sNrxRsr1 {0};		// offset 0x27
	uint8_t sNrxRd0 {0};		// offset 0x28
	uint8_t sNrxRd1 {0};		// offset 0x29
	uint8_t sNrxWr0 {0};		// offset 0x2A
	uint8_t sNrxWr1 {0};		// offset 0x2B
	uint8_t sNimr {0xFF};		// offset 0x2C
	uint8_t sNfrag0  {0x40};	// offset 0x2D
	uint8_t sNfrag1 {0};		// offset 0x2E
	uint8_t sNkpalvtr {0};		// offset 0x2F
} SocketRegisterBlock;

CommonRegisterBlock crb;
SocketRegisterBlock srb0;
SocketRegisterBlock srb1;
SocketRegisterBlock srb2;
SocketRegisterBlock srb3;
SocketRegisterBlock srb4;
SocketRegisterBlock srb5;
SocketRegisterBlock srb6;
SocketRegisterBlock srb7;


uint8_t rxByte {0};
uint8_t txByte {0};
uint8_t rxHello[20] = "\nHello everyone!!!!";
uint8_t mosiBytes[10] {0, 0x19, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t misoBytes[10] {0};
uint8_t rxBytes[128] {0};
uint8_t rxCounter {0};
bool rxDataIsReadyToParse {false};
uint8_t regSHA[6] {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
void writeFLASH(void);
void writeBlockFLASH(uint32_t address, uint64_t* pData64, unsigned int size);
void readFLASH(void);
void readBlockFlash(uint32_t address, uint32_t* pData32, unsigned int size);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
RingBuffer buf1;
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

  writeFLASH();

  readFLASH();

  // Cоздаем интерфейc c чипом W5500
  W5500 port1(&hspi1, W5500_CS_GPIO_Port, W5500_CS_Pin, W5500_RST_GPIO_Port, W5500_RST_Pin);

  // Включаем чип W5500
  port1.switchOn();

  // Разрешаем прием по UART
  HAL_UART_Receive_IT(&huart1, &rxByte, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (rxDataIsReadyToParse)
	  {
		  if(rxBytes[0] == 'A' && rxBytes[1] == 'T')
		  {

		  }
	  }

	  HAL_Delay(1000);
	  //HAL_GPIO_TogglePin(LED_TX_GPIO_Port, LED_TX_Pin);
	  //HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);
	  //HAL_SPI_TransmitReceive(&hspi1, mosiBytes, misoBytes, 10, 100);
	  //HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET);
	  txByte = port1.readVersion();

	  HAL_GPIO_WritePin(LED_TX_GPIO_Port, LED_TX_Pin, GPIO_PIN_RESET);
	  HAL_UART_Transmit_IT(&huart1, &txByte, 1 );

	  HAL_Delay(1000);
	  //port1.writeSHA();
	  port1.readSHA(regSHA);
	  HAL_GPIO_WritePin(LED_TX_GPIO_Port, LED_TX_Pin, GPIO_PIN_RESET);
	  HAL_UART_Transmit_IT(&huart1, regSHA, 6 );


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
  HAL_GPIO_WritePin(GPIOC, W5500_RST_Pin|W5500_CS_Pin, GPIO_PIN_RESET);

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
		HAL_GPIO_WritePin(LED_TX_GPIO_Port, LED_TX_Pin, GPIO_PIN_RESET);
		HAL_UART_Transmit_IT(&huart1, rxBytes, rxCounter);
		rxCounter = 0;
		//HAL_GPIO_WritePin(LED_RX_GPIO_Port, LED_RX_Pin, GPIO_PIN_SET);
	}
	if(htim->Instance == TIM4) //check if the interrupt comes from TIM4
	{
		HAL_TIM_Base_Stop(&htim4);
		HAL_GPIO_WritePin(LED_RX_GPIO_Port, LED_RX_Pin, GPIO_PIN_SET);
	}
}

// Последняя (128-я) страница (1 кбайт) FLASH-памяти используется для сохранения данных регистров чипа W5500
// Для одного чипа W5500 используется половина страницы 512 байт
// Карта памяти следуюшая:
// 0x0000 - 0x003F - блок Common Register
// 0x0040 - 0x0077 - блок Socket 0 Register
// 0x0078 - 0x00AF - блок Socket 1 Register
// 0x00B0 - 0x00E7 - блок Socket 2 Register
// 0x00E8 - 0x011F - блок Socket 3 Register
// 0x0120 - 0x0157 - блок Socket 4 Register
// 0x0158 - 0x018F - блок Socket 5 Register
// 0x0190 - 0x01C7 - блок Socket 6 Register
// 0x01C8 - 0x01FF - блок Socket 7 Register

// Запись во FLASH
void writeFLASH() {
	// Открываем доступ к FLASH (она закрыта от случайной записи)
	HAL_FLASH_Unlock();
	
	// Объявляем структуру, необходимую для функции стирания страницы
	FLASH_EraseInitTypeDef eraseInit; 
	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES; // Стираем постранично
	//eraseInit.Banks = FLASH_BANK_1;
	eraseInit.PageAddress = CONFIG_FLASHPAGE; // Адрес страницы для стирания
	eraseInit.NbPages = 1; // Число страниц = 1
	
	// Объявляем переменную для сохранения результата стирания FLASH (функции HAL_FLASHEx_Erase()), д.б. 0xFFFFFFFF (не используем)
	uint32_t statusFLASHerase {0};

	// Очищаем страницу
	HAL_FLASHEx_Erase(&eraseInit, &statusFLASHerase);

	// Записываем блок Common Register
	writeBlockFLASH(CONFIG_FLASHPAGE, (uint64_t*) &crb, sizeof(crb)/8);

	// Записываем блок Socket 0 Register
	writeBlockFLASH(CONFIG_FLASHPAGE + 64, (uint64_t*) &srb0, sizeof(srb0)/8);

	// Записываем блок Socket 1 Register
	writeBlockFLASH(CONFIG_FLASHPAGE + 120, (uint64_t*) &srb1, sizeof(srb1)/8);

	// Записываем блок Socket 2 Register
	writeBlockFLASH(CONFIG_FLASHPAGE + 176, (uint64_t*) &srb2, sizeof(srb2)/8);

	// Записываем блок Socket 3 Register
	writeBlockFLASH(CONFIG_FLASHPAGE + 232, (uint64_t*) &srb3, sizeof(srb3)/8);

	// Записываем блок Socket 3 Register
	writeBlockFLASH(CONFIG_FLASHPAGE + 288, (uint64_t*) &srb4, sizeof(srb4)/8);

	// Записываем блок Socket 3 Register
	writeBlockFLASH(CONFIG_FLASHPAGE + 344, (uint64_t*) &srb5, sizeof(srb5)/8);

	// Записываем блок Socket 3 Register
	writeBlockFLASH(CONFIG_FLASHPAGE + 400, (uint64_t*) &srb6, sizeof(srb6)/8);

	// Записываем блок Socket 3 Register
	writeBlockFLASH(CONFIG_FLASHPAGE + 456, (uint64_t*) &srb7, sizeof(srb7)/8);

	// Закрываем доступ к FLASH, от случайной записи
	HAL_FLASH_Lock();
	return;
}

// Запись области во FLASH, по 64бита (doubleword)
void writeBlockFLASH(uint32_t address, uint64_t* pData64, unsigned int size)
{
	// Объявляем переменную для сохранения результата записи во FLASH (функции HAL_FLASH_Program())
	HAL_StatusTypeDef statusFLASHprogram;

	// Записываем нужную область памяти
	for (unsigned int i = 0; i < size; i++)
	{
		statusFLASHprogram = HAL_FLASH_Program (FLASH_TYPEPROGRAM_DOUBLEWORD, address + i*8, *(pData64++));
		if (statusFLASHprogram != HAL_OK) break; // Если что-то пошло не так - выходим из цикла
	}
	return;
}

// Чтение из FLASH
void readFLASH()
{
	// Читаем блок Common Register
	readBlockFlash(CONFIG_FLASHPAGE, (uint32_t*) &crb, sizeof(crb)/4);

	// Читаем блок Socket 0 Register
	readBlockFlash(CONFIG_FLASHPAGE + 64, (uint32_t*) &srb0, sizeof(srb0)/4);

	return;
}

// Чтение области из FLASH
void readBlockFlash(uint32_t address, uint32_t* pData32, unsigned int size)
{
	for (unsigned int i = 0; i < size; i++)
	{
		*pData32 = *(volatile uint32_t*)(address+i*4);
	}
	return;
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
