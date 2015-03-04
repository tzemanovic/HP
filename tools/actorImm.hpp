#pragma once
#include <adt/list.hpp>
#include <core/actors/component.hpp>
// Generated using tools/immutableStruct.hs:
// gen "Actor" [("List<ActorImm>", "children"), ("List<Component>", "components")] ["adt/list.hpp", "core/actors/component.hpp"]
namespace hp_fp
{
struct ActorImm
{
const List<ActorImm> children;
const List<Component> components;
const ActorImm setChildren(const List<ActorImm> c) const
{
return ActorImm{ c, components };
 }
const ActorImm setComponents(const List<Component> c) const
{
return ActorImm{ children, c };
 }
};
}
