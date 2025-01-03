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
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "i2c.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "SRF08.h"
#include "BLE.h"
#include "MOTOR.h"
#include "ROS.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// millis time
uint32_t millis_count = 0;
// SRF-08
extern uint16_t SRF08_Distance1, SRF08_Distance2;
bool SRF = false;
// MOTOR
short velocityL_F = 0, velocityL_B = 0, velocityR_F = 0, velocityR_B = 0; // motor velocity
extern int8_t enc1, enc2, enc3, enc4; // motor encoder
uint16_t pwmL_F = 0, pwmL_B = 0, pwmR_F = 0, pwmR_B = 0; // PWM
extern uint8_t uvw1;
// BLE
extern short L_velocity, R_velocity, mode, ble_mode, ros_mode;
extern bool F_R_L, F_R_R; // direction
extern short ROS_L_speed, ROS_R_speed;
uint8_t debug = 0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId RxTaskHandle;
osThreadId SRF08TaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void Tim_init();
uint32_t millis(void);
/* USER CODE END FunctionPrototypes */

void StartRxTask(void const * argument);
void StartSRF08Task(void const * argument);

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
  /* definition and creation of RxTask */
  osThreadDef(RxTask, StartRxTask, osPriorityNormal, 0, 1024);
  RxTaskHandle = osThreadCreate(osThread(RxTask), NULL);

  /* definition and creation of SRF08Task */
  osThreadDef(SRF08Task, StartSRF08Task, osPriorityNormal, 0, 1024);
  SRF08TaskHandle = osThreadCreate(osThread(SRF08Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartRxTask */
/**
* @brief Function implementing the RxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRxTask */
void StartRxTask(void const * argument)
{
  /* USER CODE BEGIN StartRxTask */
	osDelay(300);
	BLE_Init();
	ROS_Init();
	bool state[4] = {true, true, true, true};
	uint8_t array[4] = {0};
	uint8_t m = 0;
	uint32_t previousMillis2_2 = 0;
	uint16_t last_pwmL_F = 0, last_pwmL_B = 0, last_pwmR_F = 0, last_pwmR_B = 0; // last_pwm
	uint32_t last_timeL_F = 0, last_timeL_B = 0, last_timeR_F = 0, last_timeR_B = 0; // last_time
	short last_velocityL_F = 0, last_velocityL_B = 0, last_velocityR_F = 0, last_velocityR_B = 0; // motor velocity
	short mode = 0;
   /* Infinite loop */
  for(;;)
  {
	 if(ble_mode == 2 || ble_mode == 1) mode = 2;
	 else if(ble_mode == 4) mode = 4;
	 else if(ros_mode == 3 && ble_mode != 4) mode = 3;

     switch (mode){
     	case 0:
			breakControl(0);
        case 2: // brk
        	if(!SRF){
				uint32_t Millis2 = 0;
				static uint32_t previousMillis2 = 0;
				if(state[0]){
					pwmL_F = pwmR_F = pwmL_B = pwmR_B = 0;
					breakControl(0);
					previousMillis2 = 0;
					state[0] = false;
				}

				Millis2 = millis();
				if(Millis2 - previousMillis2 >= 5){
					if(state[1]){
						Motor_Setting(&uvw1, &previousMillis2, state, array);
						previousMillis2_2 = Millis2;
					}
					else if(!state[1]){
						if(Millis2 - previousMillis2_2 >= 1000){
							Motor_Emergency(&m, array, state, &uvw1 ,&F_R_L, &velocityL_F, &pwmL_F);
							pwmR_F = pwmL_B = pwmR_B = pwmL_F;
						}
					}
					previousMillis2 = Millis2;
				}
        	}
			break;

        case 3: // jadong
        	uint32_t Millis3 = 0;
			static uint32_t previousMillis3 = 0;
			Millis3 = millis();
			if (Millis3 - previousMillis3 >= 80 && !SRF) {
				breakControl(1);
				F_RControl((ROS_L_speed == 0 ? F_R_L : ROS_L_speed > 0), (ROS_R_speed == 0 ? F_R_R : ROS_R_speed > 0));

				Motor_Drive(&F_R_L, &ROS_L_speed, &velocityL_F, &last_velocityL_F, &pwmL_F, &last_pwmL_F, &last_timeL_F);
				Motor_Drive(&F_R_R, &ROS_R_speed, &velocityR_F, &last_velocityR_F, &pwmR_F, &last_pwmR_F, &last_timeR_F);
				Motor_Drive(&F_R_L, &ROS_L_speed, &velocityL_B, &last_velocityL_B, &pwmL_B, &last_pwmL_B, &last_timeL_B);
				Motor_Drive(&F_R_R, &ROS_R_speed, &velocityR_B, &last_velocityR_B, &pwmR_B, &last_pwmR_B, &last_timeR_B);

				previousMillis3 = Millis3;
				m = 0;
				state[0] = state[1] = state[2] = state[3] = true;
				array[0] = array[1] = array[2] = array[3] = 0;
			}
			break;

        case 4: // soodong
			uint32_t Millis4 = 0;
			static uint32_t previousMillis4 = 0;
			Millis4 = millis();
			if (Millis4 - previousMillis4 >= 80 && !SRF) {
				breakControl(1);
				F_RControl((L_velocity == 0 ? F_R_L : L_velocity > 0), (R_velocity == 0 ? F_R_R : R_velocity > 0));

				Motor_Drive(&F_R_L, &L_velocity, &velocityL_F, &last_velocityL_F, &pwmL_F, &last_pwmL_F, &last_timeL_F);
				Motor_Drive(&F_R_R, &R_velocity, &velocityR_F, &last_velocityR_F, &pwmR_F, &last_pwmR_F, &last_timeR_F);
				Motor_Drive(&F_R_L, &L_velocity, &velocityL_B, &last_velocityL_B, &pwmL_B, &last_pwmL_B, &last_timeL_B);
				Motor_Drive(&F_R_R, &R_velocity, &velocityR_B, &last_velocityR_B, &pwmR_B, &last_pwmR_B, &last_timeR_B);

				previousMillis4 = Millis4;
				m = 0;
				state[0] = state[1] = state[2] = state[3] = true;
				array[0] = array[1] = array[2] = array[3] = 0;
			}
			break;
     }
  }
  /* USER CODE END StartRxTask */
}

/* USER CODE BEGIN Header_StartSRF08Task */
/**
* @brief Function implementing the SRF08Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSRF08Task */
void StartSRF08Task(void const * argument)
{
  /* USER CODE BEGIN StartSRF08Task */
	osDelay(1000);
	bool state[4] = {true, true, true, true};
	uint8_t array[4] = {0};
	uint8_t m = 0;
	uint32_t previousMillis_SRF_2 = 0;

//	change address
//	change_SRF08_I2C_address();
	/* Infinite loop */
  for(;;)
  {
	debug++;
    SRF08_READ(); // -> delay
    if((SRF08_Distance1 > 0 && SRF08_Distance1 <= 30) || (SRF08_Distance2 > 0 && SRF08_Distance2 <= 30)) {
        uint32_t Millis_SRF = 0;
		static uint32_t previousMillis_SRF = 0;
		if(state[0]){
			pwmL_F = pwmR_F = pwmL_B = pwmR_B = 0;
			breakControl(0);
			previousMillis_SRF = 0;
			state[0] = false;
		}

		Millis_SRF = millis();
		if(Millis_SRF - previousMillis_SRF >= 5){
			if(state[1]){
				Motor_Setting(&uvw1, &previousMillis_SRF, state, array);
				previousMillis_SRF_2 = Millis_SRF;
			}
			else if(!state[1]){
				if(Millis_SRF - previousMillis_SRF_2 >= 1000){
					Motor_Emergency(&m, array, state, &uvw1 ,&F_R_L, &velocityL_F, &pwmL_F);
					pwmR_F = pwmL_B = pwmR_B = pwmL_F;
				}
			}
			previousMillis_SRF = Millis_SRF;
		}
        SRF = true;
    }
	else {
		m = 0;
		state[0] = state[1] = state[2] = state[3] = true;
		array[0] = array[1] = array[2] = array[3] = 0;
		SRF = false;
	}

	uint32_t Millis = 0;
	static uint32_t previousMillis = 0;
	Millis = millis();
	if (Millis - previousMillis >= 800) {
		char pubData[30] = {0};
//		snprintf(pubData,sizeof(pubData),"*%hd,%hd,%hd,%hd\n", velocityL_F, velocityL_B, velocityR_F, velocityR_B); // velo data
		snprintf(pubData,sizeof(pubData),"*%hu,%hu,%hu,%hu\n", pwmL_F, pwmL_B, pwmR_F, pwmR_B); // pwm data
		HAL_UART_Transmit(&huart3, (uint8_t*)pubData, sizeof(pubData), HAL_MAX_DELAY);
		previousMillis = Millis;
	}
  }
  /* USER CODE END StartSRF08Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void Tim_init(){
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim4);
  HAL_TIM_Base_Start_IT(&htim5);
  HAL_TIM_Base_Start_IT(&htim9);

  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
}

uint32_t millis(void)
{
	return millis_count;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART3) BLE_CallBack();
	else if (huart->Instance == USART1) ROS_CallBack();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
   if(htim->Instance == TIM5)
   {
      static uint8_t cnt;
      if(cnt == 100){
         velocityL_F = (short)((enc1 * (785 / 90)) * 10);
         velocityL_B = (short)((enc2 * (785 / 90)) * 10);
         velocityR_F = (short)((enc3 * (785 / 90)) * 10);
         velocityR_B = (short)((enc4 * (785 / 90)) * 10);

         enc1 = enc2 = enc3 = enc4 = 0;
         cnt = 0;
      }
      if (pwmL_F > 2000) pwmL_F = 2000;
      else if (pwmL_F < 0) pwmL_F = 0;
      if (pwmL_B > 2000) pwmL_B = 2000;
      else if (pwmL_B < 0) pwmL_B = 0;
      if (pwmR_F > 2000) pwmR_F = 2000;
      else if (pwmR_F < 0) pwmR_F = 0;
      if (pwmR_B > 2000) pwmR_B = 2000;
      else if (pwmR_B < 0) pwmR_B = 0;
	  htim3.Instance -> CCR1 = pwmL_F;
	  htim3.Instance -> CCR2 = pwmL_B;
	  htim4.Instance -> CCR3 = pwmR_F;
	  htim9.Instance -> CCR1 = pwmR_B;
      cnt++;
   }
   else if (htim->Instance == TIM2) millis_count++;
   else if (htim->Instance == TIM14) HAL_IncTick();

}
/* USER CODE END Application */
