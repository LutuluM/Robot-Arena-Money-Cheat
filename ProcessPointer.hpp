#ifndef _processPointer
#define _processPointer

#include <windows.h>
#include <vector>
#include <string>
#include <iostream>

namespace pp {
class ProcessPointer {
    public:
        ProcessPointer(std::string windowname) {
            WindowName = windowname.c_str();
            try {
                getWindow();
                getProcessHandler();
            }
            catch (std::string error) {
                std::cout << error;
                throw std::string("Failed to create ProcessPointer Instance\n");
            }
        }
        void setAddress(unsigned long address, std::vector<unsigned long> offsets) {
            BaseAddress = static_cast<DWORD>(address);
            Offsets = static_cast<std::vector<DWORD>>(offsets);
            resolvePointerAddress();
        }
        void writeMem(unsigned long value) {
            DWORD Value = static_cast<DWORD>(value);
            if (WriteProcessMemory(ProcessHandle, (LPVOID)FinalPointer, &Value, sizeof(DWORD), nullptr) == 0)
                throw "Failed to write to process address";
        }
        DWORD readMem() {
            DWORD memoryValue;
            if (ReadProcessMemory(ProcessHandle, (LPCVOID)FinalPointer, &memoryValue, sizeof(memoryValue), nullptr) == 0)
                throw "Failed to read for the Memory Address: " + std::to_string(FinalPointer) + "\n";
            return memoryValue;
        }
    private:
        void getWindow() {
            WindowObject = FindWindowA(NULL, WindowName);
            if (WindowObject == NULL)
                throw "Failed to Find window " + std::string(WindowName) + "\n";
        }
        void getProcessHandler() {
            GetWindowThreadProcessId(WindowObject, &ProcessID);
            ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, false, ProcessID);
            if (ProcessHandle == NULL)
                throw "Error Failed aquire Process Handle for " + std::string(WindowName) + " : " + std::to_string(ProcessID) + '\n';
        }
        void resolvePointerAddress() {
            DWORD tempPointer;
            FinalPointer = BaseAddress;
            try {
                for (int i = 0;i < Offsets.size();i++) {
                    if (ReadProcessMemory(ProcessHandle, (LPCVOID)FinalPointer, &tempPointer, sizeof(tempPointer), nullptr) == 0)
                        throw "Failed to resolve the Pointer Address. Invalid Address " + std::to_string(FinalPointer) + "+" + std::to_string(Offsets[i]) + "\n";
                    else
                        FinalPointer = tempPointer + Offsets[i];
                }
            }
            catch (std::string error) {
                std::cout << error;
                FinalPointer = NULL;
            }
                
        }

        LPCSTR WindowName;
        HWND WindowObject;
        DWORD ProcessID;
        HANDLE ProcessHandle;
        DWORD BaseAddress;
        DWORD FinalPointer;
        std::vector<DWORD>Offsets;
};
}// namespace gp
#endif // processPointer

