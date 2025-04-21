//
//  CollisionListener.hpp
//  game_engine
//
//  Created by Ben Huston on 3/23/25.
//

#ifndef CollisionListener_hpp
#define CollisionListener_hpp

#include <stdio.h>
#include "box2d/box2d.h"
#include "Actor.hpp"
#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "EngineUtil.h"

struct Actor;

struct Collision {
    Actor* other;
    b2Vec2 point;
    b2Vec2 relative_velocity;
    b2Vec2 normal;
};




class CollisionListener : public b2ContactListener {
public:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
};

#endif /* CollisionListener_hpp */
