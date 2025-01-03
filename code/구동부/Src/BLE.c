#include "usart.h"
#include <stdio.h>
#include <string.h>

uint8_t receivedData = 0;

short L_velocity = 0, R_velocity = 0, ble_mode = 0;
uint8_t BLEdebug = 0;

void BLE_Init(void){
	HAL_UART_Receive_IT(&huart3, &receivedData, sizeof(receivedData));
}
void BLE_CallBack(void){
	BLEdebug++;
	static uint8_t Bluetoothcnt = 0;
	static char Bluetoothdata[13] = {0};

	if(receivedData == '*') {
		Bluetoothcnt = 0;
		memset(Bluetoothdata, 0, sizeof(Bluetoothdata));
	}
	else if (receivedData != '\n') Bluetoothdata[Bluetoothcnt++] = (char)receivedData;
	else if (receivedData == '\n') sscanf(Bluetoothdata,"%hd,%hd,%hd",&ble_mode, &L_velocity, &R_velocity);

	HAL_UART_Receive_IT(&huart3, &receivedData, sizeof(receivedData));
}

