#include "stm32f4xx_hal.h"
#define FINEST_STEP 0.000625f	// Eigth step proportional to 360 degrees

enum stepper_motor { 
	STEPPER_PWDR_SRC = 0x01,
	STEPPER_PWDR_SINK = 0x02,
	STEPPER_ROLLER = 0x03
};

enum step_size {
	STEPSZE_ONE = 0x01,
	STEPSZE_HALF = 0x02,
	STEPSZE_QUART = 0x04,
	STEPSZE_EIGTH = 0x08
};

enum stepper_direction{
	STEPPER_CW = 0x01,
	STEPPER_CCW = 0x02
};

void translate(enum stepper_motor motor, float dist_mm, uint8_t pitch, enum stepper_direction direction);
