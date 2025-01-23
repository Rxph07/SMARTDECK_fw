#include "ImageManager.h"

void ImageManager::startTransfer(String input, Adafruit_RA8875 &tft)
{
    // Split header into 4 parts
    String splitInput[4];
    Utils::split(input, ';', splitInput);

    // Read position from header
    String position[2];
    Utils::split(splitInput[1], ':', position);
    Position pos = positions[position[1].toInt()];

    // Read width from header
    String widthInput[2];
    Utils::split(splitInput[2], ':', widthInput);
    int width = widthInput[1].toInt();

    // Read height from header
    String heightInput[2];
    Utils::split(splitInput[3], ':', heightInput);
    int height = heightInput[1].toInt();

    // Start image transfer
    Serial.println("SMARTDECK:READY");
    int totalPixels = width * height;
    uint16_t *values = new uint16_t[totalPixels];
    int received = Serial.readBytes((byte *)values, totalPixels * sizeof(uint16_t));

    if (received == totalPixels * sizeof(uint16_t))
    {
        for (int y = 0; y < height; y++)
        {
            tft.drawPixels(values + y * width, width, pos.xpos, y + pos.ypos);
        }
    }
    else
    {
        Serial.println("SMARTDECK:NACK");
    }

    delete[] values;
    Serial.println("SMARTDECK:ACK");
}

void ImageManager::startMassTransfer(String input, Adafruit_RA8875 &tft)
{
    // Split header into 5 parts
    String splitInput[5];
    Utils::split(input, ';', splitInput);

    // Read image amount from header
    String amountInput[2];
    Utils::split(splitInput[1], ':', amountInput);
    int amount = amountInput[1].toInt();

    // Read positions from header
    String positionsInput[amount + 1];
    Utils::split(splitInput[2], ':', positionsInput);

    // Read width from header
    String widthInput[2];
    Utils::split(splitInput[3], ':', widthInput);
    int width = widthInput[1].toInt();

    // Read height from header
    String heightInput[2];
    Utils::split(splitInput[4], ':', heightInput);
    int height = heightInput[1].toInt();

    // Start image transfer
    Serial.println("SMARTDECK:READY");
    int totalPixels = width * height * amount;
    uint16_t *values = new uint16_t[totalPixels];
    int received = Serial.readBytes((byte *)values, totalPixels * sizeof(uint16_t));

    if (received == totalPixels * sizeof(uint16_t))
    {
        for (int i = 0; i < amount; i++)
        {
            for (int y = 0; y < height; y++)
            {
                tft.drawPixels(values + y * width + i * width * height, width, positions[positionsInput[i + 1].toInt()].xpos, y + positions[positionsInput[i + 1].toInt()].ypos);
            }
        }
    }
    else
    {
        Serial.println("SMARTDECK:NACK");
    }

    delete[] values;
    Serial.println("SMARTDECK:ACK");
}