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
#include "../GUI/GUI.h"
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
osMutexId EncDataMutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
int8_t read_rotary_1();
int8_t read_rotary_2();
int8_t read_rotary_3();

static uint8_t prevNextCode_1 = 0;
static uint16_t store_1 = 0;

static uint8_t prevNextCode_2 = 0;
static uint16_t store_2 = 0;

static uint8_t prevNextCode_3 = 0;
static uint16_t store_3 = 0;

//Which axis is currently being tuned
// 0 = Roll, 3 = Pitch, 6 = Yaw
uint8_t tune_axis = 0;
float enc_pid[6] = { 0.00, 0.000, 0.000, 0.00, 0.000, 0.000 };

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

  /* definition and creation of EncDataMutex */
  osMutexDef(EncDataMutex);
  EncDataMutexHandle = osMutexCreate(osMutex(EncDataMutex));

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
  osThreadDef(DrawUI, StartDrawUI, osPriorityNormal, 0, 1024);
  DrawUIHandle = osThreadCreate(osThread(DrawUI), NULL);

  /* definition and creation of NRFTX */
  osThreadDef(NRFTX, startNRFTX, osPriorityRealtime, 0, 512);
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
	xSemaphoreTake(RxDataMutexHandle, portMAX_DELAY);
	initLCD(GPS);
	xSemaphoreGive(RxDataMutexHandle);

	/* Infinite loop */
	for (;;) {
		xSemaphoreTake(RxDataMutexHandle, portMAX_DELAY);
		xSemaphoreTake(SwDataMutexHandle, portMAX_DELAY);
		xSemaphoreTake(EncDataMutexHandle, portMAX_DELAY);
		drawUI(GPS, IMU, Misc, enc_pid, &tune_axis);
		xSemaphoreGive(RxDataMutexHandle);
		xSemaphoreGive(SwDataMutexHandle);
		xSemaphoreGive(EncDataMutexHandle);
		osDelay(2);
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

	Misc.kill = 0;
	Misc.airmode = 0;
	xSemaphoreTake(RxDataMutexHandle, portMAX_DELAY);
	NRF24_init(GPS);
	xSemaphoreGive(RxDataMutexHandle);
	/* Infinite loop */
	for (;;) {

		xSemaphoreTake(SwDataMutexHandle, portMAX_DELAY);
		xSemaphoreTake(EncDataMutexHandle, portMAX_DELAY);
		packData(adcArray, Misc.airmode, Misc.kill, enc_pid);
		xSemaphoreGive(SwDataMutexHandle);
		xSemaphoreGive(EncDataMutexHandle);

		xSemaphoreTake(RxDataMutexHandle, portMAX_DELAY);
		Misc.connection = sendPayload();
		xSemaphoreGive(RxDataMutexHandle);

		osDelay(5);
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

//GPIO interrupt callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	//Encoder 1 Pin A interrupt
	if (GPIO_Pin == GPIO_PIN_7) {
		if (read_rotary_1()) {

			if (prevNextCode_1 == 0x0b) {
				if (enc_pid[tune_axis + 2] > 0) {
					enc_pid[tune_axis + 2] -= 0.1;
				}
			}
			if (prevNextCode_1 == 0x07) {
				enc_pid[tune_axis + 2] += 0.1;
			}
		}
	}
	//Encoder 2 Pin A interrupt
	if (GPIO_Pin == GPIO_PIN_4) {
		if (read_rotary_2()) {

			if (prevNextCode_2 == 0x0b) {
				if (enc_pid[tune_axis + 1] > 0) {
					enc_pid[tune_axis + 1] -= 0.1;
				}
			}

			if (prevNextCode_2 == 0x07) {
				enc_pid[tune_axis + 1] += 0.1;
			}
		}
	}
	//Encoder 3 Pin A interrupt
	if (GPIO_Pin == GPIO_PIN_6) {

		if (read_rotary_3()) {

			if (prevNextCode_3 == 0x0b) {
				if (enc_pid[tune_axis + 0] > 0) {
					enc_pid[tune_axis + 0] -= 0.5;
				}
			}
			if (prevNextCode_3 == 0x07) {
				enc_pid[tune_axis + 0] += 0.5;
			}
		}
	}

}

// A vald CW or  CCW move returns 1, invalid returns 0.
int8_t read_rotary_1() {
	static int8_t rot_enc_table[] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1,
			1, 0 };

	prevNextCode_1 <<= 2;

	if (HAL_GPIO_ReadPin(GPIOF, ENC1_A_Pin) == GPIO_PIN_SET)
		prevNextCode_1 |= 0x02;
	if (HAL_GPIO_ReadPin(GPIOC, ENC1_B_Pin) == GPIO_PIN_SET)
		prevNextCode_1 |= 0x01;
	prevNextCode_1 &= 0x0f;

	// If valid then store_1 as 16 bit data.
	if (rot_enc_table[prevNextCode_1]) {
		store_1 <<= 4;
		store_1 |= prevNextCode_1;

		if ((store_1 & 0xff) == 0x2b)
			return -1;
		if ((store_1 & 0xff) == 0x17)
			return 1;
	}
	return 0;
}

// A vald CW or  CCW move returns 1, invalid returns 0.
int8_t read_rotary_2() {
	static int8_t rot_enc_table[] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1,
			1, 0 };

	prevNextCode_2 <<= 2;

	if (HAL_GPIO_ReadPin(GPIOB, ENC2_A_Pin) == GPIO_PIN_SET)
		prevNextCode_2 |= 0x02;
	if (HAL_GPIO_ReadPin(GPIOG, ENC2_B_Pin) == GPIO_PIN_SET)
		prevNextCode_2 |= 0x01;
	prevNextCode_2 &= 0x0f;

	// If valid then store_2 as 16 bit data.
	if (rot_enc_table[prevNextCode_2]) {
		store_2 <<= 4;
		store_2 |= prevNextCode_2;

		if ((store_2 & 0xff) == 0x2b)
			return -1;
		if ((store_2 & 0xff) == 0x17)
			return 1;
	}
	return 0;
}

// A vald CW or  CCW move returns 1, invalid returns 0.
int8_t read_rotary_3() {
	static int8_t rot_enc_table[] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1,
			1, 0 };

	prevNextCode_3 <<= 2;

	if (HAL_GPIO_ReadPin(GPIOH, ENC3_A_Pin) == GPIO_PIN_SET)
		prevNextCode_3 |= 0x02;
	if (HAL_GPIO_ReadPin(GPIOI, ENC3_B_Pin) == GPIO_PIN_SET)
		prevNextCode_3 |= 0x01;
	prevNextCode_3 &= 0x0f;

	// If valid then store_1 as 16 bit data.
	if (rot_enc_table[prevNextCode_3]) {
		store_3 <<= 4;
		store_3 |= prevNextCode_3;

		if ((store_3 & 0xff) == 0x2b)
			return -1;
		if ((store_3 & 0xff) == 0x17)
			return 1;
	}
	return 0;
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
