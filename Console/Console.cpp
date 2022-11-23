#include <iostream>
#include <Reader.h>

int main()
{
    std::cout << "Hello World!\n";

    std::vector<Vertex> mesh;

    LIB::Reader::ReadFile("yote.acjl");
    //LIB::Reader::LoadMesh("boll.acjl", mesh);

}
