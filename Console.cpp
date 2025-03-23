#include "Console.h"
#include <Windows.h>
PraxoConsole::PraxoConsole() {
    memset(InputBuf, 0, sizeof(InputBuf));
}

void PraxoConsole::RedirectStdout() {
    if (OldStdout == nullptr) {  // If not already redirected
        OldStdout = std::cout.rdbuf(CaptureBuffer.rdbuf());  // Redirect std::cout
    }  // Redirect stdout
}

void PraxoConsole::ResetStdout() {
     if (OldStdout != nullptr) { 
        std::cout.rdbuf(OldStdout);
        OldStdout = nullptr;
    }  
}

void ::PraxoConsole::CaptureOutput() {

    std::string captured = CaptureBuffer.str();
    if (!captured.empty()) {
        LogBuffer.push_back(captured);
        CaptureBuffer.str("");  // Clear buffer after reading
        CaptureBuffer.clear();  // Reset error state
    }
}

void PraxoConsole :: AddLog(const char* fmt, ...){

        va_list args;
        va_start(args, fmt);
        char buf[1024];
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        LogBuffer.push_back(buf);
       

}

void PraxoConsole :: Draw(const char* title) {
    ImGui::Begin(title,nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    CaptureOutput();

    // Output log
    ImGui::BeginChild("ConsoleOutput", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (const auto& line : LogBuffer) {
        ImGui::TextUnformatted(line.c_str());
        
    }

    float scrollY = ImGui::GetScrollY();
    float scrollMaxY = ImGui::GetScrollMaxY();
    bool isAtBottom = (scrollY >= scrollMaxY - 5.0f);
        
    if (autoScroll && isAtBottom) {
        ImGui::SetScrollHereY(1.0f);
    }

   
   
    
    ImGui::EndChild();

    // User input
    ImGui::SetNextItemWidth(GetSystemMetrics(SM_CXSCREEN)-140);
    if (ImGui::InputText("Command", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
        ProcessCommand(InputBuf);
        memset(InputBuf, 0, sizeof(InputBuf));
    }

    ImGui::End();


}

void PraxoConsole::Command(const char * Cmd) {
    ProcessCommand(Cmd);
   
}




