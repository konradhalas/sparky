#include "stm32f4xx_hal.h"

#include "edf.h"

extern TIM_HandleTypeDef htim2;

void initializeEDF() {
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	TIM2->CCR1 = 0;
}

void setEDFSpeed(int speed) {
	TIM2->CCR1 = (int) ((20.0 * speed/100.0) + 20);
}

int getEDFSpeed() {
	if (TIM2->CCR1 < 20 || TIM2->CCR1 > 40) {
		return 0;
	}
	return ((TIM2->CCR1 - 20) / 20.0) * 100;
}

void enableEDF() {
	TIM2->CCR1 = 20;
}

int isEDFEnabled() {
	if (TIM2->CCR1 >= 20) {
		return 1;
	}
	return 0;
}

void disableEDF() {
	TIM2->CCR1 = 0;
}
