#include "main.h"
#include "tim.h"
#include "gpio.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "MOTOR.h"

int8_t enc1 = 0, enc2 = 0, enc3 = 0, enc4 = 0;
bool F_R_L = true, F_R_R = true; // direction
uint8_t uvw1 = 0, uvw2 = 0, uvw3 = 0, uvw4 = 0;

uint32_t millis();

void Motor_Setting(uint8_t *uvw, uint32_t *previous_Millis, bool *state, uint8_t *array){
	if(state[1] && *previous_Millis != 0){
		array[0] = *uvw;
	 	state[1] = false;
	}
}

void Motor_Emergency(uint8_t *m, uint8_t *array, bool *state, uint8_t *uvw, bool *F_R, short *velocity, uint16_t *speed_PWM){
	if(*velocity <= -80 || *velocity >= 80){
		if (array[0] != *uvw && array[1] == 0) array[1] = *uvw;
	    else if (array[1] != *uvw && array[1] != 0 && array[2] == 0) array[2] = *uvw;
	    else if (array[2] != *uvw && array[1] != 0 && array[2] != 0 && array[3] == 0){
			array[3] = *uvw;
			state[2] = false;
	    }

	    if(state[2] == false){
			if((array[0] - array[1] == -1 && array[2] - array[3] == -1) || (array[0] - array[1] == -2 && array[2] - array[3] == -2)
			|| (array[0] - array[1] == 3 && array[2] - array[3] == 3) || (array[1] - array[2] == -2) || (array[1] - array[2] == 3)
			|| (array[1] - array[2] == -1)) *m = 1;

			else if((array[0] - array[1] == 1 && array[2] - array[3] == 1) || (array[0] - array[1] == 2 && array[2] - array[3] == 2)
				 || (array[0] - array[1] == -3 && array[2] - array[3] == -3) ||(array[1] - array[2] == 2) || (array[1] - array[2] == -3)
				 || (array[1] - array[2] == 1)) *m = 2;

			state[2] = true;
		}

		uint32_t Millis = 0;
		static uint32_t previousMillis = 0;

		Millis = millis();
		if(Millis - previousMillis >= 100 && state[2] == true){
			if (*m == 1){ // back
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, 1); // no brk
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 0);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);

				if (*velocity > 0 && *speed_PWM < 100) *speed_PWM += 5;
				else if (*velocity < 0 && *speed_PWM > 0) *speed_PWM -= 5;
			}
			else if (*m == 2){ // go
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, 1);  // no brk
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 1);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 0);

				if (*velocity < 0 && *speed_PWM < 100) *speed_PWM += 5;
				else if (*velocity > 0 && *speed_PWM > 0) *speed_PWM -= 5;
			}
			previousMillis = Millis;
		}
	}
}

				// 방향, 입력 속도, 현재 속도, 전 속도, 현재 pwm, 전 pwm, 전 시간
void Motor_Drive(bool *F_R, short *input_velocity, short *velocity, short *last_velocity, uint16_t *speed_pwm, uint16_t *last_pwm, uint32_t *last_time){
	const uint16_t pwm_limit = 2000;        			// PWM 최대값
	const uint8_t pwm_min_limit = 0;       				// PWM 최소값
	const uint8_t pwm_max_step = 10;       				// PWM 최대 증가/감소량 (1주기당)
	const uint8_t velocity_error_limit = 120; 			// 속도 오차 허용 범위
	const uint8_t pwm_change_limit = 50;  				// 한 주기(1.0초)동안의 최대 PWM 변화량

//	if(((*F_R && *input_velocity - velocity_error_limit > *velocity) || (!*F_R && *input_velocity + velocity_error_limit < *velocity)) && *speed_pwm < pwm_limit)
//		*speed_pwm += pwm_max_step;
//	else if (((*F_R && *input_velocity + velocity_error_limit < *velocity) || (!*F_R && *input_velocity - velocity_error_limit > *velocity)) && *speed_pwm > pwm_min_limit)
//		*speed_pwm -= pwm_max_step;
//
//	uint32_t Millis = 0;
//	Millis = millis();
//	if(Millis - *last_time >= 1000){
//		uint16_t pwm_change = *speed_pwm - *last_pwm;  // PWM 변화량 계산
//		// 입력 속도와 그 전 입력속도차이가 +- 120 이면서 그 전 pwm 값이 많이 올라갈경우
//		if((*input_velocity <= *last_velocity + 20 && *input_velocity >= *last_velocity - 20)
//		&& *input_velocity + velocity_error_limit < *velocity && pwm_change > pwm_change_limit && *speed_pwm > *last_pwm)
//			*speed_pwm -= 100; 	// PWM 값을 줄여 급발진 방지
//		*last_time = Millis; 	// 마지막 시간 저장
//	}
//
//    if (*speed_pwm > pwm_limit) *speed_pwm = pwm_limit;
//    else if (*speed_pwm < pwm_min_limit) *speed_pwm = pwm_min_limit;
//    *last_pwm = *speed_pwm;		// 마지막 pwm을 저장
//    *last_velocity = *input_velocity; // 마지막 속도를 저장

//    // v1 이거 먼저 해보기 원본임
	if(((*F_R && *input_velocity - velocity_error_limit > *velocity) || (!*F_R && *input_velocity + velocity_error_limit < *velocity)) && *speed_pwm < pwm_limit) *speed_pwm += pwm_max_step;
	//	else if (*speed - 120 < *velocity && *speed + 120 > *velocity){}
	else if (((*F_R && *input_velocity + velocity_error_limit < *velocity) || (!*F_R && *input_velocity - velocity_error_limit > *velocity)) && *speed_pwm > pwm_min_limit) *speed_pwm -= pwm_max_step;

//	v2
//	if(((*F_R && *speed - 120 > *velocity) || (!*F_R && *speed + 120 < *velocity)) && *speed_PWM < 2000) *speed_PWM += 10;
////	else if (*speed - 120 < *velocity && *speed + 120 > *velocity){}
//	else if (((*F_R && *speed + 120 < *velocity) || (!*F_R && *speed - 120 > *velocity)) && *speed_PWM > 20) {
//		*speed_PWM -= 20;
//		if (*speed_PWM == 10 && *speed_PWM == 20) *speed_PWM = 0;
//	}

	// v3
//	if(((*F_R && *speed - 120 > *velocity) || (!*F_R && *speed + 120 < *velocity)) && *speed_PWM < 2000) *speed_PWM += 10;
////	else if (*speed - 120 < *velocity && *speed + 120 > *velocity){}
//	else if (((*F_R && *speed + 120 < *velocity) || (!*F_R && *speed - 120 > *velocity)) && *speed_PWM > 0) {
//		*speed_PWM -= 20;
//		if (*speed_PWM > 2500) *speed_PWM = 0;
//	}

//	 v4
//	if(((*F_R && *input_velocity - 120 > *velocity) || (!*F_R && *input_velocity + 120 < *velocity)) && *speed_PWM < 2000)
//		*speed_PWM += 10;
//	else if (((*F_R && *input_velocity + 120 < *velocity) || (!*F_R && *input_velocity - 120 > *velocity)) && *speed_PWM > 0)
//		*speed_PWM -= 30;
//	if (*speed_PWM > 2000) *speed_PWM = 2000;
//	if (*speed_PWM < 0) *speed_PWM = 0;
}

void readL_F_HallSensors()
{
	uint8_t u = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9);
	uint8_t v = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_10);
	uint8_t w = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11);
	static uint8_t old_uvw1;
	uvw1 = (u << 2) + (v << 1) + w;

	if(F_R_L && uvw1 != old_uvw1) enc1++;
	else if(!F_R_L && uvw1 != old_uvw1) enc1--;

	old_uvw1 = uvw1;
}

void readL_B_HallSensors()
{
	uint8_t u = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3);
	uint8_t v = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7);
	uint8_t w = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4);
	static uint8_t old_uvw2;
	uvw2 = (u << 2) + (v << 1) + w;

	if(F_R_L && uvw2 != old_uvw2) enc2++;
	else if(!F_R_L && uvw2 != old_uvw2) enc2--;

	old_uvw2 = uvw2;
}

void readR_F_HallSensors()
{
	uint8_t u = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0);
	uint8_t v = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1);
	uint8_t w = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2);
	static uint8_t old_uvw3;
	uvw3 = (u << 2) + (v << 1) + w;

	if(F_R_R && uvw3 != old_uvw3) enc3++;
	else if(!F_R_R && uvw3 != old_uvw3) enc3--;

	old_uvw3 = uvw3;
}

void readR_B_HallSensors()
{
	uint8_t u = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6);
	uint8_t v = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);
	uint8_t w = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5);
	static uint8_t old_uvw4;
	uvw4 = (u << 2) + (v << 1) + w;

	if(F_R_R && uvw4 != old_uvw4) enc4++;
	else if(!F_R_R && uvw4 != old_uvw4) enc4--;

	old_uvw4 = uvw4;
}

bool breakControl(bool brk){ // 0 = brk on
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, brk);
	return brk;
}

void F_RControl(bool Left, bool Right){ // 1, 1
	F_R_R = Right;
	F_R_L = Left;
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, Right); // Right
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, !Left); // Left
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	/* Prevent unused argument(s) compilation warning */
	if(GPIO_Pin == GPIO_PIN_9 || GPIO_Pin == GPIO_PIN_10 || GPIO_Pin == GPIO_PIN_11) readL_F_HallSensors();
	else if(GPIO_Pin == GPIO_PIN_3 || GPIO_Pin == GPIO_PIN_7 || GPIO_Pin == GPIO_PIN_4) readL_B_HallSensors();
	else if(GPIO_Pin == GPIO_PIN_0 || GPIO_Pin == GPIO_PIN_1 || GPIO_Pin == GPIO_PIN_2) readR_F_HallSensors();
	else if(GPIO_Pin == GPIO_PIN_6 || GPIO_Pin == GPIO_PIN_8 || GPIO_Pin == GPIO_PIN_5) readR_B_HallSensors();
}
