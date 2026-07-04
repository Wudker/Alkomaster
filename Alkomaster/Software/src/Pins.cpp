#include <Arduino.h>
#define _TIMERINTERRUPT_LOGLEVEL_ 4
#include "ESP32TimerInterrupt.h"
#include <Pins.h>
#include <General.h>
#define BATTERY_CHECKER_INTERVAL_US 10UL * 1000UL * 1000UL
#define SENSOR_WARMUP_INTERVAL_US 60UL * 1000UL * 1000UL
ESP32Timer ITimer0(0);
ESP32Timer ITimer1(1);
extern Power_state Peripheral_power;
volatile bool sensorMinuteTimerExpiredFlag = false;
bool Wakeup_flag=false;

void PINS_init()
{
    pinMode(ADC_PIN_SENSOR, INPUT);
    pinMode(Wake_up_pin, INPUT);
    pinMode(Display_power_pin, OUTPUT);
    pinMode(Sensor_power_pin, OUTPUT);
    pinMode(Battery_voltage_pin, INPUT);
    digitalWrite(Display_power_pin, LOW);
    digitalWrite(Sensor_power_pin, LOW);
    ITimer1.attachInterruptInterval(BATTERY_CHECKER_INTERVAL_US, Timer_Battery_checker);
}

void Deep_sleep_init()
{
    digitalWrite(Display_power_pin, LOW);
    digitalWrite(Sensor_power_pin, LOW);
    delay(50);

    const gpio_num_t wakePin = (gpio_num_t)Wake_up_pin;
    const uint64_t wakeMask = 1ULL << Wake_up_pin;

    gpio_pullup_en(wakePin);
    gpio_pulldown_dis(wakePin);

    esp_deep_sleep_enable_gpio_wakeup(wakeMask, ESP_GPIO_WAKEUP_GPIO_LOW);

    esp_deep_sleep_start();
}

void startSensorMinuteTimer()
{
    sensorMinuteTimerExpiredFlag = false;
    ITimer0.attachInterruptInterval(SENSOR_WARMUP_INTERVAL_US, Timer_SensorMinute);
}

bool sensorMinuteTimerExpired()
{
    return sensorMinuteTimerExpiredFlag;
}

bool IRAM_ATTR Timer_SensorMinute(void *timerNo)
{
    sensorMinuteTimerExpiredFlag = true;
    return true;
}

bool IRAM_ATTR Timer_Battery_checker(void *timerNo)
{
    if (Peripheral_power == HEATING || Peripheral_power == MEASURING)
    {
        float batteryPercent = Battery_percentage();
        Serial.print("Battery: ");
        Serial.print(batteryPercent);
        Serial.println("%");
    }
    return true;
}