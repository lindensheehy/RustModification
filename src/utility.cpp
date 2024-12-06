#include "../include/utility.h"

void writeLittleEndian(int8bytes value, BYTE* target, int startIndex) {

    int8bytes valueCopy = value;

    for (int i = 0; i < 8; ++i) {

        target[i + startIndex] = valueCopy & 0xFF;
        valueCopy >>= 8;

    }

}

char* wCharToChar(wchar_t* wChar) {

    // Check for null input
    if (wChar == nullptr) return nullptr;

    // Find the length of the wide string
    int length = 0;
    for (; wChar[length] != L'\0'; length++);

    // Allocate memory for the char* string (add 1 for null terminator)
    char* charStr = new char[length + 1];

    // Populate the char string from the wchar string
    for (int i = 0; i < length; ++i) {
        wchar_t wideChar = wChar[i];
        if (wideChar > 0x7F) {
            // Handle non-ASCII characters; this assumes the data can be lossy
            charStr[i] = '?'; // Replace non-ASCII characters with '?'
        } else {
            charStr[i] = (char)wideChar; // Safe cast for ASCII characters
        }
    }

    // Null-terminate the char string
    charStr[length] = '\0';

    return charStr;

}

int8bytes findProcessId(const char* processName) {

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot == INVALID_HANDLE_VALUE) {
        logWrite("Error creating process snapshot");
        return 0;
    }

    const char* thisProcessName;

    if (Process32First(hSnapshot, &processEntry)) {
        do {

            // Create a copy of the process name as a const
            thisProcessName = (char*) processEntry.szExeFile;
            
            //logWrite(thisProcessName, true);

            // Check all the chars of the process name against the requested name
            for (int i = 0;; i++) {

                // Get current char
                char currentChar = thisProcessName[i];

                // Check the char against the requested char list
                if (currentChar != processName[i]) break;

                // If the char is a null char and the loop hasnt broken yet, this is the process
                if (currentChar == '\0') {
                    CloseHandle(hSnapshot);
                    return processEntry.th32ProcessID;
                }

            }

        // If the loop broke before hitting return, get next process and repeat
        } while (Process32Next(hSnapshot, &processEntry));
    }

    CloseHandle(hSnapshot);

    return -1;
    
}
