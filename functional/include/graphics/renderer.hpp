#pragma once
#include <adt/maybe.hpp>
#include <graphics/rendererMut.hpp>
#include <graphics/vertex.hpp>
#include <window/windowConfigImm.hpp>
namespace hp_fp
{
	Maybe<RendererMut> init_IO( WindowHandle windowHandle, const WindowConfigImm& windowConfig );
	void preRender_IO( RendererMut& renderer );
	void present_IO( RendererMut& renderer );
	bool createVertexBuffer_IO( RendererMut& renderer, ID3D11Buffer* vertexBuffer, UInt32 byteWidth,
		const Vertex* initData );
	bool createIndexBuffer_IO( RendererMut& renderer, ID3D11Buffer* indexBuffer, UInt32 byteWidth,
		const Index* initData );
	void setVertexBuffers_IO( RendererMut& renderer, ID3D11Buffer* vertexBuffer, UInt32* stride,
		UInt32* offset );
	void setIndexBuffer_IO( RendererMut& renderer, ID3D11Buffer* indexBuffer );
	void drawIndexed_IO( RendererMut& renderer, UInt32 indexCount, UInt32 startIndexLocation,
		UInt32 baseVertexLocation );
}