#pragma once
#include "ACJL_Format.h"
class Light
{
private:
	FbxNode* fbxNode;
	ACJL::Light light;
public:
	Light() {}
	Light(FbxNode* node) { this->fbxNode = node; }
	FbxNode* const getFbxNode() { return this->fbxNode; }
	FbxLight* const getFbxLight() { return (FbxLight*)this->fbxNode->GetNodeAttribute(); }
	ACJL::Light& getLight() { return this->light; }
};


void GetLightData(FbxNode* node, ACJL::Light &light)
{
	FbxLight* lLight = (FbxLight*)node->GetNodeAttribute();
	FbxString lightType[] = { "Point", "Directional", "Spot", "Area", "Volume" };
	FbxString name = node->GetName();
	FbxAMatrix matrix = node->EvaluateGlobalTransform();

	FbxDouble3 position = node->LclTranslation.Get();

	FbxVector4 transform = matrix.GetT();
	FbxVector4 rotation = matrix.GetR();
	FbxVector4 scale = matrix.GetS();

	light.lt = (ACJL::LightType)lLight->LightType.Get();
	strcpy_s(light.name, name);

	for (int i = 0; i < 3; i++)
	{
		light.trans[i] = transform[i];
		light.rot[i] = rotation[i];
		light.scale[i] = scale[i];
		light.rgb[i] = lLight->Color.Get()[i];
		light.shadowColor[i] = lLight->ShadowColor.Get()[i];
	}
	light.intensity = lLight->Intensity.Get();
	light.outerAngle = lLight->OuterAngle;
	light.fog = lLight->Fog.Get();
	light.innerAngle = lLight->InnerAngle.Get();

	light.castLight = lLight->CastLight.Get();
	light.drawVolumetric = lLight->DrawVolumetricLight.Get();
	light.drawGroundProj = lLight->DrawGroundProjection.Get();
	light.drawFrontFacingVolumetricLight = lLight->DrawFrontFacingVolumetricLight.Get();
	light.bottomBarnDoor = lLight->BottomBarnDoor.Get();
	light.decayType = (ACJL::LightDecayType)lLight->DecayType.Get();
	
	light.nearAttenuationStart = lLight->NearAttenuationStart.Get();
	light.nearAttenuationEnd = lLight->NearAttenuationEnd.Get();
	light.farAttenuationStart = lLight->FarAttenuationStart.Get();
	light.farAttenuationEnd = lLight->FarAttenuationEnd.Get();

	light.enableFarAttenuation = lLight->EnableFarAttenuation.Get();
	light.enableNearAttenuation = lLight->EnableNearAttenuation.Get();
	light.castShadow = lLight->CastShadows.Get();
	light.enableBarnDoor = lLight->EnableBarnDoor.Get();
	light.rightBarnDoor = lLight->RightBarnDoor.Get();
	light.leftBarnDoor = lLight->LeftBarnDoor.Get();

	light.lightShape = (ACJL::AreaLightShape)lLight->AreaLightShape.Get();
}