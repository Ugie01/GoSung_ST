/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

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
#define PWM4_Pin GPIO_PIN_5
#define PWM4_GPIO_Port GPIOE
#define u3_Pin GPIO_PIN_0
#define u3_GPIO_Port GPIOC
#define u3_EXTI_IRQn EXTI0_IRQn
#define v3_Pin GPIO_PIN_1
#define v3_GPIO_Port GPIOC
#define v3_EXTI_IRQn EXTI1_IRQn
#define w3_Pin GPIO_PIN_2
#define w3_GPIO_Port GPIOC
#define w3_EXTI_IRQn EXTI2_IRQn
#define u2_Pin GPIO_PIN_3
#define u2_GPIO_Port GPIOC
#define u2_EXTI_IRQn EXTI3_IRQn
#define PWM1_Pin GPIO_PIN_6
#define PWM1_GPIO_Port GPIOA
#define PWM2_Pin GPIO_PIN_7
#define PWM2_GPIO_Port GPIOA
#define w2_Pin GPIO_PIN_4
#define w2_GPIO_Port GPIOC
#define w2_EXTI_IRQn EXTI4_IRQn
#define u1_Pin GPIO_PIN_9
#define u1_GPIO_Port GPIOE
#define u1_EXTI_IRQn EXTI9_5_IRQn
#define v1_Pin GPIO_PIN_10
#define v1_GPIO_Port GPIOE
#define v1_EXTI_IRQn EXTI15_10_IRQn
#define w1_Pin GPIO_PIN_11
#define w1_GPIO_Port GPIOE
#define w1_EXTI_IRQn EXTI15_10_IRQn
#define BRK_Pin GPIO_PIN_14
#define BRK_GPIO_Port GPIOE
#define BT_TX_Pin GPIO_PIN_10
#define BT_TX_GPIO_Port GPIOB
#define BT_RX_Pin GPIO_PIN_11
#define BT_RX_GPIO_Port GPIOB
#define F_R_R_Pin GPIO_PIN_12
#define F_R_R_GPIO_Port GPIOD
#define F_R_L_Pin GPIO_PIN_13
#define F_R_L_GPIO_Port GPIOD
#define PWM3_Pin GPIO_PIN_14
#define PWM3_GPIO_Port GPIOD
#define v2_Pin GPIO_PIN_7
#define v2_GPIO_Port GPIOC
#define v2_EXTI_IRQn EXTI9_5_IRQn
#define SRF_08_SDA_2_Pin GPIO_PIN_9
#define SRF_08_SDA_2_GPIO_Port GPIOC
#define SRF_08_SCL_2_Pin GPIO_PIN_8
#define SRF_08_SCL_2_GPIO_Port GPIOA
#define ROS_TX_Pin GPIO_PIN_9
#define ROS_TX_GPIO_Port GPIOA
#define ROS_RX_Pin GPIO_PIN_10
#define ROS_RX_GPIO_Port GPIOA
#define w4_Pin GPIO_PIN_5
#define w4_GPIO_Port GPIOD
#define w4_EXTI_IRQn EXTI9_5_IRQn
#define u4_Pin GPIO_PIN_6
#define u4_GPIO_Port GPIOD
#define u4_EXTI_IRQn EXTI9_5_IRQn
#define SRF_08_SCL_1_Pin GPIO_PIN_6
#define SRF_08_SCL_1_GPIO_Port GPIOB
#define v4_Pin GPIO_PIN_8
#define v4_GPIO_Port GPIOB
#define v4_EXTI_IRQn EXTI9_5_IRQn
#define SRF_08_SDA_1_Pin GPIO_PIN_9
#define SRF_08_SDA_1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
