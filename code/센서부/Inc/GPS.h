
#ifndef INC_GPS_H_
#define INC_GPS_H_

#include <stdio.h>
#include <string.h>
#include <usart.h>
#include "stdint.h"

void GPS_Init(void);
void GPS_CallBack(void);
double convertToDegrees(double dmm);
#endif
