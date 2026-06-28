#include <Pins.h>
#include <Arduino.h>

void PINS_init(){
    pinMode(ADC_PIN_SENSOR, INPUT);
    pinMode(Wake_up_pin, INPUT);
    pinMode(Power_mosfet_driver_pin, OUTPUT);
    pinMode(Heater_power_pin, OUTPUT);

    digitalWrite(Power_mosfet_driver_pin, LOW);
    digitalWrite(Heater_power_pin, LOW);

}
