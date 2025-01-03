#include "usart.h"
#include <stdio.h>
#include <string.h>

#define GPSDATA 100

float Time = 0.0;
double Latitude = 0.0, Longitude = 0.0;
double old_Latitude = 0.0, old_Longitude = 0.0;
uint8_t GPS_rx[GPSDATA] = {0}; // Receive GPS data
uint8_t GPSdebug = 0;
double LatitudeDMM = 0.0;
double LongitudeDMM = 0.0;
char new_string[40] = {0};
char* ptr;
char* ptr1;
uint8_t checksum = 0;
char NS = 0, EW = 0, Quality = 0;
uint16_t check;

//char *GPS_r = "$GNRMC,113817.20,A,3737.91482,N,12703.28403,E,0.029,,090424,,,A*6D";

double convertToDegrees(double dmm) {
    int degrees = (int)(dmm / 100);
    double minutes = dmm - (degrees * 100);
    return degrees + (minutes / 60);
}

void GPS_Init(void){
     HAL_UART_Receive_DMA(&huart2, GPS_rx, GPSDATA); // Receive GPS data   ex) $NMEA, data1, data2, data3*checksum
}

void GPS_CallBack(void){
//   char* ptr;
   ptr = strstr((char*)GPS_rx,"$GNRMC");
   ptr1 = strstr((char*)GPS_rx,"$GNVTG");
   if(ptr != NULL && ptr1 != NULL && ptr1 > ptr){
	   GPSdebug++;
      int length = ptr1 - ptr;
      strncpy(new_string, ptr, length);
      new_string[length] = '\0';
      if(!strncmp(new_string, "$GNRMC", 6)){
         sscanf(new_string, "$GNRMC,%f,%c,%lf,%c,%lf,%c,%*[^*]*%2hX\r\n",
                      &Time, &Quality, &LatitudeDMM, &NS, &LongitudeDMM, &EW, &check);
         // DMM -> degree
         Latitude = convertToDegrees(LatitudeDMM);
         Longitude = convertToDegrees(LongitudeDMM);
      }
   }
//   for (int i = 1; i < GPSDATA; i++){
//      if (GPS_rx[i] == '*')
//         break;
//      checksum ^= GPS_rx[i];
//   }
//   if (check != checksum){
//      Latitude = old_Latitude;
//      Longitude = old_Longitude;
//   }
//   else
//   {
//      old_Latitude = Latitude;
//      old_Longitude = Longitude;
//   }
   HAL_UART_Receive_DMA(&huart2, GPS_rx, GPSDATA); // Receive GPS data
}




//#include "usart.h"
//#include <stdio.h>
//#include <string.h>
//
//#define GPSDATA 100
//
//float Time = 0.0;
//double Latitude = 0.0, Longitude = 0.0;
//uint8_t GPS_rx[GPSDATA] = {0}; // Receive GPS data
//uint8_t GPSdebug = 0;
//double LatitudeDMM = 0.0;
//double LongitudeDMM = 0.0;
//char new_string[80] = {0};
//
//uint8_t checksum = 0;
//char NS = 0, EW = 0, Quality = 0;
//uint16_t check;
//
////char *GPS_r = "$GNRMC,113817.20,A,3737.91482,N,12703.28403,E,0.029,,090424,,,A*6D";
//
//double convertToDegrees(double dmm) {
//    int degrees = (int)(dmm / 100);
//    double minutes = dmm - (degrees * 100);
//    return degrees + (minutes / 60);
//}
//
//void GPS_Init(void){
//     HAL_UART_Receive_DMA(&huart2, GPS_rx, GPSDATA); // Receive GPS data   ex) $NMEA, data1, data2, data3*checksum
//}
//
//void GPS_CallBack(void){
//	GPSdebug++;
//	char* ptr;
//	ptr = strstr((char*)GPS_rx,"$GNRMC");
//
//	if(ptr != NULL){
//		strcpy(new_string, ptr);
//		if(!strncmp(new_string, "$GNRMC", 6)){
//			sscanf(new_string, "$GNRMC,%f,%c,%lf,%c,%lf,%c,%*[^*]*%2hX\r\n",
//							 &Time, &Quality, &LatitudeDMM, &NS, &LongitudeDMM, &EW, &check);
//			// DMM -> degree
//			Latitude = convertToDegrees(LatitudeDMM);
//			Longitude = convertToDegrees(LongitudeDMM);
//		}
//	}
////   for (int i = 1; i < GPSDATA; i++){
////      if (GPS_rx[i] == '*')
////         break;
////      checksum ^= GPS_rx[i];
////   }
////   if (check != checksum){
////      Latitude = old_Latitude;
////      Longitude = old_Longitude;
////   }
////   else
////   {
////      old_Latitude = Latitude;
////      old_Longitude = Longitude;
////   }
//   HAL_UART_Receive_DMA(&huart2, GPS_rx, GPSDATA); // Receive GPS data
//}
//
