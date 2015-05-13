#include <pch.hpp>
#include "../../include/graphics/camera.hpp"
namespace hp_fp
{
	const Camera& getCamera( const CameraBuffer& cameraBuffer )
	{
		if ( cameraBuffer._first )
		{
			return cameraBuffer._cam[0];
		}
		else
		{
			return cameraBuffer._cam[1];
		}
	}
	void setCamera_IO( CameraBuffer& cameraBuffer, Camera&& camera )
	{
		if ( cameraBuffer._first )
		{
			cameraBuffer._cam[1] = camera;
		}
		else
		{
			cameraBuffer._cam[0] = camera;
		}
	}
	void swap_IO( CameraBuffer& cameraBuffer )
	{
		cameraBuffer._first = !cameraBuffer._first;
	}
}

