//
//  ComponentDB.hpp
//  game_engine
//
//  Created by Ben Huston on 2/27/25.
//

#ifndef ComponentDB_hpp
#define ComponentDB_hpp

#include <stdio.h>
#include <thread>
#include <unordered_map>
#include <unordered_map>

#include "Actor.hpp"
#include "Scene.hpp"
#include "Engine.hpp"
#include "RigidBody.hpp"
#include "CollisionListener.hpp"
#include "Physics.hpp"
#include "Event.hpp"


#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "box2d/box2d.h"


struct Actor;
class RigidBody;
class CollisionListener;

class ComponentDB {
public:
    static inline lua_State* luaState;
    //static inline std::unordered_map<std::string, luabridge::LuaRef*> components;
    static inline std::unordered_set<std::string> components;
    static inline std::unordered_map<std::string, int> templateCount;
    static inline b2World* world;
    //static inline std::shared_ptr<CollisionListener> listen;
    static inline CollisionListener* listen;
    //static inline CollisionListener listen();
    
    static void Init();
    
    static void addComponentToActor(Actor* actor, std::string key, std::string comp);
    
    static void addComponentFromTemplate(Actor* actor, std::string key, luabridge::LuaRef& parentTable);
    
    static void addComponent(std::string comp);
    
    static void establishInheritance(luabridge::LuaRef & instance_table, luabridge::LuaRef & parentTable);
    
    static void cppLog(const std::string & message);
    
    static void Quit();
    
    static void Sleep(int ms);
    
    static int GetFrame();
    
    static void OpenURL(std::string url);
    
    static void createWorld();
    
};

#endif /* ComponentDB_hpp */
