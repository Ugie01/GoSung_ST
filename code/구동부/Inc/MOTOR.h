#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "stdint.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void readL_F_HallSensors();
void readL_B_HallSensors();
void readR_F_HallSensors();
void readR_B_HallSensors();
void Motor_Setting(uint8_t *uvw, uint32_t *previous_Millis, bool *state,uint8_t *array);
void Motor_Emergency(uint8_t *m, uint8_t *array,bool *sstate,uint8_t *uvw, bool *F_R, short *velocity, uint16_t *speed_PWM);
void Motor_Drive(bool *F_R, short *input_velocity, short *velocity, short *last_velocity, uint16_t *speed_pwm, uint16_t *last_pwm, uint32_t *last_time);
bool breakControl(bool brk);
void F_RControl(bool Left,bool Right);
#endif /* INC_MOTER_H_ */
