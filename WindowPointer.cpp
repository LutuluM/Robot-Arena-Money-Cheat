#include "ProcessPointer.hpp"
#include "time.h"

int main()
{
    std::vector<unsigned long> offsets = { 0x17c };
    unsigned long baseAddress = 0x0057A9C4;

    try {
        pp::ProcessPointer gamePointer("Robot Arena");
        gamePointer.setAddress(baseAddress, offsets);
        std::cout << "Original Memory value is:" << gamePointer.readMem() << "\n";
        gamePointer.writeMem(13371337);
        std::cout << "Current Memory value is:" << gamePointer.readMem() << "\n";
    }
    catch (std::string exception) {
        std::cout << exception;
        return -1;
    }
    return 0;
}