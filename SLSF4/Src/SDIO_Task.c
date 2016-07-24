#include "fatfs.h"
#include "stm32f4xx_hal.h"
#include "stdlib.h"
#include "string.h"
#include "SDIO_Task.h"
#include "math.h"

extern UART_HandleTypeDef huart1;
extern osMessageQId queue_handle;
extern I2S_HandleTypeDef hi2s2;

extern uint16_t SDdata1[40000];
extern uint16_t SDdata2[40000];

/* FATFS variables */
static FATFS fs32;
static FIL fil;
static FRESULT res;
static TCHAR path[4];
static UINT br;
static char strbuff[15];

/* Conversion variables */
char temp[7];
int x, y;
float xf, yf;
uint16_t index;
uint16_t some_var;
uint16_t phase;

void SDIO_Setup(){
	
	MX_FATFS_Init();

	res = f_mount(&fs32, path, 0);
	
	res = f_open(&fil, "OUT.txt", FA_READ);
	
	temp[6] = (char)'\0';
	
	index = 0;
	
	some_var = 0;
	
	phase = 0;
	
}

static double xin = 0.0 , yin = 0.0;

void SDIO_Task(void * pvParams){
	
	
	while(1){

		portENTER_CRITICAL();
		res = f_read(&fil, strbuff, 15, &br);
		portEXIT_CRITICAL();
	
		if(strbuff[0] == 'C'){
		
			strncpy(temp, &strbuff[1], 6);
			xf =  (float)atoi(temp);
			
			strncpy(temp, &strbuff[8], 6);
			yf =  (float)atoi(temp);
			
			x = (((xf - 200000.0f)/200000.0f)*8388607)*-1;
			y = (((yf - 200000.0f)/200000.0f)*8388607)*-1;
			
			int m;
			
			for(m = 0; m < 1; m++){ // lower scan rate
					
				SDdata1[index++] = ((x & 0x007FFF00) >> 8) | ((x & 0x80000000) >> 16);
				
				SDdata1[index++] = ((x & 0x000000FF) << 8); 
				
				SDdata1[index++] = ((y & 0x007FFF00) >> 8) | ((y & 0x80000000) >> 16);
				
				SDdata1[index++] = ((y & 0x000000FF) << 8);
				
			}
			
			// If index exceeds 40K - produce error message
			
		} else if(strbuff[0] == 'Z'){
			
				index = index / 4;
				if(index % 2 == 1)
					index = index - 1;
				
				//xQueueSend(queue_handle, &index, portMAX_DELAY);
				
				while(hi2s2.State != HAL_I2S_STATE_READY);

				portENTER_CRITICAL();

				HAL_I2S_Transmit_DMA(&hi2s2, SDdata1, index);

				portEXIT_CRITICAL();
				
				index = 0;
		}

	}
	
}

