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
    Serial.printf("[DISPLAY] slowodruk: %s\n", tekst);
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
    Serial.println("[DISPLAY] Skipping I2C scan - going straight to init");
    
    Serial.printf("[DISPLAY] Trying to initialize OLED at 0x%02X\n", SCREEN_ADDRESS);
    
    uint8_t attempts = 0;
    while (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS) && attempts < 3)
    {
        Serial.printf("[DISPLAY] Init failed (attempt %d/3), retrying...\n", attempts + 1);
        delay(200);
        attempts++;
    }
    
    if (attempts >= 3)
    {
        Serial.println("[ERROR] OLED display not responding!");
        Serial.println("[ERROR] Continuing without display...");
        return;
    }
    
    Serial.println("[DISPLAY] Display initialized successfully!");
    display.clearDisplay();
    display.display();
}