#include <pch.hpp>
#include <core/scene.hpp>
namespace hp_fp
{
	const ActorImm addActor( const ActorImm& root, ActorImm&& actor )
	{
		return ActorImm{ root.children.push( actor ), root.components };
	}
	const SceneImm addActor( SceneImm& scene, ActorImm&& actor )
	{
		return SceneImm{ addActor( scene.root, std::move( actor ) ) };
		/*ActorImm root{ scene.root.children.push( actor ), std::move( scene.root.components ) };
		return SceneImm{ root };*/
	}
}