
# A simple graphics application written in C++ using Direct2D.

The project is a personal educational project intended to familiarize myself with the WIN32 and Direct2D API.  
Feel free to look at the source, and/or use it in your own projects. Code is intended to work as is, thus expendability  
and optimizations have not been highly considered.

**THE GOAL**:  
To create a working graphicsal Tic-Tac-Toe application where the user plays against the computer.  
The application should include a head-to-head score and a reset button to reset all scores.  

**To compile the project**  
Example:  
Run: `cmake -S . -D CMAKE_TOOLCHAIN_FILE=CMAKE/x86_64-w64-mingw32.cmake -G "MinGW Makefiles" -B .\build`  
(in project root folder)  
Has only been tested on up-to-date Windows 11, GPU drivers and VC++ Redistributables

**(WORK-IN-PROGRESS)**