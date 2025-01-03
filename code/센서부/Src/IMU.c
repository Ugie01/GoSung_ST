#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

uint8_t IMUdata_byte = 0;
float roll = 0.0, pitch = 0.0, yaw = 0.0, ax = 0.0, ay = 0.0, az = 0.0, lx = 0.0, ly = 0.0, lz = 0.0;
uint8_t IMUdata[90] = {0};
uint8_t IMUdebug = 0;
uint8_t cnt = 0;
int imu_stat = 0;
char* ptr2;

char new_string2[90] = {0};

void IMU_Init(void){
	  HAL_UART_Receive_DMA(&huart1, IMUdata, sizeof(IMUdata)); 			// Receive IMU data 	ex)*a,b,c
}

void IMU_CallBack(){
//	IMUdebug++;
	ptr2 = strstr((char*)IMUdata,"\n*");
	if(ptr2 != NULL){
		strncpy(new_string2, ptr2, 90);
		if(!strncmp(new_string2, "\n", 1)){
			sscanf(new_string2, "\n*%f,%f,%f,%f,%f,%f,%f,%f,%f", &roll, &pitch, &yaw, &ax, &ay, &az, &lx, &ly, &lz);
		}
	}
	  HAL_UART_Receive_DMA(&huart1, IMUdata, sizeof(IMUdata)); 			// Receive IMU data 	ex)*a,b,c
}



//#include "usart.h"
//#include <stdio.h>
//#include <string.h>
//#include <math.h>
//
//uint8_t IMUdata_byte = 0;
//float roll = 0.0, pitch = 0.0, yaw = 0.0, ax = 0.0, ay = 0.0, az = 0.0, lx = 0.0, ly = 0.0, lz = 0.0;
//char IMUdata[120] = {0};
//uint8_t IMUdebug = 0;
//uint8_t cnt = 0;
//int imu_stat = 0;
//
//void IMU_Init(void){
//	  HAL_UART_Receive_IT(&huart1, &IMUdata_byte, sizeof(IMUdata_byte)); 			// Receive IMU data 	ex)*a,b,c
//}
//
//void IMU_CallBack(){
////	IMUdebug++;
//	if(IMUdata_byte == '*') {
//		cnt = 0;												// If receive *, start from the beginning.
//		memset(IMUdata, 0, 120);
//	}
//	else if (IMUdata_byte != '\n')
//		IMUdata[cnt++] = (char) IMUdata_byte;			// Receive data (not *)
//	else if (IMUdata_byte == '\n')
//		imu_stat = sscanf(IMUdata, "%f,%f,%f,%f,%f,%f,%f,%f,%f", &roll, &pitch, &yaw, &ax, &ay, &az, &lx, &ly, &lz); // If receive \n, start from the beginning.

//	HAL_UART_Receive_IT(&huart1, &IMUdata_byte, sizeof(IMUdata_byte));				// Receive IMU data 	ex)*a,b,c
//}
