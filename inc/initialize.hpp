#pragma once

#include <SDL2/SDL.h>
#include "inc/window.hpp"

class Initialize {
private:
    bool initialized;
public:
    /**
     * Attempt to complete any work necessary for initialization.
     *
     * This method will not throw any exceptions on failure. The caller
     * *must* check @ref isInitialized and handle any errors.
     *
     * @param flags Any SDL2 flags that would be passed to SDL_Init
     */
    Initialize(Uint32 flags);
    ~Initialize();
    /**
     * Check if initialization was successful.
     *
     * If initialization failed, an error message would have been logged.
     */
    bool isInitialized() const;
};
