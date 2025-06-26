//
//  ComponentDB.cpp
//  game_engine
//
//  Created by Ben Huston on 2/27/25.
//

#include "ComponentDB.hpp"


void ComponentDB::Init() {
    luaState = luaL_newstate();
    luaL_openlibs(luaState);
    preloadCjson();
    
    world = nullptr;
    
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Debug")
        .addFunction("Log", ComponentDB::cppLog)
        .endNamespace();
    luabridge::getGlobalNamespace(luaState)
        .beginClass<Actor>("Actor")
        .addFunction("GetName", &Actor::GetName)
        .addFunction("GetID", &Actor::GetID)
        .addFunction("GetComponent", &Actor::GetComponent)
        .addFunction("GetComponentByKey", &Actor::GetComponentByKey)
        .addFunction("GetComponents", &Actor::GetComponents)
        .addFunction("AddComponent", &Actor::AddComponent)
        .addFunction("RemoveComponent", &Actor::RemoveComponent)
        .endClass();
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Actor")
        .addFunction("Find", Scene::Find)
        .addFunction("FindAll", Scene::FindAll)
        .addFunction("Instantiate", Scene::Instantiate)
        .addFunction("Destroy", Scene::Destroy)
        .endNamespace();
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Application")
        .addFunction("GetFrame", ComponentDB::GetFrame)
        .addFunction("Quit", ComponentDB::Quit)
        .addFunction("Sleep", ComponentDB::Sleep)
        .addFunction("OpenURL", ComponentDB::OpenURL)
        .endNamespace();
    luabridge::getGlobalNamespace(luaState)
        .beginClass<glm::vec2>("vec2")
        .addProperty("x",&glm::vec2::x)
        .addProperty("y",&glm::vec2::y)
        .endClass();
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Input")
        .addFunction("GetKey", Input::getKey)
        .addFunction("GetKeyUp", Input::getKeyUp)
        .addFunction("GetKeyDown", Input::getKeyDown)
        .addFunction("GetMousePosition", Input::getMousePosition)
        .addFunction("GetMouseButton", Input::getMouseButton)
        .addFunction("GetMouseButtonUp", Input::getMouseButtonUp)
        .addFunction("GetMouseButtonDown", Input::getMouseButtonDown)
        .addFunction("GetMouseScrollDelta", Input::getMouseScrollDelta)
        .addFunction("HideCursor", Input::hideCursor)
        .addFunction("ShowCursor", Input::showCursor)
        .addFunction("GetControllerButton", Input::getControllerButton)
        .addFunction("GetControllerButtonUp", Input::getControllerButtonUp)
        .addFunction("GetControllerButtonDown", Input::getControllerButtonDown)
        .addFunction("GetLeftJoystickDirection", Input::getLeftJoystickDirection)
        .addFunction("GetRightJoystickDirection", Input::getRightJoystickDirection)
        .endNamespace();
    luabridge::getGlobalNamespace(luaState)
            .beginNamespace("Text")
            .addFunction("Draw", TextDB::addTextToQueue)
            .endNamespace();
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Audio")
        .addFunction("Play", AudioDB::playChannel)
        .addFunction("Halt", AudioDB::haltChannel)
        .addFunction("SetVolume", AudioDB::setVolume)
        .endNamespace();
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Image")
        .addFunction("DrawUI", ImageDB::DrawUI)
        .addFunction("DrawUIEx", ImageDB::DrawUIEx)
        .addFunction("Draw", ImageDB::Draw)
        .addFunction("DrawEx", ImageDB::DrawEx)
        .addFunction("DrawPixel", ImageDB::DrawPixel)
        .endNamespace();
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Camera")
        .addFunction("SetPosition", Renderer::setCamera)
        .addFunction("GetPositionX", Renderer::getCameraX)
        .addFunction("GetPositionY", Renderer::getCameraY)
        .addFunction("SetZoom", Renderer::setZoom)
        .addFunction("GetZoom", Renderer::getZoom)
        .endNamespace();
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Scene")
        .addFunction("Load", Engine::load)
        .addFunction("GetCurrent", Engine::getSceneName)
        .addFunction("DontDestroy", Engine::dontDestroy)
        .endNamespace();
    luabridge::getGlobalNamespace(luaState)
        .beginClass<b2Vec2>("Vector2")
        .addConstructor<void(*) (float,float)>()
        .addProperty("x", &b2Vec2::x)
        .addProperty("y", &b2Vec2::y)
        .addFunction("Normalize", &b2Vec2::Normalize)
        .addFunction("Length", &b2Vec2::Length)
        .addFunction("__add", &b2Vec2::operator_add)
        .addFunction("__sub", &b2Vec2::operator_sub)
        .addFunction("__mul", &b2Vec2::operator_mult)
        .addStaticFunction("Distance", &b2Distance)
        .addStaticFunction("Dot", static_cast<float (*)(const b2Vec2&,const b2Vec2&)>(&b2Dot))
        .endClass();
    luabridge::getGlobalNamespace(luaState)
        .beginClass<Rigidbody>("Rigidbody")
        .addConstructor<void (*)()>()
        .addProperty("key", &Rigidbody::key)
        .addProperty("x", &Rigidbody::x)
        .addProperty("y", &Rigidbody::y)
        .addProperty("body_type", &Rigidbody::body_type)
        .addProperty("precise", &Rigidbody::precise)
        .addProperty("gravity_scale", &Rigidbody::gravity_scale)
        .addProperty("density", &Rigidbody::density)
        .addProperty("angular_friction", &Rigidbody::angular_friction)
        .addProperty("rotation", &Rigidbody::rotationDegrees)
        .addProperty("has_collider", &Rigidbody::has_collider)
        .addProperty("has_trigger", &Rigidbody::has_trigger)
        .addProperty("collider_type", &Rigidbody::collider_type)
        .addProperty("width", &Rigidbody::width)
        .addProperty("height", &Rigidbody::height)
        .addProperty("radius", &Rigidbody::radius)
        .addProperty("friction", &Rigidbody::friction)
        .addProperty("bounciness", &Rigidbody::bounciness)
        .addProperty("trigger_type", &Rigidbody::trigger_type)
        .addProperty("trigger_width", &Rigidbody::trigger_width)
        .addProperty("trigger_height", &Rigidbody::trigger_height)
        .addProperty("trigger_radius", &Rigidbody::trigger_radius)
        .addProperty("enabled", &Rigidbody::enabled)
        .addFunction("OnStart", &Rigidbody::OnStart)
        .addFunction("GetPosition", &Rigidbody::getPosition)
        .addFunction("GetRotation", &Rigidbody::getRotation)
        .addFunction("AddForce", &Rigidbody::addForce)
        .addFunction("SetVelocity", &Rigidbody::setVelocity)
        .addFunction("SetPosition", &Rigidbody::setPosition)
        .addFunction("SetRotation", &Rigidbody::setRotation)
        .addFunction("SetAngularVelocity", &Rigidbody::setAngularVelocity)
        .addFunction("SetGravityScale", &Rigidbody::setGravity)
        .addFunction("SetUpDirection", &Rigidbody::setUpDirection)
        .addFunction("SetRightDirection", &Rigidbody::setRightDirection)
        .addFunction("GetVelocity", &Rigidbody::getVelocity)
        .addFunction("GetAngularVelocity", &Rigidbody::getAngularVelocity)
        .addFunction("GetGravityScale", &Rigidbody::getGravity)
        .addFunction("GetUpDirection", &Rigidbody::getUpDirection)
        .addFunction("GetRightDirection", &Rigidbody::getRightDirection)
        .endClass();
    luabridge::getGlobalNamespace(luaState)
        .beginClass<Collision>("Collision")
        .addProperty("other", &Collision::other)
        .addProperty("point", &Collision::point)
        .addProperty("relative_velocity", &Collision::relative_velocity)
        .addProperty("normal", &Collision::normal)
        .endClass();
    luabridge::getGlobalNamespace(luaState)
        .beginClass<HitResult>("HitResult")
        .addProperty("actor", &HitResult::actor)
        .addProperty("point", &HitResult::point)
        .addProperty("normal", &HitResult::normal)
        .addProperty("is_trigger", &HitResult::is_trigger)
        .endClass();
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Physics")
        .addFunction("Raycast", Physics::Raycast)
        .addFunction("RaycastAll", Physics::RaycastAll)
        .endNamespace();
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Event")
        .addFunction("Publish", Event::Publish)
        .addFunction("Subscribe", Event::Subscribe)
        .addFunction("Unsubscribe", Event::Unsubscribe)
        .endNamespace();
    luabridge::getGlobalNamespace(luaState)
    .beginClass<ParticleSystem>("ParticleSystem")
    .addFunction("OnStart", &ParticleSystem::OnStart)
    .addFunction("OnUpdate", &ParticleSystem::OnUpdate)
    .addFunction("Play", &ParticleSystem::Play)
    .addFunction("Stop", &ParticleSystem::stop)
    .addFunction("Burst", &ParticleSystem::burst)
    .addProperty("enabled", &ParticleSystem::enabled)
    .addProperty("x", &ParticleSystem::x)
    .addProperty("y", &ParticleSystem::y)
    .addProperty("start_color_r", &ParticleSystem::r)
    .addProperty("start_color_g", &ParticleSystem::g)
    .addProperty("start_color_b", &ParticleSystem::b)
    .addProperty("start_color_a", &ParticleSystem::a)
    .endClass();
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Network")
        .addFunction("sendPacket", Client::sendPackage)
        .addFunction("Subscribe", Client::Subscribe)
        .addFunction("Unsubscribe", Client::Unsubscribe)
        .endNamespace();

}


void ComponentDB::addComponentToActor(Actor *actor, std::string key, std::string comp) {
    if (components.find(comp) == components.end()) {
        addComponent(comp);
    }
    luabridge::LuaRef emptyTable = luabridge::newTable(luaState);
    luabridge::LuaRef parent = luabridge::getGlobal(luaState, comp.c_str());
    establishInheritance(emptyTable, parent);
    emptyTable["key"] = key;
    emptyTable["actor"] = actor;
    emptyTable["enabled"] = true;
    actor->components[key] = std::make_shared<luabridge::LuaRef>(emptyTable);
}

void ComponentDB::addComponentFromTemplate(Actor *actor, std::string key, luabridge::LuaRef &parentTable) {
    luabridge::LuaRef emptyTable = luabridge::newTable(luaState);
    establishInheritance(emptyTable, parentTable);
    emptyTable["key"] = key;
    emptyTable["actor"] = actor;
    emptyTable["enabled"] = true;
    actor->components[key] = std::make_shared<luabridge::LuaRef>(emptyTable);
    
}

void ComponentDB::addComponent(std::string comp) {
    std::string path = "resources/component_types/" + comp + ".lua";
    if (!std::filesystem::exists(path)) {
        std::cout << "error: failed to locate component " << comp;
        exit(0);
    }
    if (luaL_dofile(luaState, path.c_str()) != LUA_OK) {
        std::cout << "problem with lua file " << comp;
        exit(0);
    }
//    luabridge::LuaRef hold = luabridge::getGlobal(luaState, comp.c_str());
//    components[comp] = &hold;
    components.insert(comp);
    
}

void ComponentDB::establishInheritance(luabridge::LuaRef &instance_table, luabridge::LuaRef &parent_Table) {
    luabridge::LuaRef new_metatable = luabridge::newTable(luaState);
    new_metatable["__index"] = parent_Table;
    instance_table.push(luaState);
    new_metatable.push(luaState);
    lua_setmetatable(luaState, -2);
    lua_pop(luaState, 1);
}

void ComponentDB::cppLog(const std::string &message) {
    std::cout << message << "\n";
}

void ComponentDB::Quit() {
    exit(0);
}

void ComponentDB::Sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int ComponentDB::GetFrame() {
    return Helper::GetFrameNumber();
}

void ComponentDB::OpenURL(std::string url) {
    std::string command = "";
    
    #ifdef _WIN32
    command = "start " + url;
    #elif   defined(__APPLE__)
    command = "open " + url;
    #else
    command = "xdg-open " + url;
    #endif
    
    std::system(command.c_str());
}

void ComponentDB::createWorld() {
    b2Vec2 gravity(0.0f,9.8f);
    world = new b2World(gravity);
    //listen = std::make_shared<CollisionListener>();
    listen = new CollisionListener();
    //CollisionListener* listen = new CollisionListener();
    world->SetContactListener(listen);
}


void ComponentDB::preloadCjson() {
    lua_getglobal(luaState, "package");
        lua_getfield(luaState, -1, "preload");
        lua_pushcfunction(luaState, luaopen_cjson);
        lua_setfield(luaState, -2, "cjson");
        lua_pop(luaState, 2); // pop preload and package
    
    lua_getglobal(luaState, "require");
    lua_pushstring(luaState, "cjson");
    if (lua_pcall(luaState, 1, 1, 0) != LUA_OK) {
        std::cerr << "Failed to require cjson: " << lua_tostring(luaState, -1) << std::endl;
        lua_pop(luaState, 1);
    } else {
        lua_setglobal(luaState, "cjson");  // store returned module in _G.cjson
    }
}
