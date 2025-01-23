#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include "Utils.h"
#include "ImageManager.h"

#define RA8875_INT 46
#define RA8875_CS 10
#define RA8875_RESET 9

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;
tsMatrix_t _tsMatrix;

#define EEPROM_SIZE 512
#define EEPROM_LOCATION 100

#define BUFFER_SIZE 256
char buffer[BUFFER_SIZE];
int bufferIndex;

#define DEBOUNCE_DELAY 500
long lastTouch = 0;
bool buttonPressed = false;

bool isConnected = false;
String version = "1.1.0-beta";

void waitForConnection();
void handleCommand(char *command);

void setup()
{
    Serial.begin(6000000);

    if (!EEPROM.begin(EEPROM_SIZE) || !tft.begin(RA8875_480x272))
        while (1)
            ;

    tft.displayOn(true);
    tft.GPIOX(true);
    tft.PWM1config(true, RA8875_PWM_CLK_DIV1024);
    tft.PWM1out(255);

    tft.touchEnable(true);
    tft.fillScreen(RA8875_BLACK);
    digitalWrite(RA8875_INT, HIGH);

    if (!Utils::readCalibration(EEPROM_LOCATION, &_tsMatrix))
        while (1)
            ;
}

void loop()
{
    if (!isConnected)
        waitForConnection();

    if (Serial.available() > 0)
    {
        char c = Serial.read();

        if (c == '\n')
        {
            buffer[bufferIndex] = '\0';
            handleCommand(buffer);
            bufferIndex = 0;
        }
        else
        {
            if (bufferIndex < BUFFER_SIZE - 1)
                buffer[bufferIndex++] = c;
        }
    }

    if (tft.touched())
    {
        tsPoint_t raw;
        tsPoint_t calibrated;

        uint16_t x, y;
        tft.touchRead(&x, &y);
        raw.x = x;
        raw.y = y;

        if (Utils::calibrateTSPoint(&calibrated, &raw, &_tsMatrix) != 0)
            return;

        int buttonNumber = Utils::getButtonNumber(calibrated.x, calibrated.y);
        if (buttonNumber < 0 && buttonNumber >= 15)
            return;

        if (buttonPressed)
            return;

        String data = "SMARTDECK:BUTTON_PRESS;BUTTON:" + String(buttonNumber);
        Serial.println(data);
        buttonPressed = true;
        lastTouch = millis();
    }
    else
    {
        if (buttonPressed && (millis() - lastTouch) > DEBOUNCE_DELAY)
            buttonPressed = false;
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

void handleCommand(char *command)
{
    if (strstr(command, "SMARTDECK:DISCONNECT"))
    {
        isConnected = false;
        tft.fillScreen(RA8875_BLACK);
    }
    else if (strstr(command, "SMARTDECK:IMAGE_TRANSFER"))
    {
        ImageManager::startTransfer(command, tft);
    }
    else if (strstr(command, "SMARTDECK:MASS_IMAGE_TRANSFER"))
    {
        ImageManager::startMassTransfer(command, tft);
    }
}