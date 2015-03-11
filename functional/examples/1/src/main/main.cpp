#include <pch/pch.hpp>
#ifdef HP_DEBUG
#include <vld.h>
#endif
#include <hpFp.hpp>
#include <graphics/meshMat.hpp>
using namespace hp_fp;
int main( )
{
	EngineMut engine = init( "example1" );
	/*ActorImm actor1{ };
	SceneImm scene1{ };
	scene1 = addActor( scene1, std::move( actor1 ) );
	WorldImm world{ { scene1 } };
	setWorld_IO( engine, std::move( world ) );*/
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