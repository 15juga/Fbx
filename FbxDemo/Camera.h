#pragma once
#include <fbxsdk.h>
#include <vector>
#include <string>
#include "ACJL_Format.h"

void getCamAttr(FbxNode* node, std::vector<ACJL::Camera>& acjlCamVec)
{
	ACJL::Camera acjlCam;
	FbxCamera* camNode = (FbxCamera*)node->GetNodeAttribute();

	FbxDouble3 position = camNode->Position.Get();
	FbxDouble3 upVector = camNode->UpVector.Get();
	FbxAMatrix matrix = node->EvaluateGlobalTransform();
	FbxDouble3 forVector = node->LclRotation.Get();

	for (int i = 0; i < 3; i++)
	{
		acjlCam.pos[i] = position[i];
		acjlCam.upVec[i] = upVector[i];
		acjlCam.forVec[i] = forVector[i];
	}
	strcpy_s(acjlCam.name, node->GetName());
	acjlCam.FOV = camNode->FieldOfView.Get();
	acjlCam.nearZ = camNode->NearPlane.Get();
	acjlCam.farZ = camNode->FarPlane.Get();
	acjlCamVec.push_back(acjlCam);
}