#include <Pins.h>
extern char Battery_state[20];
extern char alcohol_display[20];
extern char high_score_display[20];
extern float high_score;
extern float battery_state;
//extern MQ3 Sensor(ADC_pin, true, 1500.0f); // Pin 4 (A4) for ADCi

void Sensor_Init();