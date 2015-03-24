#include <pch.hpp>
#include <algorithm>
#include "../../include/graphics/material.hpp"
#include "../../include/graphics/renderer.hpp"
#include "../../include/graphics/vertex.hpp"
#include "../../include/utils/stringUtils.hpp"
namespace hp_fp
{
	Material defaultMat( )
	{
		return Material
		{
			"assets/shaders/parallax.fx", // filename
			"Render", // techniqueName
			{ 1.0f, 1.0f }, // textureRepeat
			{ 0.5f, 0.5f, 0.5f }, // ambientMaterial
			{ 0.8f, 0.8f, 0.8f }, // diffuseMaterial
			{ 1.0f, 1.0f, 1.0f }, // specularMaterial
			25.0f // specularPower
		};
	}
	Maybe<Material> loadMaterial_IO( Renderer& renderer, const MaterialDef& materialDef )
	{
		Material material = defaultMat( );
		bool materialLoaded;
		if ( materialLoaded = initMaterial_IO( material, renderer ) )
		{
			// TODO: fix these in /Ox
			if ( materialDef.diffuseTextureFilename != "" )
			{
				materialLoaded &= loadTexture_IO( &material.diffuseTexture, renderer,
					materialDef.diffuseTextureFilename );
			}
			if ( materialDef.specularTextureFilename != "" )
			{
				materialLoaded &= loadTexture_IO( &material.specularTexture, renderer,
					materialDef.specularTextureFilename );
			}
			if ( materialDef.bumpTextureFilename != "" )
			{
				materialLoaded &= loadTexture_IO( &material.bumpTexture, renderer,
					materialDef.bumpTextureFilename );
			}
			if ( materialDef.parallaxTextureFilename != "" )
			{
				materialLoaded &= loadTexture_IO( &material.parallaxTexture, renderer,
					materialDef.parallaxTextureFilename );
			}
			if ( materialDef.textureRepeat.x != 0.0f && materialDef.textureRepeat.y != 0.0f )
			{
				material.textureRepeat = materialDef.textureRepeat;
			}
		}
		if ( materialLoaded )
		{
			return just( std::move( material ) );
		}
		return nothing<Material>( );
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
				ERR( "Failed to load \"" + filename + "\" texture." );
				return false;
			}
		}
		else
		{
			if ( FAILED( DirectX::CreateWICTextureFromFile( renderer.device,
				wFilename.c_str( ), nullptr, texture ) ) )
			{
				ERR( "Failed to load \"" + filename + "\" texture." );
				return false;
			}
		}
		return true;
	}
	namespace
	{
		bool initMaterial_IO( Material& material, Renderer& renderer )
		{
			if ( loadShader_IO( material, renderer ) )
			{
				material.technique = material.effect->GetTechniqueByName( material.techniqueName.c_str( ) );
				material.technique->GetDesc( &material.techniqueDesc );
				if ( createVertexLayout_IO( material, renderer ) )
				{
					// retrieve all variables using semantic
					material.worldMatrixVariable = material.effect->GetVariableBySemantic( "WORLD" )->AsMatrix( );
					material.viewMatrixVariable = material.effect->GetVariableBySemantic( "VIEW" )->AsMatrix( );
					material.projectionMatrixVariable = material.effect->GetVariableBySemantic( "PROJECTION" )->AsMatrix( );
					material.diffuseTextureVariable = material.effect->GetVariableByName( "diffuseMap" )->AsShaderResource( );
					material.specularTextureVariable = material.effect->GetVariableByName( "specularMap" )->AsShaderResource( );
					material.bumpTextureVariable = material.effect->GetVariableByName( "bumpMap" )->AsShaderResource( );
					material.parallaxTextureVariable = material.effect->GetVariableByName( "heightMap" )->AsShaderResource( );
					material.envMapVariable = material.effect->GetVariableByName( "envMap" )->AsShaderResource( );
					// lights
					material.ambientLightColourVariable = material.effect->GetVariableByName( "ambientLightColour" )->AsVector( );
					material.diffuseLightColourVariable = material.effect->GetVariableByName( "diffuseLightColour" )->AsVector( );
					material.specularLightColourVariable = material.effect->GetVariableByName( "specularLightColour" )->AsVector( );
					material.lightDirectionVariable = material.effect->GetVariableByName( "lightDirection" )->AsVector( );
					// materials
					material.ambientMaterialVariable = material.effect->GetVariableByName( "ambientMaterialColour" )->AsVector( );
					material.diffuseMaterialVariable = material.effect->GetVariableByName( "diffuseMaterialColour" )->AsVector( );
					material.specularMaterialVariable = material.effect->GetVariableByName( "specularMaterialColour" )->AsVector( );
					material.specularPowerVariable = material.effect->GetVariableByName( "specularPower" )->AsScalar( );
					material.textureRepeatVariable = material.effect->GetVariableByName( "textureRepeat" )->AsVector( );
					// camera
					material.cameraPositionVariable = material.effect->GetVariableByName( "cameraPosition" )->AsVector( );
					// booleans
					material.useDiffuseTextureVariable = material.effect->GetVariableByName( "useDiffuseTexture" )->AsScalar( );
					material.useSpecularTextureVariable = material.effect->GetVariableByName( "useSpecularTexture" )->AsScalar( );
					material.useBumpTextureVariable = material.effect->GetVariableByName( "useBumpTexture" )->AsScalar( );
					material.useParallaxTextureVariable = material.effect->GetVariableByName( "useHeightTexture" )->AsScalar( );
					return true;
				}
			}
			ERR( "Failed to initalize \"" + material.filename + "\"'s material." );
			return false;
		}
		bool loadShader_IO( Material& material, Renderer& renderer )
		{
			ID3DBlob* buffer = NULL;
			if ( !loadAndCompile_IO( material, renderer, "fx_5_0", &buffer ) )
			{
				return false;
			}
			if ( FAILED( D3DX11CreateEffectFromMemory( buffer->GetBufferPointer( ),
				buffer->GetBufferSize( ), 0, renderer.device, &material.effect ) ) )
			{
				HP_RELEASE( buffer );
				return false;
			}
			return true;
		}
		bool loadAndCompile_IO( Material& material, Renderer& renderer, const String& shaderModel,
			ID3DBlob** buffer )
		{
			std::wstring wFilePath = s2ws( material.filename );
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
		bool createVertexLayout_IO( Material& material, Renderer& renderer )
		{
			UInt32 elementsCount = ARRAYSIZE( D3D11_LAYOUT );
			D3DX11_PASS_DESC passDesc;
			material.technique->GetPassByIndex( 0 )->GetDesc( &passDesc );
			if ( FAILED( renderer.device->CreateInputLayout( D3D11_LAYOUT, elementsCount,
				passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &material.inputLayout ) ) )
			{
				return false;
			}
			return true;
		}
	}
	void setProjection_IO( Material& material, const Mat4x4& mat )
	{
		material.projectionMatrixVariable->SetMatrix( (float*) ( &mat ) );
	}
	void setView_IO( Material& material, const Mat4x4& mat )
	{
		material.viewMatrixVariable->SetMatrix( (float*) ( &mat ) );
	}
	void setWorld_IO( Material& material, const Mat4x4& mat )
	{
		material.worldMatrixVariable->SetMatrix( (float*) ( &mat ) );
	}
	void setAbientLightColor_IO( Material& material, const Color& color )
	{
		material.ambientLightColourVariable->SetFloatVector( (float*) ( &color ) );
	}
	void setDiffuseLightColor_IO( Material& material, const Color& color )
	{
		material.diffuseLightColourVariable->SetFloatVector( (float*) ( &color ) );
	}
	void setSpecularLightColor_IO( Material& material, const Color& color )
	{
		material.specularLightColourVariable->SetFloatVector( (float*) ( &color ) );
	}
	void setLightDirection_IO( Material& material, const FVec3& dir )
	{
		material.lightDirectionVariable->SetFloatVector( (float*) ( &dir ) );
	}
	void setCameraPosition_IO( Material& material, const FVec3& dir )
	{
		material.cameraPositionVariable->SetFloatVector( (float*) ( &dir ) );
	}
	void setTextureRepeat_IO( Material& material, const FVec2& repeat )
	{
		material.textureRepeat = repeat;
	}
	void setTextures_IO( Material& material )
	{
		material.diffuseTextureVariable->SetResource( material.diffuseTexture );
		material.specularTextureVariable->SetResource( material.specularTexture );
		material.bumpTextureVariable->SetResource( material.bumpTexture );
		material.parallaxTextureVariable->SetResource( material.parallaxTexture );
		material.envMapVariable->SetResource( material.envMapTexture );

		if ( material.diffuseTexture )
			material.useDiffuseTextureVariable->SetBool( true );
		if ( material.specularTexture )
			material.useSpecularTextureVariable->SetBool( true );
		if ( material.bumpTexture )
			material.useBumpTextureVariable->SetBool( true );
		if ( material.parallaxTexture )
			material.useParallaxTextureVariable->SetBool( true );

		material.textureRepeatVariable->SetFloatVector( (float*) ( &material.textureRepeat ) );
	}
	void setMaterials_IO( Material& material )
	{
		material.ambientMaterialVariable->SetFloatVector( (float*) ( material.ambientMaterial ) );
		material.diffuseMaterialVariable->SetFloatVector( (float*) ( material.diffuseMaterial ) );
		material.specularMaterialVariable->SetFloatVector( (float*) ( material.specularMaterial ) );
		material.specularPowerVariable->SetFloat( material.specularPower );
	}
	void bindInputLayout_IO( Renderer& renderer, Material& material )
	{
		renderer.deviceContext->IASetInputLayout( material.inputLayout );
	}
	UInt32 getPassCount( Material& material )
	{
		return material.techniqueDesc.Passes;
	}
	void applyPass_IO( Renderer& renderer, Material& material, UInt32 i )
	{
		material.pass = material.technique->GetPassByIndex( i );
		material.pass->Apply( 0, renderer.deviceContext );
	}
}