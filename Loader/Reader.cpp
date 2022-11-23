#include "Reader.h"

namespace LIB
{
	void Reader::LoadMesh(const char* filePath, std::vector<ACJL::Vertex>& mesh, ACJL::Material& material)
	{
		/*//std::ifstream file;

		//file.open(filePath, std::ios::binary | std::ios::in);

		//if (!file.is_open())
		//	throw "Error could not open file\n";

		//if (file.is_open())
		//{
		//	ACJL::Mesh meshT;
		//	file.read((char*)&meshT, sizeof(ACJL::Mesh));

		//	mesh.resize(meshT.nrOfVertices, Vertex());

		//	for (int i = 0; i < meshT.nrOfVertices; i++)
		//	{
		//		file.read((char*)&mesh[i], sizeof(Vertex));
		//	}

		//	file.close();
		//}*/

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
			printf("\nTrans	: %.4f, %.4f, %.4f", readMesh.trans.x, readMesh.trans.y, readMesh.trans.z);
			printf("\nRot	: %.4f, %.4f, %.4f", readMesh.rot.x, readMesh.rot.y, readMesh.rot.z);
			printf("\nScale	: %.4f, %.4f, %.4f", readMesh.scale.x, readMesh.scale.y, readMesh.scale.z);
			printf("\n\n");

			for (int i = 0; i < readMesh.nrOfVertices; i++)
			{
				ACJL::Vertex readVertex;
				file.read((char*)&readVertex, sizeof(ACJL::Vertex));
				printf("Vertex : %i", i);
				printf("\nPosition	:	%.6f %.6f %.6f\n", readVertex.pos.x, readVertex.pos.y, readVertex.pos.z);
				printf("UV		:	%.6f %.6f\n", readVertex.uv.x, readVertex.uv.y);
				printf("Normal		:	%.6f %.6f %.6f\n", readVertex.normals.x, readVertex.normals.y, readVertex.normals.z);
				printf("Tangents	:	%.6f %.6f %.6f\n", readVertex.tangents.x, readVertex.tangents.y, readVertex.tangents.z);
				printf("Bi-Tangents	:	%.6f %.6f %.6f\n\n", readVertex.bitangents.x, readVertex.tangents.y, readVertex.tangents.z);
			}

			file.close();
		}
	}
}

