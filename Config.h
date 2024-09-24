#ifndef __CONFIG_H__
#define __CONFIG_H__

#define TOUCHSCREEN
// PIN Configuration

#define BTN_PLAY    A2
#define BTN_STOP    A1
#define BTN_PREV    A0
#define BTN_NEXT    A3
#define BTN_MODE    INPUT_PULLUP

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// #define TS_LEFT 110
// #define TS_RT 914
// #define TS_TOP 58
// #define TS_BOT 904


#define LCD_RS      7
#define LCD_EN      6
#define LCD_D4      5
#define LCD_D5      4
#define LCD_D6      3
#define LCD_D7      2

#define SD_CHIPSELECT   10

#define SND_SPEAKER 8
#define SND_OUTPUT  9

#endif // __CONFIG_H__
