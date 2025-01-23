#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include "Adafruit_RA8875.h"
#include "EEPROM.h"

#define BUTTON_SIZE 72

struct Position
{
    int xpos;
    int ypos;
};

const Position positions[] = {
    {20, 14},
    {112, 14},
    {204, 14},
    {296, 14},
    {388, 14},
    {20, 100},
    {112, 100},
    {204, 100},
    {296, 100},
    {388, 100},
    {20, 186},
    {112, 186},
    {204, 186},
    {296, 186},
    {388, 186},
    {0, 0},
};

class Utils
{
public:
    static int split(const String &input, char delimiter, String result[]);
    static bool readCalibration(int location, tsMatrix_t *matrixPtr);
    static int calibrateTSPoint(tsPoint_t *displayPtr, tsPoint_t *screenPtr, tsMatrix_t *matrixPtr);
    static int getButtonNumber(int touchX, int touchY);
};

#endif
