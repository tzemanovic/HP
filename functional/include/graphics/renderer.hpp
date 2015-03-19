#pragma once
#include "directx.h"
#include "vertex.hpp"
#include "../adt/maybe.hpp"
#include "../window/windowConfigImm.hpp"
namespace hp_fp
{
	struct Renderer
	{
		Renderer( D3D_DRIVER_TYPE driverType, D3D_FEATURE_LEVEL featureLevel, ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain, ID3D11RenderTargetView* renderTargetView,
			ID3D11DepthStencilView* depthStencilView, ID3D11Texture2D* depthStencilTexture )
			: driverType( driverType ), featureLevel( featureLevel ), device( device ), deviceContext( deviceContext ), swapChain( swapChain ), renderTargetView( renderTargetView ),
			depthStencilView( depthStencilView ), depthStencilTexture( depthStencilTexture )
		{ }
		Renderer( const Renderer& ) = delete;
		Renderer( Renderer&& r ) : driverType( std::move( r.driverType ) ), featureLevel( std::move( r.featureLevel ) ), device( std::move( r.device ) ), deviceContext( std::move( r.deviceContext ) ),
			swapChain( std::move( r.swapChain ) ), renderTargetView( std::move( r.renderTargetView ) ), depthStencilView( std::move( r.depthStencilView ) ),
			depthStencilTexture( std::move( r.depthStencilTexture ) )
		{ }
		Renderer operator=( const Renderer& ) = delete;
		Renderer operator=( Renderer&& r )
		{
			return Renderer{ r.driverType, r.featureLevel, r.device, r.deviceContext, r.swapChain, r.renderTargetView, r.depthStencilView, r.depthStencilTexture };
		}
		D3D_DRIVER_TYPE driverType;
		D3D_FEATURE_LEVEL featureLevel;
		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
		IDXGISwapChain* swapChain;
		ID3D11RenderTargetView* renderTargetView;
		ID3D11DepthStencilView* depthStencilView;
		ID3D11Texture2D* depthStencilTexture;
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/
	Maybe<Renderer> init_IO( WindowHandle windowHandle, const WindowConfigImm& windowConfig );
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