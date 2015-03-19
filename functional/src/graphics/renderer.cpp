#include <pch.hpp>
#include "../../include/graphics/renderer.hpp"
namespace hp_fp
{
	Maybe<Renderer> init_IO( WindowHandle windowHandle, const WindowConfigImm& windowConfig )
	{
		Renderer renderer{ D3D_DRIVER_TYPE_NULL, D3D_FEATURE_LEVEL_11_0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		// driver types for fallback
		D3D_DRIVER_TYPE driverTypes[] = { D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE };
		UInt8 totalDriverTypes = ARRAYSIZE( driverTypes );
		// fallback feature levels
		D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
		UInt8 totalFeatureLevels = ARRAYSIZE( featureLevels );
		// swap chain description
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );
		swapChainDesc.BufferCount = windowConfig.windowStyle == WindowStyle::Fullscreen ? 2 : 1;
		swapChainDesc.BufferDesc.Width = windowConfig.width;
		swapChainDesc.BufferDesc.Height = windowConfig.height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = windowHandle;
		swapChainDesc.Windowed = windowConfig.windowStyle == WindowStyle::Window;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		// device creation flags
		UInt32 creationFlags = 0;
#       ifdef HP_DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#       endif
		HRESULT result;
		UInt8 driver = 0;
		// loop through driver types and attempt to create device
		for ( driver; driver < totalDriverTypes; ++driver )
		{
			result = D3D11CreateDeviceAndSwapChain( 0, driverTypes[driver], 0, creationFlags, featureLevels,
				totalFeatureLevels, D3D11_SDK_VERSION, &swapChainDesc, &renderer.swapChain, &renderer.device, &renderer.featureLevel,
				&renderer.deviceContext );
			if ( SUCCEEDED( result ) )
			{
				renderer.driverType = driverTypes[driver];
				break;
			}
		}
		if ( FAILED( result ) )
		{
			ERR( "Failed to create the Direct3D device!" );
			return nothing<Renderer>( );
		}
		// back buffer texture to link render target with back buffer
		ID3D11Texture2D* backBufferTexture;
		if ( FAILED( renderer.swapChain->GetBuffer( 0, _uuidof( ID3D11Texture2D ), (LPVOID*) &backBufferTexture ) ) )
		{
			ERR( "Failed to get the swap chain back buffer!" );
			return nothing<Renderer>( );
		}
		D3D11_TEXTURE2D_DESC depthDesc;
		depthDesc.Width = windowConfig.width;
		depthDesc.Height = windowConfig.height;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthDesc.SampleDesc.Count = 1;
		depthDesc.SampleDesc.Quality = 0;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthDesc.CPUAccessFlags = 0;
		depthDesc.MiscFlags = 0;
		if ( FAILED( renderer.device->CreateTexture2D( &depthDesc, NULL, &renderer.depthStencilTexture ) ) )
		{
			ERR( "Failed to create depth stencil texture!" );
			return nothing<Renderer>( );
		}
		D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
		depthViewDesc.Format = depthDesc.Format;
		depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthViewDesc.Texture2D.MipSlice = 0;
		depthViewDesc.Flags = 0;
		if ( FAILED( renderer.device->CreateDepthStencilView( renderer.depthStencilTexture, &depthViewDesc, &renderer.depthStencilView ) ) )
		{
			ERR( "Failed to create depth stencil view!" );
			return nothing<Renderer>( );
		}
		if ( FAILED( renderer.device->CreateRenderTargetView( backBufferTexture, NULL, &renderer.renderTargetView ) ) )
		{
			ERR( "Failed to create render target view!" );
			HP_RELEASE( backBufferTexture );
			return nothing<Renderer>( );
		}
		HP_RELEASE( backBufferTexture );
		renderer.deviceContext->OMSetRenderTargets( 1, &renderer.renderTargetView, renderer.depthStencilView );
		// setup the viewport
		D3D11_VIEWPORT viewport;
		viewport.Width = static_cast<FLOAT>( windowConfig.width );
		viewport.Height = static_cast<FLOAT>( windowConfig.height );
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;
		renderer.deviceContext->RSSetViewports( 1, &viewport );
		renderer.deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		return just( std::move( renderer ) );
	}
	void preRender_IO( Renderer& renderer )
	{
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
		renderer.deviceContext->ClearRenderTargetView( renderer.renderTargetView, ClearColor );
		renderer.deviceContext->ClearDepthStencilView( renderer.depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
	}
	void present_IO( Renderer& renderer )
	{
		renderer.swapChain->Present( 0, 0 );
	}
	bool createVertexBuffer_IO( Renderer& renderer, ID3D11Buffer** vertexBuffer, UInt32 byteWidth,
		const Vertex* initData )
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = byteWidth;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA subresourceData;
		subresourceData.pSysMem = initData;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;
		if ( SUCCEEDED( renderer.device->CreateBuffer( &bd, &subresourceData, vertexBuffer ) ) )
		{
			return true;
		}
		return false;
	}
	bool createIndexBuffer_IO( Renderer& renderer, ID3D11Buffer** indexBuffer, UInt32 byteWidth,
		const Index* initData )
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = byteWidth;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA subresourceData;
		subresourceData.pSysMem = initData;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;
		if ( SUCCEEDED( renderer.device->CreateBuffer( &bd, &subresourceData, indexBuffer ) ) )
		{
			return true;
		}
		return false;
	}
	void setVertexBuffers_IO( Renderer& renderer, ID3D11Buffer** vertexBuffer, UInt32* stride,
		UInt32* offset )
	{
		renderer.deviceContext->IASetVertexBuffers( 0, 1, vertexBuffer, stride, offset );
	}
	void setIndexBuffer_IO( Renderer& renderer, ID3D11Buffer** indexBuffer )
	{
		renderer.deviceContext->IASetIndexBuffer( *indexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	}
	void drawIndexed_IO( Renderer& renderer, UInt32 indexCount, UInt32 startIndexLocation,
		UInt32 baseVertexLocation )
	{
		renderer.deviceContext->DrawIndexed( indexCount, startIndexLocation, baseVertexLocation );
	}
}