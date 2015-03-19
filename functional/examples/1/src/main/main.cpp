#include <pch/pch.hpp>
#ifdef HP_DEBUG
#include <vld.h>
#endif
#include <hpFp.hpp>
#include <graphics/meshMat.hpp>
using namespace hp_fp;
std::function<ActorOutput( ActorInput& )> knightSf( const float x0, const float y0 )
{
	return [=]( ActorInput& input )
	{
		return ActorOutput{ Mat4x4::identity, input.cameraProjection,
			input.cameraView, input.cameraWorldPos };
	};
}
int main( )
{
	ActorsDef actors{
		"assets/models/characters/knight/knight.fbx", // modelFilename
		0.009f, // modelScale
		"assets/textures/characters/knight/T_Black_Knight_D.jpg", // diffuseTextureFilename
		"", // specularTextureFilename
		"assets/textures/characters/knight/T_Black_Knight_N.jpg", // bumpTextureFilename
		"", // parallaxTextureFilename
		"", // evnMapTextureFilename
		knightSf( 0.0f, 1.0f ), // sf
		{ }
	};
	Engine engine = init( "example1", std::move( actors ) );
	run_IO( engine );

	/*InputMessage msg( TextMessage{'a'} );
	std::cout << msg.size << std::endl;
	InputMessage msg2( CloseMessage{ } );
	std::cout << msg2.size << std::endl;
	InputMessage msg3( KeyMessage{ Key::A, false, true, false, false, false, false } );
	std::cout << msg3.size << std::endl;
	std::cout << UInt8( reinterpret_cast< KeyMessage& >( msg3.data ).type ) << std::endl;
	std::cout << reinterpret_cast< KeyMessage& >( msg3.data ).lAlt << std::endl;
	std::cout << reinterpret_cast< KeyMessage& >( msg3.data ).lCtrl << std::endl;
	std::cout << reinterpret_cast< KeyMessage& >( msg3.data ).lShift << std::endl;
	std::cout << reinterpret_cast< KeyMessage& >( msg3.data ).rAlt << std::endl;
	std::cout << reinterpret_cast< KeyMessage& >( msg3.data ).rCtrl << std::endl;
	std::cout << reinterpret_cast< KeyMessage& >( msg3.data ).rShift << std::endl;
	InputMessage msg4( KeyMessage{ Key::B, true, false, false, false, false, false } );
	std::cout << msg4.size << std::endl;
	auto k = KeyMessage{ Key::B, true, false, false, false, false, false };
	std::cout << sizeof(k) << std::endl;;

	Sum<int, float, short> a( 32 );
	std::cout << a.size << std::endl;
	Sum<int, float, short> a1( 1 );
	std::cout << a1.size << std::endl;
	Sum<int, float, short> a2( 1.0f );
	std::cout << a2.size << std::endl;*/
	return 0;
}