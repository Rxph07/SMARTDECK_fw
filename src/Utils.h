#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

class Utils
{
public:
    static bool contains(const String &string1, const String &string2);
    static int split(const String &input, char delimiter, String result[]);
};

#endif
