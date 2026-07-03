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
extern uint32_t inactive_time;
extern uint32_t lastActivity;
bool sensorReady = false;


void setup()
{
    Serial.begin(115200);
    delay(100);

    Wire.begin(6, 5);
    PINS_init();

    if (display_init())
    {
        Serial.println("[DISPLAY] Display initialized successfully!");
    }
    else
    {
        Serial.println("[ERROR] Display initialization failed!");
    }

}



void loop()
{
    switch (Peripheral_power)
    {
        case OFF:
        {
            Heater_off();
            screen_print(1, "OFF");
            delay(1000);

            Peripheral_power = HEATING;
            break;
        }

        case HEATING:
        {
            screen_print(1, "HEATING");
            Heater_on();


            // Kalibracja tylko w czystym powietrzu
            Sensor_Init();

            Peripheral_power = MEASURING;
            break;
        }

        case MEASURING:
        {
            screen_print(1, "MEASURING");

            for (int i = 0; i < 10000; i++)
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

                delay(5);
            }

            screen_clear();
            Peripheral_power = OFF;
            break;
        }
    }
}
