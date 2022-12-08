#pragma once
#include <Windows.h>
#include <fbxsdk.h>

bool IfAlreadyLoaded(ACJL::Texture texture, std::vector<ACJL::Texture>& compare, char albedo[], char normal[])
{
	bool result = false;

	std::string texName = texture.name;
	std::string path = texture.path;
	std::string compTex;
	std::string compPath;


	for (int i = 0; i < compare.size(); i++)
	{
		compTex = compare[i].name;
		compPath = compare[i].path;

		if (texName == compTex && path == compPath)
		{
			result = true;
			break;
		}
	}

	return result;
}

void GetMaterialID(ACJL::MaterialID& matID, FbxNode* node)
{
	int nrOfMaterials = node->GetMaterialCount();

	for (int i = 0; i < nrOfMaterials; i++)
	{
		FbxSurfaceMaterial* material = node->GetMaterial(i);

		const char* name = material->GetClassId().GetName();

		strcpy_s(matID.matName, name);
	}
}

void LoadMaterialData(std::vector<ACJL::Material>& vMaterialHeader, FbxScene* scene)
{
	int materialCount = scene->GetMaterialCount();
	//Get data from each material
	for (int i = 0; i < materialCount; i++)
	{
		ACJL::Material materialHeader;
		FbxSurfaceMaterial* material = (FbxSurfaceMaterial*)scene->GetMaterial(i);
		FbxString name = material->GetName();


		//printf("Material name: %s\n", material->GetName());
		strcpy_s(materialHeader.matName, name);
		

		FbxClassId materialType = material->GetClassId();

		//___________________________ Phong ___________________________//
		if (materialType.Is(FbxSurfacePhong::ClassId))
		{
			FbxSurfacePhong* phong = (FbxSurfacePhong*)material;

			//Ambient
			FbxDouble3 ambient = phong->Ambient;
			for (int j = 0; j < 3; j++)
			{
				materialHeader.ambient[j] = (float)ambient[j];
			}

			//Diffuse, RGB
			FbxDouble3 diffuse = phong->Diffuse;

			for (int j = 0; j < 3; j++)
			{
				materialHeader.diffuse[j] = (float)diffuse[j];
			}

			//Specular
			FbxDouble3 specular = phong->Specular;

			for (int j = 0; j < 3; j++)
			{
				materialHeader.specular[j] = (float)specular[j];
			}

			//Type
			materialHeader.mt = ACJL::MaterialType::PHONG;
		}

		//___________________________ Lambert ___________________________//
		else if (materialType.Is(FbxSurfaceLambert::ClassId))
		{
			FbxSurfaceLambert* lambert = (FbxSurfaceLambert*)material;
			FbxString test = lambert->GetNameOnly();

			//Ambient
			FbxDouble3 ambient = lambert->Ambient;
			for (int j = 0; j < 3; j++)
			{
				materialHeader.ambient[j] = (float)ambient[j];
			}
			
			//Diffuse, RGB
			FbxDouble3 diffuse = lambert->Diffuse;
			for (int j = 0; j < 3; j++)
			{
				materialHeader.diffuse[j] = (float)diffuse[j];
			}

			//Lambert doesn't use specular

			//Type
			materialHeader.mt = ACJL::MaterialType::LAMBERT;
		}
		vMaterialHeader.push_back(materialHeader);
	}

}

void initTextures(Child& child, FbxString filepath)
{
	int fileSteps = filepath.ReverseFind('/')+1;
	filepath = filepath.Left(fileSteps);
	FbxNode* node = child.GetFbxNode();
	
	FbxProperty propert;
	for (int i = 0; i < node->GetSrcObjectCount<FbxSurfaceMaterial>(); i++)
	{
		child.setNrOfMaterials(child.getNrOfMaterials() + 1);
		FbxSurfaceMaterial* material = node->GetSrcObject<FbxSurfaceMaterial>(i);
		if (material)
		{
			int textureIndex = 0;
			FBXSDK_FOR_EACH_TEXTURE(textureIndex)
			{
				propert = material->FindProperty(FbxLayerElement::sTextureChannelNames[textureIndex]);
				if (propert.IsValid())
				{
					int textureCount = propert.GetSrcObjectCount<FbxTexture>();
					for (int j = 0; j < textureCount; j++)
					{
						ACJL::Texture acTexture;
						FbxLayeredTexture* layTextr = propert.GetSrcObject<FbxLayeredTexture>(j);
						if (layTextr)
						{
							printf("Layered texture: %i", j);
						}
						else
						{
							child.setNrOfTextures(child.getNrOfTextures() + 1);
							FbxTexture* fbTexture = propert.GetSrcObject<FbxTexture>(j);
							if (fbTexture)
							{
								FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(fbTexture);
								FbxString fullPath = fileTexture->GetFileName();

								int fullSizeOfString = fullPath.GetLen();
								int steps = fullPath.ReverseFind('/') +1;
								FbxString textureName = fullPath.Right(fullSizeOfString - steps);
								strcpy_s(child.GetMaterial()[j].albedoName, textureName);

								printf("folderpath: %s\n", fullPath);
								printf("folderpath: %s\n", filepath+textureName);

								strcpy_s(acTexture.path, textureName);
								strcpy_s(acTexture.name, fileTexture->GetName());
								if (CopyFile(fullPath, filepath+textureName, false))
									printf("copied file");
								child.GetTexture().push_back(acTexture);
							}
						}
					}
				}
			}
		}
	}

}

void GetMaterialAttr(ACJL::MaterialID& matID, std::vector<ACJL::Material>& materialHeader, FbxNode* node, FbxScene* scene)
{
	GetMaterialID(matID, node);
	LoadMaterialData(materialHeader, scene);
}
