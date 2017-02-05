#include "stm32f4xx_hal.h"

#include "sensors.h"
#include "messages.h"
#include "state.h"

#define SENSORS 13
#define ADC_MAX_VALUE 4095
#define VOLTAGE_REFERENCE 3340
#define DROP_RESISTOR 9990
#define MESURE_RESISTOR 5070

extern ADC_HandleTypeDef hadc1;

uint16_t sensorsBuffor[SENSORS];

void readSensors() {
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)sensorsBuffor, SENSORS);
}

void handleSensorsMeasured() {
	if (isReadModeEnabled()) {
		sendSensorsValuesMessage();
		sendBatteryValueMessage();
	}
}

void sendSensorsValuesMessage() {
	char message[MESSAGE_LENGTH];
	snprintf(message, MESSAGE_LENGTH, "S %d %d %d %d %d %d %d %d %d %d %d\n",
			sensorsBuffor[0], sensorsBuffor[1], sensorsBuffor[2], sensorsBuffor[3],
			sensorsBuffor[4], sensorsBuffor[5], sensorsBuffor[6], sensorsBuffor[7],
			sensorsBuffor[8], sensorsBuffor[9], sensorsBuffor[10]);
	sendMessage(message);
}

int toBatteryVoltage(uint32_t batteryVoltage, uint32_t referenceVoltage) {
	uint16_t VREFINT_CAL = *(uint16_t *)0x1FFF7A2A;
	int vdd = VOLTAGE_REFERENCE * VREFINT_CAL / referenceVoltage;
	int mesuredVoltage = (batteryVoltage * vdd) / ADC_MAX_VALUE;
	return (mesuredVoltage * (DROP_RESISTOR + MESURE_RESISTOR)) / MESURE_RESISTOR;
}

void sendBatteryValueMessage() {
	char message[MESSAGE_LENGTH];
	snprintf(message, MESSAGE_LENGTH, "B %d\n", toBatteryVoltage(sensorsBuffor[11], sensorsBuffor[12]));
	sendMessage(message);
}
