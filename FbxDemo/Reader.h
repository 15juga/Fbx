#pragma once
#include "ACJL_Format.h"
#include<vector>
using namespace ACJL;

namespace LIB
{
	class Reader
	{
	private:

	public:

		static void LoadMesh(const char* filePath, std::vector<ACJL::Vertex>& mesh, ACJL::Material& material);

		static void ReadFile(const char* exportedFile);
	};
}