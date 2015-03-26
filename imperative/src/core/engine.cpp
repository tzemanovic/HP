#include <pch.hpp>
#include "../../include/core/timer.hpp"
#include "../../include/core/engine.hpp"
namespace hp_ip
{
	void Engine::run( const WindowConfig& windowConfig )
	{
		_pWindow = HP_NEW Window( _name, windowConfig );
		if ( _pWindow->open( ) )
		{
			_pRenderer = HP_NEW Renderer( windowConfig );
			if ( _pRenderer->init( _pWindow->handle( ) ) )
			{
				for ( auto& actor : _actors )
				{
					actor.init( );
				}
				Timer timer;
				while ( _pWindow->isOpen( ) )
				{
					_pWindow->processMessages( );
					double deltaMs = timer.update( );
					_pRenderer->preRender( );
					_pRenderer->present( );
				}
			}
			else
			{
				ERR( "Failed to initialize renderer." );
			}
		}
		else
		{
			ERR( "Failed to open a window." );
		}
	}
	void Engine::addActor( Actor&& actor )
	{
		_actors.push_back( std::move( actor ) );
	}
}