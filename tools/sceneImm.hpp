#pragma once
#include <adt/tree.hpp>
#include <core/actors/actor.hpp>
// Generated using tools/immutableStruct.hs:
// gen "Scene" [("Tree<ActorImm>", "actors")] ["adt/tree.hpp", "core/actors/actor.hpp"]
namespace hp_fp
{
struct SceneImm
{
const Tree<ActorImm> actors;
const SceneImm setActors(const Tree<ActorImm> a) const
{
return SceneImm{ a };
 }
};
}
