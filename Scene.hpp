//
//  Scene.hpp
//  game_engine
//
//  Created by Ben Huston on 1/28/25.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <filesystem>

#include "Actor.hpp"
#include "EngineUtil.h"
#include "ImageDB.hpp"
#include "TextDB.hpp"
#include "ComponentDB.hpp"
#include "RigidBody.hpp"
#include "ParticleSystem.hpp"
#include "Client.hpp"


#include "glm/glm.hpp"
#include "rapidjson/document.h"
#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

struct Actor;

class Scene {
public:
    
    static inline int idCount = 0;
    EngineUtil util;
    std::vector<Actor*> actors;
    std::vector<Actor*> renderOrder;
    static inline std::unordered_map<std::string, Actor*> templates;
    float zoomFactor;
    float sceneUnit;
    std::unordered_map<int, Actor*> idMap;
    std::vector<Actor*> onCooldown;
    static inline std::unordered_map<std::string,std::vector<Actor*>> nameMap;
    static inline std::vector<Actor*> lateAdds;
    static inline std::vector<std::pair<Actor*,std::string>> lateRemoves;
    static inline std::vector<Actor*> lateActorAdds;
    static inline std::vector<Actor*> lateActorDeletes;
    static inline std::vector<Actor*> newOnStartActors;
    //static inline std::vector<Actor*> newActors;
    
    Scene(bool rendering, std::string& level, std::vector<Actor*> & saveActors);
    
    int getIdCount();
    
    static void makeTemplate(std::string name);
    
    void onUpdates();
    
    void onLateUpdates();
    
    static luabridge::LuaRef Find(std::string name);
    
    static luabridge::LuaRef FindAll(std::string name);
    
    void doLateAdds();
    
    void doLateRemoves();
    
    static luabridge::LuaRef Instantiate(std::string temp);
    
    void doLateActorAdds();
    
    static void Destroy(Actor* actor);
    
    void doLateActorDeletes();
    
    void doNewOnStarts();
    
    static void createRigidBody(const rapidjson::Value& doc, std::string key, Actor* actor);
    
    void makeBody();
    
    void doOnDestroys();
        
    void loadSceneDestroys();
    
    static void createParticleSystem(const rapidjson::Value& doc, std::string key, Actor* actor);
    
    //static void createClient(const rapidjson::Value& doc, std::string key, Actor* actor);
    
    ~ Scene();
    
};

#endif /* Scene_hpp */
