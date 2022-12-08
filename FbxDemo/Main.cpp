#include <algorithm>
#include <ctime>
#include <vector>
#include <fstream>
#include "Includes.h"
#include "Child.h"
#include "Geometry.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"
#include "CommonDialogs.h"

void GetChildrenAndAttr(vector<Child>& childLib, vector<Light>& lightLib, std::vector<ACJL::Camera>& cameras, 
	unsigned int& totalCountOfChildren, unsigned int& nrOfMeshes, unsigned int& nrOfLights, unsigned int& nrOfCameras, FbxNode* node, FbxNodeAttribute* nodeAttr, FbxScene* scene, const char* filepath);

bool ExportFile(const char* exportFilePath, vector<Child>& childLib, vector<Light>& lightLib, std::vector<ACJL::Camera> cameras, 
	unsigned int& childCount, unsigned int& nrOfMeshes, unsigned int& nrOfLights, unsigned int& nrOfCameras, FbxExporter* exporter);

bool ReadFile(const char* exportedFile);

int main(int argc, char** argv)
{
	// Execute the program to export a file

	// First window will ask you to open a fbx file
	// Second window will ask you to specify a path for the new file



	//char exporterFilepath[100]{ "Resources/blend.acjl" };
	std::string openFilePath = Dialogs::OpenFile("Fbx file (*.fbx)\0*.fbx\0", "Resources\\");
	FbxString fbxFile = openFilePath.c_str();
	//FbxString fbxFile = { "Resources/blendAni.fbx"};

	std::string exportfilePath = Dialogs::SaveFile("ACJL file (*.acjl)\0*.acjl\0", "Resources\\");
	size_t offset = exportfilePath.find_last_of('.');
	if (offset == std::string::npos)
		exportfilePath.append(".acjl");

	// Checking if dialogs were canceled, if the operation is canceled the program will exit
	if (openFilePath.empty() || exportfilePath.empty())
	{
		return 1;
	}

	printf("Exporting from file:\n\t%s\nInto:\n\t%s\n\n", openFilePath.c_str(), exportfilePath.c_str());

	//___________________________________ Manager and IO Setting ___________________________________//
	FbxManager* manager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);

	//___________________________________ Import ___________________________________//
	FbxImporter* importer = FbxImporter::Create(manager, "importer");
	int fileMajor, fileMinor, fileRevision;
	bool importStatus = importer->Initialize(fbxFile, -1, manager->GetIOSettings());
	if (!importStatus)
	{
		cout << "ERROR. FAILED TO IMPORT" + fbxFile + "." << endl;
		return 0;
	}
	importer->GetFileVersion(fileMajor, fileMinor, fileRevision);

	//___________________________________ Import Scene ___________________________________//
	FbxScene* scene = FbxScene::Create(manager, "scene");
	importStatus = importer->Import(scene);
	if (!importStatus)
	{
		cout << "ERROR. FAILED TO IMPORT " + fbxFile + "." << endl;
		return 0;
	}

	//___________________________________ Node ___________________________________//
	FbxNode* root = scene->GetRootNode();
	FbxNodeAttribute* nodeAttr = root->GetNodeAttribute();

	//_______________________________________________________________________________//
	vector<Child> childLib;
	vector<Light> lightLib;
	vector<ACJL::Camera> cameras;
	unsigned int totalCountOfChildren = 0;
	unsigned int nrOfMeshes = 0;
	unsigned int nrOfLights = 0;
	unsigned int nrOfCameras = 0;

	GetChildrenAndAttr(childLib, lightLib, cameras, totalCountOfChildren, nrOfMeshes, nrOfLights, nrOfCameras, root, nodeAttr, scene, exportfilePath.c_str());

	//PrintData(childLib, totalCountOfChildren);

	//cout << nrOfMeshes << endl;
	//___________________________________ Export ___________________________________//
	FbxExporter* exporter = FbxExporter::Create(manager, "exporter");

	ExportFile(exportfilePath.c_str(), childLib, lightLib, cameras, totalCountOfChildren, nrOfMeshes, nrOfLights, nrOfCameras, exporter);
	//ReadFile(exporterFilepath);

	//___________________________________ Terminate ___________________________________//
	exporter->Destroy();
	root->Destroy();
	scene->Destroy();
	ios->Destroy();
	importer->Destroy();
	manager->Destroy();
	printf("\n\nJob's done!\nPress ENTER to close...");
	std::getchar();
	return 0;
}

//_________________________________________________________ CAN ONLY COUNT ONE CHILD AT THE MOMENT <---UNSURE _________________________________________________________//
void GetChildrenAndAttr(vector<Child>& childLib, vector<Light>& lightLib, std::vector<ACJL::Camera>& cameras, unsigned int& totalCountOfChildren, unsigned int& nrOfMeshes, unsigned int& nrOfLights, unsigned int& nrOfCameras,
	FbxNode* node, FbxNodeAttribute* nodeAttr, FbxScene* scene, const char* filepath)
{
	int childCount = node->GetChildCount();

	totalCountOfChildren++;

	for (int i = 0; i < childCount; i++)
	{
		FbxNode* childNode = node->GetChild(i);

		for (int j = 0; j < childNode->GetNodeAttributeCount(); j++)
		{
			FbxNodeAttribute* attrType = childNode->GetNodeAttributeByIndex(i);

			switch (childNode->GetNodeAttribute()->GetAttributeType())
			{
			case FbxNodeAttribute::eMesh:
			{
				nrOfMeshes++;
				Child child(childNode);

				GetMeshAttr(child, scene);
				for (int mI = 0; mI < childNode->GetMaterialCount(); mI++)
				{
					ACJL::MaterialID childMaterialID;
					child.GetMaterialID().push_back(childMaterialID);
					GetMaterialAttr(child.GetMaterialID()[mI], child.GetMaterial(), child.GetFbxNode(), scene);
					initTextures(child, filepath);
				}
				child.SetAttrCount(childNode->GetNodeAttributeCount());
				
				childLib.push_back(child);

				if (childNode->GetChildCount() > 0)
				{
					GetChildrenAndAttr(childLib, lightLib, cameras, totalCountOfChildren, nrOfMeshes, nrOfLights, nrOfCameras, childNode, nodeAttr, scene, filepath);
					//totalCountOfChildren++;
				}
				break;
			}
			case FbxNodeAttribute::eCamera:
			{
				nrOfCameras++;
				getCamAttr(childNode, cameras);
				break;
			}
			case FbxNodeAttribute::eLight:
			{
				nrOfLights++;
				Light light(childNode);

				GetLightData(childNode, light.getLight());
				//std::cout << light.getLight().name << ":\n";
				lightLib.push_back(light);
				break;
			}
			}
		}
	}
}

#define ACJL_WRITE(data) output.write((const char*)&data, sizeof(data))
#define ACJL_READ(data) reader.read((char*)&data, sizeof(data))

bool ExportFile(const char* exportFilePath, vector<Child>& childLib, vector<Light>& lightLib, std::vector<ACJL::Camera> cameras,
	unsigned int& childCount, unsigned int& nrOfMeshes, unsigned int& nrOfLights, unsigned int& camCount, FbxExporter* exporter)
{
	ofstream output;
	printf("Total nr of Children	:	%i\n\n", (const char*)childCount);

	output.open(exportFilePath, ios::out | ios::binary);

	if (output.is_open())
	{
		//Write start header
		ACJL::Start exportStart;

		exportStart.nrOfMeshes = nrOfMeshes;
		exportStart.nrOfLight = nrOfLights;
		exportStart.nrOfCams = camCount;

		output.write((const char*)&exportStart, sizeof(ACJL::Start));

		std::vector<ACJL::Material> materials;

		//Write mesh headers.
		for (int mI = 0; mI < exportStart.nrOfMeshes; mI++)
		{
			ACJL::Mesh exportMesh = childLib[mI].GetMesh();
			exportMesh.nrOfBlendShapes = childLib[mI].GetBlendVertArr().size();
			printf("Material Name :\n\t");
			for (int i = 0; i < childLib[mI].GetMaterialID().size(); i++)
			{
				printf("%s\n\t", childLib[mI].GetMaterialID()[i].matName);
			}
			printf("\nexporting mesh......	:	%s\n", childLib[mI].GetMesh().meshName);
			output.write((const char*)&exportMesh, sizeof(ACJL::Mesh));
			printf("%i vertices...\n", exportMesh.nrOfVertices);
			for (int j = 0; j < exportMesh.nrOfVertices; j++)
			{
				ACJL::Vertex exportVertex = childLib[mI].GetVertexByIndex(j);
				//printf("exporting vertex	: %i\n", &exportVertex);
				//output.write((const char*)&exportVertex, sizeof(ACJL::Vertex));
				ACJL_WRITE(exportVertex);
			}
			printf("%i materials...\n", exportMesh.nrOfMaterial);
			for (int i = 0; i < exportMesh.nrOfMaterial; i++)
			{
				ACJL::MaterialID exportMaterialID = childLib[mI].GetMaterialID()[i];
				//output.write((const char*)&exportMaterialID, sizeof(ACJL::MaterialID));
				ACJL_WRITE(exportMaterialID);
			}
			for (int i = 0; i < exportMesh.nrOfMaterial; i++)
			{
				ACJL::Material exportMat = childLib[mI].GetMaterial()[i];

				bool materialExist = false;
				for (int j = 0; j < materials.size() && !materialExist; j++)
				{
					if (exportMat.matName == materials[j].matName)
					{
						materialExist = true;
					}
				}
				if (!materialExist)
				{
					materials.emplace_back(exportMat);
				}
				ACJL_WRITE(exportMat);
			}
			printf("%i blendshapes...\n", exportMesh.nrOfBlendShapes);
			// write blendshape meshes
			for (int i = 0; i < exportMesh.nrOfBlendShapes; i++)
			{
				ACJL::BlendShapeMeshStart bsMStart;
				strcpy_s(bsMStart.name, childLib[mI].GetBlendVertArr()[i].name.c_str());
				bsMStart.numVerts = exportMesh.nrOfVertices;
				ACJL_WRITE(bsMStart);

				for (int bsvi = 0; bsvi < exportMesh.nrOfVertices; bsvi++)
				{
					ACJL::BlendShapeVertex blendVert(childLib[mI].GetBlendVertArr()[i].vertArrs[bsvi]);
					ACJL_WRITE(blendVert);
				}
			}
			// write blendshape keyframes
			// they are separated from blendmeshes in case we want to write other types of keyframes in the same area in the future
			for (int i = 0; i < exportMesh.nrOfBlendShapes; i++)
			{
				ACJL::BlendShapeKeysStart bsKeysStart;
				strcpy_s(bsKeysStart.name, childLib[mI].GetBlendVertArr()[i].name.c_str());

			printf("%s blendshape keyframes...\n", bsKeysStart.name);
				bsKeysStart.numKeyFrames = childLib[mI].GetBlendShapeChannels()[i].keyframes.size();
				ACJL_WRITE(bsKeysStart);
				for (int bsKi = 0; bsKi < bsKeysStart.numKeyFrames; bsKi++)
				{
					ACJL::BSKeyFrame kf =
						childLib[mI].GetBlendShapeChannels()[i].keyframes[bsKi];
					printf("\ttime: %i, weight: %f\n", kf.time, kf.weight);
					ACJL_WRITE(kf);
				}
				printf("\n");
			}
			//for (int j = 0; j < childLib[i].GetTexture().size(); j++)
			//{
			//	ACJL::Texture exportTexture = childLib[i].GetTexture()[j];
			//	output.write((const char*)&exportTexture, sizeof(ACJL::Texture));
			//}
			
		}
		// write light header
		for (int i = 0; i < exportStart.nrOfLight; i++)
		{
			ACJL::Light exportLight = lightLib[i].getLight();
			printf("Light: %s, type: %i\n", exportLight.name, exportLight.lt);
			output.write((const char*)&exportLight, sizeof(ACJL::Light));
		}
		printf("\nCams:\n");
		// write cam header
		for (int i = 0; i < exportStart.nrOfCams; i++)
		{
			ACJL::Camera exportCamera = cameras[i];
			printf("Cam: %s, FOV: %f\n", exportCamera.name, exportCamera.FOV);
			output.write((const char*)&exportCamera, sizeof(ACJL::Camera));
		}
		//for (int i = 0; i < materials.size(); i++)
		//{
		//	ACJL_WRITE(materials[i]);
		//}
	}

	output.close();

	return true;
}

bool ReadFile(const char* exportedFile)
{
	ifstream reader;

	reader.open(exportedFile, ios::binary | ios::in);

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
			for (int i = 0; i < readMesh.nrOfMaterial; i++)
			{
				ACJL::Material readMat;
				reader.read((char*)&readMat, sizeof(ACJL::Material));
				printf("\nAlbedo name: %s\n", readMat.albedoName);
				printf("Diffuse %f %f %f\n", readMat.diffuse[0], readMat.diffuse[2], readMat.diffuse[2]);
				printf("Ambient %f %f %f\n", readMat.ambient[0], readMat.ambient[1], readMat.ambient[2]);
				printf("Material type %i\n", readMat.mt);
				printf("Specular %f %f %f\n", readMat.specular[0], readMat.specular[1], readMat.specular[2]);
			}
			for (int i = 0; i < readMesh.nrOfBlendShapes; i++)
			{
				ACJL::BlendShapeMeshStart bsMStart;
				ACJL_READ(bsMStart);
				printf("Blend Shape Name: %s\n", bsMStart.name);
				printf("Blend Shape numVerts: %i\n", bsMStart.numVerts);

				for (int bsvi = 0; bsvi < readMesh.nrOfVertices; bsvi++)
				{
					ACJL::BlendShapeVertex blendVert;
					ACJL_READ(blendVert);
					
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
				ACJL_READ(bsKeysStart);
				printf("BlendShape: %s\n", bsKeysStart.name);
				for (int bsKi = 0; bsKi < bsKeysStart.numKeyFrames; bsKi++)
				{
					ACJL::BSKeyFrame kf;
					ACJL_READ(kf);
					
					printf("\tTime: %i Weight: %f\n", kf.time, kf.weight);
				}
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

	return true;
}

