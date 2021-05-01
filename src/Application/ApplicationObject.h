#pragma once

// Game
#include "Common.h"

// Game
class GameApplication;

/// Base class for all objects living under the application that may need
/// access to the GameApplication. This avoids using a singleton.
class ApplicationObject
{
public:

    explicit ApplicationObject(GameApplication& gameApp);
    virtual ~ApplicationObject();

protected:

    /* References to objects with guaranteed lifetime */

    /// Game Application
    GameApplication& mo_gameApp;
};
