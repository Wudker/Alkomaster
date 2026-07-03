#include <Arduino.h>
#include <Pins.h>
#include <esp_sleep.h>
#include "driver/gpio.h"
#include <General.h>
extern Power_state Peripheral_power;
bool Wakeup_flag=false;

void PINS_init()
{
    pinMode(ADC_PIN_SENSOR, INPUT);
    pinMode(Wake_up_pin, INPUT);
    pinMode(Display_power_pin, OUTPUT);
    pinMode(Sensor_power_pin, OUTPUT);
    digitalWrite(Display_power_pin, LOW);
    digitalWrite(Sensor_power_pin, LOW);

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