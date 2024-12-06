#pragma once

#include <iostream>
#include <windows.h>
#include <iomanip>
#include <sstream>
#include <string>

#include "log.h"
#include "aob.h"
#include "utility.h"
#include "character.h"

class Process {

    /*
        This is pretty much a wrapper for the windows API calls.
        syntax is more straightforward and easier to use
        also has AOBscan
    */

    public:

        // Instance variables
        int8bytes processId;
        HANDLE handle;

        // Constructors
        Process(uintptr_t processId);
        Process(const char* processName);

        ~Process();

        // Instance functions
        BYTE readByte(int8bytes address);
        BYTE readByte(int4bytes address);

        AOB* read4Bytes(int8bytes address);
        AOB* read8Bytes(int8bytes address);

        void readxBytes(int8bytes address, BYTE* array, int size);

        void writeByte(int8bytes address, BYTE value);

        void write4bytesValue(int8bytes address, int4bytes value);
        void write4bytesAOB(int8bytes address, int4bytes value);

        void write8bytesValue(int8bytes address, int8bytes value);
        void write8bytesAOB(int8bytes address, int8bytes value);

        void writexbytes(int8bytes address, BYTE* bytes, int length);

        int8bytes aobScan(BYTE* requestedAOB, int length, int8bytes from = 0, int8bytes to = 0xFFFFFFFFFFFFFFFF);
        int8bytes aobScan(const BYTE* requestedAOB, int length, int8bytes from = 0, int8bytes to = 0xFFFFFFFFFFFFFFFF);
        int8bytes aobScanDebug(BYTE* requestedAOB, int length, int8bytes from = 0, int8bytes to = 0xFFFFFFFFFFFFFFFF);

        int4bytes getPointer4(int8bytes address, int8bytes offset = 0);
        int8bytes getPointer8(int8bytes address, int8bytes offset = 0);

        int8bytes getModuleBase(const char* moduleName);

    private:
    
};

class Cheat {

    /*
        This is a wrapper for the Process class.
        this class is much more specialized for this specific program, while Process is very general
    */

    public:

        // Subclasses
        class Injection {

            public:

                // Values for the executable portion of the injection
                const int size = 191;

                BYTE* bytes;

                // Indexes of data which needs to be overwritten
                const int dataPointerAddressIndex = 6;
                const int jumpBackAddressIndex = size - 8; // last 8 bytes

                Injection() {

                    // List assignment
                    this->bytes = new BYTE[this->size] {

                        // Store registers
                        0x50,                                                           // push     rax
                        0x51,                                                           // push     rcx
                        0x52,                                                           // push     rdx
                        0x56,                                                           // push     rsi

                        // My code
                        0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // mov      rax,        address         (insert injected data base address here)
                        0x48, 0x8B, 0xF0,                                               // mov      rsi,        rax
                        0x8A, 0x48, 0x10,                                               // mov      cl,         [rax+0x10]
                        0x84, 0xC9,                                                     // test     cl,         cl
                        0x48, 0x8B, 0xC8,                                               // mov      rcx,        rax
                        0x0F, 0x85, 0x36, 0x00, 0x00, 0x00,                             // jne      +0x36
                        0x48, 0x81, 0xC1, 0x38, 0x03, 0x00, 0x00,                       // add      rcx,        0x338
                        0x48, 0x8B, 0x16,                                               // mov      rdx,        [rsi]
                        0x48, 0x39, 0xCA,                                               // cmp      rdx,        rcx
                        0x0F, 0x84, 0x6D, 0x00, 0x00, 0x00,                             // je       +0x6D
                        0x48, 0x8B, 0xC8,                                               // mov      rcx,        rax
                        0x48, 0x83, 0xC1, 0x18,                                         // add      rcx,        0x18
                        0x48, 0x83, 0xEA, 0x08,                                         // sub      rdx,        0x8
                        0x48, 0x39, 0xCA,                                               // cmp      rdx,        rcx
                        0x0F, 0x8C, 0x4C, 0x00, 0x00, 0x00,                             // jl       +0x4C
                        0x48, 0x39, 0x1A,                                               // cmp      [rdx],      rbx
                        0x0F, 0x84, 0x50, 0x00, 0x00, 0x00,                             // je       +0x50
                        0x48, 0x83, 0xEA, 0x08,                                         // sub      rdx,        0x8
                        0xEB, 0xE8,                                                     // jmp      -0x18
                        0x48, 0x83, 0xC6, 0x08,                                         // add      rsi,        0x8
                        0x48, 0x81, 0xC1, 0x58, 0x06, 0x00, 0x00,                       // add      rcx,        0x658
                        0x48, 0x8B, 0x16,                                               // mov      rdx,        [rsi]
                        0x48, 0x39, 0xCA,                                               // cmp      rdx,        rcx
                        0x0F, 0x84, 0x33, 0x00, 0x00, 0x00,                             // je       +0x33
                        0x48, 0x8B, 0xC8,                                               // mov      rcx,        rax
                        0x48, 0x81, 0xC1, 0x38, 0x03, 0x00, 0x00,                       // add      rcx,        0x338
                        0x48, 0x83, 0xEA, 0x08,                                         // sub      rdx,        0x8
                        0x48, 0x39, 0xCA,                                               // cmp      rdx,        rcx
                        0x0F, 0x8C, 0x0F, 0x00, 0x00, 0x00,                             // jl       +0xF
                        0x48, 0x39, 0x1A,                                               // cmp      [rdx],      rbx
                        0x0F, 0x84, 0x13, 0x00, 0x00, 0x00,                             // je       +0x13
                        0x48, 0x83, 0xEA, 0x08,                                         // sub      rdx,        0x8
                        0xEB, 0xE8,                                                     // jmp      -0x18
                        0x48, 0x8B, 0x0E,                                               // mov      rcx,        [rsi]
                        0x48, 0x89, 0x19,                                               // mov      [rcx],      rbx
                        0x48, 0x83, 0xC1, 0x08,                                         // add      rcx,        0x8
                        0x48, 0x89, 0x0E,                                               // mov      [rsi],      rcx

                        // Restore registers
                        0x5E,                                                           // pop rsi
                        0x5A,                                                           // pop rdx
                        0x59,                                                           // pop rcx
                        0x58,                                                           // pop rax

                        // Original code
                        0xF2, 0x0F, 0x11, 0x83, 0xD0, 0x01, 0x00, 0x00,                 // movsd    [rbx+000001D0],     xmm0
                        0x89, 0x83, 0xD8, 0x01, 0x00, 0x00,                             // mov      [rbx+000001D8],     eax

                        // Jump back
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // jmp GameAssembly.dll+11BA71F     (needs to be filled dynamically)

                    };


                }

                ~Injection() {
                    delete[] this->bytes;
                }

        };

        class Hook {

            public:

                static const int size = 14;

                BYTE* enableBytes;
                BYTE* disableBytes;

                // Indexes of data which needs to be overwritten
                const int JumpAddressIndex = 6;

                bool isActive = false;

                Hook() {

                    enableBytes = new BYTE[this->size] {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,                 // jmp instruction
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00      // destination address (little endian)
                    };

                    disableBytes = new BYTE[this->size] {
                        0xF2, 0x0F, 0x11, 0x83, 0xD0, 0x01, 0x00, 0x00,                 // movsd    [rbx+000001D0],     xmm0
                        0x89, 0x83, 0xD8, 0x01, 0x00, 0x00,                             // mov      [rbx+000001D8],     eax     
                    };

                }

                ~Hook() {
                    delete[] enableBytes;
                    delete[] disableBytes;
                }

        };

        // Struct to group addresses
        struct Addresses {

            // General
            int8bytes gameAssemblyDll;
            int8bytes hookInstruction;
            int8bytes allocatedMemory;
            int8bytes data;
            int8bytes code;

            // Pointers to variables
            int8bytes list1Pointer;
            int8bytes list2Pointer;
            int8bytes listFlag;
            int8bytes list1Start;
            int8bytes list2Start;

        };

        Process* process;
        Injection* injection;
        Hook* hook;

        Addresses addr;

        // Contructor
        Cheat(Process* process);

        // Destructor
        ~Cheat();

        // Instance Functions
        void doInjection();

        void toggleHook();

        // Getters for internal data
        int getList1Length();
        int getList2Length();

        // Returns the list flag
        bool getListFlag();

        // Flips the list flag and returns the old value
        bool flipListFlag();

        CharacterList* getList1();
        CharacterList* getList2();

        void wipeList1();
        void wipeList2();

        CharacterList* getList();

    private:

        // 800 BYTEs set to 0x00
        BYTE zeroBytes[800] = {};

};