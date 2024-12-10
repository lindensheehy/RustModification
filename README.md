# **Rust GUI Overlay Modification**

This project demonstrates the development of a graphical overlay tool for the game **Rust**, designed to render information dynamically over the game window. The goal was to create a proof-of-concept ESP (Extra Sensory Perception) overlay that identifies in-game entities such as players and scientists. While not intended to bypass anti-cheat mechanisms, the project was built as a learning experience and functions exclusively in private server environments.

## **Disclaimer**
⚠️ **This tool was developed and tested exclusively in a private server environment.**
Using this tool on public servers **can and will** result in game bans.
I have not attempted to use this tool while the anti-cheat is running. Because I made no effort to bypass the anti-cheat, as that was never the intent of this project, this tool is not subtle at all with its memory access and therefore would most definitely be detected.

**I am not responsible for any consequences arising from the use of this project. This is intended solely for educational purposes. Please use responsibly and only in environments where it is explicitly permitted.**

## **Project Highlights**
This project involved:
- **Reverse Engineering**: Used Cheat Engine (a memory debugger) to analyze game memory and locate entity positional data.
- **Assembly Injection**: Wrote custom assembly code to hook game instructions and extract unique memory addresses.
- **Windows API Integration**: Built the overlay using **Win32 API** and **GDI+** for lightweight rendering.
- **Real-Time Data Processing**: Implemented double-buffered shared memory to minimize data races and ensure smooth operation.
- **External Overlay Rendering**: Dynamically transformed positional data into graphical elements drawn over the game window.

## **Development Process**
1. **Reverse Engineering with Cheat Engine**
   - Analyzed game memory over 10-12 hours to locate a key instruction accessing positional data for players and scientists.
   - Discovered the instruction used post-culling data, which provided visible positions but excluded data occluded by terrain or structures.
   - Despite limitations, leveraged this data as a foundation for the project.

2. **Static Memory Addressing in C++**
   - Transitioned from Cheat Engine to C++, using some resources from my Plants Vs. Zombies modding API, including a Windows API wrapper and helper functions for memory manipulation.
   - Located the instruction in memory by finding the base address of the `GameAssembly.dll` module and using static offsets.

3. **Assembly Hooking**
   - Wrote assembly code to hook the target instruction and intercept data.
   - Created logic to implement a set (only containing unique elements) in memory, capturing base addresses for in-game entities.
   - Tested the hook using Cheat Engine's auto assembler before porting the final bytecode to C++ arrays for injection.

4. **Data Handling**
   - Designed a double-buffered memory structure to store positional data and avoid race conditions.
   - Built C++ structures to read, process, and organize extracted data into a format suitable for external rendering.

5. **Overlay Development**
   - Used **Win32 API** and **GDI+** to create a transparent overlay window.
   - Began transforming positional data into visual elements (e.g., rectangles) rendered over the game window.

## **Technical Challenges and Solutions**
- **Memory Analysis**: Understanding and interpreting game memory layouts required extensive experimentation and patience.
- **Assembly Injection**: Writing and testing custom assembly code iteratively ensured functionality without crashes.
- **Shared Memory Synchronization**: Employed a double-buffer approach to handle concurrency between the hooked instruction and the external reader.

## **Requirements**
- **Windows OS**: Supports GDI+ and Win32 API.
- **Development Environment**:
  - Cheat Engine for reverse engineering.
  - A C++ compiler with support for Windows API.
- **Private Server or Local Testing**: Anti-cheat mechanisms must be disabled.

## **Setup**
1. Clone the repository. 
2. Build the project using the provided script:
    ```bash
    build/BuildAll.bat
    ```
    (Requires **g++** or a compatible C++ compiler.)
3. Run the resulting executable while the game is open on a private server.

## **Future Improvements**
- **Enhanced Data Extraction**: Store specific data rather than base addresses internally. This would reduce the amount of calls needed to the Windows API and should significantly increase performance.
- **Improve Rendering Accuracy**: Overhaul my current projection pipeline. This step is tough because Unity is a complex game engine, and as such, its projection is complex. Replicating it is a big task, so currently I just do a standard perspective projection, and I tweaked the values until it was similar.
- **Improved UI**: Add support for interactive overlays with distance values, health bars, entity types (player or scientist), render toggles, filtering options, and more.
