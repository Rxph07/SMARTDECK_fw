#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <Arduino.h>
#include "Adafruit_RA8875.h"
#include "Utils.h"

class ImageManager
{
public:
    static void startTransfer(String input, Adafruit_RA8875 &tft);
    static void startMassTransfer(String input, Adafruit_RA8875 &tft);
};

#endif
