/*
 * Engine.cpp
 * ==========
 * Basic game loop and initialization
 * */
#include <chrono>
#include <ctype.h>
#include <iostream>
#include <memory>
#include <thread>

#include "Engine.h"
#include "Exception.h"

#define FRAME_CAP 1000/60

namespace __CORE
{ 
    Engine::Engine() : Engine("system/settings.json") {} 
    Engine::Engine(std::string configLoc) : Engine(configLoc, "log/_engine.log") {}

    Engine::Engine(std::string configLoc, std::string logLoc)
    {
        _Log = std::make_shared<__logger::cLogger>(logLoc); //Create new log member
        _Log->start().detach(); //LAZY: Could store the thread just to check in on the guy.
        _Log->info("[Engine] Engine v", _major_v, ".", _minor_v, " initializing!");

        /* We're alive! */
        dead = false;
        try
        {
            loadConfig(configLoc);
            initSDL();
            debugBox = false;
        }
        catch (ConfigurationException e)
        {
            /* We're probably misconfigured */
            _Log->err("[Engine] A configuration exception has occurred. ", e.what());
            dead = true; //Kill before we even try to enter game loop
        }
        catch (EngineException e)
        {
            /* SDL/Other core elements errored */
            _Log->err("[Engine] Engine has critically failed. ", e.what());
            dead = true;
        }
        catch (std::exception e)
        {
            _Log->err("[Engine] A generic exception has been caught. Error: ", e.what());
            dead = true;
        }
    }

    Engine::~Engine()
    {
        _Log->info("[Engine] Dying");
        
        destroySDL();
        _Log->info("[Engine] Killing logging. Goodbye!");
        _Log->kill();
        //Make sure all data has been logged.
        _Log->done.lock();
        _Log->done.unlock();
    }

    void Engine::Run()
    {
        while(!this->dead)
        { //Lets lock FPS to 60 ATM and keep physics here
            auto start = std::chrono::high_resolution_clock::now();
            ProcessEvents();
            Update(); //FIXME: Move physics to seperate thread
            Render();
            auto end = std::chrono::high_resolution_clock::now();
            uint32_t diff = (end-start).count();
            if (diff < FRAME_CAP) SDL_Delay(FRAME_CAP-diff); //Sleep for a bit to keep ~60 FPS
        }
    }

    void Engine::initSDL()
    {
        _Log->info("[Engine] Initializing SDL");
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            throw EngineException("SDL_Error: ", SDL_GetError());
        }
        _Log->info("[Engine] Creating Window");
        _Window = SDL_CreateWindow(
            _config["engine"]["name"].GetString(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            _config["engine"]["width"].GetInt(), _config["engine"]["height"].GetInt(),
            SDL_WINDOW_SHOWN
        );

        _Renderer = SDL_CreateRenderer(_Window, -1, SDL_RENDERER_ACCELERATED);
        if (!_Renderer)
        {
            throw EngineException("SDL_Error: ", SDL_GetError());
        }
        _Log->info("[Engine] Finished intializing SDL.");
    }

    void Engine::destroySDL()
    {
        _Log->info("[Engine] Destroying SDL");
        _Log->info("[Engine] Destroy Renderer");
        SDL_DestroyRenderer(_Renderer);
        _Log->info("[Engine] Destroying Window");
        SDL_DestroyWindow(_Window);
        _Log->info("[Engine] Done with killing SDL");
    }

    void Engine::Update()
    {
        //TODO - Just iterate through all objects and call tick() on all of them.
    }

    void Engine::Render()
    {
        //TODO: Remove stub
        SDL_SetRenderDrawColor(_Renderer, 255, 255, 255, 0); //Clear the screen. Do nothing else
        SDL_RenderClear(_Renderer);
        /* Draw objects here */


        /* Don't draw on top of debug box */
        if (debugBox)
        {
            SDL_SetRenderDrawColor(_Renderer, 255, 0, 0, 0);
            SDL_Rect r;
            r.x = _config["engine"]["width"].GetInt()/2 - 5; r.y = _config["engine"]["height"].GetInt()/2 - 5; r.h = 10; r.w = 10;
            SDL_RenderFillRect(_Renderer, &r);
        }

        SDL_RenderPresent(_Renderer);
    }

    void Engine::ProcessEvents()
    {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0)
        {
            switch(e.type)
            {
                case SDL_QUIT:
                    _Sudoku();
                    break;

                case SDL_KEYDOWN:
                    HandleUserKeyboard(
                        e.key.keysym.sym,
                        e.key.type == SDL_KEYDOWN,
                        e.key.keysym.mod
                    );
                    break;

                case SDL_KEYUP:
                    HandleUserKeyboard(
                        e.key.keysym.sym,
                        e.key.type == SDL_KEYDOWN,
                        e.key.keysym.mod
                    );
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    HandleUserMouse(
                        e.button.x,
                        e.button.y,
                        e.button.button,
                        e.button.type==SDL_MOUSEBUTTONDOWN
                    );
                    break;

                case SDL_MOUSEBUTTONUP:
                    HandleUserMouse(
                        e.button.x,
                        e.button.y,
                        e.button.button,
                        e.button.type==SDL_MOUSEBUTTONDOWN
                    );
                    break;

                case SDL_MOUSEWHEEL:
                    HandleUserScroll(
                        e.wheel.x,
                        e.wheel.y
                    );
                    break;

                case SDL_MOUSEMOTION:
                    HandleUserMouse(
                        e.motion.x,
                        e.motion.y,
                        -1, //Uninitialized mouse button
                        0 //Ignored anyway because no button is found
                    );
                    break;

                default:
                    //Unsupported event
                    _Log->info("Unsupported event recieved!. SDL_Event.type = ", e.type);
                    break;
            };
        }
    }

} 