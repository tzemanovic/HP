#pragma once
#include "directx.hpp"
#include "vertex.hpp"
#include "../window/window.hpp"
namespace hp_ip
{
	class Renderer
	{
	public:
		Renderer( const WindowConfig& windowConfig ) : _driverType( D3D_DRIVER_TYPE_NULL ),
			_featureLevel( D3D_FEATURE_LEVEL_11_0 ), _pDevice( nullptr ),
			_pDeviceContext( nullptr ), _pSwapChain( nullptr ), _pRenderTargetView( nullptr ),
			_pDepthStencilView( nullptr ), _pDepthStencilTexture( nullptr ),
			_windowConfig( windowConfig )
		{ }
		bool init( WindowHandle windowHandle );
		void preRender( );
		void present( );
		bool createVertexBuffer_IO( ID3D11Buffer** vertexBuffer, UInt32 byteWidth,
			const Vertex* initData );
		bool createIndexBuffer_IO( ID3D11Buffer** indexBuffer, UInt32 byteWidth,
			const Index* initData );
		void setVertexBuffers_IO( ID3D11Buffer** vertexBuffer, UInt32* stride,
			UInt32* offset );
		void setIndexBuffer_IO( ID3D11Buffer** indexBuffer );
		void drawIndexed_IO( UInt32 indexCount, UInt32 startIndexLocation,
			UInt32 baseVertexLocation );
	private:
		D3D_DRIVER_TYPE _driverType;
		D3D_FEATURE_LEVEL _featureLevel;
		ID3D11Device* _pDevice;
		ID3D11DeviceContext* _pDeviceContext;
		IDXGISwapChain* _pSwapChain;
		ID3D11RenderTargetView* _pRenderTargetView;
		ID3D11DepthStencilView* _pDepthStencilView;
		ID3D11Texture2D* _pDepthStencilTexture;
		WindowConfig _windowConfig;
	};
}