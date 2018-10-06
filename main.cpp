#include <iostream>
#include <windows.h>
#include <ctime>
#include <string>

#define Null 0

using namespace std;

LPCSTR WindowName = "Robot Arena";
HWND WindowObject;
DWORD ProcessID;
HANDLE ProcessHandle;

BYTE Money[] = {0x00,0x00,0x10,0x00}; //LSB -> MSB
DWORD baseAddress = 0x0057A9C4;
DWORD Offset[] = {0x17c};

DWORD GetAddress(int NumberofOffsets,DWORD BasePTR,DWORD Offset[])
{
    DWORD pointer = BasePTR;
    DWORD tempPTR;
    if(NumberofOffsets == 0) return BasePTR;
    for(int i=0;i<NumberofOffsets;i++)
    {
        if(0 == ReadProcessMemory(ProcessHandle,(LPCVOID)pointer,&tempPTR,sizeof(tempPTR),Null))
        {
            cout << "Error Reading";
            exit(0);
        }
        else
        {
            pointer = tempPTR+Offset[i];
        }
    }
    return pointer;
}
int main()
{
    WindowObject = FindWindowA(Null,WindowName);
    if(WindowObject == Null) cout << "Failed to Find Window";
    else
    {
        cout << "Found Window\n";
        GetWindowThreadProcessId(WindowObject,&ProcessID);
        ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS,false,ProcessID);
        if(ProcessHandle != 0)
        {
            cout << "Process Handle Created\n";
            DWORD NewAddress = GetAddress(1,baseAddress,Offset);
            if(WriteProcessMemory(ProcessHandle,(LPVOID)NewAddress,Money,sizeof(Money),0))
            {
                cout << "Write Success";
            }
            else cout << "WriteFailed";
        }
        else
        {
            cout << "Process Handle Error";
            cout << GetLastError();
        }
    }
    cin.get(); //pauses window to check if succeeded
    return 0;
}
