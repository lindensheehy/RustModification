:: Build libraries

:: Paths
set srcdir=../src/
set objdir=../obj/


:: Log
g++ -c %srcdir%log.cpp ^
    -o %objdir%log.o

:: Math
g++ -c %srcdir%math.cpp ^
    -o %objdir%math.o

:: Utility
g++ -c %srcdir%utility.cpp ^
    -o %objdir%utility.o

:: Vec
g++ -c %srcdir%vec.cpp ^
    -o %objdir%vec.o

:: AOB
g++ -c %srcdir%aob.cpp ^
    -o %objdir%aob.o

:: Process
g++ -c %srcdir%process.cpp ^
    -o %objdir%process.o

:: Character
g++ -c %srcdir%character.cpp ^
    -o %objdir%character.o

:: Character
g++ -c %srcdir%projection.cpp ^
    -o %objdir%projection.o


:: Compile main.exe
g++ %srcdir%main.cpp ^
    %objdir%log.o ^
    %objdir%math.o ^
    %objdir%utility.o ^
    %objdir%vec.o ^
    %objdir%aob.o ^
    %objdir%process.o ^
    %objdir%character.o ^
    %objdir%projection.o ^
    -lgdi32 ^
    -lgdiplus ^
    -o main.exe


:: pause for debugging purposes
pause
