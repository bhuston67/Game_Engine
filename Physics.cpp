//
//  Physics.cpp
//  game_engine
//
//  Created by Ben Huston on 3/24/25.
//

#include "Physics.hpp"


luabridge::LuaRef Physics::Raycast(b2Vec2 pos, b2Vec2 dir, float dist) {
    if (ComponentDB::world == nullptr || dist <= 0) {
        return luabridge::LuaRef(ComponentDB::luaState);
    }
    Raycaster ray;
    dir.Normalize();
    dir.x *= dist;
    dir.y *= dist;
    b2Vec2 dest = pos + dir;
    ComponentDB::world->RayCast(&ray, pos, dest);
    if (ray.hits.empty()) {
        return luabridge::LuaRef(ComponentDB::luaState);
    }
    ray.sortHits();
    HitResult hit;
    hit.actor = reinterpret_cast<Actor*>(ray.hits[0].fixture->GetUserData().pointer);
    hit.point = ray.hits[0].point;
    hit.is_trigger = ray.hits[0].fixture->IsSensor();
    hit.normal = ray.hits[0].normal;
    luabridge::LuaRef ref(ComponentDB::luaState,hit);
    return ref;

}

luabridge::LuaRef Physics::RaycastAll(b2Vec2 pos, b2Vec2 dir, float dist) {
    if (ComponentDB::world == nullptr || dist <= 0) {
        return luabridge::LuaRef(ComponentDB::luaState);
    }
    Raycaster ray;
    dir.Normalize();
    dir.x *= dist;
    dir.y *= dist;
    b2Vec2 dest = pos + dir;
    ComponentDB::world->RayCast(&ray, pos, dest);
//    if (ray.hits.empty()) {
//        return luabridge::LuaRef(ComponentDB::luaState);
//    }
    ray.sortHits();
    luabridge::LuaRef newTable = luabridge::newTable(ComponentDB::luaState);
    int i = 1;
    for (auto hit : ray.hits) {
        HitResult hitval;
        hitval.actor = reinterpret_cast<Actor*>(hit.fixture->GetUserData().pointer);
        hitval.point = hit.point;
        hitval.is_trigger = hit.fixture->IsSensor();
        hitval.normal = hit.normal;
        newTable[i] = hitval;
        i++;
    }
    return newTable;
}
