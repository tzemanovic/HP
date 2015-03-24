#pragma once
#include "../math/mat4x4.hpp"
namespace hp_fp
{
	struct Camera
	{
		Mat4x4 projection;
		Mat4x4 transform;
	};
	/*Camera defaultCamera( )
	{
	return Camera{};
	}*/
	struct CameraBuffer
	{
	private:
		bool _first;
		Camera _cam[2];
	public:
		friend const Camera& getCamera( const CameraBuffer& cameraBuffer );
		friend void setCamera_IO( CameraBuffer& cameraBuffer, Camera&& camera );
		friend void swap_IO( CameraBuffer& cameraBuffer );
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/

	const Camera& getCamera( const CameraBuffer& cameraBuffer );
	void setCamera_IO( CameraBuffer& cameraBuffer, Camera&& camera );
	void swap_IO( CameraBuffer& cameraBuffer );
}