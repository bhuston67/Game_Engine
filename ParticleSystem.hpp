//
//  ParticleSystem.hpp
//  game_engine
//
//  Created by Ben Huston on 4/2/25.
//

#ifndef ParticleSystem_hpp
#define ParticleSystem_hpp

#include <stdio.h>
#include <vector>
#include <queue>
#include <set>

#include "glm/glm.hpp"
#include "Helper.h"
#include "ImageDB.hpp"

struct Particle {
    float x;
    float y;
    float scale;
    float rotation;
};

class ParticleSystem {
public:
    bool enabled;
    int particleSlotCount;
    float x;
    float y;
    std::vector<Particle> activeParticles;
    int currentFrame;
    int framesBetweenBursts;
    int burstQuantity;
    float startScaleMin;
    float startScaleMax;
    float rotationMin;
    float rotationMax;
    float radiusMin;
    float radiusMax;
    float angleMin;
    float angleMax;
    float speedMin;
    float speedMax;
    float angSpeedMin;
    float angSpeedMax;
    int r;
    int g;
    int b;
    int a;
    int sort;
    int duration;
    float xgravity;
    float ygravity;
    float drag;
    float angDrag;
    float endScale;
    int endr;
    int endg;
    int endb;
    int enda;
    std::string image;
    RandomEngine angleDist;
    RandomEngine radiusDist;
    RandomEngine rotationDist;
    RandomEngine scaleDist;
    RandomEngine speedDist;
    RandomEngine angSpeedDist;
    std::queue<int> freeList;
    std::vector<float> xs;
    std::vector<float> ys;
    std::vector<float> scales;
    std::vector<float> startScales;
    std::vector<float> rotations;
    std::vector<bool> isActive;
    std::vector<int> startFrame;
    std::vector<float> xvels;
    std::vector<float> yvels;
    std::vector<float> angvels;
    std::vector<int> reds;
    std::vector<int> greens;
    std::vector<int> blues;
    std::vector<int> as;
    std::vector<float> progresses;
    std::set<int> activeIndex;
    
    bool play;
    
    
    ParticleSystem() : enabled(true), particleSlotCount(0), x(0.0f), y(0.0f), currentFrame(0), framesBetweenBursts(1), burstQuantity(1), startScaleMin(1.0f), startScaleMax(1.0f), rotationMin(0.0f), rotationMax(0.0f), radiusMin(0.0f), radiusMax(0.5f), angleMin(0.0f), angleMax(360.0f), speedMin(0.0f), speedMax(0.0f), angSpeedMin(0.0f), angSpeedMax(0.0f), r(255), g(255), b(255), a(255), sort(9999), image(""), duration(300), xgravity(0.0f), ygravity(0.0f), drag(1.0f), angDrag(1.0f), endScale(-1.0f), endr(-1), endb(-1), endg(-1), enda(-1), play(true)  {}
    
    void OnUpdate();
    
    void OnStart();
    
    void Play();
    
    void stop();
    
    void burst();
    
    
    
};

#endif /* ParticleSystem_hpp */
