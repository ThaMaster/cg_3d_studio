#include "logger.h"
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

void Logger::addLog(std::string logInput, ...) 
{
    char* logEntry = strcpy(logEntry, logInput.c_str());
    int prevSize = textBuffer.size();
    va_list args;
    va_start(args, logEntry);
    textBuffer.appendfv(logEntry, args);
    va_end(args);
    for(int newSize = textBuffer.size(); prevSize < newSize; prevSize++) {
        if(textBuffer[prevSize] == '\n') lineOffsets.push_back(prevSize+1);
    }
}

void Logger::draw(const char* title, bool showLog = NULL) 
{
    if (!ImGui::Begin(title, &showLog))
    {
        ImGui::End();
        return;
    }

    bool clearBuf = ImGui::Button("Clear"); ImGui::SameLine();
    bool copyLog = ImGui::Button("Copy");
    ImGui::Separator();

    if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
    {
        if (clearBuf) clear();
        if (copyLog) ImGui::LogToClipboard();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf_begin = textBuffer.begin();
        const char* buf_end = textBuffer.end();
        ImGui::TextUnformatted(buf_begin, buf_end);
        ImGui::PopStyleVar();

        // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
        // Using a scrollbar or mouse-wheel will take away from the bottom edge.
        if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
    }
    ImGui::EndChild();
    ImGui::End();
}