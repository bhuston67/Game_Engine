//
//  Client.hpp
//  game_engine
//
//  Created by Ben Huston on 6/19/25.
//

#ifndef Client_hpp
#define Client_hpp

#include <stdio.h>
#include <string>
#include "enet/enet.h"
#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "ComponentDB.hpp"

class Client{
public:
    static inline ENetHost* client;
    static inline ENetAddress address;
    static inline ENetPeer* peer;
    
    static inline std::vector<std::pair<std::string,std::pair<luabridge::LuaRef,luabridge::LuaRef>>> lateSubs;
    static inline std::vector<std::pair<std::string,std::pair<luabridge::LuaRef,luabridge::LuaRef>>> lateUnsubs;
    static inline std::unordered_map<std::string, std::vector<std::pair<luabridge::LuaRef,luabridge::LuaRef>>> subs;
    static inline std::unordered_map<std::string, std::vector<std::pair<luabridge::LuaRef,luabridge::LuaRef>>> unsubs;
    
    
    Client() {
        this->client = enet_host_create(
                NULL,     // NULL means this is a client
                1,        // Only one peer (the server)
                2,        // Two channels (you can increase this as needed)
                0, 0      // No bandwidth limits
            );
    }
    
    static void setAddress(std::string host, int port);
    
    static void sendPackage(std::string msg);
    
    static void processEvents();
    
    static luabridge::LuaRef decodeJsonToLuaTable(const std::string& jsonString);
    
    static void Subscribe(std::string event, luabridge::LuaRef comp, luabridge::LuaRef function);
    
    static void Unsubscribe(std::string event, luabridge::LuaRef comp, luabridge::LuaRef function);
    
    static void lateHandleEvents();
};

#endif /* Client_hpp */
