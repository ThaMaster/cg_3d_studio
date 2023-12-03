#include "logger.h"
#include <cstdarg>
#include <string>

Logger::Logger() 
{
    autoScroll = true;
    clear();
}

void Logger::clear()
{
    textBuffer.clear();
    lineOffsets.clear();
    lineOffsets.push_back(0);
}

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