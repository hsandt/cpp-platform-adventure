#include "SpatialObjectHandle.h"

// Game
#include "Space/World.h"

SpatialObjectHandle::SpatialObjectHandle(World& world) :
    mo_world(world),
    ms_isSet(false)
{
}

SpatialObjectHandle::SpatialObjectHandle(World& world, Handle handle) :
    mo_world(world),
    ms_isSet(true),
    ms_handle(handle)
{
}

SpatialObjectHandle::~SpatialObjectHandle()
{
}

void SpatialObjectHandle::set(Handle handle)
{
    ms_isSet = true;
    ms_handle = handle;
}

std::optional<std::reference_wrapper<SpatialObject>> SpatialObjectHandle::findObject() const
{
    return ms_isSet ? mo_world.get().findSpatialObject(ms_handle) : std::nullopt;
}
