/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "crc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "IMU.h"
#include "GPS.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define GPSDATA 100
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// millis time
uint32_t millis_count = 0;
// IMU data
extern float roll, pitch, yaw, ax, ay, az, lx, ly, lz;
// GPS
extern double Latitude, Longitude;
extern float Time; // test
extern uint8_t GPS_rx[GPSDATA];
extern char Quality;
char pubData[150] = {0};
extern int imu_stat;
uint8_t debug = 0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId TxTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void Tim_init();
uint32_t millis(void);
/* USER CODE END FunctionPrototypes */

void StartTxTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  Tim_init();

  /* USER CODE END Init */

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
  /* definition and creation of TxTask */
  osThreadDef(TxTask, StartTxTask, osPriorityNormal, 0, 512);
  TxTaskHandle = osThreadCreate(osThread(TxTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartTxTask */
/**
  * @brief  Function implementing the TxTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTxTask */
void StartTxTask(void const * argument)
{
  /* USER CODE BEGIN StartTxTask */
	IMU_Init();
	GPS_Init();
  /* Infinite loop */
  for(;;)
  {
//	  debug++;

//	if((roll != 0.0 && roll <= 0.00000001 && roll >= -0.00000001)
//	&& (pitch != 0.0 &&  pitch <= 0.00000001 &&  pitch >= -0.00000001)
//	&& (yaw != 0.0 && yaw <= 0.00000001 && yaw >= -0.00000001)){
////		if(roll != 0.0 && imu_stat != 9){
////			NVIC_SystemReset();
////		IMU_Init();
//		IMU_CallBack();
////		}
//	}
// 	  else if (Latitude != 0.0 && Longitude != 0.0){
		  snprintf(pubData, sizeof(pubData),"*%f,%f,%f,%f,%f,%f,%f,%f,%f,%lf,%lf\n", roll, pitch, yaw, ax, ay, az, lx, ly, lz, Latitude, Longitude); // real data
	//	  snprintf(pubData,sizeof(pubData),"*%f,%f,%f,%f,%f,%f,%f,%f,%f,%lf,%c\n", roll, pitch, yaw, ax, ay, az, lx, ly, lz, Latitude, Quality); // test data
		  HAL_UART_Transmit(&huart4, (uint8_t*)pubData, sizeof(pubData), HAL_MAX_DELAY);
		  memset(pubData, 0, sizeof(pubData));
//	  }
  }
  /* USER CODE END StartTxTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void Tim_init(){
  HAL_TIM_Base_Start_IT(&htim2);
}

uint32_t millis(void)
{
  return millis_count;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1) IMU_CallBack();
	else if(huart->Instance == USART2) GPS_CallBack();
}

/* USER CODE END Application */
