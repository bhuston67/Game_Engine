//
//  Engine.cpp
//  game_engine
//
//  Created by Ben Huston on 1/20/25.
//

#include "Engine.hpp"


Engine::Engine() : util(EngineUtil()), scene(nullptr), document(rapidjson::Document()), gameTitle(""), hasCustomResolution(false) {
    currentSceneName = "";
    changeScene = false;
    if (!std::filesystem::exists("resources")) {
        std::cout << "error: resources/ missing";
        exit(0);
    }
    if (!std::filesystem::exists("resources/game.config")) {
        std::cout << "error: resources/game.config missing";
        exit(0);
    }
    if (std::filesystem::exists("resources/rendering.config")) {
        hasCustomResolution = true;
    }
    ComponentDB::Init();
    setupLua();
      Input::Init();
      Renderer::Init(hasCustomResolution);
      Renderer::createwindow(gameTitle.c_str());
      Renderer::createRenderer();
      util.ReadJsonFile("resources/game.config", document);
      jsonValues();
      ImageDB::Init(this->document);
      TextDB::Init(this->document, Renderer::resolution);
      AudioDB::Init(this->document);
    
      loadScene(currentSceneName);
}

void Engine::game_loop() {
    bool exit = false;
    while (!exit) {
        
        
        SDL_Event e;
        while (Helper::SDL_PollEvent(&e)) {
            Input::processEvent(e);
            if (e.type == SDL_QUIT) {
                exit = true;
            }
        }
        
        
        scene->doLateAdds();
        //scene->doLateActorAdds();
        scene->doNewOnStarts();
        
        if (Client::client != nullptr) {
            Client::processEvents();
        }
        
        scene->onUpdates();
        
        scene->onLateUpdates();
        
        scene->doOnDestroys();
        
        Event::lateHandleEvents();
        Client::lateHandleEvents();
        
        if (ComponentDB::world != nullptr) {
            ComponentDB::world->Step(1.0f/60.0f, 8, 3);
        }
        
        Renderer::render();
        
        Input::lateUpdate();
        scene->doLateRemoves();
        scene->doLateActorAdds();
        scene->doLateActorDeletes();
        
        if (changeScene) {
            loadScene(currentSceneName);
        }
        
        
    }
}


void Engine::jsonValues() {
    auto it = document.FindMember("game_title");
    if (it != document.MemberEnd()) {
        gameTitle = it->value.GetString();
    }
    it = document.FindMember("initial_scene");
    if (it != document.MemberEnd()) {
        currentSceneName = it->value.GetString();
    }
    else {
        std::cout << "error: initial_scene unspecified";
        exit(0);
    }
    it = document.FindMember("server");
    if (it != document.MemberEnd()) {
        auto server = it->value.GetObject();
        std::string host = "0";
        int port = -1;
        auto it2 = server.FindMember("host");
        if (it2 != server.MemberEnd()) {
            host = it2->value.GetString();
        }
        it2 = server.FindMember("port");
        if (it2 != server.MemberEnd()) {
            port = it2->value.GetInt();
        }
        Client::setAddress(host, port);
    }
}

void Engine::loadScene(std::string &sceneName) {
    std::string scenePath = "resources/scenes/" + sceneName + ".scene";
    if (!std::filesystem::exists(scenePath)) {
        std::cout << "error: scene " + sceneName + " is missing";
        exit(0);
    }
    if (this->scene != nullptr) {
        scene->loadSceneDestroys();
        delete this->scene;
    }
    Scene::lateAdds.clear();
    //Scene::newActors.clear();
    Scene::lateActorAdds.clear();
    this->scene = new Scene(hasCustomResolution, scenePath, saveActors);
    this->currentSceneName = sceneName;
    this->changeScene = false;
}


std::string Engine::getSceneName() {
    return currentSceneName;
}

void Engine::load(std::string sceneName) {
    changeScene = true;
    currentSceneName = sceneName;
}

void Engine::dontDestroy(Actor *actor) {
    if (saveSet.find(actor) == saveSet.end()) {
        saveActors.push_back(actor);
        saveSet.insert(actor);
    }
}

bool Engine::canDestroy(Actor *actor) {
    if (saveSet.find(actor) != saveSet.end()) {
        return false;
    }
    return true;
}

void Engine::clearSaves() {
    saveSet.clear();
    saveActors.clear();
}


Engine::~Engine() {
    delete scene;
//    for (Actor * actor : saveActors) {
//        delete actor;
//    }
}

extern "C" {
    int luaopen_cjson(lua_State* L);
}
void Engine::setupLua() {
    
    lua_getglobal(ComponentDB::luaState, "package");
    lua_getfield(ComponentDB::luaState, -1, "preload");
    lua_pushcfunction(ComponentDB::luaState, luaopen_cjson);
    lua_setfield(ComponentDB::luaState, -2, "cjson");
    lua_pop(ComponentDB::luaState, 2); // pop 'preload' and 'package'
}
