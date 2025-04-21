//
//  Actor.cpp
//  game_engine
//
//  Created by Ben Huston on 2/28/25.
//

#include "Actor.hpp"

void Actor::injectReference(std::shared_ptr<luabridge::LuaRef> ref) {
    (*ref)["actor"] = this;
}

std::string Actor::GetName() {
    return this->name;
}

int Actor::GetID() {
    return this->id;
}

luabridge::LuaRef Actor::GetComponentByKey(std::string key) {
    if (this->components.find(key) != this->components.end()) {
        return *(this->components[key]);
    }
    return luabridge::LuaRef(ComponentDB::luaState);
}

luabridge::LuaRef Actor::GetComponent(std::string type) {
    std::vector vec =  this->types[type];
    if (!vec.empty()) {
        return *(this->components[vec[0]]);
    }
    return luabridge::LuaRef(ComponentDB::luaState);
}

luabridge::LuaRef Actor::GetComponents(std::string type) {
    luabridge::LuaRef newTable = luabridge::newTable(ComponentDB::luaState);
    std::vector vec =  this->types[type];
    if (vec.empty()) {
        return luabridge::LuaRef(ComponentDB::luaState);
    }
    for (int i =0; i < vec.size(); i++) {
        newTable[i+1] = *(this->components[vec[i]]);
    }
    return newTable;
}

luabridge::LuaRef Actor::AddComponent(std::string type) {
    if (ComponentDB::templateCount.find(type) == ComponentDB::templateCount.end()) {
        ComponentDB::templateCount[type] = 0;
    }
    int n = ComponentDB::templateCount[type]++;
    std::string key = "r" + std::to_string(n);
    
    if (type == "Rigidbody") {
        if (ComponentDB::world == nullptr) {
            ComponentDB::createWorld();
        }
        std::shared_ptr<Rigidbody> rb = std::make_shared<Rigidbody>();
        rb->actor = this;
        rb->key = key;
        luabridge::LuaRef ref(ComponentDB::luaState, *rb);
        lateAdd.push_back({key,type});
        this->components[key] = std::make_shared<luabridge::LuaRef>(ref);
        Scene::lateAdds.push_back(this);
        return ref;
    }
    else {
        if (ComponentDB::components.find(type) == ComponentDB::components.end()) {
            ComponentDB::addComponent(type);
        }
        luabridge::LuaRef emptyTable = luabridge::newTable(ComponentDB::luaState);
        luabridge::LuaRef parent = luabridge::getGlobal(ComponentDB::luaState, type.c_str());
        ComponentDB::establishInheritance(emptyTable, parent);
        emptyTable["key"] = key;
        emptyTable["actor"] = this;
        emptyTable["enabled"] = true;
        lateAdd.push_back({key,type});
        this->components[key] = std::make_shared<luabridge::LuaRef>(emptyTable);
        Scene::lateAdds.push_back(this);
        return emptyTable;
    }
}

void Actor::RemoveComponent(luabridge::LuaRef ref) {
    ref["enabled"] = false;
    luabridge::LuaRef keyLua = ref["key"];
    std::string key = keyLua.cast<std::string>();
    std::string type;
    for (auto comp : this->types) {
        if (std::find(comp.second.begin(), comp.second.end(), key) != comp.second.end()) {
            //types[comp.first].erase(std::find(comp.second.begin(), comp.second.end(), key));
            type = comp.first;
            break;
        }
    }
    if (key == "") {
        Rigidbody rb = ref.cast<Rigidbody>();
        key = rb.key;
        type = "Rigidbody";
    }
    types[type].erase(std::find(types[type].begin(),types[type].end(),key));
    Scene::lateRemoves.push_back({this,key});
    //lateRemove.push_back(key);
    //components.erase(key.cast<std::string>());
}

