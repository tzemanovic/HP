#pragma once
#include <core/sceneImm.hpp>
#include <core/actors/actorImm.hpp>
namespace hp_fp
{
	const ActorImm addActor( const ActorImm& root, ActorImm&& actor );
	const SceneImm addActor( SceneImm& scene, ActorImm&& actor );
}