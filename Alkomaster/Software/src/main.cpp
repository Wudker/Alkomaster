#include <Arduino.h>
#include <MQ3.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Screen.h>
#include <Pins.h>
#include <General.h>
#include <esp_sleep.h>



extern float v_ref, resolution;
extern Power_state Peripheral_power;
extern uint32_t inactive_time;
extern uint32_t lastActivity;
bool sensorReady = false;
extern bool Wakeup_flag;
float high_score = 0.0f;



void setup()
{
    Serial.begin(115200);
    delay(100);

    PINS_init();

         
               
    Wire.begin(6, 5);



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
            Deep_sleep_init();
            break;
        }

        case HEATING:
        {
            Display_on();
            display_init();
            delay(500);
            screen_print(1, "HEATING");//debug

            Sensor_on();

            uint32_t heatingStartTime = millis();
            while (millis() - heatingStartTime < Heating_time)
            {
                char buffer[20];
                snprintf(buffer, sizeof(buffer), "Heating: %lu s", (Heating_time - (millis() - heatingStartTime)) / 1000);
                screen_print(2, buffer);
                delay(100);
            }

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
                float voltage = raw * v_ref / resolution;
                float alcohol = sensor_read();

                if (isnan(alcohol) || isinf(alcohol))
                {
                    screen_print(2, "ERR");
                }
                else
                {
                     high_score = alcohol;
                    if (high_score < alcohol)
                    {
                        high_score = alcohol;
                    }
                    screen_printf(2, 1, "%.4f", high_score);
                }

                screen_printf(3, 1, "ADC:%d", raw);
                delay(5);
            }

            screen_clear();
            Peripheral_power = SHOW;
            break;


            case SHOW:
            {
                screen_clear();
                screen_print(1, "SHOW");//debug
                screen_printf(2, 1, "Alcohol: %.4f", high_score);
                lastActivity = millis();
                while (millis() - lastActivity < inactive_time){
                    delay(1);
                }
                Peripheral_power = SLEEP;
                break;
            }
        }
    }
}



