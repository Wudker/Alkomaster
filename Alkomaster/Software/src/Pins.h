#pragma once
#include <Arduino.h>
#define ADC_PIN_SENSOR 4
#define Wake_up_pin 2
#define Sensor_power_pin 8
#define Display_power_pin 21
#define Battery_voltage_pin 0


void PINS_init();
void Deep_sleep_init();
void startSensorMinuteTimer();
bool sensorMinuteTimerExpired();
bool IRAM_ATTR Timer_SensorMinute(void *timerNo);
bool IRAM_ATTR Timer_Battery_checker(void *timerNo);
float readBatteryVoltage();
void handle_bettry_checker();