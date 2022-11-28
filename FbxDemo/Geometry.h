#pragma once

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

struct BSKeyFrame
{
	unsigned int time;
	float weight;
};

inline void GetAnimCurves(FbxAnimCurve* pCurve)
{
	FbxTime pTime;
	int keyCount = pCurve->KeyGetCount();

	std::vector<BSKeyFrame> keyFrames;

	for (int i = 0; i < keyCount; i++)
	{
		pTime = pCurve->KeyGetTime(i);
		BSKeyFrame keyFrame;
		keyFrame.weight = (float)pCurve->KeyGetValue(i);


		//keyFrame.time = pTime.GetTimeString()
		keyFrames.emplace_back(keyFrame);
	}
}

inline void GetMorphData(ACJL::Mesh& meshHeader, FbxMesh* pMesh, 
	std::vector<ACJL::BlendShape>& outMorph, std::vector<fbxsdk::FbxVector4*>& outblendVerts, FbxAnimLayer* pAnimLayer)
{
	int nOfBlendShapes = pMesh->GetDeformerCount();
	FbxNodeAttribute* nodeAttr = pMesh->GetNode()->GetNodeAttribute();
	FbxGeometry* pGeometry = (FbxGeometry*)nodeAttr;

	if (nodeAttr)
	{
		ACJL::BlendShape bs;
		for (int bi = 0; bi < nOfBlendShapes; bi++)
		{
			FbxBlendShape* pFbxBs = (FbxBlendShape*)pMesh->GetDeformer(bi, FbxDeformer::eBlendShape);
			int nOChannels = pFbxBs->GetBlendShapeChannelCount();

			for (int j = 0; j < nOChannels; j++)
			{
				FbxBlendShapeChannel* blendShapeChannel = pFbxBs->GetBlendShapeChannel(j);

				const char* channelName = blendShapeChannel->GetName();
				FbxAnimCurve* pAnimCurve = pGeometry->GetShapeChannel(bi, j, pAnimLayer);

				if (pAnimCurve)
				{
					cout << "Shape: " << channelName << " ";
					GetAnimCurves(pAnimCurve);
				}

				//int noShapes = blendShapeChannel->GetTargetShapeCount();
				//for (int k = 0; k < noShapes; k++)
				//{
				//	FbxShape* pShape = blendShapeChannel->GetTargetShape(k);
				//	int cpCount = pShape->GetControlPointsCount();
				//	FbxVector4* cp = pShape->GetControlPoints();
				//	std::vector<FbxVector4> vec;
				//	outblendVerts.emplace_back(cp);
				//}
				

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

					vertex.tangents[t] = tangent[t];
				}
			}

			//___________________________________________ BITAN ___________________________________________//
			for (int b = 0; b < mesh->GetElementBinormalCount(); b++)
			{
				FbxGeometryElementBinormal* elementBi = mesh->GetElementBinormal(b);

				if (elementBi->GetMappingMode() == FbxGeometryElement::eByPolygonVertex && elementBi->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					FbxVector4 biTan = elementBi->GetDirectArray().GetAt(vertexID);

					vertex.bitangents[b] = biTan[b];
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

	std::vector<fbxsdk::FbxVector4*> blendVerts;
	FbxAnimStack* pAnimStack = pScene->GetSrcObject<FbxAnimStack>();
	int numAnimLayer = pAnimStack->GetMemberCount<FbxAnimLayer>();
	for (int i = 0; i < numAnimLayer; i++)
	{
		GetMorphData(currentChild.GetMesh(), currentChild.GetFbxNode()->GetMesh(), currentChild.GetBlendShape(),
			blendVerts, pAnimStack->GetMember<FbxAnimLayer>(i));
	}

	
}