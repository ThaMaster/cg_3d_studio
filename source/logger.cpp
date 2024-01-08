#include "logger.h"

/**
 * The logger is present such that all errors and
 * other text that would otherwise be directed to
 * the terminal can now be visible inside the 
 * 3D studio.
 * 
 * The logger is created by following the guide
 * that is provided in the ImGui example code 
 * and can be found here:
 * 
 * https://github.com/ocornut/imgui
 * 
 * Author: Christoffer Nordlander (c20cnr@cs.umu.se)
 * 
 * Version information:
 *      2024-01-08: v1.0, first version.
 */
Logger::Logger() 
{
    autoScroll = true;
    clear();
}

/**
 * Function for clearing the contents of
 * the char buffer.
 */
void Logger::clear()
{
    textBuffer.clear();
    lineOffsets.clear();
    lineOffsets.push_back(0);
}

/**
 * Function for adding an entry to the be displayed in
 * the logger. It functions as a regular printf function.
 * 
 * @param logEntry: The entry to add to the char buffer.
 */
void Logger::addLog(const char* logEntry, ...) 
{
    int prevSize = textBuffer.size();
    va_list args;
    va_start(args, logEntry);
    textBuffer.appendfv(logEntry, args);
    va_end(args);
    
    for(int newSize = textBuffer.size(); prevSize < newSize; prevSize++) {
        if(textBuffer[prevSize] == '\n') lineOffsets.push_back(prevSize+1);
    }
}