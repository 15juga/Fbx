#include <iostream>
#include <Reader.h>
#include "CommonDialogs.h"

int main()
{
    //std::string path = Dialogs::OpenFile("Model (*.acjl)\0*.acjl;\0", "..\\FbxDemo\\Resources\\");
    //if (!path.empty())
    //{
    //    LIB::ACJLReader::ReadFile(path.c_str());
    //}

    std::string path = "../FbxDemo/Resources/cat.acjl";
    LIB::ACJLReader::ReadFile(path.c_str());
    //LIB::Reader::ReadFile("cube.acjl");
}
