#include <pch/pch.hpp>
#ifdef HP_DEBUG
#include <vld.h>
#endif
#include <hpIp.hpp>
using namespace hp_ip;
int main( )
{
	Engine engine{ "example1" };
	engine.run( );
	return 0;
}