#include <General.h>
#include <MQ3.h>
#include <pins.h>
#include <Screen.h>
Power_state Peripheral_power = OFF;
uint32_t Heating_time = 5UL * 60UL * 1000UL;   // 5 min
uint32_t inactive_time = 1UL * 60UL * 1000UL;  // 1 min

uint32_t heatingStart = 0;
uint32_t lastActivity = 0;

 MQ3 Sensor(ADC_PIN_SENSOR, true, 1500.0f);

 void Sensor_Init(){
    Sensor.begin();
}

void Peripheral_power_supply_ON(){
    digitalWrite(Power_mosfet_driver_pin, HIGH);
}

void Peripheral_power_supply_OFF(){
    digitalWrite(Power_mosfet_driver_pin, LOW);
}

void Heater_on(){
    digitalWrite(Heater_power_pin, HIGH);
}

void Heater_off(){
    digitalWrite(Heater_power_pin, LOW);
}

float sensor_read(){
    float data = Sensor.readAlcoholConcentration(PERCENT_BAC);
    return data;
}

void Start_system(){
    Peripheral_power_supply_ON();
    delay(100);

    display_init();

    Heater_on();

    heatingStart = millis();
    lastActivity = millis();

    Peripheral_power = ON;

    slowodruk("Prosze zaczekac", 1, 1);
}
