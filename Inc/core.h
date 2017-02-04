#ifndef _CORE_H_
#define _CORE_H_

#define START_STOP_COMMAND 'T'
#define MESSAGE_LENGTH 20
#define COMMAND_LENGTH 20

#define MOTOR_LEFT 1
#define MOTOR_RIGHT 2
#define FORWARD 1
#define BACKWARD 2

#define MOTOR_LEFT_TIM_CCR TIM1->CCR1
#define MOTOR_RIGHT_TIM_CCR TIM1->CCR4
#define MOTORS_TIM_ENC TIM10
#define MOTOR_LEFT_TIM_ENC_CNT TIM3->CNT
#define MOTOR_RIGHT_TIM_ENC_CNT TIM4->CNT

#define ENCODER_MAX_SPEED 20000
#define ENCODER_MAX_VALUE 65535

#define EDF_TIM_CCR TIM2->CCR1

extern uint8_t commandsBuffor[];

void handleCommand();

void sendMessage(char *message);
void sendPendingMessages(UART_HandleTypeDef *huart);
int hasMessagesInQueue();
int addMessageToQueue(char *message);
int getMessageFromQueue(char **message);

void setMotorSpeed(int motor, int direction, int speed);
int getMotorSetpointSpeed(int motor);
void toggleMotor(int motor);
void handleMotorsEncodersTimerPeriodElapsed();
int calculateSpeed(int currentCount, int previousCount);


#endif
