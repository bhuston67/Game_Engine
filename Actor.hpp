//
//  Actor.hpp
//  game_engine
//
//  Created by Ben Huston on 1/28/25.
//

#ifndef Actor_hpp
#define Actor_hpp


#include <stdio.h>
#include <string>
#include <optional>
#include <unordered_set>

#include "glm/glm.hpp"
#include "Helper.h"
#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "box2d/box2d.h"

#include "ComponentDB.hpp"


struct Actor
{
public:
    std::string name;
    int id;
    std::unordered_map<std::string, std::shared_ptr<luabridge::LuaRef>> components;
    std::unordered_map<std::string, std::vector<std::string>> types;
    std::vector<std::string> onStarts;
    std::vector<std::string> sortedKeys;
    std::unordered_map<std::string, int> templateCount;
    std::vector<std::pair<std::string,std::string>> lateAdd;
    std::vector<std::string> lateRemove;
    std::vector<std::string> onUpdates;
    std::vector<std::string> onLateUpdates;
    std::vector<std::string> onEnters;
    std::vector<std::string> onExits;
    std::vector<std::string> onTriggerEnters;
    std::vector<std::string> onTriggerExits;
    std::vector<std::string> onDestroys;
    std::unordered_map<std::string, std::string> keyToType;
    //RigidBody* rb;
    
    
    Actor(int Id) : name(""), id(Id) {}
    
    void injectReference(std::shared_ptr<luabridge::LuaRef> ref);
    
    std::string GetName();
    
    int GetID();
    
    luabridge::LuaRef GetComponentByKey(std::string key);
    
    luabridge::LuaRef GetComponent(std::string type);
    
    luabridge::LuaRef GetComponents(std::string type);
    
    luabridge::LuaRef AddComponent(std::string type);
    
    void RemoveComponent(luabridge::LuaRef ref);
    
    
};

#endif /* Actor_hpp */
