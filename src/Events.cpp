//
// Created by mistarhee on 24/04/18.
//

#include "Engine.h"


namespace __CORE
{
    void Engine::HandleUserKeyboard(SDL_Keycode key, bool isDown, uint32_t modifiers)
    {
        if (key == SDLK_RETURN && isDown)
        {
            debugBox = true; //TODO: DELETE THIS
        }
    }

    void Engine::HandleUserMouse(int32_t x, int32_t y, int32_t button, bool isDown)
    {

    }

    void Engine::HandleUserScroll(int32_t dx, int32_t y)
    {

    }

    void Engine::_Sudoku()
    {
        _Log->info("Committing sudoku.");
        this->dead = true;
    }
}