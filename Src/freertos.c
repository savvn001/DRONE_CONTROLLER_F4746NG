/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "../Drivers/UI.h"
#include "../Drivers/NRF24_Top.h"
#include "adc.h"
#include "tim.h"

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
/* USER CODE BEGIN Variables */
uint32_t adcArray[4];

bool enc1_A;
bool enc1_B;

/* USER CODE END Variables */
osThreadId DrawUIHandle;
osThreadId NRFTXHandle;
osThreadId ADCHandle;
osMutexId RxDataMutexHandle;
osMutexId SwDataMutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDrawUI(void const * argument);
void startNRFTX(void const * argument);
void StartADC(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Create the mutex(es) */
  /* definition and creation of RxDataMutex */
  osMutexDef(RxDataMutex);
  RxDataMutexHandle = osMutexCreate(osMutex(RxDataMutex));

  /* definition and creation of SwDataMutex */
  osMutexDef(SwDataMutex);
  SwDataMutexHandle = osMutexCreate(osMutex(SwDataMutex));

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of DrawUI */
  osThreadDef(DrawUI, StartDrawUI, osPriorityNormal, 0, 128);
  DrawUIHandle = osThreadCreate(osThread(DrawUI), NULL);

  /* definition and creation of NRFTX */
  osThreadDef(NRFTX, startNRFTX, osPriorityNormal, 0, 128);
  NRFTXHandle = osThreadCreate(osThread(NRFTX), NULL);

  /* definition and creation of ADC */
  osThreadDef(ADC, StartADC, osPriorityLow, 0, 128);
  ADCHandle = osThreadCreate(osThread(ADC), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDrawUI */
/**
 * @brief  Function implementing the DrawUI thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDrawUI */
void StartDrawUI(void const * argument)
{

  /* USER CODE BEGIN StartDrawUI */

	initLCD();

	/* Infinite loop */
	for (;;) {
		xSemaphoreTake(RxDataMutexHandle, portMAX_DELAY);
		xSemaphoreTake(SwDataMutexHandle, portMAX_DELAY);
		drawMainScreen(GPS, IMU, Misc);
		xSemaphoreGive(RxDataMutexHandle);
		xSemaphoreGive(SwDataMutexHandle);
		osDelay(20);
	}
  /* USER CODE END StartDrawUI */
}

/* USER CODE BEGIN Header_startNRFTX */
/**
 * @brief Function implementing the NRFTX thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_startNRFTX */
void startNRFTX(void const * argument)
{
  /* USER CODE BEGIN startNRFTX */

	NRF24_init();


	/* Infinite loop */
	for (;;) {

		xSemaphoreTake(SwDataMutexHandle, portMAX_DELAY);
		packData(adcArray, Misc.airmode, Misc.kill);
		xSemaphoreGive(SwDataMutexHandle);
		Misc.connection = sendPayload();

		if (Misc.connection) {

			xSemaphoreTake(RxDataMutexHandle, portMAX_DELAY);
			unpackAckPayload(GPS, IMU);
			xSemaphoreGive(RxDataMutexHandle);

		}

		osDelay(1);
	}
  /* USER CODE END startNRFTX */
}

/* USER CODE BEGIN Header_StartADC */
/**
 * @brief Function implementing the ADC thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartADC */
void StartADC(void const * argument)
{
  /* USER CODE BEGIN StartADC */

	//	//Init ADC for DMA
	if (HAL_ADC_Start(&hadc3) == HAL_OK) {
		HAL_ADC_Start_DMA(&hadc3, (uint32_t *) adcArray, 4);
		HAL_TIM_Base_Start(&htim1);

	}

	/* Infinite loop */
	for (;;) {

		xSemaphoreTake(SwDataMutexHandle, portMAX_DELAY);
		if (HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_2) == GPIO_PIN_SET) {
			Misc.airmode = 1;
		} else {
			Misc.airmode = 0;
		}

		//SW2 - Kill switch
		if (HAL_GPIO_ReadPin(GPIOI, SW2_Pin) == GPIO_PIN_RESET) {
			Misc.kill = 1;
		} else {
			Misc.kill = 0;
		}
		xSemaphoreGive(SwDataMutexHandle);
		osDelay(50);
	}
  /* USER CODE END StartADC */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
