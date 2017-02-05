#ifndef _SENSORS_H_
#define _SENSORS_H_

// PUBLIC
void readSensors();
void handleSensorsMeasured();

// PRIVATE
void sendSensorsValuesMessage();
void sendBatteryValueMessage();

#endif
