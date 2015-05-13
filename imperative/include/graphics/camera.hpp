#pragma once
#include "../math/mat4x4.hpp"
namespace hp_ip
{
	struct CameraDef
	{
		float nearClipDist;
		float farClipDist;
	};
	struct Camera
	{
		Mat4x4 projection;
		Mat4x4 transform;
	};
	class CameraBuffer
	{
	public:
		const Camera& getCamera( );
		void setCamera( Camera&& camera );
		void swap( );
	private:
		bool _first;
		Camera _cam[2];
	};
}

