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

    /// Set whether flag item is in the inventory
    void setHasFlag(bool value) { ms_hasFlag = value; }

private:

    /* State */

    /// True iff flag item is in the inventory
    bool ms_hasFlag;
};
