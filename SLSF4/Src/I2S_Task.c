#include "stm32f4xx_hal.h"
#include "I2S_Task.h"
#include "cmsis_os.h"
#include "string.h"
#include "stdlib.h"

extern UART_HandleTypeDef huart1;
extern I2S_HandleTypeDef hi2s2;
extern osMessageQId queue_handle;

extern uint16_t SDdata1[40000];
extern uint16_t SDdata2[40000];

extern osThreadId SDIO_Handle;
extern osThreadId I2S_Handle;

void I2S_Task(void * pvParams){

	uint16_t count;
	
	while(1){
		
		xQueueReceive(queue_handle, &count, portMAX_DELAY);
		
		while(hi2s2.State != HAL_I2S_STATE_READY);

		portENTER_CRITICAL();

		HAL_I2S_Transmit_DMA(&hi2s2, SDdata1, count);

		portEXIT_CRITICAL();
		
		HAL_UART_Transmit(&huart1, "I2S \n\r", 6 ,100);

	}
	
}
