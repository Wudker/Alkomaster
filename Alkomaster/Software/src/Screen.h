#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define SCREEN_CHAR_HEIGHT 8
#define SCREEN_PRINTF_BUFFER 40

extern Adafruit_SSD1306 display;

bool display_init();
bool display_ready();

void screen_clear(bool refresh = true);
void screen_clear_line(uint8_t line, uint8_t size = 1, bool refresh = false);
void screen_refresh();
void screen_power(bool on);

void screen_print_cstr(const char *text,
                       uint8_t line,
                       uint8_t size = 1,
                       bool clear = true,
                       bool refresh = true);

void screen_printf(uint8_t line,
                   uint8_t size,
                   const char *fmt,
                   ...);

// Główna funkcja: przyjmuje int, float, double, String, const char*, char itd.
template <typename T>
void screen_print(uint8_t line,
                  const T &value,
                  uint8_t size = 1,
                  bool clear = true,
                  bool refresh = true)
{
    if (!display_ready())
        return;

    if (size == 0)
        size = 1;

    uint16_t y = (uint16_t)line * SCREEN_CHAR_HEIGHT;

    if (y >= SCREEN_HEIGHT)
        return;

    if (clear)
        screen_clear_line(line, size, false);

    display.setFont();
    display.setTextSize(size);
    display.setTextColor(SSD1306_WHITE);
    display.setTextWrap(false);
    display.setCursor(0, y);

    display.print(value);

    if (refresh)
        display.display();
}

template <typename T>
void screen_print_at(int16_t x,
                     int16_t y,
                     const T &value,
                     uint8_t size = 1,
                     bool refresh = true)
{
    if (!display_ready())
        return;

    if (size == 0)
        size = 1;

    display.setFont();
    display.setTextSize(size);
    display.setTextColor(SSD1306_WHITE);
    display.setTextWrap(false);
    display.setCursor(x, y);

    display.print(value);

    if (refresh)
        display.display();
}

// Aliasy dla starego kodu, żeby nie rozwalić reszty projektu.
inline void Clearline(uint8_t linia, uint8_t size)
{
    screen_clear_line(linia, size, false);
}

inline void slowodruk(const char *tekst, uint8_t linia, int Size)
{
    screen_print_cstr(tekst, linia, Size, true, true);
}