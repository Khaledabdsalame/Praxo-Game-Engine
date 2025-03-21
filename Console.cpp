#include "Console.h"
#include <windows.h>
ImGuiConsole::ImGuiConsole() {
    memset(InputBuf, 0, sizeof(InputBuf));
}

void ImGuiConsole::RedirectStdout() {
    if (OldStdout == nullptr) {  // If not already redirected
        OldStdout = std::cout.rdbuf(CaptureBuffer.rdbuf());  // Redirect std::cout
    }  // Redirect stdout
}

void ImGuiConsole::ResetStdout() {
     if (OldStdout != nullptr) { 
        std::cout.rdbuf(OldStdout);
        OldStdout = nullptr;
    }  
}

void ::ImGuiConsole::CaptureOutput() {

    std::string captured = CaptureBuffer.str();
    if (!captured.empty()) {
        LogBuffer.push_back(captured);
        CaptureBuffer.str("");  // Clear buffer after reading
        CaptureBuffer.clear();  // Reset error state
    }
}

void ImGuiConsole :: AddLog(const char* fmt, ...){

        va_list args;
        va_start(args, fmt);
        char buf[1024];
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        LogBuffer.push_back(buf);
        ScrollToBottom = true;

}

void ImGuiConsole :: Draw(const char* title) {
    ImGui::Begin(title,nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    CaptureOutput();

    // Output log
    ImGui::BeginChild("ConsoleOutput", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (const auto& line : LogBuffer)
        ImGui::TextUnformatted(line.c_str());
    if (ScrollToBottom)
        ImGui::SetScrollHereY(1.0f);
    ScrollToBottom = false;
    ImGui::EndChild();

    // User input
    ImGui::SetNextItemWidth(GetSystemMetrics(SM_CXSCREEN)-140);
    if (ImGui::InputText("Command", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
        ProcessCommand(InputBuf);
        memset(InputBuf, 0, sizeof(InputBuf));
    }

    ImGui::End();


}

void ImGuiConsole::Command(const char * Cmd) {
    ProcessCommand(Cmd);
   
}




