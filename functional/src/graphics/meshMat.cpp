#include <pch.hpp>
#include <algorithm>
#include "../../include/graphics/meshMat.hpp"
#include "../../include/graphics/renderer.hpp"
#include "../../include/graphics/vertex.hpp"
#include "../../include/utils/stringUtils.hpp"
namespace hp_fp
{
	MeshMat defaultMat( )
	{
		return MeshMat
		{
			"assets/shaders/parallax.fx",
			"Render",
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			{ },
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			{ 1.0f, 1.0f },
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			{ 0.5f, 0.5f, 0.5f },
			{ 0.8f, 0.8f, 0.8f },
			{ 1.0f, 1.0f, 1.0f },
			25.0f
		};
	}
	bool init_IO( MeshMat& meshMat, Renderer& renderer )
	{
		if ( loadShader_IO( meshMat, renderer ) )
		{
			meshMat.technique = meshMat.effect->GetTechniqueByName( meshMat.techniqueName.c_str( ) );
			meshMat.technique->GetDesc( &meshMat.techniqueDesc );
			if ( createVertexLayout_IO( meshMat, renderer ) )
			{
				// retrieve all variables using semantic
				meshMat.worldMatrixVariable = meshMat.effect->GetVariableBySemantic( "WORLD" )->AsMatrix( );
				meshMat.viewMatrixVariable = meshMat.effect->GetVariableBySemantic( "VIEW" )->AsMatrix( );
				meshMat.projectionMatrixVariable = meshMat.effect->GetVariableBySemantic( "PROJECTION" )->AsMatrix( );
				meshMat.diffuseTextureVariable = meshMat.effect->GetVariableByName( "diffuseMap" )->AsShaderResource( );
				meshMat.specularTextureVariable = meshMat.effect->GetVariableByName( "specularMap" )->AsShaderResource( );
				meshMat.bumpTextureVariable = meshMat.effect->GetVariableByName( "bumpMap" )->AsShaderResource( );
				meshMat.parallaxTextureVariable = meshMat.effect->GetVariableByName( "heightMap" )->AsShaderResource( );
				meshMat.envMapVariable = meshMat.effect->GetVariableByName( "envMap" )->AsShaderResource( );
				// lights
				meshMat.ambientLightColourVariable = meshMat.effect->GetVariableByName( "ambientLightColour" )->AsVector( );
				meshMat.diffuseLightColourVariable = meshMat.effect->GetVariableByName( "diffuseLightColour" )->AsVector( );
				meshMat.specularLightColourVariable = meshMat.effect->GetVariableByName( "specularLightColour" )->AsVector( );
				meshMat.lightDirectionVariable = meshMat.effect->GetVariableByName( "lightDirection" )->AsVector( );
				// materials
				meshMat.ambientMaterialVariable = meshMat.effect->GetVariableByName( "ambientMaterialColour" )->AsVector( );
				meshMat.diffuseMaterialVariable = meshMat.effect->GetVariableByName( "diffuseMaterialColour" )->AsVector( );
				meshMat.specularMaterialVariable = meshMat.effect->GetVariableByName( "specularMaterialColour" )->AsVector( );
				meshMat.specularPowerVariable = meshMat.effect->GetVariableByName( "specularPower" )->AsScalar( );
				meshMat.textureRepeatVariable = meshMat.effect->GetVariableByName( "textureRepeat" )->AsVector( );
				// camera
				meshMat.cameraPositionVariable = meshMat.effect->GetVariableByName( "cameraPosition" )->AsVector( );
				// booleans
				meshMat.useDiffuseTextureVariable = meshMat.effect->GetVariableByName( "useDiffuseTexture" )->AsScalar( );
				meshMat.useSpecularTextureVariable = meshMat.effect->GetVariableByName( "useSpecularTexture" )->AsScalar( );
				meshMat.useBumpTextureVariable = meshMat.effect->GetVariableByName( "useBumpTexture" )->AsScalar( );
				meshMat.useParallaxTextureVariable = meshMat.effect->GetVariableByName( "useHeightTexture" )->AsScalar( );
				return true;
			}
		}
		return false;
	}
	bool loadTexture_IO( ID3D11ShaderResourceView** texture, Renderer& renderer,
		const String& filename )
	{
		String fileExt = filename.substr( filename.find( '.' ) + 1 );
		std::transform( fileExt.begin( ), fileExt.end( ), fileExt.begin( ), ::tolower );
		std::wstring wFilename = s2ws( filename );
		if ( fileExt.compare( "dds" ) == 0 )
		{
			if ( FAILED( DirectX::CreateDDSTextureFromFile( renderer.device,
				wFilename.c_str( ), nullptr, texture ) ) )
			{
				return false;
			}
		}
		else
		{
			if ( FAILED( DirectX::CreateWICTextureFromFile( renderer.device,
				wFilename.c_str( ), nullptr, texture ) ) )
			{
				return false;
			}
		}
		return true;
	}
	namespace
	{
		bool loadShader_IO( MeshMat& meshMat, Renderer& renderer )
		{
			ID3DBlob* buffer = NULL;
			if ( !loadAndCompile_IO( meshMat, renderer, "fx_5_0", &buffer ) )
			{
				return false;
			}
			if ( FAILED( D3DX11CreateEffectFromMemory( buffer->GetBufferPointer( ),
				buffer->GetBufferSize( ), 0, renderer.device, &meshMat.effect ) ) )
			{
				HP_RELEASE( buffer );
				return false;
			}
			return true;
		}
		bool loadAndCompile_IO( MeshMat& meshMat, Renderer& renderer, const String& shaderModel,
			ID3DBlob** buffer )
		{
			std::wstring wFilePath = s2ws( meshMat.filename );
			DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#       ifdef HP_DEBUG
			shaderFlags |= D3DCOMPILE_DEBUG;
#       endif
			ID3DBlob* errorBuffer = 0;
			HRESULT result = D3DCompileFromFile( wFilePath.c_str( ), 0, 0, 0, shaderModel.c_str( ),
				shaderFlags, 0, buffer, &errorBuffer );
			if ( FAILED( result ) )
			{
				HP_RELEASE( errorBuffer );
				return false;
			}
			HP_RELEASE( errorBuffer );
			return true;
		}
		bool createVertexLayout_IO( MeshMat& meshMat, Renderer& renderer )
		{
			UInt32 elementsCount = ARRAYSIZE( D3D11_LAYOUT );
			D3DX11_PASS_DESC passDesc;
			meshMat.technique->GetPassByIndex( 0 )->GetDesc( &passDesc );
			if ( FAILED( renderer.device->CreateInputLayout( D3D11_LAYOUT, elementsCount,
				passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &meshMat.inputLayout ) ) )
			{
				return false;
			}
			return true;
		}
	}
	void setProjection( MeshMat& meshMat, const Mat4x4& mat )
	{
		meshMat.projectionMatrixVariable->SetMatrix( (float*) ( &mat ) );
	}
	void setView( MeshMat& meshMat, const Mat4x4& mat )
	{
		meshMat.viewMatrixVariable->SetMatrix( (float*) ( &mat ) );
	}
	void setWorld( MeshMat& meshMat, const Mat4x4& mat )
	{
		meshMat.worldMatrixVariable->SetMatrix( (float*) ( &mat ) );
	}
	void setAbientLightColor( MeshMat& meshMat, const Color& color )
	{
		meshMat.ambientLightColourVariable->SetFloatVector( (float*) ( &color ) );
	}
	void setDiffuseLightColor( MeshMat& meshMat, const Color& color )
	{
		meshMat.diffuseLightColourVariable->SetFloatVector( (float*) ( &color ) );
	}
	void setSpecularLightColor( MeshMat& meshMat, const Color& color )
	{
		meshMat.specularLightColourVariable->SetFloatVector( (float*) ( &color ) );
	}
	void setLightDirection( MeshMat& meshMat, const FVec3& dir )
	{
		meshMat.lightDirectionVariable->SetFloatVector( (float*) ( &dir ) );
	}
	void setCameraPosition( MeshMat& meshMat, const FVec3& dir )
	{
		meshMat.cameraPositionVariable->SetFloatVector( (float*) ( &dir ) );
	}
	void setTextureRepeat( MeshMat& meshMat, const FVec2& repeat )
	{
		meshMat.textureRepeat = repeat;
	}
	void setTextures( MeshMat& meshMat )
	{
		meshMat.diffuseTextureVariable->SetResource( meshMat.diffuseTexture );
		meshMat.specularTextureVariable->SetResource( meshMat.specularTexture );
		meshMat.bumpTextureVariable->SetResource( meshMat.bumpTexture );
		meshMat.parallaxTextureVariable->SetResource( meshMat.parallaxTexture );
		meshMat.envMapVariable->SetResource( meshMat.envMapTexture );

		if ( meshMat.diffuseTexture )
			meshMat.useDiffuseTextureVariable->SetBool( true );
		if ( meshMat.specularTexture )
			meshMat.useSpecularTextureVariable->SetBool( true );
		if ( meshMat.bumpTexture )
			meshMat.useBumpTextureVariable->SetBool( true );
		if ( meshMat.parallaxTexture )
			meshMat.useParallaxTextureVariable->SetBool( true );

		meshMat.textureRepeatVariable->SetFloatVector( (float*) ( &meshMat.textureRepeat ) );
	}
	void setMaterials( MeshMat& meshMat )
	{
		meshMat.ambientMaterialVariable->SetFloatVector( (float*) ( meshMat.ambientMaterial ) );
		meshMat.diffuseMaterialVariable->SetFloatVector( (float*) ( meshMat.diffuseMaterial ) );
		meshMat.specularMaterialVariable->SetFloatVector( (float*) ( meshMat.specularMaterial ) );
		meshMat.specularPowerVariable->SetFloat( meshMat.specularPower );
	}
	void bindInputLayout( Renderer& renderer, MeshMat& meshMat )
	{
		renderer.deviceContext->IASetInputLayout( meshMat.inputLayout );
	}
	UInt32 getPassCount( MeshMat& meshMat )
	{
		return meshMat.techniqueDesc.Passes;
	}
	void applyPass( Renderer& renderer, MeshMat& meshMat, UInt32 i )
	{
		meshMat.pass = meshMat.technique->GetPassByIndex( i );
		meshMat.pass->Apply( 0, renderer.deviceContext );
	}
}