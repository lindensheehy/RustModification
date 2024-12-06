#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <windows.h>

class AOB {
    public:

        // Instance variables
        BYTE* bytes;
        int length;

        // Contructor
        AOB(long long bytes);

        AOB(BYTE* bytes, int length);

        // Destructor
        ~AOB();

        // Instance functions
        void pushBackBytes(long long bytes);

        BYTE getByte(int index);

        std::string toString();

    private:
};
