#pragma once

//std
#include <optional>
#include <functional>

// Game
#include "Common.h"

// Game
class SpatialObject;
class World;

/// Smart handle for SpatialObject
/// Unlike Handle type, it acts like a weak pointer.
/// It knows what broad type of object it points to and how to access it.
class SpatialObjectHandle
{
public:
    SpatialObjectHandle(World& world, Handle handle);

    ~SpatialObjectHandle();

    SpatialObjectHandle(const SpatialObjectHandle&) = default;
    SpatialObjectHandle& operator=(const SpatialObjectHandle&) = default;

    /// Return optional reference to target SpatialObject
    /// Unlike std::weak_ptr, there is no locking mechanic.
    /// Instead, we must guarantee that objects are never destroyed on parallel threads.
    std::optional<std::reference_wrapper<SpatialObject>> get() const;

private:

    /* References to objects with guaranteed lifetime */

    /// World
    /// Reference wrapper allows class to be copyable
    std::reference_wrapper<World> mo_world;


    /* Parameters */

    /// Handle to spatial object. Key of spatial object map in World.
    Handle mp_handle;
};
