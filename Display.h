#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <Arduino.h>


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