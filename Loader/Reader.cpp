#include "Reader.h"

namespace LIB
{
	void ACJLReader::ReadFile(const char* exportedFile)
	{
		std::ifstream reader;

		reader.open(exportedFile, std::ios::binary | std::ios::in);

		if (reader.is_open())
		{
			ACJL::Start readStart;
			reader.read((char*)&readStart, sizeof(ACJL::Start));
			printf("Start values from ACJL:\n");
			printf("Meshes: %i Lights: %i Cams: %i\n", readStart.nrOfMeshes, readStart.nrOfLight, readStart.nrOfCams);

			for (int mI = 0; mI < readStart.nrOfMeshes; mI++)
			{
				ACJL::Mesh readMesh;
				reader.read((char*)&readMesh, sizeof(ACJL::Mesh));
				printf("\nReading from ACJL___________________________________\n");
				printf("\nMesh name from ACJL:	%s\n", readMesh.meshName);
				printf("\nNr of Blendshapes: %i\n", readMesh.nrOfBlendShapes);

				for (int i = 0; i < readMesh.nrOfVertices; i++)
				{
					ACJL::Vertex readVertex;
					reader.read((char*)&readVertex, sizeof(ACJL::Vertex));
					printf("\n");
					printf("Vertex : %i\n", i);
					printf("Pos		: %.6f, %.6f, %.6f\n", readVertex.pos[0], readVertex.pos[1], readVertex.pos[2]);
					printf("UV		: %.6f, %.6f,\n", readVertex.uv[0], readVertex.uv[1]);
					printf("Normal		: %.6f, %.6f, %.6f\n", readVertex.normals[0], readVertex.normals[1], readVertex.normals[2]);
					printf("Tangent		: %.6f, %.6f, %.6f\n", readVertex.tangents[0], readVertex.tangents[1], readVertex.tangents[2]);
					printf("Bitangent	: %.6f, %.6f, %.6f\n", readVertex.bitangents[0], readVertex.bitangents[1], readVertex.bitangents[2]);
				}

				for (int i = 0; i < readMesh.nrOfMaterial; i++)
				{
					ACJL::MaterialID readMaterialID;
					reader.read((char*)&readMaterialID, sizeof(ACJL::MaterialID));
					printf("\nMaterial ID %s\n", readMaterialID.matName);
				}
				for (int i = 0; i < readMesh.nrOfBlendShapes; i++)
				{
					ACJL::BlendShapeMeshStart bsMStart;
					LIB::ACJLBlendShape acjlBS;

					reader.read((char*)&bsMStart, sizeof(ACJL::BlendShapeMeshStart));
					acjlBS.name = bsMStart.name;
					printf("Blend Shape Name: %s\n", bsMStart.name);
					printf("Blend Shape numVerts: %i\n", bsMStart.numVerts);
					


					for (int bsvi = 0; bsvi < readMesh.nrOfVertices; bsvi++)
					{
						ACJL::BlendShapeVertex blendVert;
						reader.read((char*)&blendVert, sizeof(ACJL::BlendShapeVertex));

						printf("\tVertex %i\n", bsvi);
						printf("\t\tPos: %f %f %f\n", blendVert.pos[0], blendVert.pos[1], blendVert.pos[2]);
						printf("\t\tNormal: %f %f %f\n", blendVert.normal[0], blendVert.normal[1], blendVert.normal[2]);
					}
				}
				// write blendshape keyframes
				// they are separated from blendmeshes in case we want to write other types of keyframes in the same area in the future
				for (int i = 0; i < readMesh.nrOfBlendShapes; i++)
				{
					ACJL::BlendShapeKeysStart bsKeysStart;
					LIB::ACJLBSKeyFrameSet ksSet;

					reader.read((char*)&bsKeysStart, sizeof(ACJL::BlendShapeKeysStart));
					ksSet.blendShapeName = bsKeysStart.name;
					printf("BlendShape: %s\n", bsKeysStart.name);

					for (int bsKi = 0; bsKi < bsKeysStart.numKeyFrames; bsKi++)
					{
						ACJL::BSKeyFrame kf;
						reader.read((char*)&kf, sizeof(ACJL::BSKeyFrame));
						printf("\tTime: %i Weight: %f\n", kf.time, kf.weight);
					}

					m_scene.m_keyframeSet.emplace_back(ksSet);
				}
			}
			for (int lI = 0; lI < readStart.nrOfLight; lI++)
			{
				ACJL::Light readLight;
				reader.read((char*)&readLight, sizeof(ACJL::Light));
				printf("\nLight name: %s\n", readLight.name);
				printf("LightType: %i\n", readLight.lt);
				printf("RGB %f %f %f\n", readLight.rgb[0], readLight.rgb[1], readLight.rgb[2]);
				printf("Intensity %f\n", readLight.intensity);
				printf("Fog %f\n", readLight.fog);
			}
			for (int cI = 0; cI < readStart.nrOfCams; cI++)
			{
				ACJL::Camera readCam;
				reader.read((char*)&readCam, sizeof(ACJL::Camera));
				printf("Camera name: %s\n", readCam.name);
				printf("Position: %f %f %f\n", readCam.pos[0], readCam.pos[1], readCam.pos[2]);
				printf("NearZ/FarZ: %f/%f\n", readCam.nearZ, readCam.farZ);
			}

			reader.close();
		}

		std::getchar();
	}
}

