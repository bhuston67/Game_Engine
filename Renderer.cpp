//
//  Renderer.cpp
//  game_engine
//
//  Created by Ben Huston on 2/4/25.
//

#include "Renderer.hpp"


void Renderer::Init(bool hasCustomResolution) {
    window = nullptr;
    renderer = nullptr;
    sceneDB = nullptr;
    resolution = glm::vec2(640,360);
    camPosition = glm::vec2(0,0);
    camOffset = glm::vec2(0,0);
    red = 255;
    green = 255;
    blue = 255;
    intro = false;
    gamePlayAudioTracker = false;
    endMusic = false;
    zoomFactor = 1.0f;
    sceneUnit = 100.0f;
    camEase = 1.0f;
        rapidjson::Document document;
        EngineUtil util;
        if (hasCustomResolution) {
            util.ReadJsonFile("resources/rendering.config", document);
            auto it = document.FindMember("x_resolution");
            if (it != document.MemberEnd()) {
                resolution[0] = it->value.GetInt();
            }
            it = document.FindMember("y_resolution");
            if (it != document.MemberEnd()) {
                resolution[1] = it->value.GetInt();
            }
            it = document.FindMember("clear_color_r");
            if (it != document.MemberEnd()) {
                red = it->value.GetInt();
            }
            it = document.FindMember("clear_color_g");
            if (it != document.MemberEnd()) {
                green = it->value.GetInt();
            }
            it = document.FindMember("clear_color_b");
            if (it != document.MemberEnd()) {
                blue = it->value.GetInt();
            }
            it = document.FindMember("cam_offset_x");
            if (it != document.MemberEnd()) {
                camOffset[0] = it->value.GetFloat();
            }
            it = document.FindMember("cam_offset_y");
            if (it != document.MemberEnd()) {
                camOffset[1] = it->value.GetFloat();
            }
            it = document.FindMember("zoom_factor");
            if (it != document.MemberEnd()) {
                zoomFactor = it->value.GetFloat();
            }
            it = document.FindMember("cam_ease_factor");
            if (it != document.MemberEnd()) {
                camEase = it->value.GetFloat();
            }
            
        }
    camCenter = glm::vec2(resolution[0]*0.5f,resolution[1]*0.5f)/zoomFactor;
}

void Renderer::createwindow(const char* title) {
    window = Helper::SDL_CreateWindow(title, 100, 100, resolution[0], resolution[1], 0);
}

void Renderer::createRenderer() {
    renderer = Helper::SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);
    SDL_RenderSetScale(renderer, zoomFactor, zoomFactor);
    SDL_SetRenderDrawBlendMode(Renderer::renderer, SDL_BLENDMODE_BLEND);
}

void Renderer::render(){
    SDL_RenderClear(renderer);
    ImageDB::renderAndClearAllImages();
    ImageDB::renderUIRequests();
    TextDB::DrawTexts();
    ImageDB::renderPixels();
    
    Helper::SDL_RenderPresent(renderer);
}

void Renderer::setSceneDB(Scene *scn) {
    sceneDB = scn;
}

void Renderer::resetColor() {
    SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
}

void Renderer::setCamera(float x, float y) {
    camPosition.x = x;
    camPosition.y = y;
}

float Renderer::getCameraX() {
    return camPosition.x;
}

float Renderer::getCameraY() {
    return camPosition.y;
}

void Renderer::setZoom(float z) {
    zoomFactor = z;
}

float Renderer::getZoom() {
    return zoomFactor;
}
