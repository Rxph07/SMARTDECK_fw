#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include "Utils.h"
#include "ImageManager.h"

#define RA8875_INT 46
#define RA8875_CS 10
#define RA8875_RESET 9

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;

void waitForConnection();

bool isConnected = false;
String version = "1.0.0-alpha";

void setup()
{
    Serial.begin(6000000);

    if (!tft.begin(RA8875_480x272))
        return;

    tft.displayOn(true);
    tft.GPIOX(true);
    tft.PWM1config(true, RA8875_PWM_CLK_DIV1024);
    tft.PWM1out(255);

    tft.fillScreen(RA8875_BLACK);
}

void loop()
{
    if (!Serial.available())
        return;

    if (!isConnected)
        waitForConnection();

    String input = Serial.readStringUntil('\n');
    input.trim();

    if (Utils::contains(input, "SMARTDECK:DISCONNECT"))
    {
        isConnected = false;
    }
    else if (Utils::contains(input, "SMARTDECK:IMAGE_TRANSFER"))
    {
        ImageManager::startTransfer(input, tft);
    }
    else if (Utils::contains(input, "SMARTDECK:MASS_IMAGE_TRANSFER"))
    {
        ImageManager::startMassTransfer(input, tft);
    }
}

void waitForConnection()
{
    while (1)
    {
        String receivedMessage = Serial.readStringUntil('\n');
        if (receivedMessage.equals("SMARTDECK:PING"))
        {
            Serial.println("SMARTDECK:READY;VER:" + version);
            isConnected = true;
            return;
        }
    }
}