#pragma once
#include <vector>
#include "vertex.hpp"
namespace hp_ip
{
	class Mesh
	{
		friend class Model;
		friend class Renderer;
	public:
		Mesh( ) : _vertexBuffer( nullptr ), _indexBuffer( nullptr )
		{ }
		~Mesh( )
		{
			HP_RELEASE( _indexBuffer );
			HP_RELEASE( _vertexBuffer );
		}
		size_t indicesSize( )
		{
			return _indices.size( );
		}
		/*std::vector<Vertex>& vertices( )
		{
		return _vertices;
		}
		std::vector<Index>& indices( )
		{
		return _indices;
		}*/
	private:
		std::vector<Vertex> _vertices;
		std::vector<Index> _indices;
		ID3D11Buffer* _vertexBuffer;
		ID3D11Buffer* _indexBuffer;
	};
}

