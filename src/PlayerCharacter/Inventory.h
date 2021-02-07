#pragma once

// Game
#include "Common.h"

/// Player character inventory
class Inventory
{
public:

    Inventory();
    ~Inventory();

    /// Is flag item is in the inventory?
    bool hasFlag() const { return ms_hasFlag; }

public:

    /* State */

    /// True iff flag item is in the inventory
    bool ms_hasFlag;
};
