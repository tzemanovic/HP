#pragma once
#include <vector>
#include <tuple>
#include "mesh.hpp"
namespace hp_ip
{
	class Renderer;
	enum class ModelType : UInt8
	{
		Loaded, BuiltIn, Count
	};
	enum struct BuiltInModelType : UInt8
	{
		Box, Count
	};
	typedef std::tuple<String, float> LoadedModelDef;
	typedef std::tuple<BuiltInModelType, FVec3> BuiltInModelDef;
	class Model
	{
	public:
		friend class ModelComponent;
		Model( );
		static Model* loadModelFromFile( Renderer* pRenderer,
			const LoadedModelDef& loadedModelDef );
		static Model* loadModelFromFBXFile( Renderer* pRenderer, const String& filename,
			const float scale );
		static Model* cubeMesh( Renderer* pRenderer, const FVec3& dimensions );
	private:
		static void computeTangentsAndBinormals( Vertex* vertices, UInt32 vertexCount,
			UInt32* indices, UInt32 indexCount );
		static void addVertex( Mesh& mesh, const Vertex vertex );
		static void addIndex( Mesh& mesh, const Index index );
		static bool createBuffers( Renderer* pRenderer, Mesh& mesh );
		static Model* cubeMesh( const FVec3& dimensions );
	private:
		std::vector<Mesh> _meshes;
	};
}