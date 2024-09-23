#include "Config.h"
#include "Texts.h"
#include "Display.h"
#include "SDCard.h"
#include "Sound.h"

#ifndef TOUCHSCREEN

#include "Button.h"

// Global Object

Button<BTN_PLAY> buttonPlay(BTN_MODE);
Button<BTN_STOP> buttonStop(BTN_MODE);
Button<BTN_PREV> buttonPrev(BTN_MODE);
Button<BTN_NEXT> buttonNext(BTN_MODE);

// Player State
#endif


#define MODE_BROWSE         0
#define MODE_PLAYING        1
#define MODE_PAUSED         2
#define MODE_HALTED         3

uint8_t playerMode = MODE_BROWSE;

unsigned long counterTime = 0;
uint16_t counter = 0;
uint8_t percentages = 0;

// TZX Interface

void TZXPlay();
bool TZXLoop();
void TZXStop();

//
// Functions
//

void printFileInfo()
{
    if (*getFileName())
    {
        if (isDir())
            printLine(STR_DIR, 0);
        else
            printLine(STR_FILE, 0);
        printLine(getFileName(), 1);
    }
    else
    {
        // this DIR is empty
        printLine(STR_DIR, 0);
        printLine(STR_EMPTY_DIR, 1);
    }
}

void setPlayerMode(uint8_t newPlayerMode)
{
    if (playerMode != MODE_HALTED)
        playerMode = newPlayerMode;
}

void stopFile()
{
    TZXStop();
    if (playerMode == MODE_PLAYING || playerMode == MODE_PAUSED)
    {
        printLine(STR_STOPPED_FULL, 0);
        setPlayerMode(MODE_BROWSE);
    }
}

//
// Setup
//

void setup()
{
    // LCD

    setupDisplay();

    // SD

    if (setupSD(SD_CHIPSELECT))
    {
        nextFile();
        printFileInfo();
    }
    else
    {
        printError(STR_ERR_NOSDCARD);
        setPlayerMode(MODE_HALTED);
    }

    // Sound Output

    setupSound();
}

//
// Loops and Modes
//

void loopBrowse()
{
  #ifndef TOUCHSCREEN
    buttonPlay.tick();
    buttonStop.tick();
    buttonPrev.tick();
    buttonNext.tick();

    if (buttonPlay.press())
    {
        if (isDir())
        {
            if (childDir())
            {
                nextFile();
                printFileInfo();
            }
        }
        else
        {
            if (isFileExists())
            {
                setPlayerMode(MODE_PLAYING);
                counterTime = millis();
                counter = 0;
                percentages = UINT8_MAX;    // set unreal value for first printing
                TZXPlay();
                printLine(STR_PLAYING_FULL, 0);
                printPercentages(0);
                printCounter(0);
                printLine(getFileName(), 1);
            }
            else
                printError(STR_ERR_NOFILE);
        }
    }

    if (buttonStop.press())
    {
        if (parentDir())
        {
            nextFile();
            printFileInfo();
        }
    }

    if (buttonPrev.press())
    {
        prevFile();
        printFileInfo();
    }

    if (buttonNext.press())
    {
        nextFile();
        printFileInfo();
    }
  #endif

}
  
    
void loopPlaying()
{
    // TZXLoop only runs if a file is playing, and keeps the buffer full.
    if (TZXLoop())
    {
        // percents of playing and right-side counter 000
        uint8_t newPercentages = (100 * bytesRead) / getFileSize();
        if (percentages != newPercentages)
        {
            percentages = newPercentages;
            printPercentages(percentages);
        }

        if (percentages < 100)
        {
            // check switch every second
            if (millis() - counterTime > 1000)
            {
                counterTime = millis();
                printCounter(counter++);
            }
        }
    }
#ifndef TOUCHSCREEN
    buttonPlay.tick();
    buttonStop.tick();

    if (buttonPlay.press())
    {
        printAt(STR_PAUSED_8, 0, 0);
        setPlayerMode(MODE_PAUSED);
    }

    if (buttonStop.press())
        stopFile();
#endif

}

void loopPaused()
{
    // Part of TZXLoop
    noInterrupts();
    pauseSound(HIGH);
    interrupts();
#ifndef TOUCHSCREEN
    buttonPlay.tick();
    buttonStop.tick();

    if (buttonPlay.press())
    {
        printAt(STR_PLAYING_8, 0, 0);
        setPlayerMode(MODE_PLAYING);
    }

    if (buttonStop.press())
        stopFile();
#endif
}

void loop(void)
{
    if (playerMode != MODE_PLAYING)
        sound(LOW);

    switch (playerMode)
    {
    case MODE_BROWSE: loopBrowse(); break;
    case MODE_PLAYING: loopPlaying(); break;
    case MODE_PAUSED: loopPaused(); break;
    case MODE_HALTED: delay(1000); break;
    }
}
