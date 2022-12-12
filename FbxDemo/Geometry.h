#pragma once
#include "ACJL_Format.h"

void GetMeshData(ACJL::Mesh& meshHeader, FbxNode* node)
{
	FbxMesh* mesh = node->GetMesh();

	FbxAMatrix transform = node->EvaluateGlobalTransform();
	FbxVector4 trans = transform.GetT();
	FbxVector4 scale = transform.GetS();
	FbxQuaternion rot = transform.GetQ();

	meshHeader.trans[0] = (float)trans[0];
	meshHeader.trans[1] = (float)trans[1];
	meshHeader.trans[2] = (float)trans[2];

	meshHeader.scale[0] = (float)scale[0];
	meshHeader.scale[1] = (float)scale[1];
	meshHeader.scale[2] = (float)scale[2];

	meshHeader.rot[0] = (float)rot[0];
	meshHeader.rot[1] = (float)rot[1];
	meshHeader.rot[2] = (float)rot[2];

	//Will be taken in from the other functions
	meshHeader.nrOfAnimation = 0;
	meshHeader.nrOfMaterial = 0;
	meshHeader.nrOfVertices = 0;
}

inline std::vector<ACJL::BSKeyFrame> GetAnimCurves(FbxAnimCurve* pCurve)
{
	FbxTime pTime;
	int keyCount = pCurve->KeyGetCount();

	std::vector<ACJL::BSKeyFrame> keyFrames;

	for (int i = 0; i < keyCount; i++)
	{
		pTime = pCurve->KeyGetTime(i);

		ACJL::BSKeyFrame keyFrame;
		keyFrame.weight = (float)pCurve->KeyGetValue(i);
		//pTime.GetFramedTime()
		char timeString[256];
		pTime.GetTimeString(timeString, FbxUShort(256));

		keyFrame.time = atoi(timeString); // temporary solution, might fix later

		//keyFrame.time = pTime.GetTimeString()
		keyFrames.emplace_back(keyFrame);
	}
	return keyFrames;
}



inline void GetMorphData(ACJL::Mesh& meshHeader, FbxMesh* pMesh,
	std::vector<DYNAMIC::BlendShape>& outblendVerts, FbxAnimLayer* pAnimLayer, std::vector<DYNAMIC::ACJLBlendShapeChannel>& channels)
{
	int nOfBlendShapes = pMesh->GetDeformerCount();
	FbxNodeAttribute* nodeAttr = pMesh->GetNode()->GetNodeAttribute();
	FbxGeometry* pGeometry = (FbxGeometry*)nodeAttr;

	if (nodeAttr)
	{
		for (int bi = 0; bi < nOfBlendShapes; bi++)
		{
			FbxBlendShape* pFbxBs = (FbxBlendShape*)pMesh->GetDeformer(bi, FbxDeformer::eBlendShape);
			int nOChannels = pFbxBs->GetBlendShapeChannelCount();
			//std::vector<std::pair<char[64], std::vector<BSKeyFrame>>> channels;

			for (int j = 0; j < nOChannels; j++)
			{
				FbxBlendShapeChannel* blendShapeChannel = pFbxBs->GetBlendShapeChannel(j);

				const char* channelName = blendShapeChannel->GetName();
				FbxAnimCurve* pAnimCurve = pGeometry->GetShapeChannel(bi, j, pAnimLayer);

				if (pAnimCurve)
				{
					cout << "Shape: " << channelName << " ";
					DYNAMIC::ACJLBlendShapeChannel acjlChannel;
					//strcpy_s(acjlChannel.name, channelName);
					acjlChannel.name = channelName;
					acjlChannel.keyframes = GetAnimCurves(pAnimCurve);
					channels.emplace_back(acjlChannel);
				}

				int noShapes = blendShapeChannel->GetTargetShapeCount();
				std::vector<DYNAMIC::BlendShape> blendShapes;
				//for (int k = 0; k < noShapes; k++)
				//{
				FbxShape* pShape = blendShapeChannel->GetTargetShape(0);
				int cpCount = pShape->GetControlPointsCount();
				FbxVector4* cp = pShape->GetControlPoints();
				DYNAMIC::BlendShape bs;

				int numPolygon = pMesh->GetPolygonCount();
				int absoluteVertexIndex = 0;
				for (int p = 0; p < numPolygon; p++)
				{
					int vertexCount = pMesh->GetPolygonSize(p);
					
					for (int pv = 0; pv < vertexCount; pv++)
					{
						ACJL::BlendShapeVertex bsVert;
						int vertexIndex = pMesh->GetPolygonVertex(p, pv);

						bsVert.pos[0] = (float)cp[vertexIndex][0];
						bsVert.pos[1] = (float)cp[vertexIndex][1];
						bsVert.pos[2] = (float)cp[vertexIndex][2];
						
						for (int nI = 0; nI < pShape->GetElementNormalCount(); nI++)
						{
							FbxGeometryElementNormal* pElementNormal = pShape->GetElementNormal(nI);

							if (pElementNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex && 
								pElementNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
							{
								FbxVector4 normal = pElementNormal->GetDirectArray().GetAt(/*absoluteVertexIndex +*/ pv);
							

								bsVert.normal[0] = (float)normal[0];
								bsVert.normal[1] = (float)normal[1];
								bsVert.normal[2] = (float)normal[2];

								if (bsVert.normal[0] == 0.f && bsVert.normal[1] == 0.f && bsVert.normal[2] == 0.f)
								{
									printf("");
								}
							}
						}

						bs.vertArrs.emplace_back(bsVert);
						absoluteVertexIndex += vertexCount;
						//uint cpIndex = pShape->;
					}
				}
				bs.name = channelName;
				outblendVerts.emplace_back(bs);
				bs.vertArrs.clear();
			}
		}
	}
}

void GetMeshName(ACJL::Mesh& meshHeader, FbxNode* node)
{
	//Get name
	const char* name = node->GetName();
	strcpy_s(meshHeader.meshName, name);
}

void GetVertexData(ACJL::Mesh& meshHeader, vector<ACJL::Vertex>& vertexHeader, FbxNode* node)
{
	FbxMesh* mesh = node->GetMesh();
	FbxVector4* controlPoints = mesh->GetControlPoints();
	size_t polyCount = mesh->GetPolygonCount();
	int nrOfVertices = mesh->GetControlPointsCount();
	FbxArray<int> nr = mesh->mPolygonVertices;
	//int* test = mesh->GetPolygonVertices();

	meshHeader.nrOfVertices = nr.Size();

	int vertexID = 0;
	for (int i = 0; i < polyCount; i++)
	{
		//___________________________________________ Element Polygon Group ___________________________________________//
	/*	for (int e = 0; e < mesh->GetElementPolygonGroupCount(); e++)
		{
			FbxLayerElementPolygonGroup* elementPolyGrp = mesh->GetElementPolygonGroup(e);

			if (elementPolyGrp->GetMappingMode() == FbxGeometryElement::eByPolygon && elementPolyGrp->GetReferenceMode() == FbxGeometryElement::eIndex)
			{
				int polyGrpID = elementPolyGrp->GetIndexArray().GetAt(i);
			}
		}
		*/

		size_t vertexCount = mesh->GetPolygonSize(i);

		for (int j = 0; j < vertexCount; j++)
		{
			ACJL::Vertex vertex;
			//___________________________________________ POS ___________________________________________//
			size_t vertexIndex = mesh->GetPolygonVertex(i, j);

			for (int v = 0; v < 3; v++)
			{
				vertex.pos[v] = (float)controlPoints[vertexIndex][v];
			}


			//___________________________________________ UV ___________________________________________//
			for (int u = 0; u < mesh->GetElementUVCount(); u++)
			{
				FbxGeometryElementUV* elementUV = mesh->GetElementUV(u);

				//********** ONLY USING UV'S BY POLYGON VERTEX **********//
				if (elementUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex && elementUV->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
				{
					int UVIndex = mesh->GetTextureUVIndex(i, j);
					FbxVector2 uv = elementUV->GetDirectArray().GetAt(UVIndex);
					vertex.uv[0] = uv[0];
					vertex.uv[1] = uv[1];
				}
			}

			//___________________________________________ NORMAL___________________________________________//
			for (int n = 0; n < mesh->GetElementNormalCount(); n++)
			{
				FbxGeometryElementNormal* elementNormal = mesh->GetElementNormal(n);

				if (elementNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex && elementNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					FbxVector4 normal = elementNormal->GetDirectArray().GetAt(vertexID);
					vertex.normals[0] = normal[0];
					vertex.normals[1] = normal[1];
					vertex.normals[2] = normal[2];
				}
			}

			//___________________________________________ TAN ___________________________________________//
			for (int t = 0; t < mesh->GetElementTangentCount(); t++)
			{
				FbxGeometryElementTangent* elementTangent = mesh->GetElementTangent(t);

				if (elementTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex && elementTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					FbxVector4 tangent = elementTangent->GetDirectArray().GetAt(vertexID);

					vertex.tangents[0] = tangent[0];
					vertex.tangents[1] = tangent[1];
					vertex.tangents[2] = tangent[2];
				}
			}

			//___________________________________________ BITAN ___________________________________________//
			for (int b = 0; b < mesh->GetElementBinormalCount(); b++)
			{
				FbxGeometryElementBinormal* elementBi = mesh->GetElementBinormal(b);

				if (elementBi->GetMappingMode() == FbxGeometryElement::eByPolygonVertex && elementBi->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					FbxVector4 biTan = elementBi->GetDirectArray().GetAt(vertexID);

					vertex.bitangents[0] = biTan[0];
					vertex.bitangents[1] = biTan[1];
					vertex.bitangents[2] = biTan[2];
				}
			}

			//_________________________________________________________________________________________________________________________//
			vertexHeader.push_back(vertex);
			vertexID++;
		}


	}
}

void GetMaterials(ACJL::Mesh& meshHeader, FbxNode* node)
{
	FbxMesh* mesh = node->GetMesh();
	int polygonCount = mesh->GetPolygonCount();

	//Check if the materials are the same
	bool isAllSame = true;
	for (int i = 0; i < mesh->GetElementMaterialCount(); i++)
	{
		FbxGeometryElementMaterial* ElementMaterial = mesh->GetElementMaterial();
		if (ElementMaterial->GetMappingMode() == FbxGeometryElement::eByPolygon)
		{
			isAllSame = false;

			//Get the number of materials if not the same
			meshHeader.nrOfMaterial = mesh->GetElementMaterialCount();
			break;
		}
	}

	//If all materials are the same
	int j = 0;
	if (isAllSame)
	{
		for (j = 0; j < mesh->GetElementMaterialCount(); j++)
		{
			FbxGeometryElementMaterial* materialElement = mesh->GetElementMaterial(j);

			if (materialElement->GetMappingMode() == FbxGeometryElement::eAllSame)
			{
				FbxSurfaceMaterial* material = mesh->GetNode()->GetMaterial(materialElement->GetIndexArray().GetAt(0));

				//Number of materials is one
				meshHeader.nrOfMaterial = 1;
			}

		}

		//No material
		if (j == 0)
		{
			meshHeader.nrOfMaterial = 0;
		}
	}

}
//(__________________________________________________________)\\


void PrintData(vector<Child>childLib, int childCount)
{
	std::cout << childCount << endl;
	for (int i = 0; i < childCount; i++)
	{
		printf("Mesh name	:	%s\n", childLib[i].GetMesh().meshName);
		printf("Translation	:	%.4f %.4f %.4f\n", childLib[i].GetMesh().trans[0], childLib[i].GetMesh().trans[1], childLib[i].GetMesh().trans[2]);
		printf("Rotation	:	%.4f %.4f %.4f\n", childLib[i].GetMesh().rot[0], childLib[i].GetMesh().rot[1], childLib[i].GetMesh().rot[2]);
		printf("Scale		:	%.4f %.4f %.4f\n", childLib[i].GetMesh().scale[0], childLib[i].GetMesh().scale[1], childLib[i].GetMesh().scale[2]);

		for (int j = 0; j < childLib[i].GetMesh().nrOfVertices; j++)
		{
			printf("\nVertex %i\n", j);

			printf("Position	:	%.6f %.6f %.6f\n", childLib[i].GetVertexByIndex(j).pos[0], childLib[i].GetVertexByIndex(j).pos[1], childLib[i].GetVertexByIndex(j).pos[2]);
			printf("UV		:	%.6f %.6f\n", childLib[i].GetVertexByIndex(j).uv[0], childLib[i].GetVertexByIndex(j).uv[1]);
			printf("Normal		:	%.6f %.6f %.6f\n", childLib[i].GetVertexByIndex(j).normals[0], childLib[i].GetVertexByIndex(j).normals[1], childLib[i].GetVertexByIndex(j).normals[2]);
			printf("Tangents	:	%.6f %.6f %.6f\n", childLib[i].GetVertexByIndex(j).tangents[0], childLib[i].GetVertexByIndex(j).tangents[1], childLib[i].GetVertexByIndex(j).tangents[2]);
			printf("Bi-Tangents	:	%.6f %.6f %.6f\n", childLib[i].GetVertexByIndex(j).bitangents[0], childLib[i].GetVertexByIndex(j).bitangents[1], childLib[i].GetVertexByIndex(j).bitangents[2]);
		}
	}
}

void GetMeshAttr(Child& currentChild, FbxScene* pScene)
{
	GetMeshName(currentChild.GetMesh(), currentChild.GetFbxNode());

	GetMeshData(currentChild.GetMesh(), currentChild.GetFbxNode());

	GetVertexData(currentChild.GetMesh(), currentChild.GetVertices(), currentChild.GetFbxNode());

	GetMaterials(currentChild.GetMesh(), currentChild.GetFbxNode());

	FbxAnimStack* pAnimStack = pScene->GetSrcObject<FbxAnimStack>();

	GetMorphData(currentChild.GetMesh(), currentChild.GetFbxNode()->GetMesh(),
		currentChild.GetBlendVertArr(), pAnimStack->GetMember<FbxAnimLayer>(), currentChild.GetBlendShapeChannels());
}