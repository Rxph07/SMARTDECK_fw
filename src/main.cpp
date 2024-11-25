#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"

#define RA8875_INT 46
#define RA8875_CS 10
#define RA8875_RESET 9

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;

int splitString(String str, char delimiter, String resultArray[]);

struct Position
{
    int xpos;
    int ypos;
};

// Vordefinierte Positionen (1–15)
const Position positions[] = {
    {20, 14},   // Position 1
    {112, 14},  // Position 2
    {204, 14},  // Position 3
    {296, 14},  // Position 4
    {388, 14},  // Position 5
    {20, 100},  // Position 6
    {112, 100}, // Position 7
    {204, 100}, // Position 8
    {296, 100}, // Position 9
    {388, 100}, // Position 10
    {20, 186},  // Position 11
    {112, 186}, // Position 12
    {204, 186}, // Position 13
    {296, 186}, // Position 14
    {388, 186}  // Position 15
};

void setup()
{
    Serial.begin(4000000);
    Serial.println("RA8875 start");

    if (!tft.begin(RA8875_480x272))
    {
        Serial.println("RA8875 Not Found!");
        while (1)
            ;
    }

    Serial.println("Found RA8875");

    tft.displayOn(true);
    tft.GPIOX(true);                              // Enable TFT - display enable tied to GPIOX
    tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
    tft.PWM1out(255);

    tft.fillScreen(RA8875_WHITE);
}

void loop()
{
    if (Serial.available())
    {
        String input = Serial.readStringUntil('\n');
        input.trim();

        if (input.indexOf("SendingImage") >= 0)
        {
            String splitInput[3];
            splitString(input, ':', splitInput);

            String dimensions[2];
            splitString(splitInput[1], 'x', dimensions);

            int pos = splitInput[2].toInt();

            int width = dimensions[0].toInt();
            int height = dimensions[1].toInt();

            int totalPixels = width * height;
            uint16_t *values = new uint16_t[totalPixels];

            // Lese die gesamten Bilddaten auf einmal ein
            Serial.readBytes((byte *)values, totalPixels * sizeof(uint16_t));

            // Zeichne das Bild zeilenweise
            for (int y = 0; y < height; y++)
            {
                tft.drawPixels(values + y * width, width, positions[pos].xpos, y + positions[pos].ypos);
            }
            delete[] values;
        }
    }
}

int splitString(String str, char delimiter, String resultArray[])
{
    int index = 0;
    int startIndex = 0;

    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == delimiter)
        {
            resultArray[index++] = str.substring(startIndex, i);
            startIndex = i + 1;
            if (index >= 3)
            {
                break;
            }
        }
    }

    resultArray[index++] = str.substring(startIndex);
    return index;
}