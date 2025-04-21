//
//  Physics.hpp
//  game_engine
//
//  Created by Ben Huston on 3/24/25.
//

#ifndef Physics_hpp
#define Physics_hpp

#include <stdio.h>
#include <vector>

#include "Actor.hpp"
#include "ComponentDB.hpp"

#include "box2d/box2d.h"
#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"



struct HitResult {
    Actor* actor;
    b2Vec2 point;
    b2Vec2 normal;
    bool is_trigger;
};



class Raycaster : public b2RayCastCallback {
public:
    
    struct Hit {
            b2Fixture* fixture;
            b2Vec2 point;
            b2Vec2 normal;
            float fraction;
        
        bool operator<(const Hit& other) const {
                return fraction < other.fraction;
            }
        };

        std::vector<Hit> hits;

        float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
                            const b2Vec2& normal, float fraction) override {
            if (reinterpret_cast<Actor*>(fixture->GetUserData().pointer) == nullptr) {
                return -1.0f;
            }
            hits.push_back({fixture, point, normal, fraction});
            return 1.0f; // Continue raycast to find more hits
        }
    
    void sortHits() {
        std::sort(hits.begin(), hits.end());
    }
};




class Physics {
public:
    
    static luabridge::LuaRef Raycast(b2Vec2 pos, b2Vec2 dir, float dist);
    
    static luabridge::LuaRef RaycastAll(b2Vec2 pos, b2Vec2 dir, float dist);
};


#endif /* Physics_hpp */
