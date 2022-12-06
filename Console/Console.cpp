#include <iostream>
#include <Reader.h>
#include "CommonDialogs.h"

int main()
{
	std::cout << "1. Load File\n2. Exit" << std::endl;
	int x = 0;
	std::cin >> x;

	std::string path;
	while (x != 2) {
		if (x == 1) {
			path = Dialogs::OpenFile("Model (*.acjl)\0*.acjl;\0", "..\\FbxDemo\\Resources\\");
			if (!path.empty())
			{
				LIB::ACJLReader::ReadFile(path.c_str(), false);

				std::vector<LIB::ACJLMesh> meshes = LIB::ACJLReader::Get<LIB::ACJLMesh>();
				std::vector<LIB::ACJLBlendShape> bsShapes = LIB::ACJLReader::Get<LIB::ACJLBlendShape>();
				std::vector<LIB::ACJLBSKeyFrameSet> bsKeySet = LIB::ACJLReader::Get<LIB::ACJLBSKeyFrameSet>();
				std::vector<LIB::ACJLCamera> cams = LIB::ACJLReader::Get<LIB::ACJLCamera>();
				std::vector<LIB::ACJLLight> lights = LIB::ACJLReader::Get<LIB::ACJLLight>();

				for (int i = 0; i < meshes.size(); i++)
				{
					printf("Mesh: %s\n", meshes[i].name.c_str());

					for (int v = 0; v < meshes[i].vertexes.size(); v++)
					{
						printf("\tVertex %i: \t\t\nPosition: %f %f %f\n", i,
							meshes[i].vertexes[v].pos[0],
							meshes[i].vertexes[v].pos[1],
							meshes[i].vertexes[v].pos[2]);

						printf("\t\tuv: %f %f\n", meshes[i].vertexes[v].uv[0], meshes[i].vertexes[v].uv[1]);

						printf("\t\tNormal: %f %f %f\n",
							meshes[i].vertexes[v].normals[0],
							meshes[i].vertexes[v].normals[1],
							meshes[i].vertexes[v].normals[2]);

						printf("\t\tTangent: %f %f %f\n",
							meshes[i].vertexes[v].tangents[0],
							meshes[i].vertexes[v].tangents[1],
							meshes[i].vertexes[v].tangents[2]);

						printf("\t\tBitangent: %f %f %f\n",
							meshes[i].vertexes[v].bitangents[0],
							meshes[i].vertexes[v].bitangents[1],
							meshes[i].vertexes[v].bitangents[2]);
					}
				}
				printf("");
				for (int i = 0; i < bsShapes.size(); i++)
				{
					printf("\nBlendShape name: %s\n", bsShapes[i].name.c_str());

					for (int v = 0; v < bsShapes[i].vertexes.size(); v++)
					{
						printf("\tVertex %i: \t\t\nPosition: %f %f %f\n", i,
							bsShapes[i].vertexes[v].pos[0],
							bsShapes[i].vertexes[v].pos[1],
							bsShapes[i].vertexes[v].pos[2]);

						printf("\t\tNormal: %f %f %f\n",
							bsShapes[i].vertexes[v].normal[0],
							bsShapes[i].vertexes[v].normal[1],
							bsShapes[i].vertexes[v].normal[2]);
					}
				}
				printf("");
				for (int i = 0; i < bsKeySet.size(); i++)
				{
					printf("\nBlendShape keyframeSet name: %s\n", bsKeySet[i].blendShapeName.c_str());
					for (int j = 0; j < bsKeySet[i].keyframes.size(); j++)
					{
						printf("Keyframe: %i, Time: %i, Weight %f\n", j,
							bsKeySet[i].keyframes[j].time,
							bsKeySet[i].keyframes[j].weight);

					}
				}
				printf("");
				for (int i = 0; i < cams.size(); i++)
				{
					printf("\nCam %s\n", cams[i].name);
					printf("\tfov: %f\n", cams[i].FOV);
					printf("\nnearZ: %f\n", cams[i].nearZ);
					printf("\tfarZ: %f\n", cams[i].farZ);
					printf("\tpos: %f %f %f\n", cams[i].pos[0], cams[i].pos[1], cams[i].pos[2]);
					printf("\tforVec: %f %f %f\n", cams[i].forVec[0], cams[i].forVec[1], cams[i].forVec[2]);
					printf("\tupVec: %f %f %f\n", cams[i].upVec[0], cams[i].upVec[1], cams[i].upVec[2]);
				}
				printf("");
				for (int i = 0; i < lights.size(); i++)
				{
					printf("Light: %s\n", lights[i].name);
					printf("\tPosition: %f %f %f\n", lights[i].pos[0], lights[i].pos[1], lights[i].pos[2]);
					printf("\tRotation: %f %f %f\n", lights[i].rot[0], lights[i].rot[1], lights[i].rot[2]);
					printf("\tRgb: %f %f %f\n", lights[i].rgb[0], lights[i].rgb[1], lights[i].rgb[2]);
				}

			}
		}
		std::cout << "\n\n1. Load File\n2. Exit" << std::endl;
		std::cin >> x;
	}



	//LIB::ACJLReader::SelectLoadedFile("")

	//std::vector<LIB::ACJLBlendShape> bs = LIB::ACJLReader::Get<LIB::ACJLBlendShape>();

	std::getchar();
	return 0;
	//LIB::Reader::LoadMesh("boll.acjl", mesh);
}
