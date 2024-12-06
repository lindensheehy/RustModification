#pragma once

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

#include "log.h"
#include "aob.h"
#include "process.h"

// Write the given value as little endian (backwards) into the array. NOTE: this function is NOT safe, it does not check array bounds
void writeLittleEndian(int8bytes value, BYTE* target, int startIndex);

char* wCharToChar(wchar_t* wChar);

int8bytes findProcessId(const char* processName);