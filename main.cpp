//
//  main.cpp
//  game_engine
//
//  Created by Ben Huston on 1/20/25.
//

#include <stdio.h>
#include <iostream>
#include "Engine.hpp"
#include "SDL2/SDL.h"
#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "box2d/box2d.h"



int main(int argc, char* argv[]) {
    
    if (enet_initialize() != 0) {
            printf("ENet initialization failed.\n");
            return EXIT_FAILURE;
        }
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return 1;
        }
    
//    SDL_GameController* controller = nullptr;
//        for (int i = 0; i < SDL_NumJoysticks(); ++i) {
//            if (SDL_IsGameController(i)) {
//                controller = SDL_GameControllerOpen(i);
//                if (controller) {
//                    std::cout << "Controller connected: " << SDL_GameControllerName(controller) << "\n";
//                    break;
//                }
//            }
//        }

    Engine engine = Engine();
    engine.game_loop();
    
//    std::cout << "hello from cpp\n";
//    lua_State* state = luaL_newstate();
//    luaL_openlibs(state);
//    
//    luabridge::setGlobal(state, "tricky time", "secret_message");
    
//    const char* str = "print('hello from lua')";
//    luaL_dostring(state, str);
    //luaL_dofile(state, "code.lua");
    //std::cout << "win";
    atexit(enet_deinitialize);

    return 0;
}
