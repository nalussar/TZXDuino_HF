#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <Arduino.h>

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define ORIENTATION   1

bool setupDisplay();
size_t printAt(int x, int y, int sz, const __FlashStringHelper *msg);
size_t printAt(int x, int y, int sz, const char *msg);
size_t printAt(const __FlashStringHelper *sz, uint8_t col, uint8_t row);
size_t printLine(const __FlashStringHelper *sz, uint8_t row);
size_t printLine(const char* sz, uint8_t row);
void printCounter(unsigned int value);
void printPercentages(uint8_t value);
void printError(const __FlashStringHelper *sz);

#endif // __DISPLAY_H__