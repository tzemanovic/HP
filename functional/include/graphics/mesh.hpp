#pragma once
#include <vector>
#include "vertex.hpp"
#include "../adt/list.hpp"
#include "../adt/maybe.hpp"
#include "../math/vec3.hpp"
namespace hp_fp
{
	// [const][cop-c][cop-a][mov-c][mov-a]
	// [  +  ][  0  ][  0  ][  0  ][  0  ]
	struct Mesh
	{
		Mesh( ) : vertexBuffer( nullptr ), indexBuffer( nullptr )
		{
			LOG( "const" );
		}
		Mesh( const Mesh&  m ) : vertices( m.vertices ), indices( m.indices ),
			vertexBuffer( m.vertexBuffer ), indexBuffer( m.indexBuffer )
		{
			LOG( "cop-c" );
		}
		Mesh operator=( const Mesh& m )
		{
			LOG( "cop-a" );
			return Mesh( m );
		}
		~Mesh( )
		{
			//HP_RELEASE( indexBuffer );
			//HP_RELEASE( vertexBuffer );
		}
		std::vector<Vertex> vertices;
		std::vector<Index> indices;
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
	};
	struct Meshes
	{
		Meshes( )
		{ }
		Meshes( const Meshes&  m ) : meshes( m.meshes )
		{ }
		Meshes operator=( const Meshes& m )
		{
			return Meshes( m );
		}
		std::vector<Mesh> meshes;
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/
	struct Renderer;
	void addVertex_IO( Mesh& mesh, const Vertex vertex );
	void addIndex_IO( Mesh& mesh, const Index index );
	bool createBuffers_IO( Renderer& renderer, Mesh& mesh );
	void setBuffers_IO( Renderer& renderer, Mesh& mesh );
	Maybe<Meshes> loadModelFromFile_IO( const String& filename, const float scale );
	Meshes cubeMesh( const FVec3& dimensions );
	namespace
	{
		void addMesh_IO( Meshes& meshes, Mesh&& mesh );
		Maybe<Meshes> loadModelFromFBXFile_IO( const String& filename, const float scale );
		void computeTangentsAndBinormals_IO( Vertex* pVerticies, UInt32 vertexCount, UInt32* pIndices,
			UInt32 indexCount );
	}
}