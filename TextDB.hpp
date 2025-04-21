//
//  TextDB.hpp
//  game_engine
//
//  Created by Ben Huston on 2/5/25.
//

#ifndef TextDB_hpp
#define TextDB_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <filesystem>
#include <queue>
#include <unordered_map>


#include "SDL_image.h"
#include "Helper.h"
#include "rapidjson/document.h"
#include "EngineUtil.h"
#include "SDL_ttf.h"
#include "glm/glm.hpp"
#include "Renderer.hpp"

struct TextWrite{
    std::string content;
    float x;
    float y;
    std::string font;
    int font_size;
    int r;
    int g;
    int b;
    int a;
};


struct TextInfo {
    SDL_Texture* texture;
    float width;
    float height;
};

class TextDB {
public:
    
    static inline std::vector<std::string> introTexts;
    static inline std::vector<TextWrite> textStructs;
    static inline TTF_Font* font;
    static inline std::unordered_map<std::string, TextInfo> textMap;
    static inline std::queue<TextWrite> textsToDraw;
    static inline std::unordered_map<std::string, std::unordered_map<int, TTF_Font*>> fonts;

    
    
    TextDB(rapidjson::Document &document, SDL_Renderer* render, glm::ivec2 pos);
    
    static void Init(rapidjson::Document &document,glm::ivec2 pos);
    
    static void addTextToQueue(std::string content, float x, float y, std::string font, float fontSize, float r, float g, float b, float a);
        
    static void DrawTexts();

    
    
};

#endif /* TextDB_hpp */
