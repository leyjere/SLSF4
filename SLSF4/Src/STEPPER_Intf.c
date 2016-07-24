#include "stm32f4xx_hal.h"
#include "stdlib.h"
#include "STEPPER_Intf.h"
#include "math.h"

void perform_steps(uint32_t step_count){
	
	int i;
	
	for(i = 0; i < step_count*2; i++){
		if(i % 2 == 0)
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		
		HAL_Delay(1);
	}
}

void set_resolution(enum step_size resolution){
	
		// Set MS lines according to resolution
	
	switch(resolution){
		case STEPSZE_ONE:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5 | GPIO_PIN_6, GPIO_PIN_RESET);
			break;
		case STEPSZE_HALF:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
			break;
		case STEPSZE_QUART:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
			break;
		case STEPSZE_EIGTH:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5 | GPIO_PIN_6, GPIO_PIN_SET);
			break;
		default:
			break;
	}
	
}

// Break down the distance into 1, 1/2, 1/4/, and 1/8 steps
uint8_t resolve_distance(enum step_size resolution, float dist_mm, uint8_t pitch){
	
	
	float granularity = 1.8f*(1.0f/(float)resolution);	// 1.8, 0.9, 0.45, or 0.225 degrees
	float step_unit = ((granularity/360.0f)*(float)pitch);	// factor by pitch
	
	if((dist_mm / step_unit) >= 1.0f){	// at least 1 step can be performed at this resolution
		
		uint32_t step_count = (dist_mm / step_unit);
		float remaining_dist = dist_mm - (step_count * step_unit); // Deduct distance covered at this resolution
		
		set_resolution(resolution);
		perform_steps(step_count);
		
		if(remaining_dist >= (float)(FINEST_STEP*pitch))
			return resolve_distance((enum step_size)((uint8_t)resolution*2), remaining_dist, pitch);
			
	} else if(dist_mm >= (float)(FINEST_STEP*pitch)){
		// recursively call func with next finest resolution
		return resolve_distance((enum step_size)((uint8_t)resolution*2), dist_mm, pitch);
	}
	
	if(dist_mm > 0)
		return 0;	// Return 0 if specified distance could not be fully resolved
	else
		return 1;
}

void translate(enum stepper_motor motor, float dist_mm, uint8_t pitch, enum stepper_direction direction){

	
	// Set direction
	if(direction == STEPPER_CW){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
	}
	
	// Select stepper
	
		switch(motor){
		
		case STEPPER_PWDR_SRC:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
			break;
		case STEPPER_PWDR_SINK:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
			break;
		case STEPPER_ROLLER:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
			break;
		default:
			break;
		
	};
	
	// Generate PWM
	resolve_distance(STEPSZE_ONE, dist_mm, pitch);
	
	// Reset stepper selection
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4, GPIO_PIN_SET);
	// Reset stepper resolution
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5 | GPIO_PIN_6, GPIO_PIN_RESET);
	
}
