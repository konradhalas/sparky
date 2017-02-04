#ifndef _MOTORS_H_
#define _MOTORS_H_

#define MOTOR_LEFT 1
#define MOTOR_RIGHT 2
#define FORWARD 1
#define BACKWARD 2

// PUBLIC
void initializeMotors();
void toggleMotor(int motor);
void setMotorSpeed(int motor, int direction, int speed);
void handleMotorsEncodersTimerPeriodElapsed();

// PRIVATE
int getMotorSetpointSpeed(int motor);
int calculateSpeed(int currentCount, int previousCount);
void sendMotorsSpeedMessage();

#endif
