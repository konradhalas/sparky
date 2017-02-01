#include "stm32f4xx_hal.h"

#include "core.h"
#include "main.h"

void handleCommand(uint8_t *received) {
	if (received[0] == START_STOP_COMMAND) {
		toggleMotor(MOTOR_LEFT);
	}
}

void toggleMotor(int motor) {
	if (getMotorSetpointSpeed(motor) == 0) {
		setMotorSpeed(motor, FORWARD, 100);
	} else {
		setMotorSpeed(motor, FORWARD, 0);
	}
}

int getMotorSetpointSpeed(int motor) {
	return motor == MOTOR_LEFT ? MOTOR_LEFT_TIM_CCR : MOTOR_RIGHT_TIM_CCR;
}

void setMotorSpeed(int motor, int direction, int speed) {
	if (motor == MOTOR_LEFT) {
		MOTOR_LEFT_TIM_CCR = speed;
		HAL_GPIO_WritePin(MOTOR_L_DIR_1_GPIO_Port, MOTOR_L_DIR_1_Pin, direction == FORWARD ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOTOR_L_DIR_2_GPIO_Port, MOTOR_L_DIR_2_Pin, direction == FORWARD ? GPIO_PIN_RESET : GPIO_PIN_SET);
	} else if (motor == MOTOR_RIGHT) {
		MOTOR_RIGHT_TIM_CCR = speed;
		HAL_GPIO_WritePin(MOTOR_R_DIR_1_GPIO_Port, MOTOR_R_DIR_1_Pin, direction == FORWARD ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOTOR_R_DIR_2_GPIO_Port, MOTOR_R_DIR_2_Pin, direction == FORWARD ? GPIO_PIN_RESET : GPIO_PIN_SET);
	}
}
