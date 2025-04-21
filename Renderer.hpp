//
//  Renderer.hpp
//  game_engine
//
//  Created by Ben Huston on 2/4/25.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <stdio.h>

#include "Helper.h"
#include "ImageDB.hpp"
#include "glm/glm.hpp"
#include "rapidjson/document.h"
#include "EngineUtil.h"
#include "TextDB.hpp"
#include "SDL_ttf.h"
#include "AudioDB.hpp"
#include "Scene.hpp"
#include "Input.hpp"

class Scene;
struct Actor;


class Renderer {
public:
    static inline SDL_Window* window;
    static inline SDL_Renderer* renderer;
    static inline Scene* sceneDB;
    static inline glm::vec2 resolution;
    static inline glm::vec2 camCenter;
    static inline glm::vec2 camPosition;
    static inline glm::vec2 camOffset;
    //static inline SDL_FRect rect;
    static inline SDL_Color textColor;
    static inline int pictureCount;
    static inline int textCount;
    static inline int red;
    static inline int green;
    static inline int blue;
    static inline bool intro;
    static inline bool gamePlayAudioTracker;
    static inline bool endMusic;
    static inline float zoomFactor;
    static inline float sceneUnit;
    static inline float camEase;
    
    
    static void Init(bool hasCustomResolution);
    
    static void createwindow(const char* title);
    
    static void createRenderer();
    
    static void setSceneDB(Scene *scn);
    
    static void render();
    
    static void resetColor();
    
    static void setCamera(float x, float y);
    
    static float getCameraX();
    
    static float getCameraY();
    
    static void setZoom(float z);
    
    static float getZoom();
    
    
private:
};

#endif /* Renderer_hpp */
