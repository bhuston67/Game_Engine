//
//  ImageDB.hpp
//  game_engine
//
//  Created by Ben Huston on 2/5/25.
//

#ifndef ImageDB_hpp
#define ImageDB_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <filesystem>
#include <queue>

#include "SDL_image.h"
#include "rapidjson/document.h"
#include "EngineUtil.h"
#include "Helper.h"
#include "Renderer.hpp"

//struct ImageInfo {
//    SDL_Texture* texture;
//    float width;
//    float height;
//};

struct ImageInfo {
    std::string image;
    float x;
    float y;
    int rotation;
    float scaleX;
    float scaleY;
    float pivotX;
    float pivotY;
    int r;
    int g;
    int b;
    int a;
    int sort;
};

struct PointInfo {
    float x;
    float y;
    int r;
    int g;
    int b;
    int a;
};

struct requestComparator {
    bool operator()(ImageInfo a, ImageInfo b) const {
        return a.sort < b.sort;
    }
};

class ImageDB {
public:
    static inline std::vector<ImageInfo> introImages;
    static inline std::unordered_map<std::string, SDL_Texture*> images;
    static inline std::vector<ImageInfo> drawRequestQueue;
    static inline std::vector<ImageInfo> drawRequestUIQueue;
    static inline std::queue<PointInfo> drawPointQueue;
    static inline SDL_Texture* defaultParticle;
    
    
    ImageDB(rapidjson::Document &document, SDL_Renderer* render);
    
    static void Init(rapidjson::Document &document);
    
    static void addImage(std::string name);
    
    static void renderAndClearAllImages();
    
    static void renderUIRequests();
    
    static SDL_Texture* getTexture(std::string image);
    
    static void DrawUI(std::string image, float x, float y);
    
    static void DrawUIEx(std::string image, float x, float y, float r, float g, float b, float a, float sort);
    
    static void Draw(std::string image, float x, float y);
    
    static void DrawEx(std::string image, float x, float y, float rotation, float scalex, float scaley, float pivotx, float pivoty, float r, float g, float b, float a, float sort);
    
    static void DrawPixel(float x, float y, float r, float g, float b, float a);
    
    static void renderPixels();
    
};

#endif /* ImageDB_hpp */
