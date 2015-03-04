#pragma once
#include <adt/list.hpp>
#include <core/actors/component.hpp>
// Generated using tools/immutableStruct.hs:
// gen "Actor" [("List<ActorImm>", "children"), ("List<Component>", "components")] ["adt/list.hpp", "core/actors/component.hpp"]
namespace hp_fp
{
	struct ActorImm
	{
		ActorImm( )
		{ }
		ActorImm( List<ActorImm>& children, const List<Component>& components ) : children( children ), components( components )
		{ }
		const List<ActorImm> children;
		const List<Component> components;
		const ActorImm setChildren( List<ActorImm>& c ) const
		{
			return ActorImm{ c, components };
		}
		/*const ActorImm setComponents( const List<Component> c ) const
		{
		return ActorImm{ children, c };
		}*/
	};
}
