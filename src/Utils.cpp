#include "Utils.h"

int Utils::split(const String &input, char delimiter, String result[])
{
    int count = 0;
    int startIndex = 0;

    while (true)
    {
        int delimiterIndex = input.indexOf(delimiter, startIndex);
        if (delimiterIndex == -1)
        {
            result[count++] = input.substring(startIndex);
            break;
        }

        result[count++] = input.substring(startIndex, delimiterIndex);
        startIndex = delimiterIndex + 1;
    }

    return count;
}

bool Utils::readCalibration(int location, tsMatrix_t *matrixPtr)
{
    if (matrixPtr == nullptr)
        return false;

    for (size_t i = 0; i < sizeof(tsMatrix_t); i++)
    {
        *((uint8_t *)matrixPtr + i) = EEPROM.read(location + i);
    }

    return true;
}

int Utils::calibrateTSPoint(tsPoint_t *displayPtr, tsPoint_t *screenPtr, tsMatrix_t *matrixPtr)
{
    if (matrixPtr->Divider != 0)
    {
        displayPtr->x = ((matrixPtr->An * screenPtr->x) +
                         (matrixPtr->Bn * screenPtr->y) +
                         matrixPtr->Cn) /
                        matrixPtr->Divider;

        displayPtr->y = ((matrixPtr->Dn * screenPtr->x) +
                         (matrixPtr->En * screenPtr->y) +
                         matrixPtr->Fn) /
                        matrixPtr->Divider;
        return 0;
    }
    return -1;
}

int Utils::getButtonNumber(int touchX, int touchY)
{
    for (int i = 0; i < 15; i++)
    {
        int xStart = positions[i].xpos;
        int xEnd = xStart + BUTTON_SIZE;
        int yStart = positions[i].ypos;
        int yEnd = yStart + BUTTON_SIZE;

        if (touchX >= xStart && touchX <= xEnd && touchY >= yStart && touchY <= yEnd)
        {
            return i;
        }
    }
    return -1;
}