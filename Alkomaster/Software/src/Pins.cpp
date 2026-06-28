#include <Arduino.h>
#include <Pins.h>
#include <esp_sleep.h>
#include "driver/gpio.h"

void PINS_init()
{
    pinMode(ADC_PIN_SENSOR, INPUT);
    pinMode(Wake_up_pin, INPUT_PULLDOWN);

    pinMode(Power_mosfet_driver_pin, OUTPUT);
    pinMode(Heater_power_pin, OUTPUT);

    digitalWrite(Power_mosfet_driver_pin, LOW);
    digitalWrite(Heater_power_pin, LOW);
}

void Enter_deep_sleep()
{
    digitalWrite(Power_mosfet_driver_pin, LOW);
    digitalWrite(Heater_power_pin, LOW);

    delay(50);

    const gpio_num_t wakePin = (gpio_num_t)Wake_up_pin;
    const uint64_t wakeMask = 1ULL << Wake_up_pin;

    gpio_pulldown_en(wakePin);

    esp_deep_sleep_enable_gpio_wakeup(wakeMask, ESP_GPIO_WAKEUP_GPIO_HIGH);

    esp_deep_sleep_start();
}