#pragma once
#include "Data.h"
#include <string>
#include <vector>

/*
This header contains all structs meant to be used by the user. Any struct under the ACJL namespace is not meanty to be handled by users
unless there is a "using" alternative below

The reasoning for this is because none of the ACJL datatypes are dynamic and have set sizes
*/
// Namespace LIB contains all datatypes meant to be used by the user of the lib
namespace LIB
{
	/// <summary>
	/// Material DataType Intended be used outside the library
	/// </summary>
	using ACJLMaterial = ACJL::Material;

	/// <summary>
	/// Vertex DataType Intended to be used outside the library
	/// </summary>
	using ACJLVertex = ACJL::Vertex;

	/// <summary>
	/// Material Datatype Intended to be used outside the library
	/// </summary>
	using ACJLMaterialID = ACJL::MaterialID;

	/// <summary>
	/// Light DataType Intended to be used outside the library
	/// </summary>
	using ACJLLight = ACJL::Light;

	/// <summary>
	/// Camera DataType Intended to be used outside the library
	/// </summary>
	using ACJLCamera = ACJL::Camera;

	/// <summary>
	/// Mesh DataType Intended to be used outside the library
	/// </summary>
	struct ACJLMesh
	{
		std::string name;
		std::vector<ACJLVertex> vertexes;
		std::vector<ACJLMaterialID> matId;
		std::vector<ACJLMaterial> mat;
	};
	/// <summary>
	/// Blendshape DataType Intended for the user
	/// </summary>
	struct ACJLBlendShape
	{
		std::string name;
		std::vector<ACJL::BlendShapeVertex> vertexes;
	};
	/// <summary>
	/// Keyframe set/BlendShape channel used by morph animation intended for the user
	/// </summary>
	struct ACJLBSKeyFrameSet
	{
		std::string blendShapeName;
		std::vector<ACJL::BSKeyFrame> keyframes;
	};
	/// <summary>
	/// Main scene struct
	/// </summary>
	struct ACJLScene
	{
		std::vector<ACJLMesh>			 meshes;
		std::vector<ACJL::Light>		 m_lights;
		std::vector<ACJL::Camera>		 m_cams;
		std::vector<ACJLBlendShape>		 m_blendshapes;
		std::vector<ACJLBSKeyFrameSet>	 m_keyframeSet;
		std::vector<ACJL::Material>		 m_materials;
	};


	/// <summary>
	/// ACJL Reader class. To get started using ACJL call ReadFile(), then call Get using desired datatype as template. The function
	/// returns a vector (std::vector) of all the components from the file
	/// </summary>
	class ACJLReader
	{
	private:
		static std::vector<ACJLScene> m_scene;
		static std::vector<std::string> m_sceneNames;
		static int m_sceneIndex;

		static int NameToIndex(std::string name);
	public:
		/// <summary>
		/// Deletes all previosly loaded data
		/// </summary>
		static void ClearMemory();


		static bool SelectLoadedFile(std::string name);

		/// <summary>
		/// Reads a ACJL file and loads all assets into memory
		/// </summary>
		/// <param name="exportedFile: Path to the file"></param>
		/// <param name="printData: Prints contents of file to the console if true"></param>
		static void ReadFile(const char* exportedFile, bool printData = true);
		/// <summary>
		/// Returns std::vector of entered datatype from the previosly loaded file. Will throw if the entered datatype is not suported
		/// The supported datatypes are listed in ACJLScene
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template<class T>
		static std::vector<T> Get() { throw; return std::vector<T>(); };
		/// <summary>
		/// Returns the meshes from the file
		/// </summary>
		/// <returns></returns>
		template<>
		static std::vector<ACJLMesh> Get<ACJLMesh>() { return m_scene[m_sceneIndex].meshes; };
		/// <summary>
		/// Returns the meshes from the file
		/// </summary>
		/// <returns></returns>
		template<>
		static std::vector<ACJL::Light> Get<ACJLLight>() { return m_scene[m_sceneIndex].m_lights; };
		/// <summary>
		/// Returns the lights from the file
		/// </summary>
		/// <returns></returns>
		template<>
		static std::vector<ACJL::Camera> Get<ACJLCamera>() { return m_scene[m_sceneIndex].m_cams; };
		/// <summary>
		/// Returns the cameras from the file
		/// </summary>
		/// <returns></returns>
		template<>
		static std::vector<ACJLBlendShape> Get<ACJLBlendShape>() { return m_scene[m_sceneIndex].m_blendshapes; };
		/// <summary>
		/// Returns the blendshape meshes from the file
		/// </summary>
		/// <returns></returns>
		template<>
		static std::vector<ACJLBSKeyFrameSet> Get<ACJLBSKeyFrameSet>() { return m_scene[m_sceneIndex].m_keyframeSet; };
		/// <summary>
		/// Returns the materials from the file
		/// </summary>
		/// <returns></returns>
		template<>
		static std::vector<ACJLMaterial> Get<ACJLMaterial>() { return m_scene[m_sceneIndex].m_materials; };
	};
}