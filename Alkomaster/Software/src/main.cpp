#include <Arduino.h>
#include <MQ3.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Screen.h>

char Battery_state[] = "Battery: 90%";
char alcohol_display[20];
char high_score_display[20];
float high_score = 0.0f;
const uint32_t battery_pin 3;
const uint32_t Power_pin 4;
const uint32_t Senor_power_pin 5;
MQ3 Sensor(4, true, 1500.0f);  // Pin 4 (A4) for ADC

void setup()
{
  Serial.begin(115200);
  Sensor.begin();
  display_init();
  pinMode(Sensor_power_pin, output)
}

void loop()
{
  if(Senor_power_pin==LOW){
    digitalWrite(Senor_power_pin, high);
  float ppm = Sensor.readAlcoholConcentration(PPM);
  float promille = ppm / 10.0f;  // Convert PPM to promille

  if (high_score < promille)
  {
    high_score = promille;
  }
  }
  // Format floats to strings
  sprintf(alcohol_display, "Promille: %.2f", promille);
  sprintf(high_score_display, "Max: %.2f", high_score);

  slowodruk(Battery_state, 0, 1);
  slowodruk(alcohol_display, 2, 1);
  slowodruk(high_score_display, 3, 1);
  delay(10);
}
