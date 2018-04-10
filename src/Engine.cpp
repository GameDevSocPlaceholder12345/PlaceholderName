/*
 * Engine.cpp
 * ==========
 * Basic game loop and initialization
 * */
#include <iostream>
#include <thread>


#include "Engine.h"
#include "Exception.hpp"

#include "boost/make_shared.hpp"

namespace __CORE
{

    Engine::Engine() : Engine("system/settings.json") {}

    Engine::Engine(std::string configLoc) : Engine(configLoc, "log/_engine.log") {}

    Engine::Engine(std::string configLoc, std::string logLoc)
    {
        _Log = boost::make_shared<__logger::cLogger>(new __logger::cLogger(logLoc)); //Create new log member
        _Log->start().detach(); //LAZY: Could store the thread just to check in on the guy.
        _Log->info("[Engine] Engine v", _major_version, ".", _minor_version, " initializing!");

        /* We're alive! */
        this->dead = 0;
        try
        {
            loadConfig(configLoc);
            initSDL();
        }
        catch (EngineException e)
        {
            /* We're probably misconfigured */
            this->dead = true; //Kill before we even try to enter game loop
        }
    }

    Engine::~Engine()
    {
        _Log->info("[Engine] Dying");
        
        destroySDL();
        _Log->info("[Engine] Killing logging. Goodbye!");
        _Log->kill();
        //Nesure all data has been logged.
        _Log->done.lock();
        _Log->done.unlock()
    }

    void Engine::Run()
    {
        while(!this->dead)
        { //Lets lock FPS to 60 ATM and keep physics here
            ProcessEvents();
            Update(); //FIXME: Move physics to seperate thread
            Render();
        }
    }

    void Engine::initSDL()
    {
        _Log->info("[Engine] Initializing SDL");
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            _Log->error("[Engine] Failed to initialize SDL. SDL_Error: ", SDL_GetError());
        }
        _Log->info("[Engine] Creating Window");
        _Window = SDL_CreateWindow(

        );
    }

    void Engine::loadConfig(std::string configLoc)
    {
        
    }

}
