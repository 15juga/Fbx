#pragma once
#include "Data.h"
#include <string>
#include <vector>

namespace LIB
{
	enum ACJL_Load_Flags
	{
		ACJL_Load_Flags_PrintData = 0,
		ACJL_Load_Flags_SaveData = 1,
	};

	struct ACJLMesh
	{
		std::string name;
		std::vector<ACJL::Vertex> vertexes;
		std::vector<ACJL::MaterialID> matId;
		std::vector<ACJL::Material> mat;
	};

	struct ACJLBlendShape
	{
		std::string name;
		std::vector<ACJL::BlendShapeVertex> vertexes;
	};

	struct ACJLBSKeyFrameSet
	{
		std::string blendShapeName;
		std::vector<ACJL::BSKeyFrame> keyframes;
	};

	struct ACJLScene
	{
		std::vector<ACJLMesh>			 meshes;
		std::vector<ACJL::Light>		 m_lights;
		std::vector<ACJL::Camera>		 m_cams;
		std::vector<ACJLBlendShape>		 m_blendshapes;
		std::vector<ACJLBSKeyFrameSet>	 m_keyframeSet;
		std::vector<ACJL::Material>		 m_materials;
	};

	class ACJLReader
	{
	private:
		static ACJLScene m_scene;
	public:
		/// <summary>
		/// Reads a ACJL file and loads all assets into memory
		/// </summary>
		/// <param name="exportedFile: Path to the file"></param>
		/// <param name="printData: prints contents of file to the console if true"></param>
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
		static std::vector<ACJLMesh> Get<ACJLMesh>() { return m_scene.meshes; };
		/// <summary>
		/// Returns the meshes from the file
		/// </summary>
		/// <returns></returns>
		template<>
		static std::vector<ACJL::Light> Get<ACJL::Light>() { return m_scene.m_lights; };
		/// <summary>
		/// Returns the lights from the file
		/// </summary>
		/// <returns></returns>
		template<>
		static std::vector<ACJL::Camera> Get<ACJL::Camera>() { return m_scene.m_cams; };
		/// <summary>
		/// Returns the cameras from the file
		/// </summary>
		/// <returns></returns>
		template<>
		static std::vector<ACJLBlendShape> Get<ACJLBlendShape>() { return m_scene.m_blendshapes; };
		/// <summary>
		/// Returns the blendshape meshes from the file
		/// </summary>
		/// <returns></returns>
		template<>
		static std::vector<ACJLBSKeyFrameSet> Get<ACJLBSKeyFrameSet>() { return m_scene.m_keyframeSet; };
		/// <summary>
		/// Returns the materials from the file
		/// </summary>
		/// <returns></returns>
		template<>
		static std::vector<ACJL::Material> Get<ACJL::Material>() { return m_scene.m_materials; };
	};


}