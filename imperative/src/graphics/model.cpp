#include <pch.hpp>
#include "../../include/graphics/model.hpp"
#include <algorithm>
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")
#include "../../include/graphics/renderer.hpp"
namespace hp_ip
{
	Model::Model( )
	{

	}
	Model* Model::loadModelFromFile( Renderer* pRenderer, const LoadedModelDef& loadedModelDef )
	{
		String filename = std::get<0>( loadedModelDef );
		String fileExt{ filename.substr( filename.find( '.' ) + 1 ) };
		std::transform( fileExt.begin( ), fileExt.end( ), fileExt.begin( ), ::tolower );
		if ( fileExt.compare( "fbx" ) == 0 )
		{
			return loadModelFromFBXFile( pRenderer, filename, std::get<1>( loadedModelDef ) );
		}
		return nullptr;
	}
	Model* Model::loadModelFromFBXFile( Renderer* pRenderer, const String& filename,
		const float scale )
	{
		Model* model = HP_NEW Model( );
		FbxManager* manager = FbxManager::Create( );
		FbxIOSettings* settings = FbxIOSettings::Create( manager, IOSROOT );
		manager->SetIOSettings( settings );
		FbxImporter* importer = FbxImporter::Create( manager, "" );
		FbxGeometryConverter converter( manager );
		if ( !importer->Initialize( filename.c_str( ), -1, manager->GetIOSettings( ) ) )
		{
			HP_DELETE( model );
			return nullptr;
		}
		FbxScene* scene = FbxScene::Create( manager, "myScene" );
		FbxAxisSystem axisSystem = scene->GetGlobalSettings( ).GetAxisSystem( );
		importer->Import( scene );
		importer->Destroy( );
		FbxNode* rootNode = scene->GetRootNode( );
		FbxMesh* fbxMesh = NULL;
		if ( rootNode )
		{
			for ( Int32 i = 0; i < rootNode->GetChildCount( ); ++i )
			{
				FbxNode* childNode = rootNode->GetChild( i );
				for ( Int32 j = 0; j < childNode->GetNodeAttributeCount( ); ++j )
				{
					FbxNodeAttribute* nodeAttribute = childNode->GetNodeAttributeByIndex( j );
					converter.Triangulate( nodeAttribute, true );
					if ( nodeAttribute->GetAttributeType( ) == FbxNodeAttribute::eMesh )
					{
						fbxMesh = (FbxMesh*) ( nodeAttribute );
						if ( fbxMesh )
						{
							Mesh mesh;
							FbxVector4* fbxVertices = fbxMesh->GetControlPoints( );
							UInt32 vertexCount = fbxMesh->GetControlPointsCount( );
							UInt32 indexCount = fbxMesh->GetPolygonVertexCount( );
							Index* indices = (Index*) fbxMesh->GetPolygonVertices( );
							Vertex* vertices = new Vertex[vertexCount];
							for ( UInt32 k = 0; k < vertexCount; ++k )
							{
								vertices[k].position.x = float( fbxVertices[k][0] * scale );
								vertices[k].position.y = float( fbxVertices[k][1] * scale );
								vertices[k].position.z = float( fbxVertices[k][2] * scale );
							}

							for ( Int32 polyIndex = 0; polyIndex < fbxMesh->GetPolygonCount( ); ++polyIndex )
							{
								for ( UInt32 vertexIndex = 0; vertexIndex < 3; ++vertexIndex )
								{
									UInt32 cornerIndex = fbxMesh->GetPolygonVertex( polyIndex, vertexIndex );
									FbxVector4 fbxVertex = fbxVertices[cornerIndex];
									FbxVector4 fbxNormal;
									fbxMesh->GetPolygonVertexNormal( polyIndex, vertexIndex, fbxNormal );
									fbxNormal.Normalize( );
									vertices[cornerIndex].normal = FVec3{ float( fbxNormal[0] ),
										float( fbxNormal[1] ), float( fbxNormal[2] ) };
									FbxVector2 fbxUV = FbxVector2( 0, 0 );
									FbxLayerElementUV* fbxUVs = fbxMesh->GetLayer( 0 )->GetUVs( );
									if ( fbxUVs )
									{
										UInt32 UVIndex = 0;
										switch ( fbxUVs->GetMappingMode( ) )
										{
										case FbxLayerElement::eByControlPoint:
											UVIndex = cornerIndex;
											break;
										case FbxLayerElement::eByPolygonVertex:
											UVIndex = fbxMesh->GetTextureUVIndex( polyIndex, vertexIndex, FbxLayerElement::eTextureDiffuse );
											break;
										}
										fbxUV = fbxUVs->GetDirectArray( ).GetAt( UVIndex );
										vertices[cornerIndex].texCoord.x = float( fbxUV[0] );
										vertices[cornerIndex].texCoord.y = float( 1.0 - fbxUV[1] );
									}
								}
							}
							computeTangentsAndBinormals( vertices, vertexCount, indices, indexCount );
							for ( UInt32 k = 0; k < vertexCount; ++k )
							{
								addVertex( mesh, vertices[k] );
							}
							for ( UInt32 k = 0; k < indexCount; ++k )
							{
								addIndex( mesh, indices[k] );
							}
							HP_DELETE_ARRAY( vertices );
							model->_meshes.push_back( std::move( mesh ) );
						}
					}
				}
			}
			bool buffersCreated = true;
			for ( auto& mesh : model->_meshes )
			{
				buffersCreated &= createBuffers( pRenderer, mesh );
			}
			if ( buffersCreated )
			{
				settings->Destroy( );
				manager->Destroy( );
				return model;
			}
			else
			{
				ERR( "Failed to initalize \"" + filename + "\"'s buffers." );
			}
		}
		ERR( "Failed to load \"" + filename + "\" model." );
		settings->Destroy( );
		manager->Destroy( );
		HP_DELETE( model );
		return nullptr;
	}
	Model* Model::cubeMesh( Renderer* pRenderer, const FVec3& dimensions )
	{
		Model* model = cubeMesh( dimensions );
		bool buffersCreated = true;
		for ( auto& mesh : model->_meshes )
		{
			buffersCreated &= createBuffers( pRenderer, mesh );
		}
		if ( buffersCreated )
		{
			return model;
		}
		ERR( "Failed to initalize cube's buffers." );
		HP_DELETE( model );
		return nullptr;
	}
	void Model::computeTangentsAndBinormals( Vertex* vertices, UInt32 vertexCount,
		UInt32* indices, UInt32 indexCount )
	{
		UInt32 triCount = indexCount / 3;
		FVec3* tangents = HP_NEW FVec3[vertexCount];
		FVec3* binormals = HP_NEW FVec3[vertexCount];
		for ( UInt32 i = 0; i < triCount; i += 3 )
		{
			FVec3 v1 = vertices[indices[i]].position;
			FVec3 v2 = vertices[indices[i + 1]].position;
			FVec3 v3 = vertices[indices[i + 2]].position;
			FVec2 uv1 = vertices[indices[i]].texCoord;
			FVec2 uv2 = vertices[indices[i + 1]].texCoord;
			FVec2 uv3 = vertices[indices[i + 2]].texCoord;
			FVec3 edge1 = v2 - v1;
			FVec3 edge2 = v3 - v1;
			FVec2 edge1uv = uv2 - uv1;
			FVec2 edge2uv = uv3 - uv1;
			float cp = edge1uv.x * edge2uv.y - edge1uv.y * edge2uv.x;
			if ( cp != 0.0f )
			{
				float mul = 1.0f / cp;
				FVec3 tan = ( edge1 * edge2uv.y - edge2 * edge1uv.y ) * mul;
				FVec3 binorm = ( edge1 * edge2uv.x - edge2 * edge1uv.x ) * mul;
				tangents[indices[i]] += tan;
				binormals[indices[i]] += binorm;
				tangents[indices[i + 1]] += tan;
				binormals[indices[i + 1]] += binorm;
				tangents[indices[i + 2]] += tan;
				binormals[indices[i + 2]] += binorm;
			}
		}
		for ( UInt32 i = 0; i < vertexCount; ++i )
		{
			vertices[i].tangent = FVec3::normalize( tangents[i] );
			vertices[i].binormal = FVec3::normalize( binormals[i] );
		}
		HP_DELETE_ARRAY( tangents );
		HP_DELETE_ARRAY( binormals );
	}
	void Model::addVertex( Mesh& mesh, const Vertex vertex )
	{
		mesh._vertices.push_back( vertex );
	}
	void Model::addIndex( Mesh& mesh, const Index index )
	{
		mesh._indices.push_back( index );
	}
	bool Model::createBuffers( Renderer* pRenderer, Mesh& mesh )
	{
		if ( pRenderer->createVertexBuffer( &mesh._vertexBuffer,
			sizeof( Vertex ) * mesh._vertices.size( ), &mesh._vertices.at( 0 ) ) )
		{
			if ( pRenderer->createIndexBuffer( &mesh._indexBuffer,
				sizeof( Index ) * mesh._indices.size( ), &mesh._indices.at( 0 ) ) )
			{
				return true;
			}
		}
		return false;
	}
	Model* Model::cubeMesh( const FVec3& dimensions )
	{
		Model* model = HP_NEW Model( );
		Mesh mesh;
		float halfWidth = dimensions.x / 2.f;
		float halfHeight = dimensions.y / 2.f;
		float halfLength = dimensions.z / 2.f;
		Vertex vertices[] =
		{
			{ FVec3{ -halfWidth, halfHeight, -halfLength }, Color( ), FVec2{ 0.0f, 0.0f }, FVec3::up }, // 0 +Y (top face)
			{ FVec3{ halfWidth, halfHeight, -halfLength }, Color( ), FVec2{ 1.0f, 0.0f }, FVec3::up }, // 1
			{ FVec3{ halfWidth, halfHeight, halfLength }, Color( ), FVec2{ 1.0f, 1.0f }, FVec3::up }, // 2
			{ FVec3{ -halfWidth, halfHeight, halfLength }, Color( ), FVec2{ 0.0f, 1.0f }, FVec3::up }, // 3

			{ FVec3{ -halfWidth, -halfHeight, halfLength }, Color( ), FVec2{ 0.0f, 0.0f }, -1 * FVec3::up }, // 4 -Y (bottom face)
			{ FVec3{ halfWidth, -halfHeight, halfLength }, Color( ), FVec2{ 1.0f, 0.0f }, -1 * FVec3::up }, // 5
			{ FVec3{ halfWidth, -halfHeight, -halfLength }, Color( ), FVec2{ 1.0f, 1.0f }, -1 * FVec3::up }, // 6
			{ FVec3{ -halfWidth, -halfHeight, -halfLength }, Color( ), FVec2{ 0.0f, 1.0f }, -1 * FVec3::up }, // 7

			{ FVec3{ halfWidth, halfHeight, halfLength }, Color( ), FVec2{ 0.0f, 0.0f }, FVec3::right }, // 8 +X (right face)
			{ FVec3{ halfWidth, halfHeight, -halfLength }, Color( ), FVec2{ 1.0f, 0.0f }, FVec3::right }, // 9
			{ FVec3{ halfWidth, -halfHeight, -halfLength }, Color( ), FVec2{ 1.0f, 1.0f }, FVec3::right }, // 10
			{ FVec3{ halfWidth, -halfHeight, halfLength }, Color( ), FVec2{ 0.0f, 1.0f }, FVec3::right }, // 11

			{ FVec3{ -halfWidth, halfHeight, -halfLength }, Color( ), FVec2{ 0.0f, 0.0f }, -1 * FVec3::right }, // 12 -X (left face)
			{ FVec3{ -halfWidth, halfHeight, halfLength }, Color( ), FVec2{ 1.0f, 0.0f }, -1 * FVec3::right }, // 13
			{ FVec3{ -halfWidth, -halfHeight, halfLength }, Color( ), FVec2{ 1.0f, 1.0f }, -1 * FVec3::right }, // 14
			{ FVec3{ -halfWidth, -halfHeight, -halfLength }, Color( ), FVec2{ 0.0f, 1.0f }, -1 * FVec3::right }, // 15

			{ FVec3{ -halfWidth, halfHeight, halfLength }, Color( ), FVec2{ 0.0f, 0.0f }, FVec3::forward }, // 16 +Z (front face)
			{ FVec3{ halfWidth, halfHeight, halfLength }, Color( ), FVec2{ 1.0f, 0.0f }, FVec3::forward }, // 17
			{ FVec3{ halfWidth, -halfHeight, halfLength }, Color( ), FVec2{ 1.0f, 1.0f }, FVec3::forward }, // 18
			{ FVec3{ -halfWidth, -halfHeight, halfLength }, Color( ), FVec2{ 0.0f, 1.0f }, FVec3::forward }, // 19

			{ FVec3{ halfWidth, halfHeight, -halfLength }, Color( ), FVec2{ 0.0f, 0.0f }, -1 * FVec3::forward }, // 20 -Z (back face)
			{ FVec3{ -halfWidth, halfHeight, -halfLength }, Color( ), FVec2{ 1.0f, 0.0f }, -1 * FVec3::forward }, // 21
			{ FVec3{ -halfWidth, -halfHeight, -halfLength }, Color( ), FVec2{ 1.0f, 1.0f }, -1 * FVec3::forward }, // 22
			{ FVec3{ halfWidth, -halfHeight, -halfLength }, Color( ), FVec2{ 0.0f, 1.0f }, -1 * FVec3::forward }, // 23
		};
		Index indices[] =
		{
			1, 0, 2, 2, 0, 3, // top face
			5, 4, 6, 6, 4, 7, // bottom
			9, 8, 10, 10, 8, 11, // right
			13, 12, 14, 14, 12, 15, // left
			17, 16, 18, 18, 16, 19, // front
			21, 20, 22, 22, 20, 23 // back
		};
		computeTangentsAndBinormals( vertices, 24, indices, 36 );
		for ( UInt16 i = 0; i < 24; i++ )
		{
			addVertex( mesh, vertices[i] );
		}
		for ( UInt16 i = 0; i < 36; i++ )
		{
			addIndex( mesh, indices[i] );
		}
		model->_meshes.push_back( std::move( mesh ) );
		return model;
	}
}