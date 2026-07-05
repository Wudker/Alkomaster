#pragma once
#include <Pins.h>
extern char Battery_state[20];
extern char alcohol_display[20];
extern char high_score_display[20];
enum Power_state
{
    SLEEP,
    HEATING,
    MEASURING,
    SHOW
};

extern uint32_t Heating_time;
extern uint32_t inactive_time;
void Sensor_Init();
void Display_on();
void Display_off();
void Sensor_off();
void Sensor_on();
float sensor_read();
float Battery_percentage();
void heating_init();

