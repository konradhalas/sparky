#include <stdlib.h>
#include <string.h>

#include "stm32f4xx_hal.h"

#include "main.h"
#include "core.h"

extern UART_HandleTypeDef huart1;

#define MESSAGES_QUEUE_ELEMENTS 100
#define MESSAGES_QUEUE_SIZE (MESSAGES_QUEUE_ELEMENTS + 1)

char messagesQueue[MESSAGES_QUEUE_SIZE][MESSAGE_LENGTH];
int messagesQueueIn = 0, messagesQueueOut = 0;

uint8_t commandsBuffor[COMMAND_LENGTH] = {};

int isReadModeEnabled = 1;

int motorLeftEncoderPreviousCount = -1;
int motorRightEncoderPreviousCount = -1;
int motorLeftSpeed = 0;
int motorRightSpeed = 0;

void handleCommand() {
	if (commandsBuffor[0] == START_STOP_COMMAND) {
		toggleMotor(MOTOR_LEFT);
	}
}

void sendMessage(char *message) {
	addMessageToQueue(message);
	sendPendingMessages(&huart1);
}

void sendPendingMessages(UART_HandleTypeDef *huart) {
	if (hasMessagesInQueue() && HAL_UART_GetState(huart) != HAL_UART_STATE_BUSY_TX && HAL_UART_GetState(huart) != HAL_UART_STATE_BUSY_TX_RX) {
		char *message = NULL;
		if (getMessageFromQueue(&message) != 1 && message != NULL) {
			HAL_UART_Transmit_IT(huart, (uint8_t*)message, strlen(message));
		}
	}
}

int hasMessagesInQueue() {
	if (messagesQueueIn != messagesQueueOut) {
		return 1;
	} else {
		return 0;
	}
}

int addMessageToQueue(char *message)
{
    if(messagesQueueIn == (( messagesQueueOut - 1 + MESSAGES_QUEUE_SIZE) % MESSAGES_QUEUE_SIZE)) {
        return -1;
    }

    strcpy(messagesQueue[messagesQueueIn], message);

    messagesQueueIn = (messagesQueueIn + 1) % MESSAGES_QUEUE_SIZE;

    return 0;
}

int getMessageFromQueue(char **message)
{
    if(messagesQueueIn == messagesQueueOut) {
        return -1;
    }

    *message = messagesQueue[messagesQueueOut];

    messagesQueueOut = (messagesQueueOut + 1) % MESSAGES_QUEUE_SIZE;

    return 0;
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

int calculateMotorSpeed(int currentCount, int previousCount) {
	 int speed = currentCount - previousCount;
	 if (abs(speed) > ENCODER_MAX_SPEED) {
		 speed = (ENCODER_MAX_VALUE - abs(speed)) * (speed > 0 ? -1 : 1);
	 }
	 return speed;
}

void sendMotorsSpeedMessage() {
	char message[MESSAGE_LENGTH];
	snprintf(message, MESSAGE_LENGTH, "M %d %d\n", motorLeftSpeed, motorRightSpeed);
	sendMessage(message);
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
	 if (isReadModeEnabled == 1) {
		 sendMotorsSpeedMessage();
	 }
}
