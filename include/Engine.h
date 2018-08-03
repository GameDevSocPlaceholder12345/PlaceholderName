#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#ifdef _WIN32
#include "SDL2_Win/SDL.h"
#else
#include "SDL2/SDL.h"
#endif
/* Logging lib */
#include "logger.hpp"
/* JSON lib */
#include "rapidjson/document.h"

typedef std::shared_ptr<__logger::cLogger> logger_ptr;

namespace __CORE
{

    class Engine
    {
    public:
        Engine();
        Engine(std::string configLoc);
        Engine(std::string configLoc, std::string logLoc);
        ~Engine();
        void Run();

    protected:
        bool dead;


    private:

        /* Init functions */
        void initSDL();
        void destroySDL();
        void loadConfig(std::string confLoc);

        /* Main loop */
        void ProcessEvents();
        void Update();
        void Render();
        void _Sudoku(); //Exiting main loop

        /* Event processing */
        void HandleUserKeyboard(SDL_Keycode key, bool isDown, uint32_t modifiers);
        void HandleUserMouse(int32_t x, int32_t y, int32_t button, bool isDown);
        void HandleUserScroll(int32_t dx, int32_t dy);

        /* Configuration */
        rapidjson::Document _config;

        /* System data */
        SDL_Window* _Window;
        SDL_Renderer* _Renderer;

        /* Debugging */ 
        logger_ptr _Log;
        bool debugBox;

        const int _major_v = 0;
        const int _minor_v = 1;
    };

}

#endif //ENGINE_H
