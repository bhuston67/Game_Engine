//
//  Engine.hpp
//  game_engine
//
//  Created by Ben Huston on 1/20/25.
//

#ifndef Engine_hpp
#define Engine_hpp


#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <filesystem>
#include <unordered_set>



#include "EngineUtil.h"
#include "Scene.hpp"
#include "Renderer.hpp"
#include "ImageDB.hpp"
#include "TextDB.hpp"
#include "Input.hpp"
#include "ComponentDB.hpp"
#include "Event.hpp"
#include "Client.hpp"


#include "lua/lua.hpp"
#include "glm/glm.hpp"
#include "SDL.h"
#include "SDL_image.h"

#include "rapidjson/document.h"


class Scene;
struct Actor;




class Engine {
public:
    
    Engine();
    
    void game_loop();
    
    void jsonValues();
    
    void loadScene(std::string &sceneName);
    
    static std::string getSceneName();
    
    static void load(std::string sceneName);
    
    static void dontDestroy(Actor* actor);
    
    static bool canDestroy(Actor* actor);
    
    static void clearSaves();
    
    void setupLua();
    
    
    
    ~Engine();
    
    EngineUtil util;
    Scene *scene;
    rapidjson::Document document;
    static inline bool changeScene;
    static inline std::string currentSceneName;
    std::string gameTitle;
    bool hasCustomResolution;
    static inline std::vector<Actor*> saveActors;
    static inline std::unordered_set<Actor*> saveSet;
    
};

#endif /* Engine_hpp */
