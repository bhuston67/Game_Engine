//
//  RigidBody.cpp
//  game_engine
//
//  Created by Ben Huston on 3/20/25.
//

#include "RigidBody.hpp"

void Rigidbody::OnStart() {
    float rad = rotationDegrees * (b2_pi / 180.0f);
    if (rad != this->rotation) {
        this->rotation = rad;
    }
    
    b2BodyDef def;
    if (this->body_type == "dynamic") {
        def.type = b2_dynamicBody;
    }
    else if (this->body_type == "kinematic") {
        def.type = b2_kinematicBody;
    }
    else if (this->body_type == "static") {
        def.type = b2_staticBody;
    }
    def.gravityScale = this->gravity_scale;
    def.position = b2Vec2(this->x,this->y);
    def.bullet = this->precise;
    def.angularDamping = this->angular_friction;
    def.angle = this->rotation;
    this->body = ComponentDB::world->CreateBody(&def);
    if (!this->has_trigger && !this->has_collider) {
        b2PolygonShape shape;
        shape.SetAsBox(0.5f, 0.5f);
        b2FixtureDef fixdef;
        fixdef.shape = &shape;
        fixdef.density = this->density;
        fixdef.isSensor = true;
        body->CreateFixture(&fixdef);
        
    }
    else {
        if (this->has_collider) {
            b2FixtureDef fixdef;
            fixdef.isSensor = false;
            b2PolygonShape shape;
            b2CircleShape shape2;
            if (this->collider_type == "box") {
                
                shape.SetAsBox(0.5f * this->width, 0.5f * this->height);
                fixdef.shape = &shape;
            }
            else {
                shape2.m_radius = this->radius;
                fixdef.shape = &shape2;
            }
            fixdef.density = this->density;
            fixdef.friction = this->friction;
            fixdef.restitution = this->bounciness;
            fixdef.userData.pointer = reinterpret_cast<uintptr_t>(this->actor);
            body->CreateFixture(&fixdef);
        }
        if (this->has_trigger) {
            b2FixtureDef fixdef;
            fixdef.isSensor = true;
            b2PolygonShape shape;
            b2CircleShape shape2;
            if (this->trigger_type == "box") {
                
                shape.SetAsBox(0.5f * this->trigger_width, 0.5f * this->trigger_height);
                fixdef.shape = &shape;
            }
            else {
                shape2.m_radius = this->trigger_radius;
                fixdef.shape = &shape2;
            }
            fixdef.density = this->density;
            fixdef.userData.pointer = reinterpret_cast<uintptr_t>(this->actor);
            body->CreateFixture(&fixdef);
        }
    }
}

b2Vec2 Rigidbody::getPosition() {
    if (this->body == nullptr) {
        return b2Vec2(this->x,this->y);
    }
    else {
        return body->GetPosition();
    }
}

float Rigidbody::getRotation() {
    if (this->body == nullptr) {
        return this->rotation;
    }
    else {
        return (body->GetAngle() * (180.0f/b2_pi));
    }
}

void Rigidbody::addForce(b2Vec2 &force) {
    if (this->body != nullptr) {
        body->ApplyForceToCenter(force, true);
    }
}

void Rigidbody::setVelocity(b2Vec2 &force) {
    if (this->body != nullptr) {
        body->SetLinearVelocity(force);
    }
}

void Rigidbody::setPosition(b2Vec2 &pos) {
    if (this->body == nullptr) {
        this->x = pos.x;
        this->y = pos.y;
    }
    else {
        body->SetTransform(pos, body->GetAngle());
    }
}

void Rigidbody::setRotation(float deg) {
    float rad = deg * (b2_pi / 180.0f);
    if (this->body == nullptr) {
        this->rotation = rad;
    }
    else {
        body->SetTransform(body->GetPosition(), rad);
    }
}

void Rigidbody::setAngularVelocity(float deg) {
    float rad = deg * (b2_pi / 180.0f);
    if (this->body != nullptr) {
        body->SetAngularVelocity(rad);
    }
}

void Rigidbody::setGravity(float grav) {
    if (this->body == nullptr) {
        this->gravity_scale = grav;
    }
    else {
        body->SetGravityScale(grav);
    }
}

void Rigidbody::setUpDirection(b2Vec2 dir) {
    dir.Normalize();
    float rad = glm::atan(dir.x,-dir.y);
    if (this->body == nullptr) {
        this->rotation = rad;
    }
    else {
        body->SetTransform(body->GetPosition(), rad);
    }
}

void Rigidbody::setRightDirection(b2Vec2 dir) {
    dir.Normalize();
    float rad = glm::atan(dir.x,-dir.y) - (b2_pi / 2.0f);
    if (this->body == nullptr) {
        this->rotation = rad;
    }
    else {
        body->SetTransform(body->GetPosition(), rad);
    }
}

b2Vec2 Rigidbody::getVelocity() {
    if (this->body == nullptr) {
        return b2Vec2(0.0f,0.0f);
    }
    else {
        return body->GetLinearVelocity();
    }
}

float Rigidbody::getAngularVelocity() {
    float rad;
    if (this->body == nullptr) {
        rad = 0.0f;
    }
    else {
        rad = body->GetAngularVelocity();
    }
    return (rad * (180.0f / b2_pi));
}

float Rigidbody::getGravity() {
    if (this->body == nullptr) {
        return this->gravity_scale;
    }
    else {
        return body->GetGravityScale();
    }
}

b2Vec2 Rigidbody::getUpDirection() {
    float angle;
    if (this->body == nullptr) {
        angle = this->rotation - (b2_pi / 2.0f);
    }
    else {
        angle = body->GetAngle() - (b2_pi / 2.0f);
    }
    b2Vec2 dir = b2Vec2(glm::cos(angle),glm::sin(angle));
    dir.Normalize();
    return dir;
}

b2Vec2 Rigidbody::getRightDirection() {
    float angle;
    if (this->body == nullptr) {
        angle = this->rotation;
    }
    else {
        angle = body->GetAngle();
    }
    b2Vec2 dir = b2Vec2(glm::cos(angle),glm::sin(angle));
    dir.Normalize();
    return dir;
}


void Rigidbody::OnDestroy() {
    ComponentDB::world->DestroyBody(this->body);
}
