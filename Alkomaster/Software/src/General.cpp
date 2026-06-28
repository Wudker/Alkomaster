#include <General.h>
#include <MQ3.h>
#include <pins.h>
Power_state Peripheral_power = OFF;
void Sensor_Init()
{
    MQ3 Sensor(ADC_PIN_SENSOR, true, 1500.0f);
    Sensor.begin();
}

void Peripheral_power_supply_ON(){
    digitalWrite(Power_mosfet_driver_pin, HIGH);
}

void Peripheral_power_supply_OFF()
{
    digitalWrite(Power_mosfet_driver_pin, LOW);
}

void Heater_on(){
    digitalWrite(Heater_power_pin, HIGH);
}
void Heater_off()
{
    digitalWrite(Heater_power_pin, LOW);
}