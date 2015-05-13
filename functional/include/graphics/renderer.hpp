#pragma once
#include "camera.hpp"
#include "directx.hpp"
#include "vertex.hpp"
#include "../adt/maybe.hpp"
#include "../window/window.hpp"
namespace hp_fp
{
	struct WindowConfig;
	struct Renderer
	{
		Renderer( D3D_DRIVER_TYPE driverType, D3D_FEATURE_LEVEL featureLevel,
			const WindowConfig& windowConfig ) : driverType( driverType ),
			featureLevel( featureLevel ), device( nullptr ), deviceContext( nullptr ),
			swapChain( nullptr ), renderTargetView( nullptr ),
			depthStencilView( nullptr ), windowConfig( windowConfig )
		{ }
		Renderer( const Renderer& ) = delete;
		Renderer( Renderer&& r ) : driverType( std::move( r.driverType ) ), featureLevel( std::move( r.featureLevel ) ), device( std::move( r.device ) ), deviceContext( std::move( r.deviceContext ) ),
			swapChain( std::move( r.swapChain ) ), renderTargetView( std::move( r.renderTargetView ) ), depthStencilView( std::move( r.depthStencilView ) ),
			windowConfig( std::move( r.windowConfig ) )
		{ }
		Renderer operator = ( const Renderer& ) = delete;
		Renderer operator = ( Renderer&& r )
		{
			return Renderer{ std::move( r ) };
		}
		D3D_DRIVER_TYPE driverType;
		D3D_FEATURE_LEVEL featureLevel;
		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
		IDXGISwapChain* swapChain;
		ID3D11RenderTargetView* renderTargetView;
		ID3D11DepthStencilView* depthStencilView;
		CameraBuffer cameraBuffer;
		WindowConfig windowConfig;
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/

	Maybe<Renderer> init_IO( WindowHandle windowHandle, const WindowConfig& windowConfig );
	void preRender_IO( Renderer& renderer );
	void present_IO( Renderer& renderer );
	bool createVertexBuffer_IO( Renderer& renderer, ID3D11Buffer** vertexBuffer, UInt32 byteWidth,
		const Vertex* initData );
	bool createIndexBuffer_IO( Renderer& renderer, ID3D11Buffer** indexBuffer, UInt32 byteWidth,
		const Index* initData );
	void setVertexBuffers_IO( Renderer& renderer, ID3D11Buffer** vertexBuffer, UInt32* stride,
		UInt32* offset );
	void setIndexBuffer_IO( Renderer& renderer, ID3D11Buffer** indexBuffer );
	void drawIndexed_IO( Renderer& renderer, UInt32 indexCount, UInt32 startIndexLocation,
		UInt32 baseVertexLocation );
}

