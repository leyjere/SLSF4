#include "MULTIPAGE.h"
#include "cmsis_os.h"
#include "TS_Task.h"
#include "ili9341.h"
#include "main.h"
#include "GUI.h"

extern uint8_t GUI_Initialized;
extern osThreadId GUI_Handle;
extern UART_HandleTypeDef huart1;

void BSP_Pointer_Update(void)
{
  GUI_PID_STATE TS_State;
  static TS_StateTypeDef prev_state;
  TS_StateTypeDef  ts;
  uint16_t xDiff, yDiff;  
  
  BSP_TS_GetState(&ts);
  
  TS_State.Pressed = ts.TouchDetected;
  
  xDiff = (prev_state.X > ts.X) ? (prev_state.X - ts.X) : (ts.X - prev_state.X);
  yDiff = (prev_state.Y > ts.Y) ? (prev_state.Y - ts.Y) : (ts.Y - prev_state.Y);
  
  if(ts.TouchDetected)
  {
    if((prev_state.TouchDetected != ts.TouchDetected )||
       (xDiff > 3 )||
         (yDiff > 3))
    {
      prev_state = ts;
      
      TS_State.Layer = 0;
      TS_State.x = ts.X;
      TS_State.y = ts.Y;
      
      GUI_TOUCH_StoreStateEx(&TS_State);
			
			TS_State.Pressed  = 0;
			
			GUI_TOUCH_StoreStateEx(&TS_State);
			
			GUI_Delay(250);
			
			vTaskResume(GUI_Handle);
    }
  }
}

void BSP_Background(void)
{
  BSP_LED_Toggle(LED3);
  BSP_LED_Toggle(LED4);
  
  /* Capture input event and update cursor */
  if(GUI_Initialized == 1)
  {
    BSP_Pointer_Update();
  }
}

void BSP_Config(void){
  /* Initialize STM32F429I-DISCO's LEDs */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  
  /* Initializes the SDRAM device */
  BSP_SDRAM_Init();
  
  /* Initialize the Touch screen */
  BSP_TS_Init(240, 320);
  
  /* Enable the CRC Module */
  __HAL_RCC_CRC_CLK_ENABLE();
}

void TS_Task(void * pvParams){
	
	vTaskSuspend(NULL);

	while(1){
		BSP_Background();
		vTaskDelay(20);
	}
}
