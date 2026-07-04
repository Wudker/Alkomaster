#include <Arduino.h>
#include <MQ3.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Screen.h>
#include <Pins.h>
#include <General.h>
#include <esp_sleep.h>
// These define's must be placed at the beginning before #include "ESP32_New_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
#define _TIMERINTERRUPT_LOGLEVEL_ 4

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ESP32TimerInterrupt.h"
#define PIN_D19 19 // Pin D19 mapped to pin GPIO9 of ESP32
#define PIN_D3 3   // Pin D3 mapped to pin GPIO3/RX0 of ESP32

#define TIMER0_INTERVAL_MS 1000
#define TIMER0_DURATION_MS 5000

#define TIMER1_INTERVAL_MS 3000
#define TIMER1_DURATION_MS 15000

// Init ESP32 timer 0 and 1
ESP32Timer ITimer0(0);
ESP32Timer ITimer1(1);

extern Power_state Peripheral_power;
extern uint32_t inactive_time;
extern uint32_t lastActivity;
bool sensorReady = false;
extern bool Wakeup_flag;

bool IRAM_ATTR TimerHandler0(void *timerNo)
{


    return true;
}

bool IRAM_ATTR TimerHandler1(void *timerNo)
{


    return true;
}

void setup()
{
    Serial.begin(115200);
    delay(100);

    PINS_init();

    Display_on();          
    delay(200);           
    Wire.begin(6, 5);
    ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0);
    ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1);

    if (display_init())
    {
        Serial.println("[DISPLAY] Display initialized successfully!");
        screen_print(0, "DISPLAY OK");
    }
    else
    {
        Serial.println("[ERROR] Display initialization failed!");
    }

    Sensor_off();

    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

    if (wakeup_reason == ESP_SLEEP_WAKEUP_GPIO)
    {
        Peripheral_power = HEATING;
    }
    else
    {
        Peripheral_power = SLEEP;
    }

}




void loop()
{
    switch (Peripheral_power)
    {

        
        case SLEEP:
        {
            screen_print(1, "SLEEP"); //debug
            delay(1000);
            Deep_sleep_init();
            break;
        }

        case HEATING:
        {
            Display_on();
            screen_print(1, "HEATING");//debug
            Sensor_on();

            Peripheral_power = MEASURING;
            break;
        }

        case MEASURING:
        {
            screen_print(1, "MEASURING");//debug
            lastActivity = millis();
            while (millis() - lastActivity < inactive_time)
            {
                int raw = analogRead(ADC_PIN_SENSOR);
                float voltage = raw * 3.3f / 4095.0f;
                float alcohol = sensor_read();

                Serial.printf("RAW=%d V=%.3f alcohol=%.6f\n",
                              raw,
                              voltage,
                              alcohol);

                if (isnan(alcohol) || isinf(alcohol))
                {
                    screen_print(2, "ERR");
                }
                else
                {
                    screen_printf(2, 1, "%.4f", alcohol);
                }

                screen_printf(3, 1, "ADC:%d", raw);

            }

            screen_clear();
            Peripheral_power = SLEEP;
            break;
        }
    }
}



