#include "imgui.h"
#include <cstdarg>
#include <string>

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
class Logger {
    public:
        Logger();
        void clear();
        void addLog(const char *, ...);
        ImGuiTextBuffer textBuffer;
        ImVector<int> lineOffsets;
        bool autoScroll;
};