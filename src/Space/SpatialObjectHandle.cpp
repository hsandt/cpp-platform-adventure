#include "SpatialObjectHandle.h"

// Game
#include "Space/World.h"

SpatialObjectHandle::SpatialObjectHandle(World& world, Handle handle) :
    mo_world(world),
    mp_handle(handle)
{
}

SpatialObjectHandle::~SpatialObjectHandle()
{
}

std::optional<std::reference_wrapper<SpatialObject>> SpatialObjectHandle::get(Handle handle) const
{
    return mo_world.findSpatialObject(handle);
}
