#pragma once

// std
#include <set>

// Game
#include "Common.h"

/// Player character inventory
class Inventory
{
public:

    Inventory();
    ~Inventory();

    /// Return true iff item is in the inventory, identified by data ID
    bool hasItem(DataID dataID) const
    {
        return ms_possessedItemDataIDs.contains(dataID);
    }

    /// Add item to inventory by data ID
    void addItem(DataID dataID)
    {
        ms_possessedItemDataIDs.insert(dataID);
    }

    /// Remove item from the inventory by data ID
    void removeItem(DataID dataID)
    {
        ms_possessedItemDataIDs.erase(dataID);
    }

private:

    /* State */

    /// Set of data IDs of items in inventory
    std::set<DataID> ms_possessedItemDataIDs;
};
