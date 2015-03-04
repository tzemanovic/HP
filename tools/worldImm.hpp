#pragma once
#include <adt/list.hpp>
#include <core/sceneImm.hpp>
// Generated using tools/immutableStruct.hs:
// gen "World" [("List<SceneImm>", "scenes")] ["adt/list.hpp", "core/sceneImm.hpp"]
namespace hp_fp
{
struct WorldImm
{
const List<SceneImm> scenes;
const WorldImm setScenes(const List<SceneImm> s) const
{
return WorldImm{ s };
 }
};
}
