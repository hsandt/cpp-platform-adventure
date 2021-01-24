#include "Space/SpatialObject.h"

// Game
#include "Application/GameApplication.h"
#include "Components/Transform.h"

SpatialObject::SpatialObject(GameApplication& gameApp) :
    ApplicationObject(gameApp)
{
}

SpatialObject::~SpatialObject()
{
}
