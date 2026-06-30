#include <Arduino.h>
#include <MQ3.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Screen.h>
#include <Pins.h>
#include <General.h>
#include <esp_sleep.h>



extern Power_state Peripheral_power;
extern uint32_t Heating_time;
extern uint32_t inactive_time;
extern uint32_t heatingStart; 
extern uint32_t lastActivity;
bool sensorReady = false;



void setup()
{
    PINS_init();

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_GPIO || digitalRead(Wake_up_pin) == HIGH)
    {
        Start_system();
    }
    else
    {
        Peripheral_power = OFF;
    }
}

void loop()
{
    switch (Peripheral_power)
    {
        case OFF:
        {
            Heater_off();
            Peripheral_power_supply_OFF();
            Enter_deep_sleep();
            break;
        }

        case ON:
        {
            if (millis() - heatingStart >= Heating_time)
            {
                Sensor_Init();
                sensorReady = true;
                Peripheral_power = READY;  
                lastActivity = millis();

                slowodruk("Gotowe", 1, 1);
                slowodruk("Dmuchnij", 2, 1);
            }

            break;
        }

        case READY:
{
    if (digitalRead(Wake_up_pin) == HIGH)
    {
        float High_score = 0.0f;

        for (uint8_t i = 0; i < 100; i++)
        {
            float data = sensor_read(); // wynik w % BAC

            if (data > High_score)
            {
                High_score = data;
            }

            delay(20);
        }

        float promile = High_score * 10.0f;

        char dane_string[20];
        snprintf(dane_string, sizeof(dane_string), "%.2f prom", promile);

        slowodruk(dane_string, 2, 1);

        lastActivity = millis();

        delay(300);
    }

    if (millis() - lastActivity >= inactive_time)
    {
        sensorReady = false;
        Peripheral_power = OFF;
    }

    break;
        }
    }
}