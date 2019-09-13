/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../Drivers/MY_NRF24.h"
#include "../Drivers/dwt_delay.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
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
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc3;

CRC_HandleTypeDef hcrc;

DMA2D_HandleTypeDef hdma2d;

LTDC_HandleTypeDef hltdc;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;

SDRAM_HandleTypeDef hsdram1;

/* USER CODE BEGIN PV */

uint32_t adcArray[4];

uint32_t A0, A1, A2, A3;
bool airmode = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC3_Init(void);
static void MX_CRC_Init(void);
static void MX_DMA2D_Init(void);
static void MX_FMC_Init(void);
static void MX_LTDC_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void packData();
void unpackAckPayload();
float map(int x, int in_min, int in_max, int out_min, int out_max);
int8_t read_rotary_1();
int8_t read_rotary_2();
int8_t read_rotary_3();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//NRF24 Module variables
uint64_t TxpipeAddrs = 0x44332211AA;//0x11223344AA;
char TxData[32];
char AckPayload[32];

int counter = 0;

//Touch screen variables
TS_StateTypeDef ts;

char xTouchStr[10];

//GPS module data struct
typedef struct {

	float speed;
	float longitude;
	float latitude;
	float altitude;
	float course;
	char satellites;
	char day;
	char month;
	char year;
	char second;
	char minute;
	char hour;

} GPS_typedef;

GPS_typedef GPS;


int droneBatteryLvl = 3900;

float roll = 0;
float pitch = 0;
float yaw = 0;

float roll_p = 0;
float roll_i = 0;
float roll_d = 0;

bool enc1_A;
bool enc1_B;

bool connection = 0;
bool cleared = 0;
bool kill = 0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	/* Enable the CPU Cache */
	/* Enable I-Cache */
	SCB_EnableICache();
	/* Enable D-Cache */
	SCB_EnableDCache();
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
  MX_DMA_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_ADC3_Init();
  MX_CRC_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_LTDC_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */


  	//Init ADC for DMA
  if( HAL_ADC_Start(&hadc3) == HAL_OK) {

	//  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adcArray, 4);
	//  HAL_TIM_Base_Start(&htim1);

  }



	//NRF24 Module Initialisation functions


	DWT_Init(); //For uS delays
	NRF24_begin(GPIOA, nrf_CSN_PIN, nrf_CE_PIN, hspi2);
	nrf24_DebugUART_Init(huart1);

	//**** TRANSMIT - ACK ****//
	NRF24_stopListening();
	NRF24_openWritingPipe(TxpipeAddrs);

	printRadioSettings();

	for (int i = 0; i < 31; ++i) {
		TxData[i] = 0;
	}

	//LCD Initialisation functions

//	BSP_SDRAM_Init(); /* Initializes the SDRAM device */
//	__HAL_RCC_CRC_CLK_ENABLE(); /* Enable the CRC Module */
//
//	BSP_TS_Init(480, 272);
//
//	BSP_LCD_Init();
//	BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
//	BSP_LCD_DisplayOn();
//
//	BSP_LCD_SelectLayer(0);
//	BSP_LCD_Clear(LCD_COLOR_BLACK);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


//		////////////////////////////////////////////////////////////////////////////////////////////////////
//		////////////////////////////////////// Get position of switches ////////////////////////////////////
//		////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		//SW1
//		if (HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_2) == GPIO_PIN_SET) {
//			airmode = 1;
//		} else {
//			airmode = 0;
//		}
//
//		//SW2 - Kill switch
//		if (HAL_GPIO_ReadPin(GPIOI, SW2_Pin) == GPIO_PIN_RESET) {
//			kill = 1;
//		} else {
//			kill = 0;
//		}
//		////////////////////////////////////////////////////////////////////////////////////////////////////
//		///////////////////////////////////// NRF24 Module Stuff ///////////////////////////////////////////
//		////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		packData();
//
//		//Transmit payload to drone
//		if (NRF24_write(TxData, 32)) {
//
//			NRF24_read(AckPayload, 32);
//			HAL_UART_Transmit(&huart1,
//					(uint8_t *) "Transmitted Successfully\r\n",
//					strlen("Transmitted Successfully\r\n"), 10);
//
//			char myDataack[32];
//			sprintf(myDataack, "AckPayload:  %s \r\n", AckPayload);
//			HAL_UART_Transmit(&huart1, (uint8_t *) myDataack, strlen(myDataack),
//					10);
//			connection = 1;
//		} else {
//			connection = 0;
//		}
//
//		unpackAckPayload();
//
//		////////////////////////////////////////////////////////////////////////////////////////////////////
//		////////////////////////////////////////// LCD UI //////////////////////////////////////////////////
//		////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		HAL_Delay(2);
//
//		///////////////////////////////// Plotting fixed text //////////////////////////////////////////
//
//		BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
//		BSP_LCD_SetTextColor(LCD_COLOR_RED);
//
//		BSP_LCD_SetFont(&Font16);
//		BSP_LCD_DisplayStringAt(14, 60, (uint8_t *) "LONG.", LEFT_MODE);
//
//		BSP_LCD_DisplayStringAt(14, 100, (uint8_t *) "LAT.", LEFT_MODE);
//
//		BSP_LCD_DisplayStringAt(14, 140, (uint8_t *) "SPEED.", LEFT_MODE);
//
//		BSP_LCD_DisplayStringAt(14, 180, (uint8_t *) "ALT.", LEFT_MODE);
//		//
//		BSP_LCD_DisplayStringAt(14, 220, (uint8_t *) "THROTTLE ", LEFT_MODE);
//
//		BSP_LCD_DisplayStringAt(14, 260, (uint8_t *) "AIRMODE ", LEFT_MODE);
//
//		if (airmode) {
//			BSP_LCD_DisplayStringAt(100, 260, (uint8_t *) "ON ", LEFT_MODE);
//		} else {
//			BSP_LCD_DisplayStringAt(100, 260, (uint8_t *) "OFF", LEFT_MODE);
//		}
//
//		BSP_LCD_DisplayStringAt(20, 60, (uint8_t *) "ROLL", RIGHT_MODE);
//		BSP_LCD_DisplayStringAt(20, 100, (uint8_t *) "PITCH", RIGHT_MODE);
//		BSP_LCD_DisplayStringAt(20, 140, (uint8_t *) "YAW", RIGHT_MODE);
//
//		BSP_LCD_DisplayStringAt(20, 180, (uint8_t *) "P", RIGHT_MODE);
//		BSP_LCD_DisplayStringAt(20, 200, (uint8_t *) "I", RIGHT_MODE);
//		BSP_LCD_DisplayStringAt(20, 220, (uint8_t *) "D", RIGHT_MODE);
//
//		//Top bar text
//		BSP_LCD_DisplayStringAt(120, 15, (uint8_t *) "QUAD", RIGHT_MODE);
//
//		////////////////////////////// Plotting variable text //////////////////////////////////////////
//
//		char longitude_str[6];
//		snprintf(longitude_str, sizeof(longitude_str), "%f", -1.554715);
//		BSP_LCD_DisplayStringAt(100, 60, (uint8_t *) longitude_str, LEFT_MODE);
//
//		char lattitude_str[6];
//		snprintf(lattitude_str, sizeof(lattitude_str), "%f", 53.809404);
//		BSP_LCD_DisplayStringAt(100, 100, (uint8_t *) lattitude_str, LEFT_MODE);
//
//		char speed_str[6];
//		snprintf(speed_str, sizeof(speed_str), "%f", 0.000000);
//		BSP_LCD_DisplayStringAt(100, 140, (uint8_t *) speed_str, LEFT_MODE);
//
//		char alt_str[6];
//		snprintf(alt_str, sizeof(alt_str), "%f", 94.0000000);
//		BSP_LCD_DisplayStringAt(100, 180, (uint8_t *) alt_str, LEFT_MODE);
//
//		int throttle = map(A1, 880, 3300, 0, 100);
//
//		char throttle_str[3];
//		snprintf(throttle_str, sizeof(throttle_str), "%d", throttle);
//		BSP_LCD_DisplayStringAt(140, 220, (uint8_t *) throttle_str, LEFT_MODE);
//
//		char roll_str[3];
//		snprintf(roll_str, sizeof(roll_str), "%f", roll);
//		BSP_LCD_DisplayStringAt(320, 60, (uint8_t *) roll_str, LEFT_MODE);
//
//		char pitch_str[3];
//		snprintf(pitch_str, sizeof(pitch_str), "%f", pitch);
//		BSP_LCD_DisplayStringAt(320, 100, (uint8_t *) pitch_str, LEFT_MODE);
//
//		char yaw_str[3];
//		snprintf(yaw_str, sizeof(yaw_str), "%f", yaw);
//		BSP_LCD_DisplayStringAt(320, 140, (uint8_t *) yaw_str, LEFT_MODE);
//
//		int battery_pct = map(droneBatteryLvl, 3545, 3800, 0, 100);
//		float battery_voltage = map(droneBatteryLvl, 3545, 3800, 12.6, 11.1);
//
//		char battery_lvl_str[3];
//		snprintf(battery_lvl_str, sizeof(battery_lvl_str), "%d", battery_pct);
//		BSP_LCD_DisplayStringAt(85, 15, (uint8_t *) battery_lvl_str,
//				RIGHT_MODE);
//
//		char battery_lvl_str2[5];
//		snprintf(battery_lvl_str2, sizeof(battery_lvl_str2), "%f V",
//				battery_voltage);
//		BSP_LCD_DisplayStringAt(30, 15, (uint8_t *) battery_lvl_str2,
//				RIGHT_MODE);
//
//		//PID
//		char p_str[4];
//		snprintf(p_str, sizeof(p_str), "%f", roll_p);
//		BSP_LCD_DisplayStringAt(40, 180, (uint8_t *) p_str, RIGHT_MODE);
//
//		char i_str[4];
//		snprintf(i_str, sizeof(i_str), "%f", roll_i);
//		BSP_LCD_DisplayStringAt(40, 200, (uint8_t *) i_str, RIGHT_MODE);
//
//		char d_str[4];
//		snprintf(d_str, sizeof(d_str), "%f", roll_d);
//		BSP_LCD_DisplayStringAt(40, 220, (uint8_t *) d_str, RIGHT_MODE);
//
//		if (!connection) {
//
//			BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
//			BSP_LCD_SetTextColor(LCD_COLOR_RED);
//			BSP_LCD_SetFont(&Font24);
//
//			BSP_LCD_DisplayStringAt(0, 120,
//					(uint8_t *) "NO CONNECTION TO DRONE", CENTER_MODE);
//			cleared = 0;
//		} else {
//			if (!cleared) {
//				BSP_LCD_Clear(LCD_COLOR_BLACK);
//			}
//			cleared = 1;
//
//		}

		//		BSP_TS_GetState(&ts);
		//		sprintf(xTouchStr, "X: %3d", ts.touchX[0]);
		//		BSP_LCD_DisplayStringAt(20, 20, (uint8_t *) xTouchStr, LEFT_MODE);
		//
		//		sprintf(xTouchStr, "Y: %3d", ts.touchY[0]);
		//		BSP_LCD_DisplayStringAt(20, 60, (uint8_t *) xTouchStr, LEFT_MODE);

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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_USART1;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = ENABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING;
  hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_TRGO;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 4;
  hadc3.Init.DMAContinuousRequests = ENABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief DMA2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA2D_Init(void)
{

  /* USER CODE BEGIN DMA2D_Init 0 */

  /* USER CODE END DMA2D_Init 0 */

  /* USER CODE BEGIN DMA2D_Init 1 */

  /* USER CODE END DMA2D_Init 1 */
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_M2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0;
  if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DMA2D_Init 2 */

  /* USER CODE END DMA2D_Init 2 */

}

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};
  LTDC_LayerCfgTypeDef pLayerCfg1 = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 7;
  hltdc.Init.VerticalSync = 3;
  hltdc.Init.AccumulatedHBP = 14;
  hltdc.Init.AccumulatedVBP = 5;
  hltdc.Init.AccumulatedActiveW = 654;
  hltdc.Init.AccumulatedActiveH = 485;
  hltdc.Init.TotalWidth = 660;
  hltdc.Init.TotalHeigh = 487;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 0;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 0;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg.Alpha = 0;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0;
  pLayerCfg.ImageWidth = 0;
  pLayerCfg.ImageHeight = 0;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg1.WindowX0 = 0;
  pLayerCfg1.WindowX1 = 0;
  pLayerCfg1.WindowY0 = 0;
  pLayerCfg1.WindowY1 = 0;
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg1.Alpha = 0;
  pLayerCfg1.Alpha0 = 0;
  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg1.FBStartAdress = 0;
  pLayerCfg1.ImageWidth = 0;
  pLayerCfg1.ImageHeight = 0;
  pLayerCfg1.Backcolor.Blue = 0;
  pLayerCfg1.Backcolor.Green = 0;
  pLayerCfg1.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */

  /* USER CODE END LTDC_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 100-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 216-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}
/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_11;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_2;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_1;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_DISABLE;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 16;
  SdramTiming.ExitSelfRefreshDelay = 16;
  SdramTiming.SelfRefreshTime = 16;
  SdramTiming.RowCycleDelay = 16;
  SdramTiming.WriteRecoveryTime = 16;
  SdramTiming.RPDelay = 16;
  SdramTiming.RCDDelay = 16;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, CE_Pin|CSN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : ENC2_A_Pin */
  GPIO_InitStruct.Pin = ENC2_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ENC2_A_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CE_Pin CSN_Pin */
  GPIO_InitStruct.Pin = CE_Pin|CSN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ENC3_B_Pin SW1_Pin SW2_Pin */
  GPIO_InitStruct.Pin = ENC3_B_Pin|SW1_Pin|SW2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pin : ENC1_B_Pin */
  GPIO_InitStruct.Pin = ENC1_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ENC1_B_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ENC2_B_Pin */
  GPIO_InitStruct.Pin = ENC2_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ENC2_B_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ENC1_A_Pin */
  GPIO_InitStruct.Pin = ENC1_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ENC1_A_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ENC3_A_Pin */
  GPIO_InitStruct.Pin = ENC3_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ENC3_A_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */
void packData() {

	//Reverse A2 and A3 as joystick is upside down
	adcArray[2] = 4096 - adcArray[2];
	adcArray[4] = 4096 - adcArray[4];

	//L Joystick X
	//First byte = first byte of A0
	TxData[0] = adcArray[0];
	//Second bytes = last byte of A0
	TxData[1] = adcArray[0] >> 8;

	//L Joystick Y
	TxData[2] = adcArray[3];
	//4th byte = last 8 bits of A1
	TxData[3] = adcArray[3] >> 8;

	//R Joystick X
	TxData[4] = adcArray[2];
	TxData[5] = adcArray[2] >> 8;
	//R Joystick Y
	TxData[6] = adcArray[4];
	TxData[7] = adcArray[4] >> 8;

	//Air mode bit
	if (airmode) {
		TxData[8] |= 1 << 0;
	} else {
		TxData[8] &= ~(1 << 0);
	}

	//Kill switch
	if (kill) {

		TxData[8] |= 1 << 1;
	} else {
		TxData[8] &= ~(1 << 1);
	}


	uint16_t roll_p_tx = round(roll_p * 100);
	uint16_t roll_i_tx = round(roll_i * 100) ;
	uint16_t roll_d_tx = round(roll_d * 100) ;

	TxData[9] = roll_p_tx;
	TxData[10] = roll_p_tx >> 8;

	TxData[11] = roll_i_tx;
	TxData[12] = roll_i_tx >> 8;

	TxData[13] = roll_d_tx;
	TxData[14] = roll_d_tx >> 8;

}

void unpackAckPayload() {

	//Check if this is packet 0 or packet 1
	if (AckPayload[0] == 0x00) {

		droneBatteryLvl = (AckPayload[1] & 0xFF) | (AckPayload[2] << 8);
		int16_t roll_rx = (AckPayload[3] & 0xFF) | (AckPayload[4] << 8);
		roll = roll_rx / 100;


		int16_t pitch_rx = (AckPayload[5] & 0xFF) | (AckPayload[6] << 8);
		pitch = pitch_rx / 100;
	}

	//This is packet 1, only sent every
	if (AckPayload[0] == 0xFF) {

	}

}



float map(int x, int in_min, int in_max, int out_min, int out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


static uint8_t prevNextCode_1 = 0;
static uint16_t store_1=0;

static uint8_t prevNextCode_2 = 0;
static uint16_t store_2=0;


static uint8_t prevNextCode_3 = 0;
static uint16_t store_3=0;


//GPIO interrupt callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	//Encoder 1 Pin A interrupt
	if (GPIO_Pin == GPIO_PIN_7) {


		if(read_rotary_1()){

			if ( prevNextCode_1==0x0b) {
				if (roll_d > 0) {
					roll_d -= 0.5;
				}
			}

			if ( prevNextCode_1==0x07) {
				roll_d += 0.5;
			}
		}

	}

	//Encoder 2 Pin A interrupt
	if (GPIO_Pin == GPIO_PIN_4) {

		if(read_rotary_2()){

			if ( prevNextCode_2==0x0b) {
				if (roll_i > 0) {
					roll_i -= 0.5;
				}
			}

			if ( prevNextCode_2==0x07) {
				roll_i += 0.5;
			}
		}

	}

	//Encoder 3 Pin A interrupt
	if (GPIO_Pin == GPIO_PIN_6) {

		if(read_rotary_3()){

			if ( prevNextCode_3==0x0b) {
				if (roll_p > 0) {
					roll_p -= 0.5;
				}
			}

			if ( prevNextCode_3==0x07) {
				roll_p += 0.5;
			}
		}

	}

}




// A vald CW or  CCW move returns 1, invalid returns 0.
int8_t read_rotary_1() {
	static int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};

	prevNextCode_1 <<= 2;

	if (HAL_GPIO_ReadPin(GPIOF, ENC1_A_Pin) == GPIO_PIN_SET) prevNextCode_1 |= 0x02;
	if (HAL_GPIO_ReadPin(GPIOC, ENC1_B_Pin) == GPIO_PIN_SET) prevNextCode_1 |= 0x01;
	prevNextCode_1 &= 0x0f;

	// If valid then store_1 as 16 bit data.
	if  (rot_enc_table[prevNextCode_1] ) {
		store_1 <<= 4;
		store_1 |= prevNextCode_1;

		if ((store_1&0xff)==0x2b) return -1;
		if ((store_1&0xff)==0x17) return 1;
	}
	return 0;
}


// A vald CW or  CCW move returns 1, invalid returns 0.
int8_t read_rotary_2() {
	static int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};

	prevNextCode_2 <<= 2;

	if (HAL_GPIO_ReadPin(GPIOB, ENC2_A_Pin) == GPIO_PIN_SET) prevNextCode_2 |= 0x02;
	if (HAL_GPIO_ReadPin(GPIOG, ENC2_B_Pin) == GPIO_PIN_SET) prevNextCode_2 |= 0x01;
	prevNextCode_2 &= 0x0f;

	// If valid then store_2 as 16 bit data.
	if  (rot_enc_table[prevNextCode_2] ) {
		store_2 <<= 4;
		store_2 |= prevNextCode_2;

		if ((store_2&0xff)==0x2b) return -1;
		if ((store_2&0xff)==0x17) return 1;
	}
	return 0;
}




// A vald CW or  CCW move returns 1, invalid returns 0.
int8_t read_rotary_3() {
	static int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};

	prevNextCode_3 <<= 2;

	if (HAL_GPIO_ReadPin(GPIOH, ENC3_A_Pin) == GPIO_PIN_SET) prevNextCode_3 |= 0x02;
	if (HAL_GPIO_ReadPin(GPIOI, ENC3_B_Pin) == GPIO_PIN_SET) prevNextCode_3 |= 0x01;
	prevNextCode_3 &= 0x0f;

	// If valid then store_1 as 16 bit data.
	if  (rot_enc_table[prevNextCode_3] ) {
		store_3 <<= 4;
		store_3 |= prevNextCode_3;

		if ((store_3&0xff)==0x2b) return -1;
		if ((store_3&0xff)==0x17) return 1;
	}
	return 0;
}



void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){




}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc){





}

void       HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc){


	if (hadc->Instance == ADC3) {



	    }

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if (htim->Instance == TIM6) {

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
