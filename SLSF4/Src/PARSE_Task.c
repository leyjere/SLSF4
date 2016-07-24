#include "fatfs.h"
#include "stm32f4xx_hal.h"
#include "stdlib.h"
#include "string.h"
#include "PARSE_Task.h"
#include "GUI_Task.h"
#include "STEPPER_Intf.h"
#include "main.h"
#include "math.h"
extern osMessageQId queue_handle;
extern I2S_HandleTypeDef hi2s3;
extern UART_HandleTypeDef huart1;
extern osThreadId TS_Handle;

static uint16_t * SDdata;

/* Conversion variables */
char temp[7];
int x, y;
static float zf, zl;
int stepDelay = 1000, stepFact = 10;
volatile float xbase = 288.0f, ybase = 288.0f;
uint32_t pos;
uint16_t speed;

void PARSE_Setup(){
	
	MX_FATFS_Init();
	
	temp[6] = (char)'\0';
	
	SDdata = (uint16_t *)(0xD0000000);
	
	pos = 0;
	
	stopPWM();
	
}

void setDelay(int delay){
	stepFact = delay;
}

int setHeight(int zh){
	
	int len;
	len = tan(0.5235)*zh;
	xbase = (float)len;
	ybase = (float)len;
	return len;
}


void TEST_Task(void * pvParams){
	uint32_t i, j;
	uint16_t data[4];
	
	int test = 0;
	
//	for(test = 0; test < 500; test++){
//	
//	int16_t rx, ry = -327;
//		
//		
//	for(rx = -327; rx < 327; rx++){
//		
//		data[0] = rx*100; data[1] = rx*100;
//		data[2] = ry*100; data[3] = ry*100;
//		
//		HAL_I2S_Transmit(&hi2s3, (uint16_t *)data, 2, 100);
//		for(j = 0; j < stepDelay*stepFact; j++);
//		
//	}
//	
//	for(ry = -327; ry < 327; ry++){
//		
//		data[0] = rx*100; data[1] = rx*100;
//		data[2] = ry*100; data[3] = ry*100;
//		
//		HAL_I2S_Transmit(&hi2s3, (uint16_t *)data, 2, 100);
//		for(j = 0; j < stepDelay*stepFact; j++);
//		
//	}
//	
//	for(rx = 327; rx > -327; rx--){
//		
//		data[0] = rx*100; data[1] = rx*100;
//		data[2] = ry*100; data[3] = ry*100;
//		
//		HAL_I2S_Transmit(&hi2s3, (uint16_t *)data, 2, 100);
//		for(j = 0; j < stepDelay*stepFact; j++);
//		
//	}
//	
//	for(ry = 327; ry > -327; ry--){
//		
//		data[0] = rx*100; data[1] = rx*100;
//		data[2] = ry*100; data[3] = ry*100;
//		
//		HAL_I2S_Transmit(&hi2s3, (uint16_t *)data, 2, 100);
//		for(j = 0; j < stepDelay*stepFact; j++);
//		
//	}
//	
//	}
	 
	double x,y;
	int cx, cy;
	for(x = 0.0; x < 30000.0; x += 0.01){
		y = x; //20 degree correction factor
		cx = (int)(sin(x)*-8388607.0);
		cy = (int)(cos(y)*-8388607.0);
		
		data[0] = ((cx & 0x007FFF00) >> 8) | ((cx & 0x80000000) >> 16);
		
		data[1] = ((cx & 0x000000FF) << 8); 
		
		data[2] = ((cy & 0x007FFF00) >> 8) | ((cy & 0x80000000) >> 16);
		
		data[3] = ((cy & 0x000000FF) << 8);
		
		HAL_I2S_Transmit(&hi2s3, (uint16_t *)data, 2, 100);
		for(j = 0; j < stepDelay; j++);

		
	}

//	int m, rx, ry = 0;
//	for(m = 0; m < 1000; m++){
//	
//	for(rx = 0; rx < 327; rx++){
//		
//		data[0] = rx*100; data[1] = rx*100;
//		data[2] = ry*100; data[3] = ry*100;
//		
//		HAL_I2S_Transmit(&hi2s3, (uint16_t *)data, 2, 100);
//		for(j = 0; j < stepDelay*stepFact; j++);
//		
//	}
//		
//	for(rx = 326; rx > -327; rx--){
//		
//		data[0] = rx*100; data[1] = rx*100;
//		data[2] = ry*100; data[3] = ry*100;
//		
//		HAL_I2S_Transmit(&hi2s3, (uint16_t *)data, 2, 100);
//		for(j = 0; j < stepDelay*stepFact; j++);
//		
//	}
//	
//	for(rx = -327; rx < 0; rx++){
//		
//		data[0] = rx*100; data[1] = rx*100;
//		data[2] = ry*100; data[3] = ry*100;
//		
//		HAL_I2S_Transmit(&hi2s3, (uint16_t *)data, 2, 100);
//		for(j = 0; j < stepDelay*stepFact; j++);
//		
//	}
//	
//	rx = 0;
//	
//	for(ry = 0; ry < 327; ry++){
//		
//		data[0] = rx*100; data[1] = rx*100;
//		data[2] = ry*100; data[3] = ry*100;
//		
//		HAL_I2S_Transmit(&hi2s3, (uint16_t *)data, 2, 100);
//		for(j = 0; j < stepDelay*stepFact; j++);
//		
//	}
//		
//	for(ry = 326; ry > -327; ry--){
//		
//		data[0] = rx*100; data[1] = rx*100;
//		data[2] = ry*100; data[3] = ry*100;
//		
//		HAL_I2S_Transmit(&hi2s3, (uint16_t *)data, 2, 100);
//		for(j = 0; j < stepDelay*stepFact; j++);
//		
//	}
//	
//	for(ry = -327; ry < 0; ry++){
//		
//		data[0] = rx*100; data[1] = rx*100;
//		data[2] = ry*100; data[3] = ry*100;
//		
//		HAL_I2S_Transmit(&hi2s3, (uint16_t *)data, 2, 100);
//		for(j = 0; j < stepDelay*stepFact; j++);
//		
//	}
//	
//	ry = 0;
//				
//			for(o = 0; o < 655; o++){
//				
//				data[0] = (((uint16_t)0 & 0x007FFF00) >> 8) | (((uint16_t)0 & 0x80000000) >> 16);
//		
//				data[1] = (((uint16_t)0 & 0x000000FF) << 8); 
//		
//				data[2] = ((o*100 & 0x007FFF00) >> 8) | ((o*100 & 0x80000000) >> 16);
//		
//				data[3] = ((o*100 & 0x000000FF) << 8);
//				
//				HAL_I2S_Transmit(&hi2s3, (uint16_t *)data, 2, 100);
//				for(j = 0; j < stepDelay; j++);
//			}
		
//	}

	vTaskDelete(NULL);
}


int decode(char * buff){
	
	int start = 0, end = 0;
	int x, y;
	float xf, yf;
	char str[32];
	char * token;
	
	token = strtok(buff, " ");
	
	while(token != NULL){
		
		if(strncmp(token, "M84", 4) == 0){	//Complete
			return 1;
		}
		
		if(strncmp(token, "G1", 3) == 0){
			
			token = strtok(NULL, " ");
			
			if(token[0] == 'Z'){	//Perform current set of instructions
				strncpy(temp, &token[1], 6);
				zf =  (float)atof(temp);
				//Potentially need to account for speed adjustment
				if(pos != 0)
					return 1;
				else 
					return 0;
			}
			
			if(token[0] == 'X'){
				
					strncpy(temp, &token[1], 6);
					xf =  (float)atof(temp);
				
					token = strtok(NULL, " ");
				
					strncpy(temp, &token[1], 6);
					yf =  (float)atof(temp);
				
					if(xf > xbase*2 || yf > ybase*2){ 
						areaError(); 
					}
				
					x = (int)(((xf - xbase)/xbase)*-8388607);
					y = (int)(((yf - ybase)/ybase)*-8388607);
				
					SDdata[pos++] = ((x & 0x007FFF00) >> 8) | ((x & 0x80000000) >> 16);
					
					SDdata[pos++] = ((x & 0x000000FF) << 8); 
					
					SDdata[pos++] = ((y & 0x007FFF00) >> 8) | ((y & 0x80000000) >> 16);
					
					SDdata[pos++] = ((y & 0x000000FF) << 8);
				
					token = strtok(NULL, " ");
					
					// Only consider E & F is there are coordinates to match (don't turn laser on arbitrarily) 
					
					if(token[0] == 'E'){
						SDdata[pos++] = 1;
						token = strtok(NULL, " ");
					} else {
						SDdata[pos++] = 0;
					}
			
					if(token[0] == 'F'){
							strncpy(temp, &token[1], 6);
							SDdata[pos++] = (uint16_t)atof(temp);
							speed = (uint16_t)atof(temp);
					} else {
							SDdata[pos++] = speed;
					}
				} 
			}

		token = strtok(NULL, " ");
	}
	
	return 0;
}

void PARSE_Task(void * pvParams){
	
	char strbuff[128];
	int exec, lineCnt = 0, laserState = 0;

	//vTaskSuspend(NULL);
	
	while(1){

		if(fileOpen){
			
			portENTER_CRITICAL();
			readLine(strbuff, 128);
			portEXIT_CRITICAL();
			
			updateProgbar(getFP(), getFileSize());
			
			
			exec = decode(strbuff);
				
			if(exec){
				
					pos = pos / 6;
					if(pos % 2 == 1)
						pos = pos - 1;
				
					int z,d;
					for(z = 0; z < pos; z++){
						
						if(SDdata[((z*6)+4)] && !laserState){ 
							startPWM(); 
							laserState = 1;
						} else if(!SDdata[((z*6)+4)] && laserState){
							stopPWM();
							laserState = 0;
						}
						stepDelay = SDdata[((z*6)+5)]*stepFact;
						while(hi2s3.State != HAL_I2S_STATE_READY);
						HAL_I2S_Transmit(&hi2s3, (uint16_t *)(&SDdata[z*6]), 2, 100);
						for(d = 0; d < stepDelay; d++);
					}
					
					pos = 0;
					if(laserState){
						stopPWM();
						laserState = 0;
					}

					translate(STEPPER_PWDR_SINK, (zf - zl), 2, STEPPER_CW);
					translate(STEPPER_PWDR_SRC, (zf - zl), 2, STEPPER_CCW);
					zl = zf;
//					translate(STEPPER_ROLLER, 510.0, 8, STEPPER_CW);
//					translate(STEPPER_ROLLER, 510.0, 8, STEPPER_CCW);
			}
			
			
		
//			if(strbuff[0] == 'C'){
//				
//				strncpy(temp, &strbuff[1], 6);
//				xf =  (float)atoi(temp);
//				
//				strncpy(temp, &strbuff[8], 6);
//				yf =  (float)atoi(temp);
//				
//				x = (int)((((xf - xbase)/xbase)*8388607)*-1);
//				y = (int)((((yf - ybase)/ybase)*8388607)*-1);
//				
//				int m;
//				
//				for(m = 0; m < 1; m++){ // lower scan rate
//						
//					SDdata[pos++] = ((x & 0x007FFF00) >> 8) | ((x & 0x80000000) >> 16);
//					
//					SDdata[pos++] = ((x & 0x000000FF) << 8); 
//					
//					SDdata[pos++] = ((y & 0x007FFF00) >> 8) | ((y & 0x80000000) >> 16);
//					
//					SDdata[pos++] = ((y & 0x000000FF) << 8);
//					
//				}
//				
//				// If pos exceeds 40K - produce error message
//				
//			} else if(strbuff[0] == 'Z'){
//				
//					pos = pos / 4;
//					if(pos % 2 == 1)
//						pos = pos - 1;
//					
//					//xQueueSend(queue_handle, &pos, portMAX_DELAY);
//					
//					//translate(STEPPER_PWDR_SRC, 10.0f, 2, STEPPER_CW);
//					
//					//translate(STEPPER_PWDR_SINK, 10.0f, 2, STEPPER_CCW);
//					
//					//while(hi2s3.State != HAL_I2S_STATE_READY);

//					//portENTER_CRITICAL();
//					
//					//If pos is greater than 0 - turn Laser on
//					
//					int z,d;
//					for(z = 0; z < pos; z++){
//						while(hi2s3.State != HAL_I2S_STATE_READY);
//						HAL_I2S_Transmit(&hi2s3, (uint16_t *)(&SDdata[z*4]), 2, 100);
//						bytesProc += 15;
//						for(d = 0; d < stepDelay; d++);
//					}

//					updateProgbar(bytesProc, getFileSize());
//					
//					//portEXIT_CRITICAL();
//					
//					pos = 0;
//					
//					// Turn Laser off after segment complete
//					
//					vTaskDelay(20);
//			}

		} else {
				vTaskSuspend(NULL);
		}

	}
}

