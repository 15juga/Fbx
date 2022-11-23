#include "Reader.h"
#include<fstream>

namespace LIB
{
	void Reader::LoadMesh(const char* filePath, std::vector<ACJL::Vertex>& mesh, ACJL::Material& material)
	{
		std::ifstream file;
		file.open(filePath, std::ios::binary | std::ios::in);
		if (!file.is_open())
			throw "Error could not open file\n";
		else
		{
			ACJL::Start startS;
			file.read((char*)&startS, sizeof(ACJL::Start));

			for (int mI = 0; mI < startS.nrOfMeshes; mI++)
			{
				ACJL::Mesh meshS;
				file.read((char*)&meshS, sizeof(ACJL::Mesh));
				mesh.resize(meshS.nrOfVertices, Vertex());
				for (int vI = 0; vI < meshS.nrOfVertices; vI++)
				{
					file.read((char*)&mesh[vI], sizeof(ACJL::Vertex));
				}
				for (int i = 0; i < meshS.nrOfMaterial; i++)
				{
					ACJL::MaterialID matID;
					file.read((char*)&matID, sizeof(ACJL::MaterialID));
				}
				for (int i = 0; i < meshS.nrOfMaterial; i++)
				{
					file.read((char*)&material, sizeof(ACJL::Material));
				}
			}


			file.close();
		}
	}

	void Reader::ReadFile(const char* exportedFile)
	{
		std::ifstream file;

		file.open(exportedFile, std::ios::binary | std::ios::in);

		if (file.is_open())
		{
			ACJL::Start readStart;

			ACJL::Mesh readMesh;
			file.read((char*)&readMesh, sizeof(ACJL::Mesh)); // hämtar data från filen
			printf("\nReading from ACJL___________________________________\n");
			printf("\nMesh name from ACJL:	%s", readMesh.meshName);
			printf("\nTrans	: %.4f, %.4f, %.4f", readMesh.trans[0], readMesh.trans[1], readMesh.trans[2]);
			printf("\nRot	: %.4f, %.4f, %.4f", readMesh.rot[0], readMesh.rot[1], readMesh.rot[2]);
			printf("\nScale	: %.4f, %.4f, %.4f", readMesh.scale[0], readMesh.scale[1], readMesh.scale[2]);
			printf("\n\n");

			for (int i = 0; i < readMesh.nrOfVertices; i++)
			{
				ACJL::Vertex readVertex;
				file.read((char*)&readVertex, sizeof(ACJL::Vertex));
				printf("Vertex : %i", i);
				printf("\nPosition	:	%.6f %.6f %.6f\n", readVertex.pos[0], readVertex.pos[1], readVertex.pos[2]);
				printf("UV		:	%.6f %.6f\n", readVertex.uv[0], readVertex.uv[1]);
				printf("Normal		:	%.6f %.6f %.6f\n", readVertex.normals[0], readVertex.normals[1], readVertex.normals[2]);
				printf("Tangents	:	%.6f %.6f %.6f\n", readVertex.tangents[0], readVertex.tangents[1], readVertex.tangents[2]);
				printf("Bi-Tangents	:	%.6f %.6f %.6f\n\n", readVertex.bitangents[0], readVertex.tangents[1], readVertex.tangents[2]);
			}

			file.close();
		}
	}
}

