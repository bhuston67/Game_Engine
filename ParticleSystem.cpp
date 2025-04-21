//
//  ParticleSystem.cpp
//  game_engine
//
//  Created by Ben Huston on 4/2/25.
//

#include "ParticleSystem.hpp"


void ParticleSystem::OnUpdate() {
    if (play) {
        if (currentFrame % framesBetweenBursts == 0) {
            for (int i = 0; i < burstQuantity; i++) {
                float angle = glm::radians(angleDist.Sample());
                float radius = radiusDist.Sample();
                float cos = glm::cos(angle);
                float sin = glm::sin(angle);
                float newx = x + cos * radius;
                float newy = y + sin * radius;
                float speed = speedDist.Sample();
                float xvel = cos * speed;
                float yvel = sin * speed;
                if (freeList.empty()) {
                    activeIndex.insert(particleSlotCount);
                    particleSlotCount++;
                    xs.push_back(newx);
                    ys.push_back(newy);
                    rotations.push_back(rotationDist.Sample());
                    scales.push_back(scaleDist.Sample());
                    startScales.push_back(scales[particleSlotCount-1]);
                    isActive.push_back(true);
                    startFrame.push_back(currentFrame);
                    xvels.push_back(xvel);
                    yvels.push_back(yvel);
                    angvels.push_back(angSpeedDist.Sample());
                    reds.push_back(r);
                    greens.push_back(g);
                    blues.push_back(b);
                    as.push_back(a);
                    progresses.push_back(0.0f);
                }
                else {
                    int index = freeList.front();
                    activeIndex.insert(index);
                    freeList.pop();
                    xs[index] = newx;
                    ys[index] = newy;
                    rotations[index] = rotationDist.Sample();
                    scales[index] = scaleDist.Sample();
                    startScales[index] = scales[index];
                    isActive[index] = true;
                    startFrame[index] = currentFrame;
                    xvels[index] = xvel;
                    yvels[index] = yvel;
                    angvels[index] = angSpeedDist.Sample();
                    reds[index] = r;
                    greens[index] = g;
                    blues[index] = b;
                    as[index] = a;
                    progresses[index] = 0.0f;
                }
                
            }
        }
    }
    
    std::vector<int> toRemove;
    for (int i : activeIndex) {
        
        int timeAlive = currentFrame - startFrame[i];
        if (timeAlive >= duration) {
            //activeIndex.erase(i);
            toRemove.push_back(i);
            freeList.push(i);
        }
    }
    for (int i : toRemove) {
        activeIndex.erase(i);
    }
    for (int i : activeIndex) {
        xvels[i] += xgravity;
        xvels[i] *= drag;
    }
    for (int i : activeIndex) {
        yvels[i] += ygravity;
        yvels[i] *= drag;
    }
    for (int i : activeIndex) {
        angvels[i] *= angDrag;
    }
    
    for (int i : activeIndex) {
        xs[i] += xvels[i];
    }
    for (int i : activeIndex) {
        ys[i] += yvels[i];
    }
    for (int i : activeIndex) {
        rotations[i] += angvels[i];
    }
    for (int i : activeIndex) {
        progresses[i] = static_cast<float>(currentFrame - startFrame[i]) / duration;
    }
        if (endScale != -1.0f) {
            for (int i : activeIndex) {
                scales[i] = glm::mix(startScales[i], endScale, progresses[i]);
            }
        }
        if (endr != -1) {
            for (int i : activeIndex) {
                reds[i] = glm::mix(r, endr, progresses[i]);
            }
        }
        if (endb != -1) {
            for (int i : activeIndex) {
                blues[i] = glm::mix(b, endb, progresses[i]);
            }
        }
        if (endg != -1) {
            for (int i : activeIndex) {
                greens[i] = glm::mix(g, endg, progresses[i]);
            }
        }
        if (enda != -1) {
            for (int i : activeIndex) {
                as[i] = glm::mix(a, enda, progresses[i]);
            }
        }
    for (int i : activeIndex) {
        if (isActive[i]) {
            ImageDB::drawRequestQueue.push_back(ImageInfo{ image,xs[i],ys[i],static_cast<int>(rotations[i]),scales[i],scales[i],0.5f,0.5f,reds[i],greens[i],blues[i],as[i],sort});
        }
    }
    
        
        
//        for (int i = 0; i <particleSlotCount; i++) {
//            if (!isActive[i]) {
//                continue;
//            }
//            int timeAlive = currentFrame - startFrame[i];
//            if (timeAlive >= duration) {
//                isActive[i] = false;
//                freeList.push(i);
//                continue;
//            }
//            xvels[i] += xgravity;
//            xvels[i] *= drag;
//            yvels[i] += ygravity;
//            yvels[i] *= drag;
//            
//            angvels[i] *= angDrag;
//            
//            xs[i] += xvels[i];
//            ys[i] += yvels[i];
//            
//            rotations[i] += angvels[i];
//            
//            float progress = static_cast<float>(timeAlive) / duration;
//            if (endScale != -1.0f) {
//                scales[i] = glm::mix(startScales[i], endScale, progress);
//            }
//            if (endr != -1) {
//                reds[i] = glm::mix(r, endr, progress);
//            }
//            if (endb != -1) {
//                blues[i] = glm::mix(b, endb, progress);
//            }
//            if (endg != -1) {
//                greens[i] = glm::mix(g, endg, progress);
//            }
//            if (enda != -1) {
//                as[i] = glm::mix(a, enda, progress);
//            }
//            
//            if (isActive[i]) {
//                ImageDB::drawRequestQueue.push_back(ImageInfo{ image,xs[i],ys[i],static_cast<int>(rotations[i]),scales[i],scales[i],0.5f,0.5f,reds[i],greens[i],blues[i],as[i],sort});
//            }
//        }
        
        currentFrame++;
}


void ParticleSystem::OnStart() {
    radiusDist = RandomEngine(radiusMin, radiusMax, 404);
    angleDist = RandomEngine(angleMin, angleMax, 298);
    rotationDist = RandomEngine(rotationMin, rotationMax, 440);
    scaleDist = RandomEngine(startScaleMin, startScaleMax, 494);
    speedDist = RandomEngine(speedMin, speedMax, 498);
    angSpeedDist = RandomEngine(angSpeedMin, angSpeedMax, 305);
    if (image == "") {
        image = "defaultParticle";
    }
}

void ParticleSystem::Play() {
    play = true;
}

void ParticleSystem::stop() {
    play = false;
}

void ParticleSystem::burst() {
    for (int i = 0; i < burstQuantity; i++) {
        float angle = glm::radians(angleDist.Sample());
        float radius = radiusDist.Sample();
        float cos = glm::cos(angle);
        float sin = glm::sin(angle);
        float newx = x + cos * radius;
        float newy = y + sin * radius;
        float speed = speedDist.Sample();
        float xvel = cos * speed;
        float yvel = sin * speed;
        if (freeList.empty()) {
            activeIndex.insert(particleSlotCount);
            particleSlotCount++;
            xs.push_back(newx);
            ys.push_back(newy);
            rotations.push_back(rotationDist.Sample());
            scales.push_back(scaleDist.Sample());
            startScales.push_back(scales[particleSlotCount-1]);
            isActive.push_back(true);
            startFrame.push_back(currentFrame);
            xvels.push_back(xvel);
            yvels.push_back(yvel);
            angvels.push_back(angSpeedDist.Sample());
            reds.push_back(r);
            greens.push_back(g);
            blues.push_back(b);
            as.push_back(a);
            progresses.push_back(0.0f);
        }
        else {
            int index = freeList.front();
            activeIndex.insert(index);
            freeList.pop();
            xs[index] = newx;
            ys[index] = newy;
            rotations[index] = rotationDist.Sample();
            scales[index] = scaleDist.Sample();
            startScales[index] = scales[index];
            isActive[index] = true;
            startFrame[index] = currentFrame;
            xvels[index] = xvel;
            yvels[index] = yvel;
            angvels[index] = angSpeedDist.Sample();
            reds[index] = r;
            greens[index] = g;
            blues[index] = b;
            as[index] = a;
            progresses[index] = 0.0f;
        }
        
    }
}
