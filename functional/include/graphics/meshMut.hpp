#pragma once
#include <vector>
#include <adt/list.hpp>
#include <graphics/vertex.hpp>
// [const][cop-c][cop-a][mov-c][mov-a]
// [  +  ][  0  ][  0  ][  0  ][  0  ]
namespace hp_fp
{
	struct MeshMut
	{
		MeshMut( ) : vertexBuffer( nullptr ), indexBuffer( nullptr )
		{ }
		~MeshMut( )
		{
			HP_RELEASE( indexBuffer );
			HP_RELEASE( vertexBuffer );
		}
		std::vector<Vertex> vertices;
		std::vector<Index> indices;
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
	};
	struct MeshesMut
	{
		std::vector<MeshMut> meshes;
	};
}