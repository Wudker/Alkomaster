#include <General.h>
#include <MQ3.h>
#include <pins.h>
#include <Screen.h>
Power_state Peripheral_power = OFF;
uint32_t Heating_time =   30UL * 1000UL;   // 30 sec
uint32_t inactive_time = 1UL * 60UL * 1000UL;  // 1 min

uint32_t heatingStart = 0;
uint32_t lastActivity = 0;

 MQ3 Sensor(ADC_PIN_SENSOR, true, 1500.0f);

 void Sensor_Init(){
    Sensor.begin(Heating_time);
}

void Peripheral_power_supply_ON(){
    digitalWrite(Power_mosfet_driver_pin, HIGH);
}

void Peripheral_power_supply_OFF(){
    digitalWrite(Power_mosfet_driver_pin, LOW);
}

void Heater_on(){
    digitalWrite(Power_mosfet_driver_pin, HIGH);
    Sensor_Init();
    
}

void Heater_off(){
    digitalWrite(Heater_power_pin, LOW);
}

float sensor_read()
{
    float percentBAC = Sensor.readAlcoholConcentration(PERCENT_BAC);

    if (isnan(percentBAC) || isinf(percentBAC))
    {
        return NAN;
    }

    float promile = percentBAC * 10.0f;
    return promile;
}

