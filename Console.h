#ifndef PRAXO_CONSOLE_H
#define PRAXO_CONSOLE_H

#include "imgui.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class PraxoConsole {
public:
    std::vector<std::string> LogBuffer;  // Stores console output
    char InputBuf[256];                  // User input buffer
    bool ScrollToBottom = true;          // Auto-scroll
    void AddLog(const char* fmt, ...);
    void Draw(const char* title);
    std::stringstream CaptureBuffer;
    std::streambuf* OldStdout = nullptr;
   PraxoConsole();
    void RedirectStdout();
    void ResetStdout();
    void CaptureOutput();
    void Command(const char * Cmd);
    
   
private :
    void ProcessCommand(const char* command) {
        
           
            std::string cmdStr(command);
            LogBuffer.push_back("> " + cmdStr);

            if (cmdStr == "clear") {
                LogBuffer.clear();
            }
            else if (cmdStr == "Debug") {

                RedirectStdout();

            }
            else if (cmdStr == "StopDebug") {

                ResetStdout();  // Restore stdout
                AddLog("Debug mode disabled.");
            }
            else {
                AddLog("Unknown command: %s", cmdStr.c_str());
            }
          }
        
        
};





#endif 
