#pragma once
#include "ACJL_Format.h"
class Child
{
private:
	FbxNode* fbxNode;
	int attrCount;

	ACJL::Mesh mesh;
	std::vector<ACJL::Vertex> vertices;
	std::vector<ACJL::MaterialID> materialID;
	std::vector<ACJL::Texture> textures;
	std::vector<ACJL::Material> material;
	unsigned int nrOfMaterials = 0;
	unsigned int nrOfTextures = 0;

	vector<ACJL::BlendShape> morph;
	vector<FbxVector4*> blendVerts;
public:
	Child() {}
	Child(FbxNode* node)
	{
		fbxNode = node;
		attrCount = 0;
	}
	~Child() {}

	//_____________________________________________________________________________//
	void SetAttrCount(int count) { attrCount = count; }
	int GetAttrCount() { return attrCount; }

	FbxNode* GetFbxNode() const { return fbxNode; }
	FbxMesh* GetFbxMesh() const { return fbxNode->GetMesh(); }

	ACJL::Mesh &GetMesh() { return mesh; }
	std::vector<ACJL::Vertex> &GetVertices() { return vertices; }
	std::vector<ACJL::MaterialID>& GetMaterialID() { return materialID; }
	std::vector<ACJL::Material>& GetMaterial() { return material; }
	std::vector<ACJL::Texture>& GetTexture() { return textures; }
	std::vector<ACJL::BlendShape>& GetBlendShape() { return morph; }

	ACJL::Vertex& GetVertexByIndex(int index) { return vertices[index]; }

	unsigned int getNrOfMaterials() const { return nrOfMaterials; }
	unsigned int getNrOfTextures() const { return nrOfTextures; }
	void setNrOfMaterials(unsigned int amount) { nrOfMaterials = amount; }
	void setNrOfTextures(unsigned int amount) { nrOfTextures = amount; }


};