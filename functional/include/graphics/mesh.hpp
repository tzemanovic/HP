#pragma once
#include <adt/maybe.hpp>
#include <graphics/meshMut.hpp>
#include <graphics/vertex.hpp>
#include <math/vec3.hpp>
namespace hp_fp
{
	struct RendererMut;
	void addVertex_IO( MeshMut& mesh, const Vertex vertex );
	void addIndex_IO( MeshMut& mesh, const Index index );
	bool createBuffers_IO( RendererMut& renderer, MeshMut& mesh );
	void setBuffers_IO( RendererMut& renderer, MeshMut& mesh );
	Maybe<MeshesMut> loadModelFromFile_IO( String&& filename, const float scale );
	MeshesMut cubeMesh( const FVec3& dimensions );
	namespace
	{
		void addMesh_IO( MeshesMut& meshes, MeshMut&& mesh );
		Maybe<MeshesMut> loadModelFromFBXFile_IO( String&& filename, const float scale );
		void computeTangentsAndBinormals_IO( Vertex* pVerticies, UInt32 vertexCount, UInt32* pIndices,
			UInt32 indexCount );
	}
}