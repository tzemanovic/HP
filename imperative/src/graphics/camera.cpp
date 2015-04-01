#include <pch.hpp>
#include "../../include/graphics/camera.hpp"
namespace hp_ip
{
	const Camera& CameraBuffer::getCamera( )
	{
		if ( _first )
		{
			return _cam[0];
		}
		else
		{
			return _cam[1];
		}
	}
	void CameraBuffer::setCamera( Camera&& camera )
	{
		if ( _first )
		{
			_cam[1] = camera;
		}
		else
		{
			_cam[0] = camera;
		}
	}
	void CameraBuffer::swap( )
	{
		_first = !_first;
	}
}