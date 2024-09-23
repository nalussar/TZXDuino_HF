#include "Config.h"
#include "Display.h"
#include "Texts.h"  // STR_EMPTY
#include <LiquidCrystal.h>

#ifdef TOUCHSCREEN
#include <MCUFRIEND_kbv.h>  // TFT Library
#include <TouchScreen.h> // Added Touchscreen support


#define MINPRESSURE 200
#define MAXPRESSURE 1000

const int XP=8,XM=A2,YP=A3,YM=9; //ID=0x9341
const int TS_LEFT=110,TS_RT=914,TS_TOP=58,TS_BOT=904;
uint8_t Orientation = ORIENTATION;
uint16_t ID, oldcolor, currentcolor;
int16_t BOXSIZE, BOXLINE;
bool touchButtonPlay, touchButtonStop, touchButtonPrev, touchButtonNext;

MCUFRIEND_kbv tft;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp; 

bool setupDisplay()
{
    uint16_t ID = tft.readID();
    if (ID == 0xD3D3) ID = 0x9481; //force ID if write-only display
    tft.begin(ID);
    tft.setRotation(1);
    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE);
    BOXSIZE = tft.width() / 4;
    BOXLINE = tft.height() - BOXSIZE;

    // Control buttons
    tft.fillRect(0, BOXLINE, BOXSIZE, BOXSIZE, CYAN);
    printAt(15, BOXLINE + 45, 1, "PREV");
    tft.fillRect(BOXSIZE, BOXLINE, BOXSIZE, BOXSIZE, GREEN);
    printAt(BOXSIZE + 15 , BOXLINE + 45, 1, "PLAY");
    tft.fillRect(BOXSIZE * 2, BOXLINE, BOXSIZE, BOXSIZE, RED);
    printAt((BOXSIZE * 2) + 15, BOXLINE + 45, 1, "STOP");
    tft.fillRect(BOXSIZE * 3, BOXLINE, BOXSIZE, BOXSIZE, YELLOW);
    printAt((BOXSIZE * 3) + 15, BOXLINE + 45, 1, "NEXT");
    // Mark active button
    tft.drawRect(0, BOXLINE, BOXSIZE, BOXSIZE, WHITE);
    currentcolor = RED;  // Red button active by default (Stop)
    delay(1000);
}

size_t printAt(int x, int y, int sz, const __FlashStringHelper *msg)
{
    tft.setTextColor(WHITE);
    tft.setCursor(x, y);
    tft.setTextSize(sz);
    tft.print(msg);
}

size_t printAt(int x, int y, int sz, const char *msg)
{
    tft.setTextColor(WHITE);
    tft.setCursor(x, y);
    tft.setTextSize(sz);
    tft.print(msg);
}

size_t printLine(const __FlashStringHelper *sz, uint8_t row)
{
    
}

size_t printLine(const char* sz, uint8_t row)
{
    
}

void printCounter(unsigned int value)
{
    
}

void printPercentages(uint8_t value)
{
    
}

void printError(const __FlashStringHelper *sz)
{
    
}

#endif

#ifndef TOUCHSCREEN
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
bool setupDisplay()
{
    lcd.begin(16, 2);
    lcd.clear();
}

size_t printAt(const __FlashStringHelper *sz, uint8_t col, uint8_t row)
{
    lcd.setCursor(col, row);
    return lcd.print(sz);
}

size_t printLine(const __FlashStringHelper *sz, uint8_t row)
{
    lcd.setCursor(0, row);
    return lcd.print(sz);
}

size_t printLine(const char* sz, uint8_t row)
{
    // clear line
    lcd.setCursor(0, row);
    lcd.print(STR_EMPTY);
    // print line
    lcd.setCursor(0, row);
    return lcd.print(sz);
}

void printCounter(unsigned int value)
{
    lcd.setCursor(13, 0);
    if (value > 999)
        lcd.print(F("999"));
    else
    {
        if (value < 10)
            lcd.print("00");
        else if (value < 100)
            lcd.print('0');
        lcd.print(value);
    }
}

void printPercentages(uint8_t value)
{
    lcd.setCursor(8, 0);
    lcd.print(value);
    lcd.print('%');
}

void printError(const __FlashStringHelper *sz)
{
    printLine(STR_ERROR, 0);
    printLine(sz, 1);
}
#endif


// Public Functions


