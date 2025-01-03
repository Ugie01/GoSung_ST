#include "usart.h"
#include <stdio.h>
#include <string.h>

uint8_t ROS_receivedData = 0;
short ros_mode;
short ROS_L_speed = 0, ROS_R_speed = 0;
uint8_t ddd=0;
void ROS_Init(void){
	HAL_UART_Receive_IT(&huart1, &ROS_receivedData, sizeof(ROS_receivedData));
}
void ROS_CallBack(void){
	ddd++;
	static uint8_t ROScnt = 0;
	static char ROSdata[13] = {0};
	ros_mode = 3;
	if(ROS_receivedData == '*') {
		ROScnt = 0;
		memset(ROSdata, 0, sizeof(ROSdata));
	}
	else if (ROS_receivedData != '\n') ROSdata[ROScnt++] = (char)ROS_receivedData;
	else if (ROS_receivedData == '\n') sscanf(ROSdata,"%hd,%hd",&ROS_L_speed, &ROS_R_speed);

	HAL_UART_Receive_IT(&huart1, &ROS_receivedData, sizeof(ROS_receivedData));
}

