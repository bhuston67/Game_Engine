//
//  Event.cpp
//  game_engine
//
//  Created by Ben Huston on 3/24/25.
//

#include "Event.hpp"


void Event::Subscribe(std::string event, luabridge::LuaRef comp, luabridge::LuaRef function) {
    lateSubs.push_back({event,{comp,function}});
}

void Event::Unsubscribe(std::string event, luabridge::LuaRef comp, luabridge::LuaRef function) {
    lateUnsubs.push_back({event,{comp,function}});
}

void Event::Publish(std::string event, luabridge::LuaRef object) {
    std::vector<std::pair<luabridge::LuaRef,luabridge::LuaRef>> refs = subs[event];
    for (auto comp : refs) {
        if (std::find(unsubs[event].begin(), unsubs[event].end(), comp) == unsubs[event].end()) {
            luabridge::LuaRef func = comp.second;
            luabridge::LuaRef self = comp.first;
            if (func.isFunction()) {
                try {
                    func(self,object);
                } catch (const luabridge::LuaException& e) {
                    
                }
            }
        }
    }
}

void Event::lateHandleEvents() {
    for (auto comp : lateSubs) {
        if (std::find(subs[comp.first].begin(), subs[comp.first].end(), comp.second) == subs[comp.first].end()) {
            subs[comp.first].push_back(comp.second);
        }
    }
    for (auto comp : lateUnsubs) {
        if (std::find(unsubs[comp.first].begin(), unsubs[comp.first].end(), comp.second) == unsubs[comp.first].end()) {
            unsubs[comp.first].push_back(comp.second);
        }
        //unsubs.insert(comp);
    }
}
