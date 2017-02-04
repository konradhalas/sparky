#ifndef _MOTORS_H_
#define _MOTORS_H_

#define MOTOR_LEFT 1
#define MOTOR_RIGHT 2
#define FORWARD 1
#define BACKWARD 2

void setMotorSpeed(int motor, int direction, int speed);
int getMotorSetpointSpeed(int motor);
void toggleMotor(int motor);
void handleMotorsEncodersTimerPeriodElapsed();
int calculateSpeed(int currentCount, int previousCount);
void sendMotorsSpeedMessage();

#endif
