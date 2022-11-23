#pragma once

namespace ACJL
{
	//____________________________________________ Types ____________________________________________//
	enum DataType
	{
		START,
		END,
		MESH,
		VERTEX,
		TEXTURE,
		MATERIAL,
		LIGHT,
		CAMERA,
		KEYFRAMES,
		BLENDSHAPES,
		ANIMATION
	};

	enum TextureType
	{
		DIFFUSE,
		NORMAL
	};

	enum MaterialType
	{
		LAMBERT,
		PHONG
	};

	enum VertexType
	{
		BASICVTX,
		SKELETONVTX
	};

	enum LightType
	{
		POINTLIGHT,
		DIRECTIONAL,
		SPOT,
		AREA,
		VOLUME
	};

	enum LightDecayType
	{
		NONE,
		LINEAR,
		QUADRATIC,
		CUBIC
	};

	enum AreaLightShape
	{
		RECTANGLE,
		SPHERE
	};

	//____________________________________________ Generals ____________________________________________//

	struct Start
	{
		DataType dt = START;

		unsigned int nrOfMeshes = 0;
		unsigned int nrOfLight = 0;
		unsigned int nrOfCams = 0;
	};

	struct End
	{
		DataType dt = END;

		char endChar = '0';
	};

	//____________________________________________ Mesh ____________________________________________//
	struct Mesh
	{
		DataType dt = MESH;
		char meshName[50] = { "" };
		VertexType vt = BASICVTX;
		float trans[3] = { 0.0f, 0.0f, 0.0f };
		float rot[3] = { 0.0f, 0.0f, 0.0f };
		float scale[3] = { 0.0f, 0.0f, 0.0f };
		int nrOfAnimation = 0;
		int nrOfMaterial = 0;
		int nrOfTextures = 0;
		int nrOfVertices = 0;
		int nrOfIndices = 0;
		int nrOfBlendShapes = 0;
	};

	//____________________________________________ Vertex ____________________________________________//
	struct Vertex
	{
		float pos[3] = { 0.0f, 0.0f, 0.0f };
		float uv[2] = { 0 ,0 };
		float normals[3] = { 0.0f, 0.0f, 0.0f };
		float tangents[3] = { 0.0f, 0.0f, 0.0f };
		float bitangents[3] = { 0.0f, 0.0f, 0.0f };

	};

	//____________________________________________ Texture ____________________________________________//
	struct Texture
	{
		DataType dt = TEXTURE;
		char path[200] = { "" };
		char name[100] = { "" };
	};

	//____________________________________________ Material ____________________________________________//
	struct MaterialID
	{
		char matName[100] = { "" };
	};

	struct Material
	{
		DataType dt = MATERIAL;
		char matName[50] = { "" };
		MaterialType mt;
		float ambient[3] = { 0.0f, 0.0f, 0.0f };
		float diffuse[3] = { 0.0f, 0.0f, 0.0f };
		float specular[3] = { 0.0f, 0.0f, 0.0f };
		float specularIntensity = 0.0f;
		char albedoName[500] = { "" };
		char normalName[100] = { "" };
	};
	//____________________________________________ Light ____________________________________________//
	struct Light
	{
		DataType dt = LIGHT;
		LightType lt;

		char name[32] = { "Error" };
		char goboFileName[64];
		char shadowTexturePath[96];

		float trans[3] = { 0.0f, 0.0f, 0.0f };
		float intensity = 0.0f;
		float rot[3] = { 0.0f, 0.0f, 0.0f };
		float outerAngle = 0.0f;
		float scale[3] = { 0.0f, 0.0f, 0.0f };
		float fog = 0.0f;
		float rgb[3] = { 0.0f, 0.0f, 0.0f };
		float innerAngle = 0;


		bool castLight = false;
		bool drawVolumetric = false;
		bool drawGroundProj = false;
		bool drawFrontFacingVolumetricLight = false;
		float bottomBarnDoor = 0;
		LightDecayType decayType = NONE;
		float decayStart = 0;

		float nearAttenuationStart = 0;
		float nearAttenuationEnd = 0;
		float farAttenuationStart = 0;
		float farAttenuationEnd = 0;

		bool enableFarAttenuation = false;
		bool enableNearAttenuation = false;
		bool castShadow = false;
		bool enableBarnDoor = false;
		float leftBarnDoor = 0;
		float rightBarnDoor = 0;
		float topBarnDoor = 0;

		float shadowColor[3] = { 0,0,0 };
		AreaLightShape lightShape = RECTANGLE;
	};

	//____________________________________________ Camera ____________________________________________//
	struct Camera
	{
		DataType dt = CAMERA;
		char name[50] = { "" };
		float pos[3] = { 0.0f, 0.0f, 0.0f };
		float upVec[3] = { 0.0f, 0.0f, 0.0f };
		float forVec[3] = { 0.0f, 0.0f, 0.0f };
		float FOV = 0.0f;
		float nearZ = 0.0f;
		float farZ = 0.0f;
	};

	//____________________________________________ Animation ____________________________________________//
	struct BlendShape
	{
		DataType dt = BLENDSHAPES;
		float morphShape[4];
	};

	struct BlendShapeCountHeader
	{
		unsigned int morphshapeCount = 0;
	};

	struct BindPose
	{
		char ParentID[50] = { "" };
		VertexType vt = SKELETONVTX;

	};

	struct AnimationStart
	{

	};

	struct SkeletalAnimation
	{
		float duration = 0.0f;
		int nrOfKeyframes = 0;
		int nrOfJoints = 0;
	};

	struct Keyframe
	{
		DataType dt = KEYFRAMES;
		float timeStamp = 0.0f;
		float trans[3] = { 0.0f, 0.0f, 0.0f };
		float rot[3] = { 0.0f, 0.0f, 0.0f };
		float scale[3] = { 0.0f, 0.0f, 0.0f };
	};

}