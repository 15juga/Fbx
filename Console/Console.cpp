#include <iostream>
#include <Reader.h>
#include "CommonDialogs.h"

int main()
{
    std::cout << "1. Load File\n2. Exit" << std::endl;
    int x = 0;
    std::cin >> x;

    while (x != 2) {
        
        if (x == 1) {
            std::string path = Dialogs::OpenFile("Model (*.acjl)\0*.acjl;\0", "Assets\\");
            x = 0;
            if (!path.empty())
            {
                LIB::Reader::ReadFile(path.c_str());
            }
        }
        std::cout << "\n\n1. Load File\n2. Exit" << std::endl;
        std::cin >> x;
    }

    //LIB::Reader::ReadFile("cube.acjl");
}
