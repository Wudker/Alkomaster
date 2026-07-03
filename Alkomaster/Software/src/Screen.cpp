#include "Screen.h"
#include <stdarg.h>
#include <stdio.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static bool oledReady = false;

bool display_ready()
{
    return oledReady;
}

bool display_init()
{
    Serial.println("[DISPLAY] OLED init");

    oledReady = false;

    for (uint8_t attempt = 1; attempt <= 3; attempt++)
    {
        Serial.printf("[DISPLAY] Trying OLED at 0x%02X, attempt %u/3\n",
                      SCREEN_ADDRESS,
                      attempt);

        if (display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
        {
            oledReady = true;
            break;
        }

        delay(200);
    }

    if (!oledReady)
    {
        Serial.println("[ERROR] OLED display not responding!");
        Serial.println("[ERROR] Continuing without display...");
        return false;
    }

    display.clearDisplay();
    display.setFont();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setTextWrap(false);
    display.display();

    Serial.println("[DISPLAY] Display initialized successfully!");
    return true;
}

void screen_clear(bool refresh)
{
    if (!display_ready())
        return;

    display.clearDisplay();

    if (refresh)
        display.display();
}

void screen_clear_line(uint8_t line, uint8_t size, bool refresh)
{
    if (!display_ready())
        return;

    if (size == 0)
        size = 1;

    uint16_t y = (uint16_t)line * SCREEN_CHAR_HEIGHT;

    if (y >= SCREEN_HEIGHT)
        return;

    uint16_t h = (uint16_t)SCREEN_CHAR_HEIGHT * size;

    if (y + h > SCREEN_HEIGHT)
        h = SCREEN_HEIGHT - y;

    display.fillRect(0, y, SCREEN_WIDTH, h, SSD1306_BLACK);

    if (refresh)
        display.display();
}

void screen_refresh()
{
    if (!display_ready())
        return;

    display.display();
}

void screen_power(bool on)
{
    if (!display_ready())
        return;

    display.ssd1306_command(on ? SSD1306_DISPLAYON : SSD1306_DISPLAYOFF);
}

void screen_print_cstr(const char *text,
                       uint8_t line,
                       uint8_t size,
                       bool clear,
                       bool refresh)
{
    screen_print(line, text, size, clear, refresh);
}

void screen_printf(uint8_t line, uint8_t size, const char *fmt, ...)
{
    if (!display_ready())
        return;

    char buffer[SCREEN_PRINTF_BUFFER];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    screen_print(line, buffer, size, true, true);
}