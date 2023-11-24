#include "imgui.h"
#include <string>

class Logger {
    public:
        Logger();
        void clear();
        void addLog(std::string, ...);
        void draw(const char*, bool); 

    protected:
    private:
        ImGuiTextBuffer textBuffer;
        ImVector<int> lineOffsets;
        bool autoScroll;
};