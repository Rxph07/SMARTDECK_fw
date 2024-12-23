#include "Utils.h"

bool Utils::contains(const String &string1, const String &string2)
{
    return string1.indexOf(string2) != -1;
}

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
