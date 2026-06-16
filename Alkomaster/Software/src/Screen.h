#pragma once
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C


void Clearline(uint8_t linia, uint8_t size);
void slowodruk(const char *tekst, uint8_t linia, int Size);
void display_init();