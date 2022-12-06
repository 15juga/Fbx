#include "Reader.h"
#include <fstream>



namespace LIB
{

#define ACJL_READ(data) reader.read((char*)&data, sizeof(data))

	std::vector<ACJLScene> ACJLReader::m_scene;
	std::vector<std::string> ACJLReader::m_sceneNames;
	int ACJLReader::m_sceneIndex = -1;

	int ACJLReader::NameToIndex(std::string name)
	{
		int len = m_scene.size();
		for (int i = 0; i < len; i++)
		{
			if (m_sceneNames[i] == name)
			{
				return i;
			}
		}
		return -1;
	}

	void ACJLReader::ClearMemory()
	{
		m_scene.clear();
		m_sceneNames.clear();
		m_sceneIndex = -1;
	}

	bool ACJLReader::SelectLoadedFile(std::string name)
	{
		return false;
	}

	void ACJLReader::ReadFile(const char* exportedFile, bool printData)
	{
		ACJLScene acjlScene;
		std::ifstream reader;

		int sceneExists = NameToIndex(exportedFile);

		reader.open(exportedFile, std::ios::binary | std::ios::in);

		if (reader.is_open() && (sceneExists == -1))
		{
			ACJL::Start readStart;
			reader.read((char*)&readStart, sizeof(ACJL::Start));
			if (printData)
			{
				printf("Start values from ACJL:\n");
				printf("Meshes: %i Lights: %i Cams: %i\n", readStart.nrOfMeshes, readStart.nrOfLight, readStart.nrOfCams);
			}


			for (int mI = 0; mI < readStart.nrOfMeshes; mI++)
			{
				ACJLMesh storeMesh;
				ACJL::Mesh readMesh;
				reader.read((char*)&readMesh, sizeof(ACJL::Mesh));
				storeMesh.name = readMesh.meshName;
				if (printData)
				{
					printf("\nReading from ACJL___________________________________\n");
					printf("\nMesh name from ACJL:	%s\n", readMesh.meshName);
				}

				for (int i = 0; i < readMesh.nrOfVertices; i++)
				{
					ACJL::Vertex readVertex;
					reader.read((char*)&readVertex, sizeof(ACJL::Vertex));
					//m_scene.meshes.emplace_back(readVertex);

					storeMesh.vertexes.emplace_back(readVertex);
					if (printData)
					{
						printf("\n");
						printf("Vertex : %i\n", i);
						printf("Pos		: %.6f, %.6f, %.6f\n", readVertex.pos[0], readVertex.pos[1], readVertex.pos[2]);
						printf("UV		: %.6f, %.6f,\n", readVertex.uv[0], readVertex.uv[1]);
						printf("Normal		: %.6f, %.6f, %.6f\n", readVertex.normals[0], readVertex.normals[1], readVertex.normals[2]);
						printf("Tangent		: %.6f, %.6f, %.6f\n", readVertex.tangents[0], readVertex.tangents[1], readVertex.tangents[2]);
						printf("Bitangent	: %.6f, %.6f, %.6f\n", readVertex.bitangents[0], readVertex.bitangents[1], readVertex.bitangents[2]);
					}

				}
				for (int i = 0; i < readMesh.nrOfMaterial; i++)
				{
					ACJL::MaterialID readMaterialID;

					reader.read((char*)&readMaterialID, sizeof(ACJL::MaterialID));
					storeMesh.matId.emplace_back(readMaterialID);

					if (printData) printf("\nMaterial ID %s\n", readMaterialID.matName);
				}
				for (int i = 0; i < readMesh.nrOfMaterial; i++)
				{
					ACJL::Material readMat;
					reader.read((char*)&readMat, sizeof(ACJL::Material));
					storeMesh.mat.emplace_back(readMat);

					if (printData)
					{
						printf("\nAlbedo name: %s\n", readMat.albedoName);
						printf("Diffuse %f %f %f\n", readMat.diffuse[0], readMat.diffuse[2], readMat.diffuse[2]);
						printf("Ambient %f %f %f\n", readMat.ambient[0], readMat.ambient[1], readMat.ambient[2]);
						printf("Material type %i\n", readMat.mt);
						printf("Specular %f %f %f\n", readMat.specular[0], readMat.specular[1], readMat.specular[2]);
						printf("Specular intensity %f\n", readMat.specularIntensity);
					}

				}
				for (int i = 0; i < readMesh.nrOfBlendShapes; i++)
				{
					ACJL::BlendShapeMeshStart bsMStart;
					LIB::ACJLBlendShape acjlBS;

					ACJL_READ(bsMStart);
					acjlBS.name = bsMStart.name;
					if (printData)
					{
						printf("Blend Shape Name: %s\n", bsMStart.name);
						printf("Blend Shape numVerts: %i\n", bsMStart.numVerts);
					}


					for (int bsvi = 0; bsvi < readMesh.nrOfVertices; bsvi++)
					{
						ACJL::BlendShapeVertex blendVert;
						ACJL_READ(blendVert);
						acjlBS.vertexes.emplace_back(blendVert);
						if (printData)
						{
							printf("\tVertex %i\n", bsvi);
							printf("\t\tPos: %f %f %f\n", blendVert.pos[0], blendVert.pos[1], blendVert.pos[2]);
							printf("\t\tNormal: %f %f %f\n", blendVert.normal[0], blendVert.normal[1], blendVert.normal[2]);
						}

					}
					acjlScene.m_blendshapes.emplace_back(acjlBS);
				}
				// write blendshape keyframes
				// they are separated from blendmeshes in case we want to write other types of keyframes in the same area in the future
				for (int i = 0; i < readMesh.nrOfBlendShapes; i++)
				{
					ACJL::BlendShapeKeysStart bsKeysStart;
					LIB::ACJLBSKeyFrameSet ksSet;
					ACJL_READ(bsKeysStart);
					ksSet.blendShapeName = bsKeysStart.name;
					if (printData)
					{
						printf("BlendShape: %s\n", bsKeysStart.name);
					}

					for (int bsKi = 0; bsKi < bsKeysStart.numKeyFrames; bsKi++)
					{
						ACJL::BSKeyFrame kf;
						ACJL_READ(kf);
						ksSet.keyframes.emplace_back(kf);

						if (printData) printf("\tTime: %i Weight: %f\n", kf.time, kf.weight);
					}

					acjlScene.m_keyframeSet.emplace_back(ksSet);
				}


				acjlScene.meshes.emplace_back(storeMesh);
			}
			for (int lI = 0; lI < readStart.nrOfLight; lI++)
			{
				ACJL::Light readLight;
				reader.read((char*)&readLight, sizeof(ACJL::Light));
				acjlScene.m_lights.emplace_back(readLight);
				if (printData)
				{
					printf("\nLight name: %s\n", readLight.name);
					printf("LightType: %i\n", readLight.lt);
					printf("RGB %f %f %f\n", readLight.rgb[0], readLight.rgb[1], readLight.rgb[2]);
					printf("Intensity %f\n", readLight.intensity);
					printf("Fog %f\n", readLight.fog);
				}

			}
			for (int cI = 0; cI < readStart.nrOfCams; cI++)
			{
				ACJL::Camera readCam;
				reader.read((char*)&readCam, sizeof(ACJL::Camera));
				acjlScene.m_cams.emplace_back(readCam);
				if (printData)
				{
					printf("Camera name: %s\n", readCam.name);
					printf("Position: %f %f %f\n", readCam.pos[0], readCam.pos[1], readCam.pos[2]);
					printf("NearZ/FarZ: %f/%f\n", readCam.nearZ, readCam.farZ);
				}

			}

			reader.close();
			m_scene.emplace_back(acjlScene);
			m_sceneNames.emplace_back(exportedFile);
			m_sceneIndex = m_scene.size() - 1;
		}
		else if (reader.is_open() && sceneExists != -1)
		{
			m_sceneIndex = sceneExists;
		}


	}
}

