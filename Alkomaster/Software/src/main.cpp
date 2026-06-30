#include <Arduino.h>
#include <MQ3.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Screen.h>
#include <Pins.h>
#include <General.h>

uint32_t Heating_time = 5*60*1000; //~ 5 min
uint32_t inactive_time = 1*60*1000; //~ 1 min
uint32_t time_start;
extern Power_state Peripheral_power;




void setup()
{
PINS_init();
display_init();
Sensor_Init();
}

void loop()
{
    extern bool Wakeup_flag;
    if (Wakeup_flag==1){Peripheral_power=ON;}
    switch(Peripheral_power){
        case Power_state::OFF :
        {
            Peripheral_power_supply_OFF();
            Heater_off();
            Enter_deep_sleep();
            break;
        }
        case Power_state::ON :
        {   
            slowodruk("Prosze zaczekać",1, 1);
            time_start = millis();
            Heater_on();
            if(time_start>Heating_time){
                Peripheral_power = ON_Heater_off;
                break;
            }
        }
        case Power_state::ON_Heater_off :
        {
            time_start = millis();
            if (digitalRead(Wake_up_pin)== 0){
                float data = sensor_read();
                char dane_string[16];
                snprintf(dane_string, sizeof(dane_string), "%d", data);
                slowodruk(dane_string, 2, 1);
                time_start = millis();
            }
            if (time_start<inactive_time){
                Peripheral_power= OFF;
                break;
            }
        }

    }
}
