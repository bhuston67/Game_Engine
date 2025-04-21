//
//  RigidBody.hpp
//  game_engine
//
//  Created by Ben Huston on 3/20/25.
//

#ifndef RigidBody_hpp
#define RigidBody_hpp

#include <stdio.h>
#include <string>

#include "box2d/box2d.h"
#include "ComponentDB.hpp"
#include "Actor.hpp"

class ComponentDB;
struct Actor;

class Rigidbody {
public:
    float x;
    float y;
    std::string body_type;
    bool precise;
    float gravity_scale;
    float density;
    float angular_friction;
    float rotation;
    bool has_collider;
    bool has_trigger;
    std::string collider_type;
    float width;
    float height;
    float radius;
    float friction;
    float bounciness;
    std::string trigger_type;
    float trigger_width;
    float trigger_height;
    float trigger_radius;
    bool enabled;
    Actor* actor;
    b2Body* body;
    std::string key;
    float rotationDegrees;
    
    Rigidbody() {
        x = 0.0f;
        y = 0.0f;
        body_type = "dynamic";
        precise = true;
        gravity_scale = 1.0f;
        density = 1.0f;
        angular_friction = 0.3f;
        rotation = 0.0f;
        has_collider = true;
        has_trigger = true;
        collider_type = "box";
        width = 1.0f;
        height = 1.0f;
        radius = 0.5f;
        friction = 0.3f;
        bounciness = 0.3f;
        trigger_type = "box";
        trigger_width = 1.0f;
        trigger_height = 1.0f;
        trigger_radius = 0.5f;
        actor = nullptr;
        body = nullptr;
        enabled = true;
        key = "";
        rotationDegrees = 0.0f;
        
    }
    
    void OnStart();
    
    b2Vec2 getPosition();
    
    float getRotation();
    
    void addForce(b2Vec2 &force);
    
    void setVelocity(b2Vec2 &force);
    
    void setPosition(b2Vec2 &pos);
    
    void setRotation(float deg);
    
    void setAngularVelocity(float deg);
    
    void setGravity(float grav);
    
    void setUpDirection(b2Vec2 dir);
    
    void setRightDirection(b2Vec2 dir);
    
    b2Vec2 getVelocity();
    
    float getAngularVelocity();
    
    float getGravity();
    
    b2Vec2 getUpDirection();
    
    b2Vec2 getRightDirection();
    
    void OnDestroy();
};

#endif /* RigidBody_hpp */
