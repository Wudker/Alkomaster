#include <General.h>
#include <MQ3.h>
#include <pins.h>
#include <Screen.h>
Power_state Peripheral_power = HEATING;
uint32_t Heating_time =  2UL *60UL * 1000UL;   // 2 min
uint32_t inactive_time = 1UL * 60UL * 1000UL;  // 1 min

uint32_t heatingStart = 0;
uint32_t lastActivity = 0;
MQ3 Sensor(ADC_PIN_SENSOR, true, 1490.0f);

 void Sensor_Init(){
    Sensor.begin(Heating_time);
}

void Display_on(){
    digitalWrite(Display_power_pin, HIGH);
}

void Display_off(){
    digitalWrite(Display_power_pin, LOW);
}

void Sensor_off(){
    digitalWrite(Sensor_power_pin, LOW);
}

void Sensor_on(){
    digitalWrite(Sensor_power_pin, HIGH);
    Sensor_Init();
    
}

float sensor_read(){
    float percentBAC = Sensor.readAlcoholConcentration(PERCENT_BAC);

    if (isnan(percentBAC) || isinf(percentBAC))
    {
        return NAN;
    }

    float promile = percentBAC * 10.0f;
    return promile;
}

