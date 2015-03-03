#pragma once
#include <D3D11.h>
#include <D3D10.h>
#include <D3Dcompiler.h>
namespace hp_fp
{
	struct RendererMut
	{
		RendererMut( D3D_DRIVER_TYPE driverType, D3D_FEATURE_LEVEL featureLevel, ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain, ID3D11RenderTargetView* renderTargetView,
			ID3D11DepthStencilView* depthStencilView, ID3D11Texture2D* depthStencilTexture )
			: driverType( driverType ), featureLevel( featureLevel ), device( device ), deviceContext( deviceContext ), swapChain( swapChain ), renderTargetView( renderTargetView ),
			depthStencilView( depthStencilView ), depthStencilTexture( depthStencilTexture )
		{ }
		RendererMut( const RendererMut& ) = delete;
		RendererMut( RendererMut&& r ) : driverType( std::move( r.driverType ) ), featureLevel( std::move( r.featureLevel ) ), device( std::move( r.device ) ), deviceContext( std::move( r.deviceContext ) ),
			swapChain( std::move( r.swapChain ) ), renderTargetView( std::move( r.renderTargetView ) ), depthStencilView( std::move( r.depthStencilView ) ),
			depthStencilTexture( std::move( r.depthStencilTexture ) )
		{ }
		RendererMut operator=( const RendererMut& ) = delete;
		RendererMut operator=( RendererMut&& r )
		{
			return RendererMut{ r.driverType, r.featureLevel, r.device, r.deviceContext, r.swapChain, r.renderTargetView, r.depthStencilView, r.depthStencilTexture };
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
}