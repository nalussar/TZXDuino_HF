#include "Config.h"
#include "Display.h"
#include "Button.h"
#include "SDCard.h"
#include "Texts.h"

// Global Object

Button<BTN_PLAY> buttonPlay(INPUT);
Button<BTN_STOP> buttonStop(INPUT);
Button<BTN_UP>   buttonUp(INPUT);
Button<BTN_DOWN> buttonDown(INPUT);

// Player State

#define MODE_BROWSE         0
#define MODE_PLAYING        1
#define MODE_PAUSED         2

uint8_t playerMode = MODE_BROWSE;

unsigned long counterTime = 0;
uint16_t counter = 0;
uint8_t percentages = 0;

// TZX Interface

void TZXSetup();
void TZXPlay();
void TZXLoop();
void TZXStop();

extern byte isStopped;

//
// Sound Functions
//

void sound(uint8_t val)
{
    digitalWrite(SND_OUTPUT, val);
}

//
// LCD Functions
//

void printFileInfo()
{
    if (isDir())
        printLine(STR_DIR, 0);
    else
        printLine(STR_FILE, 0);
    printLine(getFileName(), 1);
}

//
// Global File Operations
//

bool isFileStopped()
{
    return playerMode != MODE_PLAYING;
}

void stopFile()
{
    TZXStop();
    if (playerMode == MODE_PLAYING || playerMode == MODE_PAUSED)
    {
        printLine(STR_STOPPED_FULL, 0);
        playerMode = MODE_BROWSE;
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
        printLine(STR_ERR_NOSDCARD, 0);

    // Sound Output

    pinMode(SND_OUTPUT, OUTPUT);
    sound(LOW);

    //Setup TZX specific options

    TZXSetup();
}

//
// Loops and Modes
//

void loopBrowse()
{
    sound(LOW);

    buttonPlay.tick();
    buttonStop.tick();
    buttonUp.tick();
    buttonDown.tick();

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
                playerMode = MODE_PLAYING;
                counterTime = millis();
                counter = 0;
                percentages = UINT8_MAX;    // set unreal value for first printing
                TZXPlay();
                printLine(STR_PLAYING_FULL, 0);
                printLine(getFileName(), 1);
            }
            else
            {
                printLine(STR_ERR_NOFILE, 1);
            }
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

    if (buttonUp.press())
    {
        prevFile();
        printFileInfo();
    }

    if (buttonDown.press())
    {
        nextFile();
        printFileInfo();
    }
}

void loopPlaying()
{
    //TZXLoop only runs if a file is playing, and keeps the buffer full.
    TZXLoop();

    buttonPlay.tick();
    buttonStop.tick();

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

    if (buttonPlay.press())
    {
        printAt(STR_PAUSED_8, 0, 0);
        playerMode = MODE_PAUSED;
    }

    if (buttonStop.press())
        stopFile();
}

void loopPaused()
{
    // Part of TZXLoop
    isStopped = isFileStopped();

    sound(LOW);

    buttonPlay.tick();
    buttonStop.tick();

    if (buttonPlay.press())
    {
        printAt(STR_PLAYING_8, 0, 0);
        playerMode = MODE_PLAYING;
    }

    if (buttonStop.press())
        stopFile();
}

void loop(void)
{
    switch (playerMode)
    {
    case MODE_BROWSE: loopBrowse(); break;
    case MODE_PLAYING: loopPlaying(); break;
    case MODE_PAUSED: loopPaused(); break;
    }
}
