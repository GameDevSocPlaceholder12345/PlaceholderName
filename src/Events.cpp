//
// Created by mistarhee on 24/04/18.
//

#include "Engine.h"

#include <iostream>

namespace __CORE
{
    void Engine::HandleUserKeyboard(SDL_Keycode key, bool isDown, uint32_t modifiers)
    {
        /* Debug Box (To test if not responding) */
        if (
                key == _config["debug"]["debug_box_key"].GetInt() &&
                isDown &&
                modifiers == _config["debug"]["debug_box_modifier"].GetInt()
            )
        {
            debugBox = true;
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