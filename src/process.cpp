#include "../include/process.h"


/* --------------------- */
/* ------ Process ------ */
/* --------------------- */

// Constructor
Process::Process(int8bytes processId) {
    
    // Open the process as a HANDLE
    this->processId = processId;
    this->handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

}

Process::Process(const char* processName) {

    this->processId = findProcessId("RustClient.exe");

    if (processId == -1) {
        logWrite("No process found!", true);
        std::cout << "No Process Found!\n";
        system("pause");
        return;
    }

    this->handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->processId);

}

Process::~Process() {
    CloseHandle(this->handle);
}

// Instance functions
BYTE Process::readByte(int8bytes address) {
    BYTE returnValue;
    ReadProcessMemory(this->handle, reinterpret_cast<LPCVOID>(address), &returnValue, sizeof(BYTE), NULL);
    return returnValue;
}

BYTE Process::readByte(int4bytes address) {
    return this->readByte( (int8bytes) address );
}

AOB* Process::read4Bytes(int8bytes address) {
    BYTE bytes[4];
    ReadProcessMemory(this->handle, reinterpret_cast<LPCVOID>(address), &bytes, sizeof(BYTE) * 4, NULL);
    return new AOB(bytes, 4);
}

AOB* Process::read8Bytes(int8bytes address) {
    BYTE bytes[8];
    ReadProcessMemory(this->handle, reinterpret_cast<LPCVOID>(address), &bytes, sizeof(BYTE) * 8, NULL);
    return new AOB(bytes, 8);
}

void Process::readxBytes(int8bytes address, BYTE* array, int size) {
    ReadProcessMemory(this->handle, reinterpret_cast<LPCVOID>(address), array, size, NULL);
    return;
}

void Process::writeByte(int8bytes address, BYTE value) {
    WriteProcessMemory(this->handle, reinterpret_cast<LPVOID>(address), &value, sizeof(BYTE), NULL);
}

void Process::write4bytesValue(int8bytes address, int4bytes value) {
    WriteProcessMemory(this->handle, reinterpret_cast<LPVOID>(address), &value, sizeof(int4bytes), NULL);
}

void Process::write4bytesAOB(int8bytes address, int4bytes value) {
    int4bytes effectiveValue = 0;
    int4bytes valueCopy = value;
    // Load the bytes backwards from the input into a new variable
    // This is just how instructions are handled between actual memory and cheat engine where I got the bytes
    for (int i = 0; i < 3; i++) {
        effectiveValue += valueCopy & 0xFF;
        effectiveValue <<= 8;
        valueCopy >>= 8;
    }
    effectiveValue += valueCopy & 0xFF;
    WriteProcessMemory(this->handle, reinterpret_cast<LPVOID>(address), &effectiveValue, sizeof(int4bytes), NULL);
}

void Process::write8bytesValue(int8bytes address, int8bytes value) {
    WriteProcessMemory(this->handle, reinterpret_cast<LPVOID>(address), &value, sizeof(int8bytes), NULL);
}

void Process::write8bytesAOB(int8bytes address, int8bytes value) {
    int8bytes effectiveValue = 0;
    int8bytes valueCopy = value;
    // Load the bytes backwards from the input into a new variable
    // This is just how instructions are handled between actual memory and cheat engine where I got the bytes
    for (int i = 0; i < 7; i++) {
        effectiveValue += valueCopy & 0xFF;
        effectiveValue <<= 8;
        valueCopy >>= 8;
    }
    effectiveValue += valueCopy & 0xFF;
    WriteProcessMemory(this->handle, reinterpret_cast<LPVOID>(address), &effectiveValue, sizeof(int8bytes), NULL);
}

void Process::writexbytes(int8bytes address, BYTE* bytes, int length) {
    WriteProcessMemory(this->handle, reinterpret_cast<LPVOID>(address), bytes, length, NULL);
}

int8bytes Process::aobScan(BYTE* requestedAOB, int length, int8bytes from /* default value = 0 */, int8bytes to /* default value = 0xFFFFFFFF */) {
    
    // These are indexes for searching
    int8bytes memoryReadLocation = from; // location in memory where the next aob will come from
    int memoryByte = 0; // index of the byte within the memory aobs
    int aobByte = 0; // index of the byte within the requestedAOB

    // This list stores the AOBs from memory which are being searched
    // Size is input aob length / 8 because it needs to be able to hold enough bytes to get through the requested aob
    int4bytes memoryBytesSize = 0x10040; // 0x40 bytes of overlap between reads to avoid missed bytes, this allows aobs of size < 64
    int4bytes memoryReadLocationIncrement = 0x10000; // The read memory location changes by this much every iteration
    BYTE* memoryBytes = new BYTE[memoryBytesSize];
    this->readxBytes(memoryReadLocation, memoryBytes, memoryBytesSize);

    boolean searchFound;

    while ( (memoryReadLocation + memoryBytesSize) < to ) {
        while(memoryByte < memoryReadLocationIncrement) {

            searchFound = true;
            for (; aobByte < length; aobByte++) {

                BYTE byte1 = requestedAOB[aobByte];
                BYTE byte2 = memoryBytes[memoryByte + aobByte];

                if (byte1 != byte2) {
                    searchFound = false;
                    break;
                }
            }

            // AOB found case
            if (searchFound) return (memoryReadLocation + (int8bytes) memoryByte);

            // Iteration change
            aobByte = 0;
            memoryByte++;
        }

        // Iteration change
        memoryByte = 0;
        memoryReadLocation += memoryReadLocationIncrement;

        delete[] memoryBytes;
        memoryBytes = new BYTE[memoryBytesSize];

        this->readxBytes(memoryReadLocation, memoryBytes, memoryBytesSize);
        
    }

    return -1;
}

int8bytes Process::aobScan(const BYTE* requestedAOB, int length, int8bytes from /* default value = 0 */, int8bytes to /* default value = 0xFFFFFFFF */) {
    return this->aobScan( (BYTE*) requestedAOB, length, from, to);
}

int8bytes Process::aobScanDebug(BYTE* requestedAOB, int length, int8bytes from /* default value = 0 */, int8bytes to /* default value = 0xFFFFFFFF */) {

    // These are indexes for searching
    int8bytes memoryReadLocation = from; // location in memory where the next aob will come from
    int memoryByte = 0; // index of the byte within the memory aobs
    int aobByte = 0; // index of the byte within the requestedAOB

    // This list stores the AOBs from memory which are being searched
    // Size is input aob length / 8 because it needs to be able to hold enough bytes to get through the requested aob
    int memoryBytesSize = 0x2000;
    BYTE* memoryBytes = new BYTE[memoryBytesSize];
    this->readxBytes(memoryReadLocation, memoryBytes, memoryBytesSize);

    logWrite("Starting aob scan of length: ", false);
    logWrite(length, true);

    logWrite("aob: ", false);

    for (int i = 0; i < length; i++) {
        logWriteHex((BYTE) requestedAOB[i], false);
        logWrite(" ", false);
    }

    logNewLine();

    logWrite("address: ", false);
    logWriteHex((int8bytes) from, false);
    logWrite(" -> ", false);
    logWriteHex((int8bytes) to, false);

    logWrite("\n", true);


    boolean searchFound;

    while (((unsigned long long) memoryReadLocation + (unsigned long long) memoryBytesSize) < (unsigned long long) to) {

        logWrite("starting: ", false);
        logWriteHex((int8bytes) memoryReadLocation, false);
        logWrite(" -> ", false);
        logWriteHex((int8bytes) memoryReadLocation + memoryBytesSize, true);

        while(memoryByte < memoryBytesSize - length) {

            logWrite("byte: ", false);
            logWriteHex((int8bytes) memoryByte + memoryReadLocation, false);
            logWrite("   ---   subBytes -> 1: ", false);
            logWrite(requestedAOB[0], false);
            logWrite("  2: ", false);
            logWrite(memoryBytes[memoryByte], true);

            searchFound = true;
            for (; aobByte < length; aobByte++) {

                BYTE byte1 = requestedAOB[aobByte];
                BYTE byte2 = memoryBytes[memoryByte + aobByte];

                if (byte1 != byte2) {
                    searchFound = false;
                    break;
                }
            }

            // AOB found case
            if (searchFound) return (memoryReadLocation + (int8bytes) memoryByte);

            // Iteration change
            aobByte = 0;
            memoryByte++;
        }

        // Iteration change
        memoryByte = 0;
        memoryReadLocation += memoryBytesSize;

        delete[] memoryBytes;
        memoryBytes = new BYTE[memoryBytesSize];

        this->readxBytes(memoryReadLocation, memoryBytes, 0x2000);
        
    }

    return -1;
}

int4bytes Process::getPointer4(int8bytes address, int8bytes offset /* default value = 0 */) {

    // Get bytes
    BYTE bytes[4];
    LPCVOID effectiveAddress = reinterpret_cast<LPCVOID>(address + offset);
    ReadProcessMemory(this->handle, effectiveAddress, &bytes, sizeof(BYTE) * 4, NULL);

    // Load the bytes into a long type
    // Load them in backwards because thats how theyre treated in memory
    int4bytes ret = 0;
    ret += bytes[0];
    ret += ( (int4bytes) bytes[1] ) << 8;
    ret += ( (int4bytes) bytes[2] ) << 16;
    ret += ( (int4bytes) bytes[3] ) << 24;

    return ret;
}

int8bytes Process::getPointer8(int8bytes address, int8bytes offset /* default value = 0 */) {

    // Get bytes
    BYTE bytes[8];
    LPCVOID effectiveAddress = reinterpret_cast<LPCVOID>(address + offset);
    ReadProcessMemory(this->handle, effectiveAddress, &bytes, sizeof(BYTE) * 8, NULL);

    // Load the bytes into a int8byte type
    // Load them in backwards because thats how theyre treated in memory
    int8bytes ret = 0;
    ret += bytes[0];
    ret += ( (int8bytes) bytes[1] ) << 8;
    ret += ( (int8bytes) bytes[2] ) << 16;
    ret += ( (int8bytes) bytes[3] ) << 24;
    ret += ( (int8bytes) bytes[4] ) << 32;
    ret += ( (int8bytes) bytes[5] ) << 40;
    ret += ( (int8bytes) bytes[6] ) << 48;
    ret += ( (int8bytes) bytes[7] ) << 56;
    
    return ret;
}

int8bytes Process::getModuleBase(const char* moduleName) {

    MODULEENTRY32W moduleEntry;
    moduleEntry.dwSize = sizeof(MODULEENTRY32W);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->processId);

    if (hSnapshot == INVALID_HANDLE_VALUE) {
        logWrite("Error finding process modules");
        return 0;
    }

    const char* thisModuleName;

    if (Module32FirstW(hSnapshot, &moduleEntry)) {
        do {    

            // Create a copy of the module name as a const
            thisModuleName = wCharToChar(moduleEntry.szModule);

            //logWrite(thisModuleName, true);

            // Check all the chars of the process name against the requested name
            for (int i = 0;; i++) {

                // Get current char
                char currentChar = thisModuleName[i];

                // Check the char against the requested char list
                if (currentChar != moduleName[i]) break;

                // If the char is a null char and the loop hasnt broken yet, this is the module
                if (currentChar == '\0') {

                    CloseHandle(hSnapshot);
                    delete thisModuleName;

                    return reinterpret_cast<int8bytes>(moduleEntry.modBaseAddr);

                }

            }

        } while (Module32NextW(hSnapshot, &moduleEntry));
    }

    CloseHandle(hSnapshot);
    delete thisModuleName;

    return 0;
}



/* --------------------- */
/* ------- Cheat ------- */
/* --------------------- */

Cheat::Cheat(Process* process) {

    this->process = process;
    this->injection = new Injection();
    this->hook = new Hook();

    // Find GameAssembly.dll and hook address
    this->addr.gameAssemblyDll = this->process->getModuleBase("GameAssembly.dll");
    this->addr.hookInstruction = (this->addr.gameAssemblyDll) + 0x242FBCF;

    // Do the injection
    this->doInjection();

}

Cheat::~Cheat() {

    // Deactivate hook if needed
    if (this->hook->isActive) this->toggleHook();

    // Clean up allocated memory
    void* voidCast = reinterpret_cast<void*>(this->addr.allocatedMemory);
    VirtualFreeEx(this->process->handle, voidCast, 0, MEM_RELEASE);

    logWrite("Released allocated memory.", true);
    std::cout << "Released allocated memory.\n";

    delete this->injection;
    delete this->hook;

}

void Cheat::doInjection() {

    // Try allocating
    this->addr.allocatedMemory = reinterpret_cast<int8bytes>(VirtualAllocEx(
        this->process->handle,          // Handle to Rust
        NULL,                           // Let Windows choose the address
        2048,                           // Size in bytes
        MEM_COMMIT | MEM_RESERVE,       // Allocate memory
        PAGE_EXECUTE_READWRITE          // Make it readable, writable, and executable
    ));

    // If failed
    if (this->addr.allocatedMemory == 0) {
        std::cout << "Failed to allocate memory!" << std::endl;
        logWrite("Failed to allocate memory!", true);
        return;
    }

    // Logging
    std::cout << "Allocated memory at: " << this->addr.allocatedMemory << "\n";
    logWrite("Allocated memory at: ");
    logWriteHex(this->addr.allocatedMemory, true);

    // Populate address pointers
    this->addr.data = reinterpret_cast<int8bytes>(this->addr.allocatedMemory);
    this->addr.code = reinterpret_cast<int8bytes>(this->addr.allocatedMemory) + 0x658;

    this->addr.list1Pointer =   (this->addr.data);
    this->addr.list2Pointer =   (this->addr.data) + 0x8;
    this->addr.listFlag =       (this->addr.data) + 0x10;
    this->addr.list1Start =     (this->addr.data) + 0x18;
    this->addr.list2Start =     (this->addr.data) + 0x338;

    // Inject bytes into allocated memory
    writeLittleEndian(
        this->addr.data,
        this->injection->bytes,
        this->injection->dataPointerAddressIndex
    );
    writeLittleEndian(
        (this->addr.hookInstruction)+14,
        this->injection->bytes,
        this->injection->jumpBackAddressIndex
    );
    this->process->writexbytes(
        this->addr.code, 
        this->injection->bytes, 
        this->injection->size
    );

    // Inject list pointers into data
    this->process->write8bytesValue(
        this->addr.data,            // Address
        (this->addr.data)+0x18      // Value
    );
    this->process->write8bytesValue(
        (this->addr.data)+0x8,      // Address
        (this->addr.data)+0x338     // Value
    );

}

void Cheat::toggleHook() {

    // Activate hook
    if (!this->hook->isActive) {

        writeLittleEndian(
            this->addr.code, 
            this->hook->enableBytes, 
            this->hook->JumpAddressIndex
        );
        this->process->writexbytes(
            this->addr.hookInstruction, 
            this->hook->enableBytes, 
            this->hook->size
        );

        logWrite("Activated Hook!", true);
        std::cout << "Activated Hook!\n";

        this->hook->isActive = true;

    }

    // Deactivate hook
    else {

        this->process->writexbytes(
            this->addr.hookInstruction, 
            this->hook->disableBytes, 
            this->hook->size
        );

        logWrite("Deactivated Hook!", true);
        std::cout << "Deactivated Hook!\n";

        this->hook->isActive = false;

    }

    return;
}

int Cheat::getList1Length() {

    int8bytes ret = this->process->getPointer8(this->addr.list1Pointer);
    ret -= this->addr.list1Start;
    ret /= 8;

    return (int) ret;

}

int Cheat::getList2Length() {
    
    int8bytes ret = this->process->getPointer8(this->addr.list2Pointer);
    ret -= this->addr.list2Start;
    ret /= 8;

    return (int) ret;

}

bool Cheat::getListFlag() {
    
    BYTE ret = this->process->readByte(this->addr.listFlag);

    return (bool) ret;

}

bool Cheat::flipListFlag() {

    bool listFlag = this->getListFlag();

    if (listFlag)
        this->process->writeByte(this->addr.listFlag, 0x00);
    else
        this->process->writeByte(this->addr.listFlag, 0x01);

    return listFlag;

}

CharacterList* Cheat::getList1() {

    BYTE rawBytes[800];
    this->process->readxBytes(this->addr.list1Start, rawBytes, 800);

    int8bytes* addresses = reinterpret_cast<int8bytes*>(rawBytes);

    // Flip all the pointers, since theyre stored as little endian
    int8bytes temp;
    for (int i = 0; i < 100; i++) {
        temp = addresses[i];
        writeLittleEndian(temp, rawBytes, i * 8);
    }

    CharacterList* ret = new CharacterList();

    int listLength = this->getList1Length();

    BYTE positionByteBuffer[12];
    BYTE facingByteBuffer[12];

    int8bytes positionAddress;
    int8bytes facingAddress;

    Character* newCharacter;

    for (int i = 0; i < listLength; i++) {


        positionAddress = addresses[i] + 464;   // 0x1D0
        facingAddress = addresses[i] + 836;     // 0x344

        this->process->readxBytes(positionAddress, positionByteBuffer, 12);
        this->process->readxBytes(facingAddress, facingByteBuffer, 12);

        newCharacter = new Character(addresses[i], positionByteBuffer, facingByteBuffer);
        ret->setCharacter(newCharacter, i);

    }

    return ret;

}

CharacterList* Cheat::getList2() {
    
    BYTE rawBytes[800];
    this->process->readxBytes(this->addr.list2Start, rawBytes, 800);

    int8bytes* addresses = reinterpret_cast<int8bytes*>(rawBytes);

    // Flip all the pointers, since theyre stored as little endian
    int8bytes temp;
    for (int i = 0; i < 100; i++) {
        temp = addresses[i];
        writeLittleEndian(temp, rawBytes, i * 8);
    }

    CharacterList* ret = new CharacterList();

    int listLength = this->getList2Length();

    BYTE positionByteBuffer[12];
    BYTE facingByteBuffer[12];

    int8bytes positionAddress;
    int8bytes facingAddress;

    Character* newCharacter;

    for (int i = 0; i < listLength; i++) {


        positionAddress = addresses[i] + 464;   // 0x1D0
        facingAddress = addresses[i] + 836;     // 0x344

        this->process->readxBytes(positionAddress, positionByteBuffer, 12);
        this->process->readxBytes(facingAddress, facingByteBuffer, 12);

        newCharacter = new Character(addresses[i], positionByteBuffer, facingByteBuffer);
        ret->setCharacter(newCharacter, i);

    }

    return ret;

}

void Cheat::wipeList1() {

    this->process->writexbytes(this->addr.list1Start, this->zeroBytes, 800);
    this->process->write8bytesValue(
        this->addr.data,            // Address
        (this->addr.data)+0x18      // Value
    );

}

void Cheat::wipeList2() {

    this->process->writexbytes(this->addr.list2Start, this->zeroBytes, 800);
    this->process->write8bytesValue(
        (this->addr.data)+0x8,      // Address
        (this->addr.data)+0x338     // Value
    );

}

CharacterList* Cheat::getList() {

    CharacterList* ret;

    // The byte 0x01 indicates list2, which translates to true as a bool
    bool useList2 = this->flipListFlag();

    Sleep(20);

    if (useList2) {
        ret = this->getList2();
        this->wipeList2();
    }
    
    else {
        ret = this->getList1();
        this->wipeList1();
    }

    return ret;

}
