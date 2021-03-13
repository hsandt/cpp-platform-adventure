#include "Space/SpatialObject.h"

// Game
#include "Application/GameApplication.h"
#include "Components/Transform.h"
#include "Space/World.h"

SpatialObject::SpatialObject(GameApplication& gameApp, Handle id, bool persistentFlag) :
    ApplicationObject(gameApp),
    mp_id(id),
    mp_persistentFlag(persistentFlag),
    ms_destructionFlag(false)
{
}

SpatialObject::~SpatialObject()
{
}

void SpatialObject::destroy()
{
    ms_destructionFlag = true;
    mo_gameApp.mc_world->flagForDestruction(mp_id);
}
