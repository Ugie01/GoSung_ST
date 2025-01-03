#include "i2c.h"
#include "usart.h"
#include <stdbool.h>

// change address
#define SRF08_OLD_ADDRESS 0xE0
#define SRF08_NEW_ADDRESS 0xE2
#define SRF08_1 0xE2
#define SRF08_2 0xE0

uint16_t SRF08_Distance1 = 0;
uint16_t SRF08_Distance2 = 0;
uint8_t sdebug = 0;
uint32_t millis();

void SRF08_READ(void)
{
	uint8_t SRF08_data[5] = {0x00, 0x51, 0x18, 0x25, 0x02};	//command register, cm, range register, analog gain(1025), ?
	uint8_t SRF08_read1[2] = {0};
	uint8_t SRF08_read2[2] = {0};
	uint32_t currentMillis = 0;
	HAL_StatusTypeDef status1, status2;
	static bool time = true;
	static uint32_t previousMillis = 0;

	currentMillis = millis();
	if (time){
		sdebug = 1;
		HAL_I2C_Master_Transmit(&hi2c1, SRF08_1, SRF08_data, sizeof(SRF08_data) - 1, HAL_MAX_DELAY); // command Transmit
		HAL_I2C_Master_Transmit(&hi2c3, SRF08_2, SRF08_data, sizeof(SRF08_data) - 1, HAL_MAX_DELAY); // command Transmit
		time = false;
		sdebug = 10;
	}
	else if(currentMillis - previousMillis >= 68 && !time){
		HAL_I2C_Master_Transmit(&hi2c1, SRF08_1, &SRF08_data[4], sizeof(SRF08_data[4]), HAL_MAX_DELAY);
		HAL_I2C_Master_Transmit(&hi2c3, SRF08_2, &SRF08_data[4], sizeof(SRF08_data[4]), HAL_MAX_DELAY);
		status1 = HAL_I2C_Master_Receive(&hi2c1, SRF08_1, SRF08_read1, sizeof(SRF08_read1), 500); // data Receive
		if(status1 == HAL_OK){
			SRF08_Distance1 = SRF08_read1[1] << 8;
			SRF08_Distance1 |= SRF08_read1[0];
			SRF08_Distance1 = SRF08_Distance1/256; // Receive data -> cm change
		}
		else if(status1 == HAL_ERROR) SRF08_Distance1 = 0;

		status2 = HAL_I2C_Master_Receive(&hi2c3, SRF08_2, SRF08_read2, sizeof(SRF08_read2), 500); // data Receive
		if(status2 == HAL_OK){
			SRF08_Distance2 = SRF08_read2[1] << 8;
			SRF08_Distance2 |= SRF08_read2[0];
			SRF08_Distance2 = SRF08_Distance2/256; // Receive data -> cm change
		}
		else if(status2 == HAL_ERROR)SRF08_Distance2 = 0;
		time = true;
		previousMillis = currentMillis;
	}
}

// change address
void change_SRF08_I2C_address() {
    uint8_t unlock_sequence_1 = 0xA0;
    uint8_t unlock_sequence_2 = 0xAA;
    uint8_t unlock_sequence_3 = 0xA5;

    // Unlock sequence 1
    HAL_I2C_Mem_Write(&hi2c1, SRF08_OLD_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, &unlock_sequence_1, 1, HAL_MAX_DELAY);
    HAL_Delay(10);

    // Unlock sequence 2
    HAL_I2C_Mem_Write(&hi2c1, SRF08_OLD_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, &unlock_sequence_2, 1, HAL_MAX_DELAY);
    HAL_Delay(10);

    // Unlock sequence 3
    HAL_I2C_Mem_Write(&hi2c1, SRF08_OLD_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, &unlock_sequence_3, 1, HAL_MAX_DELAY);
    HAL_Delay(10);

    uint8_t new_address = SRF08_NEW_ADDRESS;
    HAL_I2C_Mem_Write(&hi2c1, SRF08_OLD_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, &new_address, 1, HAL_MAX_DELAY);
    HAL_Delay(10);
}

// --ORG--
//uint8_t SRF08_data1[5] = {0x00, 0x51, 0x18, 0x25, 0x02};	//command register, cm, range register, analog gain(1025), ?
//uint8_t SRF08_read1[2] = {0};
//char buff[50] = {0};

//void SRF08_READ()
//{
//   HAL_I2C_Master_Transmit(&hi2c1, SRF08_1, SRF08_data1, sizeof(SRF08_data1) - 1, HAL_MAX_DELAY); // command Transmit
//   HAL_Delay(65);
//   HAL_I2C_Master_Transmit(&hi2c1, SRF08_1, &SRF08_data1[4], sizeof(SRF08_data1[4]), HAL_MAX_DELAY); // ?
//   HAL_I2C_Master_Receive(&hi2c1, SRF08_1, SRF08_read1, sizeof(SRF08_read1), HAL_MAX_DELAY); // data Receive
//   SRF08_Distance1 = SRF08_read1[1] << 8;
//   SRF08_Distance1 |= SRF08_read1[0];
//   SRF08_Distance1 = SRF08_Distance1/256; // Receive data -> cm change
//
//}

