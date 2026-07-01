#include <Arduino.h>
#include <MQ3.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Screen.h>
#include <Pins.h>
#include <General.h>
#include <esp_sleep.h>



extern Power_state Peripheral_power;
extern uint32_t Heating_time;
extern uint32_t inactive_time;
extern uint32_t heatingStart; 
extern uint32_t lastActivity;
bool sensorReady = false;





void changeState(Power_state newState)
{
    if (Peripheral_power == newState)
        return;

    Peripheral_power = newState;

    switch (Peripheral_power)
    {
        case OFF:
        {
            Serial.println("[STATE] OFF");

            Heater_off();
            Peripheral_power_supply_OFF();

            Serial.flush();
            delay(100);

            Enter_deep_sleep();
            break;
        }

        case HEATING:
        {
            Serial.println("[STATE] HEATING");

            Peripheral_power_supply_ON();
            delay(300);

            display_init();

            Heater_on();

            heatingStart = millis();
            lastActivity = millis();

            slowodruk("Prosze zaczekac", 1, 1);
            break;
        }

        case READY:
        {
            Serial.println("[STATE] READY");

            slowodruk("Gotowe", 1, 1);
            slowodruk("Dmuchnij", 2, 1);

            lastActivity = millis();
            break;
        }

        case MEASURING:
        {
            Serial.println("[STATE] MEASURING");
            slowodruk("Pomiar...", 1, 1);
            break;
        }
    }
}

bool buttonPressed()
{
    static bool lastState = LOW;
    bool currentState = digitalRead(Wake_up_pin);

    bool pressed = (currentState == HIGH && lastState == LOW);

    lastState = currentState;

    return pressed;
}







void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n\n=== SETUP START ===");

    Wire.begin(6, 5);

    PINS_init();

    int wakeup_cause = esp_sleep_get_wakeup_cause();
    int pin_state = digitalRead(Wake_up_pin);

    Serial.printf("Wakeup cause: %d, Wake pin: %d\n", wakeup_cause, pin_state);

    if (wakeup_cause == ESP_SLEEP_WAKEUP_GPIO || pin_state == HIGH)
    {
        changeState(HEATING);
    }
    else
    {
        changeState(OFF);
    }

    Serial.println("=== SETUP END ===");
}



void loop()
{
    switch (Peripheral_power)
    {
        case OFF:
        {
            // Normalnie tutaj program nie powinien być długo,
            // bo changeState(OFF) usypia układ.
            delay(100);
            break;
        }

        case HEATING:
        {
            static uint32_t lastHeatingPrint = 0;

            if (millis() - lastHeatingPrint >= 1000)
            {
                Serial.printf("[HEATING] %lu / %lu ms\n",
                              millis() - heatingStart,
                              Heating_time);
                lastHeatingPrint = millis();
            }

            if (millis() - heatingStart >= Heating_time)
            {
                Serial.println("[HEATING] Sensor init start");

                Sensor_Init();   // tu masz dodatkowe ~19 s blokady z biblioteki MQ3

                sensorReady = true;

                Serial.println("[HEATING] Sensor init done");

                changeState(READY);
            }

            break;
        }

        case READY:
        {
            if (buttonPressed())
            {
                changeState(MEASURING);
            }

            if (millis() - lastActivity >= inactive_time)
            {
                sensorReady = false;
                changeState(OFF);
            }

            break;
        }

        case MEASURING:
        {
            float highScore = 0.0f;

            for (uint8_t i = 0; i < 100; i++)
            {
                float data = sensor_read();

                if (data > highScore)
                {
                    highScore = data;
                }

                delay(20);
            }

            float promile = highScore * 10.0f;

            char dane_string[20];
            snprintf(dane_string, sizeof(dane_string), "%.2f prom", promile);

            slowodruk(dane_string, 2, 1);

            lastActivity = millis();

            // Poczekaj aż użytkownik puści przycisk,
            // żeby nie odpalić pomiaru drugi raz od razu.
            while (digitalRead(Wake_up_pin) == HIGH)
            {
                delay(10);
            }

            delay(300);

            changeState(READY);
            break;
        }
    }
}

