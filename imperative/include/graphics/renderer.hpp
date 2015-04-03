#pragma once
#include "camera.hpp"
#include "directx.hpp"
#include "vertex.hpp"
#include "../window/window.hpp"
namespace hp_ip
{
	class Mesh;
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
		void swapCameras( );
		void present( );
		bool createVertexBuffer( ID3D11Buffer** vertexBuffer, UInt32 byteWidth,
			const Vertex* initData );
		bool createIndexBuffer( ID3D11Buffer** indexBuffer, UInt32 byteWidth,
			const Index* initData );
		void setVertexBuffers( ID3D11Buffer** vertexBuffer, UInt32* stride,
			UInt32* offset );
		void setIndexBuffer( ID3D11Buffer** indexBuffer );
		void setBuffers( Mesh& mesh );
		void drawIndexed( UInt32 indexCount, UInt32 startIndexLocation,
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
		CameraBuffer _cameraBuffer;
		WindowConfig _windowConfig;
	public:
		ID3D11Device* device( )
		{
			return _pDevice;
		}
		ID3D11DeviceContext* deviceContext( )
		{
			return _pDeviceContext;
		}
		const WindowConfig& windowConfig( ) const
		{
			return _windowConfig;
		}
		void setCamera( Camera&& camera )
		{
			_cameraBuffer.setCamera( std::forward<Camera>( camera ) );
		}
		const Camera& getCamera( )
		{
			return _cameraBuffer.getCamera( );
		}
	};
}