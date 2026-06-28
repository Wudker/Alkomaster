#include <Pins.h>
#include <Arduino.h>

void PINS_init(){
    pinMode(ADC_PIN_SENSOR, INPUT);
    pinMode(Wake_up_pin, INPUT);
    pinMode(Power_mosfet_driver_pin, OUTPUT);

}
