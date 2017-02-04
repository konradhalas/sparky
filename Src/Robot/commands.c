#include "stm32f4xx_hal.h"

#include "commands.h"
#include "motors.h"

#define COMMAND_LENGTH 20
#define START_STOP_COMMAND 'T'

extern UART_HandleTypeDef huart1;

uint8_t commandsBuffor[COMMAND_LENGTH] = {};

void handleCommand() {
	if (commandsBuffor[0] == START_STOP_COMMAND) {
		toggleMotor(MOTOR_LEFT);
	}
}

void receiveCommand() {
	HAL_UART_Receive_IT(&huart1, commandsBuffor, COMMAND_LENGTH);
}
