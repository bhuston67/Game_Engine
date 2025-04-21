//
//  TextDB.cpp
//  game_engine
//
//  Created by Ben Huston on 2/5/25.
//

#include "TextDB.hpp"


void TextDB::Init(rapidjson::Document &document, glm::ivec2 pos) {
    TTF_Init();
    textStructs = std::vector<TextWrite>();
    bool hasText = false;
//    SDL_Color textColor = {255,255,255,255};
//    auto it = document.FindMember("intro_text");
//    if (it != document.MemberEnd()) {
//        hasText = true;
//        for (auto& txt : it->value.GetArray()) {
//            introTexts.push_back(txt.GetString());
//        TextWrite hold;
//        hold.content = txt.GetString();
//        hold.font_size = 16;
//        hold.color = textColor;
//        hold.x = 25;
//        hold.y = pos[1] - 50;
//            textStructs.push_back(hold);
//        }
//    }
//    it = document.FindMember("font");
//    if (it != document.MemberEnd()) {
//        std::string fontName = it->value.GetString();
//        std::string fontPath = "resources/fonts/" + fontName + ".ttf";
//        if (!std::filesystem::exists(fontPath)) {
//            std::cout << "error: font " << fontName << " missing";
//            exit(0);
//        }
//        TTF_Init();
//        font = TTF_OpenFont(fontPath.c_str(), 16);
//    }
//    else if (hasText) {
//        std::cout << "error: text render failed. No font configured";
//        exit(0);
//    }
}

void TextDB::DrawTexts() {
//    TextInfo ti;
//    if (textMap.find(text.content) != textMap.end()) {
//        ti = textMap[text.content];
//    }
//    else {
//        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.content.c_str(), text.color);
//        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Renderer::renderer, textSurface);
//        float textW, textH;
//        Helper::SDL_QueryTexture(textTexture, &textW, &textH);
//        ti.texture = textTexture;
//        ti.width = textW;
//        ti.height = textH;
//        SDL_FreeSurface(textSurface);
//        textMap[text.content] = ti;
//    }
//    
//    SDL_FRect textRect = {text.x/zoom, text.y/zoom, ti.width/zoom, ti.height/zoom};
//    Helper::SDL_RenderCopy(Renderer::renderer, ti.texture, NULL, &textRect);
    
    while (!textsToDraw.empty()) {
            TextWrite info = textsToDraw.front();
            textsToDraw.pop();
            if (fonts[info.font].find(info.font_size) == fonts[info.font].end()) {
                std::string fontPath = "resources/fonts/" + info.font + ".ttf";
                if (!std::filesystem::exists(fontPath)) {
                    std::cout << "error: font " << info.font << " missing";
                    exit(0);
                }
                TTF_Font* font = TTF_OpenFont(fontPath.c_str(), info.font_size);
                (fonts[info.font])[info.font_size] = font;
            }
            TTF_Font* font = (fonts[info.font])[info.font_size];
            SDL_Color color = {static_cast<Uint8>(info.r),static_cast<Uint8>(info.g),static_cast<Uint8>(info.b),static_cast<Uint8>(info.a)};
            
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, info.content.c_str(), color);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Renderer::renderer, textSurface);
            float textW, textH;
            Helper::SDL_QueryTexture(textTexture, &textW, &textH);
            SDL_FreeSurface(textSurface);
            SDL_FRect textRect = {info.x, info.y, textW, textH};
            Helper::SDL_RenderCopy(Renderer::renderer, textTexture, NULL, &textRect);
        }

}

void TextDB::addTextToQueue(std::string content, float x, float y, std::string font, float fontSize, float r, float g, float b, float a) {
    textsToDraw.push(TextWrite{ content,x,y,font, static_cast<int>(fontSize),static_cast<int>(r), static_cast<int>(g), static_cast<int>(b), static_cast<int>(a) });
}

