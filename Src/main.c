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
#include "cmsis_os.h"
#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "dma2d.h"
#include "ltdc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */



uint32_t A0, A1, A2, A3;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

float map(int x, int in_min, int in_max, int out_min, int out_max);
int8_t read_rotary_1();
int8_t read_rotary_2();
int8_t read_rotary_3();
void sendPayload();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



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


int16_t droneBatteryLvl = 3900;

float roll_rx = 0;
float pitch_rx = 0;
float yaw_rx = 0;

float roll_p = 0;
float roll_i = 0;
float roll_d = 0;


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





	//NRF24 Module Initialisation functions






  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
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

/* USER CODE BEGIN 4 */





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


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{


}


/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM14 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM14) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
