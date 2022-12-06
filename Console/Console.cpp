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
            std::string path = Dialogs::OpenFile("Model (*.acjl)\0*.acjl;\0", "..\\FbxDemo\\Resources\\");
            if (!path.empty())
            {
                LIB::ACJLReader::ReadFile(path.c_str());
            }
        }
        std::cout << "\n\n1. Load File\n2. Exit" << std::endl;
        std::cin >> x;
    }

    LIB::ACJLReader::ReadFile(path);
    //LIB::ACJLMesh meshes = LIB::ACJLReader::Get<LIB::ACJLMesh>()[0];

    std::vector<LIB::ACJLMesh> meshes = LIB::ACJLReader::Get<LIB::ACJLMesh>();
    std::vector<LIB::ACJLBlendShape> bs = LIB::ACJLReader::Get<LIB::ACJLBlendShape>();

    std::getchar();
    return 0;
    //LIB::Reader::LoadMesh("boll.acjl", mesh);
}
