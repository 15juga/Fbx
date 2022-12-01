#include <iostream>
#include <Reader.h>

int main()
{
    std::cout << "Hello World!\n";

    //std::vector<Vertex> mesh;

    LIB::ACJLReader::ReadFile("blend.acjl");
    LIB::ACJLMesh meshes = LIB::ACJLReader::Get<LIB::ACJLMesh>()[0];

    //std::vector<LIB::ACJLMesh> meshes = LIB::ACJLReader::Get<LIB::ACJLMesh>();
    std::vector<LIB::ACJLBlendShape> bs = LIB::ACJLReader::Get<LIB::ACJLBlendShape>();

    std::getchar();
    return 0;
    //LIB::Reader::LoadMesh("boll.acjl", mesh);
}
