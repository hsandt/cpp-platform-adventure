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
    /// Constructor for unset handle (leaves handle uninitialized)
    explicit SpatialObjectHandle(World& world);

    /// Constructor for set handle
    SpatialObjectHandle(World& world, Handle handle);

    /// Destructor
    ~SpatialObjectHandle();

    /// Return true if handle is set
    explicit operator bool() const noexcept
    {
        return ms_isSet;
    }

    /// Set handle, replacing any existing value
    /// This doesn't check that the target object exists right now, it is up to the user to check.
    void set(Handle handle);

    /// Return optional reference to target SpatialObject
    /// If handle is unset, always return nullopt.
    /// If user needs to know whether handle is set to do extra processing (e.g. warn or clean up
    /// when handle has just been unset or the target object has been destroyed), they must check
    /// it on their own.
    /// This way, users who don't care about the reason why nullopt is returned only need one call.
    /// Unlike std::weak_ptr, there is no locking mechanic.
    /// Instead, we must guarantee that objects are never destroyed on parallel threads.
    SafePtr<SpatialObject> findObject() const;

private:

    /* References to objects with guaranteed lifetime */

    /// World
    /// Reference wrapper allows class to be copyable
    std::reference_wrapper<World> mo_world;


    /* State */

    /// True iff the handle is set
    /// If false, ms_handle is ignored.
    /// If true, it still doesn't guarantee that the handle is valid,
    /// as the target object may have been destroyed.
    bool ms_isSet;

    /// Handle to spatial object
    /// Key of spatial object map in World.
    Handle ms_handle;
};
