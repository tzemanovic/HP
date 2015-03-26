#pragma once
#include <vector>
#include "vertex.hpp"
#include "../adt/list.hpp"
#include "../adt/maybe.hpp"
#include "../adt/sum.hpp"
#include "../math/vec3.hpp"
#include "../utils/typeId.hpp"
namespace hp_fp
{
	struct Renderer;
	enum struct BuiltInModelType : UInt8
	{
		Box, Count
	};
	struct BuiltInModelDef
	{
		BuiltInModelType type;
		FVec3 scale;
		bool operator == ( const BuiltInModelDef& m ) const
		{
			return type == m.type && scale == m.scale;
		}
		bool operator < ( const BuiltInModelDef& m ) const
		{
			if ( type == m.type )
			{
				return  scale < m.scale;
			}
			return type < m.type;
		}
	};
	struct LoadedModelDef
	{
		const char* filename;
		float scale;
		bool operator == ( const LoadedModelDef& m ) const
		{
			return filename == m.filename && scale == m.scale;
		}
		bool operator < ( const LoadedModelDef& m ) const
		{
			if ( filename == m.filename )
			{
				return  scale < m.scale;
			}
			return filename < m.filename;
		}
	};
	//typedef Sum<BuiltInModelDef, LoadedModelDef> ModelDef;
	struct ModelDef
	{
		template<typename A>
		bool is( ) const
		{
			return ( _typeId == typeId<A>( ) );
		}
	private:
		TypeId _typeId;
	public:
		union
		{
			BuiltInModelDef builtIn;
			LoadedModelDef loaded;
		};
		friend ModelDef builtInModelDef( BuiltInModelDef&& m );
		friend ModelDef loadedModelDef( LoadedModelDef&& m );
	};
	// [const][cop-c][cop-a][mov-c][mov-a]
	// [  +  ][  0  ][  0  ][  +  ][  +  ]
	struct Mesh
	{
		Mesh( ) : vertexBuffer( nullptr ), indexBuffer( nullptr )
		{ }
		Mesh( const Mesh& ) = delete;
		Mesh( Mesh&&  m ) : vertices( std::move( m.vertices ) ), indices( std::move( m.indices ) ),
			vertexBuffer( std::move( m.vertexBuffer ) ), indexBuffer( std::move( m.indexBuffer ) )
		{ }
		Mesh operator = ( const Mesh& ) = delete;
		Mesh operator = ( Mesh&& m )
		{
			return Mesh( std::move( m ) );
		}
		~Mesh( )
		{
			HP_RELEASE( indexBuffer );
			HP_RELEASE( vertexBuffer );
		}
		std::vector<Vertex> vertices;
		std::vector<Index> indices;
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
	};
	// [const][cop-c][cop-a][mov-c][mov-a]
	// [  +  ][  0  ][  0  ][  +  ][  +  ]
	struct Model
	{
		Model( )
		{ }
		Model( const Model& ) = delete;
		Model( Model&&  m ) : meshes( std::move( m.meshes ) )
		{ }
		Model operator = ( const Model& ) = delete;
		Model operator = ( Model&& m )
		{
			return Model( std::move( m ) );
		}
		std::vector<Mesh> meshes;
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/

	ModelDef builtInModelDef( BuiltInModelDef&& m );
	ModelDef loadedModelDef( LoadedModelDef&& m );
	void addVertex_IO( Mesh& mesh, const Vertex vertex );
	void addIndex_IO( Mesh& mesh, const Index index );
	void setBuffers_IO( Renderer& renderer, Mesh& mesh );
	Maybe<Model> loadModelFromFile_IO( Renderer& renderer, const String& filename, const float scale );
	Maybe<Model> cubeMesh_IO( Renderer& renderer, const FVec3& dimensions );
	namespace
	{
		void addMesh_IO( Model& meshes, Mesh&& mesh );
		Maybe<Model> loadModelFromFBXFile_IO( Renderer& renderer, const String& filename, const float scale );
		void computeTangentsAndBinormals_IO( Vertex* pVerticies, UInt32 vertexCount, UInt32* pIndices,
			UInt32 indexCount );
		bool createBuffers_IO( Renderer& renderer, Mesh& mesh );
		Model cubeMesh( const FVec3& dimensions );
	}
}