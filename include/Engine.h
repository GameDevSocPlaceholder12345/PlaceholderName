#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>

#include "SDL2/SDL2.h""
/* Logging lib */
#include "logger.hpp"
/* JSON lib */
#include "rapidjson/document.hpp"

namespace __CORE
{

    class Engine
    {
    public:
        Engine();
        Engine(std::string configLoc);
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

        /* Configuration */
        rapidjson::Document _config;

        /* System data */
        SDL_Window* _Window;
        SDL_Renderer* _Renderer;

        /* Debugging */ 
        logger_ptr _Log;

        const int _major_v = 0;
        const int _minor_v = 1;

    };

}

#endif //ENGINE_HPP
