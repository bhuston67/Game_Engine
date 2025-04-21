//
//  Scene.cpp
//  game_engine
//
//  Created by Ben Huston on 1/28/25.
//

#include "Scene.hpp"

Scene::Scene(bool rendering, std::string& initialScene, std::vector<Actor*> & saveActors) : util(EngineUtil()), sceneUnit(100.0f) {
    this->nameMap.clear();
    rapidjson::Document document;
    util.ReadJsonFile(initialScene, document);
    auto it = document.FindMember("actors");
    this->actors.reserve(it->value.Size());
    this->renderOrder.reserve(it->value.Size());
    for (Actor * actor : saveActors) {
        actors.push_back(actor);
        nameMap[actor->name].push_back(actor);
    }
    //Engine::clearSaves();
    for (auto& actor : it->value.GetArray()) {
        Actor* newActor = new Actor(idCount++);
        auto obj = actor.GetObject();
        it = obj.FindMember("template");
        if (it != actor.MemberEnd()) {
            std::string temp = it->value.GetString();
            auto [it2, inserted] = templates.try_emplace(temp);
            if (inserted) {
                makeTemplate(temp);
            }
            *newActor = *it2->second;
            newActor->id = idCount++;
            newActor->components.clear();
            newActor->onUpdates.clear();
            newActor->onLateUpdates.clear();
            for (auto comp : it2->second->components) {
                luabridge::LuaRef ref1 = *comp.second;
                if (ref1.isTable()) {
                    ComponentDB::addComponentFromTemplate(newActor, comp.first, *comp.second);
                    
                    
                } else if (ref1.isUserdata()) {
                    // Check if userdata is a Rigidbody
        //            Rigidbody* rb = ref1.cast<Rigidbody*>();
                        std::shared_ptr<Rigidbody> newComp = std::make_shared<Rigidbody>();
                        Rigidbody* parent = ref1.cast<Rigidbody*>();
                        *newComp = *parent;
                        newComp->actor = newActor;
                        newComp->body = nullptr;
                        luabridge::LuaRef ref(ComponentDB::luaState, *newComp);
                        newActor->components[comp.first] = std::make_shared<luabridge::LuaRef>(ref);
                        
                }
            }
            for (auto comp : it2->second->templateCount) {
                if (ComponentDB::templateCount.find(comp.first) == ComponentDB::templateCount.end()) {
                    ComponentDB::templateCount[comp.first] = 1;
                }
                else {
                    ComponentDB::templateCount[comp.first] += comp.second;
                }
            }
        }
        it = actor.FindMember("name");
        if (it != actor.MemberEnd()) {
            newActor->name = it->value.GetString();
        }
        it = actor.FindMember("components");
        if (it != actor.MemberEnd() && it->value.IsObject()) {
            for (const auto& member : it->value.GetObject()) {
                std::string key = member.name.GetString();
                newActor->sortedKeys.push_back(key);
                for (auto compit = member.value.MemberBegin(); compit != member.value.MemberEnd(); compit++) {
                    if (strcmp(compit->name.GetString(),"type") != 0) {
                        if (compit->value.IsString()) {
                            (*newActor->components[key])[compit->name.GetString()] = compit->value.GetString();
                        }
                        else {
                            if (compit->value.IsInt()) {
                                (*newActor->components[key])[compit->name.GetString()] = compit->value.GetInt();
                            }
                            else {
                                if (compit->value.IsBool()) {
                                    (*newActor->components[key])[compit->name.GetString()] = compit->value.GetBool();
                                }
                                else {
                                    (*newActor->components[key])[compit->name.GetString()] = compit->value.GetFloat();
                                }
                            }
    
                        }
                    }
                    else {
                        std::string type = compit->value.GetString();
                        if (type == "Rigidbody") {
                            if (ComponentDB::world == nullptr) {
                                ComponentDB::createWorld();
                            }
                            createRigidBody(member.value,key,newActor);
                        }
                        else {
                            if (type == "ParticleSystem") {
                                createParticleSystem(member.value,key,newActor);
                            }
                            else {
                                ComponentDB::addComponentToActor(newActor, key, type);
                            }
                        }
                        auto pos = std::lower_bound(newActor->types[type].begin(), newActor->types[type].end(), key);
                        newActor->types[type].insert(pos, key);
                        
                        if (ComponentDB::templateCount.find(type) == ComponentDB::templateCount.end()) {
                            ComponentDB::templateCount[type] = 1;
                        }
                        else {
                            ComponentDB::templateCount[type]++;
                        }
                        if (type == "Rigidbody" || type == "ParticleSystem") {
                            break;
                        }
                    }
                }
            }
        }
        std::sort(newActor->sortedKeys.begin(), newActor->sortedKeys.end());
        std::priority_queue<std::string,std::vector<std::string>, std::greater<std::string>> order;
        for (auto& pair : newActor->components) {
            luabridge::LuaRef hold = *newActor->components[pair.first];
            luabridge::LuaRef startFunc = hold["OnStart"];
            if (startFunc.isFunction()) {
                newActor->onStarts.push_back(pair.first);
            }
            luabridge::LuaRef updateFunc = hold["OnUpdate"];
            if (updateFunc.isFunction()) {
                newActor->onUpdates.push_back(pair.first);
            }
            luabridge::LuaRef lateUpdateFunc = hold["OnLateUpdate"];
            if (lateUpdateFunc.isFunction()) {
                newActor->onLateUpdates.push_back(pair.first);
            }
            luabridge::LuaRef enterFunc = hold["OnCollisionEnter"];
            if (enterFunc.isFunction()) {
                newActor->onEnters.push_back(pair.first);
            }
            luabridge::LuaRef exitFunc = hold["OnCollisionExit"];
            if (exitFunc.isFunction()) {
                newActor->onExits.push_back(pair.first);
            }
            luabridge::LuaRef enterTFunc = hold["OnTriggerEnter"];
            if (enterTFunc.isFunction()) {
                newActor->onTriggerEnters.push_back(pair.first);
            }
            luabridge::LuaRef exitTFunc = hold["OnTriggerExit"];
            if (exitTFunc.isFunction()) {
                newActor->onTriggerExits.push_back(pair.first);
            }
            luabridge::LuaRef destroyFunc = hold["OnDestroy"];
            if (destroyFunc.isFunction()) {
                newActor->onDestroys.push_back(pair.first);
            }
            
        }
        this->actors.push_back(newActor);
        this->renderOrder.push_back(newActor);
        
        this->nameMap[newActor->name].push_back(newActor);
            
        
        idMap[newActor->id] = newActor;
    }
    for (Actor* actor : actors) {
        std::sort(actor->onStarts.begin(), actor->onStarts.end());
        std::sort(actor->onUpdates.begin(), actor->onUpdates.end());
        std::sort(actor->onLateUpdates.begin(), actor->onLateUpdates.end());
        std::sort(actor->onEnters.begin(), actor->onEnters.end());
        std::sort(actor->onExits.begin(), actor->onExits.end());
        std::sort(actor->onTriggerEnters.begin(), actor->onTriggerEnters.end());
        std::sort(actor->onTriggerExits.begin(), actor->onTriggerExits.end());
        std::sort(actor->onDestroys.begin(), actor->onDestroys.end());
        for (std::string key : actor->onStarts) {
            luabridge::LuaRef hold = *(actor->components[key]);
            luabridge::LuaRef enabled = hold["enabled"];
            if (enabled.isBool()) {
                if (!(enabled.cast<bool>()))  {
                    continue;
                }
            }
            luabridge::LuaRef startFunc = hold["OnStart"];
            if (startFunc.isFunction()) {
                try {
                    startFunc(hold);
                } catch (const luabridge::LuaException& e) {
                    EngineUtil::ReportError(actor->name, e);
                }
            }
        }
    }
}


Scene::~Scene() {
//    for (Actor * actor : actors) {
//        if (Engine::canDestroy(actor)) {
//            delete actor;
//        }
//    }
}

int Scene::getIdCount() {
    return this->idCount;
}

void Scene::makeTemplate(std::string name) {
    std::string filepath = "resources/actor_templates/" + name + ".template";
    if (!std::filesystem::exists(filepath)) {
        std::cout << "error: template " + name + " is missing";
        exit(0);
    }
    rapidjson::Document actor;
    EngineUtil::ReadJsonFile(filepath, actor);
    Actor* newActor = new Actor(-1);
    auto it = actor.FindMember("name");
    if (it != actor.MemberEnd()) {
        newActor->name = it->value.GetString();
    }
    it = actor.FindMember("components");
    if (it != actor.MemberEnd() && it->value.IsObject()) {
        for (const auto& member : it->value.GetObject()) {
            std::string key = member.name.GetString();
            newActor->sortedKeys.push_back(key);
            for (auto compit = member.value.MemberBegin(); compit != member.value.MemberEnd(); compit++) {
                if (strcmp(compit->name.GetString(),"type") != 0) {
                    if (compit->value.IsString()) {
                        (*newActor->components[key])[compit->name.GetString()] = compit->value.GetString();
                    }
                    else {
                        if (compit->value.IsInt()) {
                            (*newActor->components[key])[compit->name.GetString()] = compit->value.GetInt();
                        }
                        else {
                            if (compit->value.IsBool()) {
                                (*newActor->components[key])[compit->name.GetString()] = compit->value.GetBool();
                            }
                            else {
                                (*newActor->components[key])[compit->name.GetString()] = compit->value.GetFloat();
                            }
                        }

                    }
                }
                else {
                    std::string type = compit->value.GetString();
                    newActor->keyToType[key] = type;
                    if (type == "Rigidbody") {
                        if (ComponentDB::world == nullptr) {
                            ComponentDB::createWorld();
                        }
                        createRigidBody(member.value,key,newActor);                    }
                    else {
                        if (type == "ParticleSystem") {
                            createParticleSystem(member.value,key,newActor);
                        }
                        else {
                            ComponentDB::addComponentToActor(newActor, key, type);
                        }
                    }
                    auto pos = std::lower_bound(newActor->types[type].begin(), newActor->types[type].end(), key);
                    newActor->types[type].insert(pos, key);
                    
                    if (newActor->templateCount.find(type) == newActor->templateCount.end()) {
                        newActor->templateCount[type] = 1;
                    }
                    else {
                        newActor->templateCount[type]++;
                    }

                    if (type == "Rigidbody" || type == "ParticleSystem") {
                        break;
                    }
                }
            }
        }
    }
    
    
    for (auto& pair : newActor->components) {
        luabridge::LuaRef hold = *newActor->components[pair.first];
        luabridge::LuaRef updateFunc = hold["OnUpdate"];
        if (updateFunc.isFunction()) {
            newActor->onUpdates.push_back(pair.first);
        }
        luabridge::LuaRef lateUpdateFunc = hold["OnLateUpdate"];
        if (lateUpdateFunc.isFunction()) {
            newActor->onLateUpdates.push_back(pair.first);
        }
        luabridge::LuaRef enterFunc = hold["OnCollisionEnter"];
        if (enterFunc.isFunction()) {
            newActor->onEnters.push_back(pair.first);
        }
        luabridge::LuaRef exitFunc = hold["OnCollisionExit"];
        if (exitFunc.isFunction()) {
            newActor->onExits.push_back(pair.first);
        }
        luabridge::LuaRef enterTFunc = hold["OnTriggerEnter"];
        if (enterTFunc.isFunction()) {
            newActor->onTriggerEnters.push_back(pair.first);
        }
        luabridge::LuaRef exitTFunc = hold["OnTriggerExit"];
        if (exitTFunc.isFunction()) {
            newActor->onTriggerExits.push_back(pair.first);
        }
        luabridge::LuaRef destroyFunc = hold["OnDestroy"];
        if (destroyFunc.isFunction()) {
            newActor->onDestroys.push_back(pair.first);
        }
        
    }
    std::sort(newActor->onUpdates.begin(), newActor->onUpdates.end());
    std::sort(newActor->onLateUpdates.begin(), newActor->onLateUpdates.end());
    std::sort(newActor->onEnters.begin(), newActor->onEnters.end());
    std::sort(newActor->onExits.begin(), newActor->onExits.end());
    std::sort(newActor->onTriggerEnters.begin(), newActor->onTriggerEnters.end());
    std::sort(newActor->onTriggerExits.begin(), newActor->onTriggerExits.end());
    std::sort(newActor->onDestroys.begin(), newActor->onDestroys.end());
    
    std::sort(newActor->sortedKeys.begin(), newActor->sortedKeys.end());
    templates[name] = newActor;
}


luabridge::LuaRef Scene::Find(std::string name) {
    std::vector<Actor*> vec = nameMap[name];
    if (vec.empty()) {
        return luabridge::LuaRef(ComponentDB::luaState);
    }
    return luabridge::LuaRef(ComponentDB::luaState, vec[0]);
}

luabridge::LuaRef Scene::FindAll(std::string name) {
    luabridge::LuaRef newTable = luabridge::newTable(ComponentDB::luaState);
    std::vector<Actor*> vec = nameMap[name];
    if (vec.empty()) {
        return luabridge::LuaRef(ComponentDB::luaState);
    }
    for (int i =0; i < vec.size(); i++) {
        newTable[i+1] = vec[i];
    }
    return newTable;
}

void Scene::onUpdates() {
    for (Actor* actor : actors) {
        for (std::string key : actor->onUpdates) {
            luabridge::LuaRef hold = *actor->components[key];
            luabridge::LuaRef enabled = hold["enabled"];
            if (enabled.isBool()) {
                if (!(enabled.cast<bool>()))  {
                    continue;
                }
            }
            luabridge::LuaRef updateFunc = hold["OnUpdate"];
            //if (updateFunc.isFunction()) {
                try {
                    updateFunc(hold);
                } catch (const luabridge::LuaException& e) {
                    EngineUtil::ReportError(actor->name, e);
                }
            //}
        }
    }
}

void Scene::onLateUpdates() {
    for (Actor* actor : actors) {
        for (std::string key : actor->onLateUpdates) {
            luabridge::LuaRef hold = *actor->components[key];
            luabridge::LuaRef enabled = hold["enabled"];
            if (enabled.isBool()) {
                if (!(enabled.cast<bool>()))  {
                    continue;
                }
            }
            luabridge::LuaRef updateFunc = hold["OnLateUpdate"];
            if (updateFunc.isFunction()) {
                try {
                    updateFunc(hold);
                } catch (const luabridge::LuaException& e) {
                    EngineUtil::ReportError(actor->name, e);
                }
            }
        }
    }
}

void Scene::doLateAdds() {
    for (Actor* actor : lateAdds) {
        for (auto comp : actor->lateAdd) {
            if (actor->sortedKeys.empty()) {
                actor->sortedKeys.push_back(comp.first);
            }
            else {
                auto pos = std::lower_bound(actor->sortedKeys.begin(), actor->sortedKeys.end(), comp.first);
                actor->sortedKeys.insert(pos, comp.first);
            }
            std::vector<std::string> vec = actor->types[comp.second];
//            if (vec.empty()) {
//                vec.push_back(comp.first);
//            }
//            else {
//                auto pos2 = std::lower_bound(vec.begin(), vec.end(), comp.first);
//                vec.insert(pos2, comp.first);
//            }
            auto pos2 = std::lower_bound(actor->types[comp.second].begin(), actor->types[comp.second].end(), comp.first);
            actor->types[comp.second].insert(pos2, comp.first);
            
            luabridge::LuaRef hold = *(actor->components[comp.first]);
            
            luabridge::LuaRef updateFunc = hold["OnUpdate"];
            if (updateFunc.isFunction()) {
                auto pos2 = std::lower_bound(actor->onUpdates.begin(), actor->onUpdates.end(), comp.first);
                actor->onUpdates.insert(pos2, comp.first);
            }
            luabridge::LuaRef updateLateFunc = hold["OnLateUpdate"];
            if (updateLateFunc.isFunction()) {
                auto pos2 = std::lower_bound(actor->onLateUpdates.begin(), actor->onLateUpdates.end(), comp.first);
                actor->onLateUpdates.insert(pos2, comp.first);
            }
            luabridge::LuaRef destroyFunc = hold["OnDestroy"];
            if (destroyFunc.isFunction()) {
                auto pos2 = std::lower_bound(actor->onDestroys.begin(), actor->onDestroys.end(), comp.first);
                actor->onDestroys.insert(pos2, comp.first);
            }
            luabridge::LuaRef enterFunc = hold["OnCollisionEnter"];
            if (enterFunc.isFunction()) {
                auto pos2 = std::lower_bound(actor->onEnters.begin(), actor->onEnters.end(), comp.first);
                actor->onEnters.insert(pos2, comp.first);
            }
            luabridge::LuaRef exitFunc = hold["OnCollisionExit"];
            if (exitFunc.isFunction()) {
                auto pos2 = std::lower_bound(actor->onExits.begin(), actor->onExits.end(), comp.first);
                actor->onExits.insert(pos2, comp.first);
            }
            luabridge::LuaRef enterTFunc = hold["OnTriggerEnter"];
            if (enterTFunc.isFunction()) {
                auto pos2 = std::lower_bound(actor->onTriggerEnters.begin(), actor->onTriggerEnters.end(), comp.first);
                actor->onTriggerEnters.insert(pos2, comp.first);
            }
            luabridge::LuaRef exitTFunc = hold["OnTriggerExit"];
            if (exitTFunc.isFunction()) {
                auto pos2 = std::lower_bound(actor->onTriggerExits.begin(), actor->onTriggerExits.end(), comp.first);
                actor->onTriggerExits.insert(pos2, comp.first);
            }
            
            
            luabridge::LuaRef enabled = hold["enabled"];
            if (enabled.isBool()) {
                if (!(enabled.cast<bool>()))  {
                    continue;
                }
            }
            luabridge::LuaRef startFunc = hold["OnStart"];
            if (startFunc.isFunction()) {
                try {
                    startFunc(hold);
                } catch (const luabridge::LuaException& e) {
                    EngineUtil::ReportError(actor->name, e);
                }
            }
        }
        actor->lateAdd.clear();
    }
    lateAdds.clear();
}

void Scene::doLateRemoves() {

    for (auto p : lateRemoves) {
        p.first->sortedKeys.erase(std::find(p.first->sortedKeys.begin(), p.first->sortedKeys.end(), p.second));
        p.first->components.erase(p.second);
        auto it = std::find(p.first->onUpdates.begin(),p.first->onUpdates.end(),p.second);
        if (it != p.first->onUpdates.end()) {
            p.first->onUpdates.erase(it);
        }
        it = std::find(p.first->onLateUpdates.begin(),p.first->onLateUpdates.end(),p.second);
        if (it != p.first->onLateUpdates.end()) {
            p.first->onLateUpdates.erase(it);
        }
        it = std::find(p.first->onEnters.begin(),p.first->onEnters.end(),p.second);
        if (it != p.first->onEnters.end()) {
            p.first->onEnters.erase(it);
        }
        it = std::find(p.first->onExits.begin(),p.first->onExits.end(),p.second);
        if (it != p.first->onExits.end()) {
            p.first->onExits.erase(it);
        }
        it = std::find(p.first->onTriggerEnters.begin(),p.first->onTriggerEnters.end(),p.second);
        if (it != p.first->onTriggerEnters.end()) {
            p.first->onTriggerEnters.erase(it);
        }
        it = std::find(p.first->onTriggerExits.begin(),p.first->onTriggerExits.end(),p.second);
        if (it != p.first->onTriggerExits.end()) {
            p.first->onTriggerExits.erase(it);
        }
        it = std::find(p.first->onDestroys.begin(),p.first->onDestroys.end(),p.second);
        if (it != p.first->onDestroys.end()) {
            p.first->onDestroys.erase(it);
        }
    }
    lateRemoves.clear();
}


luabridge::LuaRef Scene::Instantiate(std::string temp) {
    Actor* newActor = new Actor(-1);
    if (templates.find(temp) == templates.end()) {
        makeTemplate(temp);
    }
    Actor* it2 = templates[temp];
    *newActor = *it2;
    newActor->id = idCount++;
    newActor->components.clear();
    for (auto comp : it2->components) {
        luabridge::LuaRef ref1 = *comp.second;
        if (ref1.isTable()) {
            ComponentDB::addComponentFromTemplate(newActor, comp.first, *comp.second);
            
            
        } else if (ref1.isUserdata()) {
            std::string type = it2->keyToType[comp.first];
            if (type == "Rigidbody") {
                Rigidbody* parent = ref1.cast<Rigidbody*>();
                std::shared_ptr<Rigidbody> newComp = std::make_shared<Rigidbody>();
                *newComp = *parent;
                newComp->actor = newActor;
                newComp->body = nullptr;
                luabridge::LuaRef ref(ComponentDB::luaState, *newComp);
                newActor->components[comp.first] = std::make_shared<luabridge::LuaRef>(ref);
            }
            if (type == "ParticleSystem") {
                ParticleSystem* parent = ref1.cast<ParticleSystem*>();
                std::shared_ptr<ParticleSystem> newComp = std::make_shared<ParticleSystem>();
                *newComp = *parent;
                luabridge::LuaRef ref(ComponentDB::luaState, *newComp);
                newActor->components[comp.first] = std::make_shared<luabridge::LuaRef>(ref);
            }
                
        }
    }
    for (auto comp : it2->templateCount) {
        if (ComponentDB::templateCount.find(comp.first) == ComponentDB::templateCount.end()) {
            ComponentDB::templateCount[comp.first] = 1;
        }
        else {
            ComponentDB::templateCount[comp.first] += comp.second;
        }
    }
    
    nameMap[newActor->name].push_back(newActor);
    lateActorAdds.push_back(newActor);
    newOnStartActors.push_back(newActor);
    return luabridge::LuaRef(ComponentDB::luaState, newActor);
}

void Scene::doLateActorAdds() {
    for (Actor* actor : lateActorAdds) {
        actors.push_back(actor);
        //newActors.push_back(actor);
    }
    lateActorAdds.clear();
}

void Scene::Destroy(Actor *actor) {
    for (std::string key : actor->sortedKeys) {
        luabridge::LuaRef ref = *actor->components[key];
        ref["enabled"] = false;
    }
    for (std::string key : actor->onDestroys) {
            luabridge::LuaRef hold = *(actor->components[key]);
            luabridge::LuaRef func = hold["OnDestroy"];
            if (func.isFunction()) {
                try {
                    func(hold);
                } catch (const luabridge::LuaException& e) {
                    EngineUtil::ReportError(actor->name, e);
                }
            }
        }
    nameMap[actor->name].erase(std::find(nameMap[actor->name].begin(), nameMap[actor->name].end(), actor));
    lateActorDeletes.push_back(actor);
}

void Scene::doLateActorDeletes() {
    for (Actor* actor : lateActorDeletes) {
        actors.erase(std::find(actors.begin(), actors.end(), actor));
        if (std::find(lateActorAdds.begin(), lateActorAdds.end(), actor) != lateActorAdds.end()) {
            lateActorAdds.erase(std::find(lateActorAdds.begin(), lateActorAdds.end(), actor));
        }
        delete actor;
    }
    lateActorDeletes.clear();
}

void Scene::doNewOnStarts() {
    for (Actor * actor : newOnStartActors) {
        //if (std::find(actors.begin(),actors.end(),actor)!= actors.end()) {
            for (std::string key : actor->sortedKeys) {
                luabridge::LuaRef hold = *(actor->components[key]);
                luabridge::LuaRef enabled = hold["enabled"];
                if (enabled.isBool()) {
                    if (!(enabled.cast<bool>()))  {
                        continue;
                    }
                }
                luabridge::LuaRef startFunc = hold["OnStart"];
                if (startFunc.isFunction()) {
                    try {
                        startFunc(hold);
                    } catch (const luabridge::LuaException& e) {
                        EngineUtil::ReportError(actor->name, e);
                    }
                }
            }
       // }
    }
    //newActors.clear();
    newOnStartActors.clear();
}

void Scene::createRigidBody(const rapidjson::Value & doc, std::string key, Actor* actor) {
    //std::shared_ptr<Rigidbody> rb = std::make_shared<Rigidbody>();
    Rigidbody* rb = new Rigidbody();
    auto it = doc.FindMember("x");
    if (it != doc.MemberEnd()) {
        rb->x = it->value.GetFloat();
    }
    it = doc.FindMember("y");
    if (it != doc.MemberEnd()) {
        rb->y = it->value.GetFloat();
    }
    it = doc.FindMember("body_type");
    if (it != doc.MemberEnd()) {
        rb->body_type = it->value.GetString();
    }
    it = doc.FindMember("precise");
    if (it != doc.MemberEnd()) {
        rb->precise = it->value.GetBool();
    }
    it = doc.FindMember("gravity_scale");
    if (it != doc.MemberEnd()) {
        rb->gravity_scale = it->value.GetFloat();
    }
    it = doc.FindMember("density");
    if (it != doc.MemberEnd()) {
        rb->density = it->value.GetFloat();
    }
    it = doc.FindMember("angular_friction");
    if (it != doc.MemberEnd()) {
        rb->angular_friction = it->value.GetFloat();
    }
    it = doc.FindMember("rotation");
    if (it != doc.MemberEnd()) {
        rb->rotationDegrees = it->value.GetFloat();
        rb->rotation = it->value.GetFloat() * (b2_pi/180.0f);
    }
    it = doc.FindMember("has_collider");
    if (it != doc.MemberEnd()) {
        rb->has_collider = it->value.GetBool();
    }
    it = doc.FindMember("has_trigger");
    if (it != doc.MemberEnd()) {
        rb->has_trigger = it->value.GetBool();
    }
    it = doc.FindMember("collider_type");
    if (it != doc.MemberEnd()) {
        rb->collider_type = it->value.GetString();
    }
    it = doc.FindMember("width");
    if (it != doc.MemberEnd()) {
        rb->width = it->value.GetFloat();
    }
    it = doc.FindMember("height");
    if (it != doc.MemberEnd()) {
        rb->height = it->value.GetFloat();
    }
    it = doc.FindMember("radius");
    if (it != doc.MemberEnd()) {
        rb->radius = it->value.GetFloat();
    }
    it = doc.FindMember("friction");
    if (it != doc.MemberEnd()) {
        rb->friction = it->value.GetFloat();
    }
    it = doc.FindMember("bounciness");
    if (it != doc.MemberEnd()) {
        rb->bounciness = it->value.GetFloat();
    }
    it = doc.FindMember("trigger_type");
    if (it != doc.MemberEnd()) {
        rb->trigger_type = it->value.GetString();
    }
    it = doc.FindMember("trigger_width");
    if (it != doc.MemberEnd()) {
        rb->trigger_width = it->value.GetFloat();
    }
    it = doc.FindMember("trigger_height");
    if (it != doc.MemberEnd()) {
        rb->trigger_height = it->value.GetFloat();
    }
    it = doc.FindMember("trigger_radius");
    if (it != doc.MemberEnd()) {
        rb->trigger_radius = it->value.GetFloat();
    }
    rb->actor = actor;
    rb->key = key;
    luabridge::LuaRef ref(ComponentDB::luaState, rb);
    actor->components[key] = std::make_shared<luabridge::LuaRef>(ref);
}

void Scene::doOnDestroys() {
    for (auto & it : lateRemoves) {
        luabridge::LuaRef hold = *(it.first->components[it.second]);
                luabridge::LuaRef func = hold["OnDestroy"];
                if (func.isFunction()) {
                    try {
                        func(hold);
                    } catch (const luabridge::LuaException& e) {
                        EngineUtil::ReportError(it.first->name, e);
                    }
                }
    }
}


void Scene::loadSceneDestroys() {
    for (Actor* actor : actors) {
        if (Engine::saveSet.find(actor) != Engine::saveSet.end()) {
            continue;
        }
        for (std::string key : actor->onDestroys) {
            luabridge::LuaRef hold = *(actor->components[key]);
            luabridge::LuaRef func = hold["OnDestroy"];
            if (func.isFunction()) {
                try {
                    func(hold);
                } catch (const luabridge::LuaException& e) {
                    EngineUtil::ReportError(actor->name, e);
                }
            }
        }
    }
}

void Scene::createParticleSystem(const rapidjson::Value& doc, std::string key, Actor* actor) {
    ParticleSystem* ps = new ParticleSystem();
    
    auto it = doc.FindMember("x");
    if (it != doc.MemberEnd()) {
        ps->x = it->value.GetFloat();
    }
    it = doc.FindMember("y");
    if (it != doc.MemberEnd()) {
        ps->y = it->value.GetFloat();
    }
    it = doc.FindMember("frames_between_bursts");
    if (it != doc.MemberEnd()) {
        if (it->value.GetInt() > 1) {
            ps->framesBetweenBursts = it->value.GetInt();
        }
    }
    it = doc.FindMember("burst_quantity");
    if (it != doc.MemberEnd()) {
        if (it->value.GetInt() > 1) {
            ps->burstQuantity = it->value.GetInt();
        }
    }
    it = doc.FindMember("start_scale_min");
    if (it != doc.MemberEnd()) {
        ps->startScaleMin = it->value.GetFloat();
    }
    it = doc.FindMember("start_scale_max");
    if (it != doc.MemberEnd()) {
        ps->startScaleMax = it->value.GetFloat();
    }
    it = doc.FindMember("rotation_min");
    if (it != doc.MemberEnd()) {
        ps->rotationMin = it->value.GetFloat();
    }
    it = doc.FindMember("rotation_max");
    if (it != doc.MemberEnd()) {
        ps->rotationMax = it->value.GetFloat();
    }
    it = doc.FindMember("start_color_r");
    if (it != doc.MemberEnd()) {
        ps->r = it->value.GetInt();
    }
    it = doc.FindMember("start_color_g");
    if (it != doc.MemberEnd()) {
        ps->g = it->value.GetInt();
    }
    it = doc.FindMember("start_color_b");
    if (it != doc.MemberEnd()) {
        ps->b = it->value.GetInt();
    }
    it = doc.FindMember("start_color_a");
    if (it != doc.MemberEnd()) {
        ps->a = it->value.GetInt();
    }
    it = doc.FindMember("emit_radius_min");
    if (it != doc.MemberEnd()) {
        ps->radiusMin = it->value.GetFloat();
    }
    it = doc.FindMember("emit_radius_max");
    if (it != doc.MemberEnd()) {
        ps->radiusMax = it->value.GetFloat();
    }
    it = doc.FindMember("emit_angle_min");
    if (it != doc.MemberEnd()) {
        ps->angleMin = it->value.GetFloat();
    }
    it = doc.FindMember("emit_angle_max");
    if (it != doc.MemberEnd()) {
        ps->angleMax = it->value.GetFloat();
    }
    it = doc.FindMember("image");
    if (it != doc.MemberEnd()) {
        ps->image = it->value.GetString();
    }
    it = doc.FindMember("sorting_order");
    if (it != doc.MemberEnd()) {
        ps->sort = it->value.GetInt();
    }
    it = doc.FindMember("duration_frames");
    if (it != doc.MemberEnd()) {
        if (it->value.GetInt() >= 1) {
            ps->duration = it->value.GetInt();
        }
        else {
            ps->duration = 1;
        }
    }
    it = doc.FindMember("start_speed_min");
    if (it != doc.MemberEnd()) {
        ps->speedMin = it->value.GetFloat();
    }
    it = doc.FindMember("start_speed_max");
    if (it != doc.MemberEnd()) {
        ps->speedMax = it->value.GetFloat();
    }
    it = doc.FindMember("rotation_speed_min");
    if (it != doc.MemberEnd()) {
        ps->angSpeedMin = it->value.GetFloat();
    }
    it = doc.FindMember("rotation_speed_max");
    if (it != doc.MemberEnd()) {
        ps->angSpeedMax = it->value.GetFloat();
    }
    it = doc.FindMember("gravity_scale_x");
    if (it != doc.MemberEnd()) {
        ps->xgravity = it->value.GetFloat();
    }
    it = doc.FindMember("gravity_scale_y");
    if (it != doc.MemberEnd()) {
        ps->ygravity = it->value.GetFloat();
    }
    it = doc.FindMember("drag_factor");
    if (it != doc.MemberEnd()) {
        ps->drag = it->value.GetFloat();
    }
    it = doc.FindMember("angular_drag_factor");
    if (it != doc.MemberEnd()) {
        ps->angDrag = it->value.GetFloat();
    }
    it = doc.FindMember("end_scale");
    if (it != doc.MemberEnd()) {
        ps->endScale = it->value.GetFloat();
    }
    it = doc.FindMember("end_color_r");
    if (it != doc.MemberEnd()) {
        ps->endr = it->value.GetInt();
    }
    it = doc.FindMember("end_color_g");
    if (it != doc.MemberEnd()) {
        ps->endg = it->value.GetInt();
    }
    it = doc.FindMember("end_color_b");
    if (it != doc.MemberEnd()) {
        ps->endb = it->value.GetInt();
    }
    it = doc.FindMember("end_color_a");
    if (it != doc.MemberEnd()) {
        ps->enda = it->value.GetInt();
    }
    
    
    luabridge::LuaRef ref(ComponentDB::luaState, ps);
    actor->components[key] = std::make_shared<luabridge::LuaRef>(ref);
}
