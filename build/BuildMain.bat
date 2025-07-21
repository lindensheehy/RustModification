:: Build libraries

:: Paths
set objdir=../obj/


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
