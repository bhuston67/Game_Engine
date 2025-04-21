//
//  Event.hpp
//  game_engine
//
//  Created by Ben Huston on 3/24/25.
//

#ifndef Event_hpp
#define Event_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "box2d/box2d.h"
#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

class Event {
public:
    
    static inline std::vector<std::pair<std::string,std::pair<luabridge::LuaRef,luabridge::LuaRef>>> lateSubs;
    static inline std::vector<std::pair<std::string,std::pair<luabridge::LuaRef,luabridge::LuaRef>>> lateUnsubs;
    static inline std::unordered_map<std::string, std::vector<std::pair<luabridge::LuaRef,luabridge::LuaRef>>> subs;
    //static inline std::unordered_set<std::pair<std::string,std::pair<luabridge::LuaRef,luabridge::LuaRef>>> unsubs;
    static inline std::unordered_map<std::string, std::vector<std::pair<luabridge::LuaRef,luabridge::LuaRef>>> unsubs;
  
    static void Publish(std::string event, luabridge::LuaRef object);
    
    static void Subscribe(std::string event, luabridge::LuaRef comp, luabridge::LuaRef function);
    
    static void Unsubscribe(std::string event, luabridge::LuaRef comp, luabridge::LuaRef function);
    
    static void lateHandleEvents();
    
};

#endif /* Event_hpp */
