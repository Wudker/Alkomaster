#include <Pins.h>
extern char Battery_state[20];
extern char alcohol_display[20];
extern char high_score_display[20];
extern float high_score;
enum Power_state
{
    OFF,
    HEATING,
    READY,
    MEASURING
};
void Sensor_Init();
void Peripheral_power_supply_ON();
void Peripheral_power_supply_OFF();
void Heater_on();
void Heater_off();
float sensor_read();
void Start_system();
