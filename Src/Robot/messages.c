#include <stdlib.h>
#include <string.h>

#include "stm32f4xx_hal.h"

#include "messages.h"

#define MESSAGES_QUEUE_ELEMENTS 100
#define MESSAGES_QUEUE_SIZE (MESSAGES_QUEUE_ELEMENTS + 1)

extern UART_HandleTypeDef huart1;

char messagesQueue[MESSAGES_QUEUE_SIZE][MESSAGE_LENGTH];
int messagesQueueIn = 0, messagesQueueOut = 0;

void sendMessage(char *message) {
	addMessageToQueue(message);
	sendPendingMessages();
}

void sendPendingMessages() {
	if (hasMessagesInQueue() && huart1.gState == HAL_UART_STATE_READY) {
		char *message = NULL;
		if (getMessageFromQueue(&message) != -1) {
			HAL_UART_Transmit_IT(&huart1, (uint8_t*)message, strlen(message));
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

int addMessageToQueue(char *message) {
    if(messagesQueueIn == (( messagesQueueOut - 1 + MESSAGES_QUEUE_SIZE) % MESSAGES_QUEUE_SIZE)) {
        return -1;
    }
    strcpy(messagesQueue[messagesQueueIn], message);
    messagesQueueIn = (messagesQueueIn + 1) % MESSAGES_QUEUE_SIZE;
    return 0;
}

int getMessageFromQueue(char **message) {
    if(messagesQueueIn == messagesQueueOut) {
        return -1;
    }
    *message = messagesQueue[messagesQueueOut];
    messagesQueueOut = (messagesQueueOut + 1) % MESSAGES_QUEUE_SIZE;
    return 0;
}

