#include <stdlib.h>

#include "stm32f4xx_hal.h"

#include "motors.h"
#include "messages.h"
#include "state.h"

#define MOTOR_LEFT_TIM_CCR TIM1->CCR1
#define MOTOR_RIGHT_TIM_CCR TIM1->CCR4
#define MOTOR_LEFT_TIM_ENC_CNT TIM3->CNT
#define MOTOR_RIGHT_TIM_ENC_CNT TIM4->CNT

#define ENCODER_MAX_SPEED 20000
#define ENCODER_MAX_VALUE 65535

#define EDF_TIM_CCR TIM2->CCR1

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim10;

int motorLeftEncoderPreviousCount = -1;
int motorRightEncoderPreviousCount = -1;
int motorLeftSpeed = 0;
int motorRightSpeed = 0;

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

int calculateMotorSpeed(int currentCount, int previousCount) {
	 int speed = currentCount - previousCount;
	 if (abs(speed) > ENCODER_MAX_SPEED) {
		 speed = (ENCODER_MAX_VALUE - abs(speed)) * (speed > 0 ? -1 : 1);
	 }
	 return speed;
}

void handleMotorsEncodersTimerPeriodElapsed() {
	if (motorLeftEncoderPreviousCount != -1) {
		 motorLeftSpeed = calculateMotorSpeed(MOTOR_LEFT_TIM_ENC_CNT, motorLeftEncoderPreviousCount);
	 }
	 if (motorRightEncoderPreviousCount != -1) {
		 motorRightSpeed = calculateMotorSpeed(MOTOR_RIGHT_TIM_ENC_CNT, motorRightEncoderPreviousCount);
	 }
	 motorLeftEncoderPreviousCount = MOTOR_LEFT_TIM_ENC_CNT;
	 motorRightEncoderPreviousCount = MOTOR_RIGHT_TIM_ENC_CNT;
	 if (isReadModeEnabled()) {
		 sendMotorsSpeedMessage();
	 }
}

void sendMotorsSpeedMessage() {
	char message[MESSAGE_LENGTH];
	snprintf(message, MESSAGE_LENGTH, "M %d %d\n", motorLeftSpeed, motorRightSpeed);
	sendMessage(message);
}

void initializeMotors() {
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	HAL_TIM_Base_Start_IT(&htim10);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
}
