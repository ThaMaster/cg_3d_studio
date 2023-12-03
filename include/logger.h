#include "imgui.h"
#include <string>

class Logger {
    public:
        Logger();
        void clear();
        void addLog(const char *, ...);
        ImGuiTextBuffer textBuffer;
        ImVector<int> lineOffsets;
        bool autoScroll;
    protected:
    private:
        
};