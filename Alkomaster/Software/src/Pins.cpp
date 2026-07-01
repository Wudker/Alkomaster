#include <Arduino.h>
#include <Pins.h>
#include <esp_sleep.h>
#include "driver/gpio.h"
#include <General.h>
extern Power_state Peripheral_power;
bool Wakeup_flag=false;

void PINS_init()
{
    Serial.println("[PINS] Starting initialization");
    
    Serial.println("[PINS] Setting ADC_PIN_SENSOR as INPUT");
    pinMode(ADC_PIN_SENSOR, INPUT);
    
    Serial.println("[PINS] Setting Wake_up_pin as INPUT_PULLDOWN");
    pinMode(Wake_up_pin, INPUT_PULLDOWN);

    Serial.println("[PINS] Setting Power_mosfet_driver_pin as OUTPUT");
    pinMode(Power_mosfet_driver_pin, OUTPUT);
    
    Serial.println("[PINS] Setting Heater_power_pin as OUTPUT");
    pinMode(Heater_power_pin, OUTPUT);

    Serial.println("[PINS] Writing LOW to outputs");
    digitalWrite(Power_mosfet_driver_pin, LOW);
    digitalWrite(Heater_power_pin, LOW);
    
    Serial.println("[PINS] PINS_init complete!");
}

void Enter_deep_sleep()
{
    digitalWrite(Power_mosfet_driver_pin, LOW);
    digitalWrite(Heater_power_pin, LOW);
    delay(50);

    Serial.println("[DEEP_SLEEP] Configuring GPIO wakeup...");
    const gpio_num_t wakePin = (gpio_num_t)Wake_up_pin;
    const uint64_t wakeMask = 1ULL << Wake_up_pin;

    gpio_pulldown_en(wakePin);
    esp_deep_sleep_enable_gpio_wakeup(wakeMask, ESP_GPIO_WAKEUP_GPIO_HIGH);

    Serial.println("[DEEP_SLEEP] Entering deep sleep - bye bye!");
    Serial.flush();  // Wyślij wszystkie logi przed sleep
    
    esp_deep_sleep_start();  // Ta funkcja nie powinna wrócić!
    
    // Jeśli tu dotrzemy - BIG PROBLEM
    Serial.println("[ERROR] Deep sleep failed!");
}