//
//  CollisionListener.cpp
//  game_engine
//
//  Created by Ben Huston on 3/23/25.
//

#include "CollisionListener.hpp"


void CollisionListener::BeginContact(b2Contact *contact) {
    b2Fixture* fixa = contact->GetFixtureA();
    b2Fixture* fixb = contact->GetFixtureB();
    Actor* actora = reinterpret_cast<Actor*>(fixa->GetUserData().pointer);
    Actor* actorb = reinterpret_cast<Actor*>(fixb->GetUserData().pointer);
    
    if (actora == nullptr || actorb == nullptr) {
        return;
    }
    
    if ((fixa->IsSensor() && !fixb->IsSensor()) || (!fixa->IsSensor() && fixb->IsSensor())) {
        return;
    }
    
    if (!fixa->IsSensor()) {
        b2WorldManifold worldManifold;
        contact->GetWorldManifold(&worldManifold);
        
        Collision col;
        col.other = actorb;
        col.point = worldManifold.points[0];
        col.relative_velocity = fixa->GetBody()->GetLinearVelocity() - fixb->GetBody()->GetLinearVelocity();
        col.normal = worldManifold.normal;
        
        luabridge::LuaRef ref(ComponentDB::luaState, col);
        
        for (std::string key : actora->onEnters) {
            luabridge::LuaRef comp = *actora->components[key];
            luabridge::LuaRef enterFunc = comp["OnCollisionEnter"];
            if (enterFunc.isFunction()) {
                try {
                    enterFunc(comp,ref);
                } catch (const luabridge::LuaException& e) {
                    EngineUtil::ReportError(actora->name, e);
                }
            }
        }
        
        col.other = actora;
        
        luabridge::LuaRef ref2(ComponentDB::luaState, col);
        
        for (std::string key : actorb->onEnters) {
            luabridge::LuaRef comp = *actorb->components[key];
            luabridge::LuaRef enterFunc = comp["OnCollisionEnter"];
            if (enterFunc.isFunction()) {
                try {
                    enterFunc(comp,ref2);
                } catch (const luabridge::LuaException& e) {
                    EngineUtil::ReportError(actorb->name, e);
                }
            }
        }
        
    }
    else {
        Collision col;
        col.other = actorb;
        col.point = b2Vec2(-999.0f,-999.0f);
        col.relative_velocity = fixa->GetBody()->GetLinearVelocity() - fixb->GetBody()->GetLinearVelocity();
        col.normal = b2Vec2(-999.0f,-999.0f);
        
        luabridge::LuaRef ref(ComponentDB::luaState, col);
        
        for (std::string key : actora->onTriggerEnters) {
            luabridge::LuaRef comp = *actora->components[key];
            luabridge::LuaRef enterFunc = comp["OnTriggerEnter"];
            if (enterFunc.isFunction()) {
                try {
                    enterFunc(comp,ref);
                } catch (const luabridge::LuaException& e) {
                    EngineUtil::ReportError(actora->name, e);
                }
            }
        }
        
        col.other = actora;
        
        luabridge::LuaRef ref2(ComponentDB::luaState, col);
        
        for (std::string key : actorb->onTriggerEnters) {
            luabridge::LuaRef comp = *actorb->components[key];
            luabridge::LuaRef enterFunc = comp["OnTriggerEnter"];
            if (enterFunc.isFunction()) {
                try {
                    enterFunc(comp,ref2);
                } catch (const luabridge::LuaException& e) {
                    EngineUtil::ReportError(actorb->name, e);
                }
            }
        }
    }
    
}

void CollisionListener::EndContact(b2Contact *contact) {
    b2Fixture* fixa = contact->GetFixtureA();
    b2Fixture* fixb = contact->GetFixtureB();
    Actor* actora = reinterpret_cast<Actor*>(fixa->GetUserData().pointer);
    Actor* actorb = reinterpret_cast<Actor*>(fixb->GetUserData().pointer);
    
    if (actora == nullptr || actorb == nullptr) {
        return;
    }
    
    if ((fixa->IsSensor() && !fixb->IsSensor()) || (!fixa->IsSensor() && fixb->IsSensor())) {
        return;
    }
    
    if (!fixa->IsSensor()) {
        
        Collision col;
        col.other = actorb;
        col.point = b2Vec2(-999.0f,-999.0f);
        col.relative_velocity = fixa->GetBody()->GetLinearVelocity() - fixb->GetBody()->GetLinearVelocity();
        col.normal = b2Vec2(-999.0f,-999.0f);
        
        luabridge::LuaRef ref(ComponentDB::luaState, col);
        
        for (std::string key : actora->onExits) {
            luabridge::LuaRef comp = *actora->components[key];
            luabridge::LuaRef enterFunc = comp["OnCollisionExit"];
            if (enterFunc.isFunction()) {
                try {
                    enterFunc(comp,ref);
                } catch (const luabridge::LuaException& e) {
                    EngineUtil::ReportError(actora->name, e);
                }
            }
        }
        
        col.other = actora;
        
        luabridge::LuaRef ref2(ComponentDB::luaState, col);
        
        for (std::string key : actorb->onExits) {
            luabridge::LuaRef comp = *actorb->components[key];
            luabridge::LuaRef enterFunc = comp["OnCollisionExit"];
            if (enterFunc.isFunction()) {
                try {
                    enterFunc(comp,ref2);
                } catch (const luabridge::LuaException& e) {
                    EngineUtil::ReportError(actorb->name, e);
                }
            }
        }
        
    }
    else {
        Collision col;
        col.other = actorb;
        col.point = b2Vec2(-999.0f,-999.0f);
        col.relative_velocity = fixa->GetBody()->GetLinearVelocity() - fixb->GetBody()->GetLinearVelocity();
        col.normal = b2Vec2(-999.0f,-999.0f);
        
        luabridge::LuaRef ref(ComponentDB::luaState, col);
        
        for (std::string key : actora->onTriggerExits) {
            luabridge::LuaRef comp = *actora->components[key];
            luabridge::LuaRef enterFunc = comp["OnTriggerExit"];
            if (enterFunc.isFunction()) {
                try {
                    enterFunc(comp,ref);
                } catch (const luabridge::LuaException& e) {
                    EngineUtil::ReportError(actora->name, e);
                }
            }
        }
        
        col.other = actora;
        
        luabridge::LuaRef ref2(ComponentDB::luaState, col);
        
        for (std::string key : actorb->onTriggerExits) {
            luabridge::LuaRef comp = *actorb->components[key];
            luabridge::LuaRef enterFunc = comp["OnTriggerExit"];
            if (enterFunc.isFunction()) {
                try {
                    enterFunc(comp,ref2);
                } catch (const luabridge::LuaException& e) {
                    EngineUtil::ReportError(actorb->name, e);
                }
            }
        }
        
    }
}
