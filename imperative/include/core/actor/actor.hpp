#pragma once
#include <vector>
#include "component/iComponent.hpp"
namespace hp_ip
{
	class Actor
	{
	public:
		void init( );
		void addComponent( iComponent&& component );
	private:
		std::vector<iComponent> _components;
	};
}