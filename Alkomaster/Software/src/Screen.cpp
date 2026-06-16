#include <Screen.h>
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void Clearline(uint8_t linia, uint8_t size)
{
    uint8_t y = linia * 8;
    uint8_t h = 8 * size;
    if (y >= SCREEN_HEIGHT)
        return;
    display.fillRect(0, y, SCREEN_WIDTH, h, SSD1306_BLACK);
}

void slowodruk(const char *tekst, uint8_t linia, int Size)
{
    Clearline(linia, Size);
    display.setTextSize(Size);
    display.setTextColor(SSD1306_WHITE);
    uint8_t y = linia * 8;
    if (y >= SCREEN_HEIGHT)
        return;
    display.setCursor(0, y);
    display.print(tekst);
    display.display();
}

void display_init(){
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        while (1)
        {
            delay(100);
        }
    }
    display.clearDisplay();
    display.display();
}