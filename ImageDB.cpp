//
//  ImageDB.cpp
//  game_engine
//
//  Created by Ben Huston on 2/5/25.
//

#include "ImageDB.hpp"


void ImageDB::Init(rapidjson::Document &document) {
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, 8, 8, 32, SDL_PIXELFORMAT_RGBA8888);
    Uint32 whitecolor = SDL_MapRGBA(surface->format, 255, 255, 255, 255);
    SDL_FillRect(surface, NULL, whitecolor);
    defaultParticle = SDL_CreateTextureFromSurface(Renderer::renderer, surface);
    SDL_FreeSurface(surface);
    images["defaultParticle"] = defaultParticle;
}

void ImageDB::addImage(std::string name) {
    std::string imgPath = "resources/images/" + name + ".png";
    if (!std::filesystem::exists(imgPath)) {
        std::cout << "error: missing image " << name;
        exit(0);
    }
    images[name] = IMG_LoadTexture(Renderer::renderer, imgPath.c_str());
}

void ImageDB::renderAndClearAllImages() {
    std::stable_sort(drawRequestQueue.begin(), drawRequestQueue.end(), requestComparator());
    float zoom = Renderer::zoomFactor;
    SDL_RenderSetScale(Renderer::renderer, zoom, zoom);
    
    for (auto& info : drawRequestQueue) {
        const int unit = 100;
        glm::vec2 finalPos = glm::vec2(info.x,info.y) - Renderer::camPosition;
        
        SDL_Texture* imageTexture = getTexture(info.image);
        SDL_FRect rect;
        Helper::SDL_QueryTexture(imageTexture, &rect.w, &rect.h);
        
        int flip = SDL_FLIP_NONE;
        if (info.scaleX < 0) {
            flip |= SDL_FLIP_HORIZONTAL;
        }
        if (info.scaleY < 0) {
            flip |= SDL_FLIP_VERTICAL;
        }
        
        float xScale = glm::abs(info.scaleX);
        float yScale = glm::abs(info.scaleY);
        
        rect.w *= xScale;
        rect.h *= yScale;
        
        SDL_FPoint pivot = { static_cast<float>(info.pivotX*rect.w), static_cast<float>(info.pivotY*rect.h)};
        
        glm::ivec2 camDims = Renderer::resolution;
        
        rect.x = static_cast<int>(finalPos.x * unit + camDims.x * 0.5f * (1.0f / zoom) - pivot.x);
        rect.y = static_cast<int>(finalPos.y * unit + camDims.y * 0.5f * (1.0f / zoom) - pivot.y);
        
        SDL_SetTextureColorMod(imageTexture, info.r, info.g, info.b);
        SDL_SetTextureAlphaMod(imageTexture, info.a);
        
        Helper::SDL_RenderCopyEx(0, "hold", Renderer::renderer, imageTexture, NULL, &rect, info.rotation, &pivot, static_cast<SDL_RendererFlip>(flip));
        
        SDL_RenderSetScale(Renderer::renderer, zoom, zoom);
        
        SDL_SetTextureColorMod(imageTexture, 255, 255, 255);
        SDL_SetTextureAlphaMod(imageTexture, 255);
    }
    
    SDL_RenderSetScale(Renderer::renderer, 1, 1);
    
    drawRequestQueue.clear();
}

void ImageDB::renderUIRequests() {
    std::stable_sort(drawRequestUIQueue.begin(), drawRequestUIQueue.end(), requestComparator());
    for (auto& info : drawRequestUIQueue) {
        SDL_Texture* imageTexture = getTexture(info.image);
        SDL_FRect rect;
        //Helper::SDL_QueryTexture(imageTexture, NULL, NULL, &rect.w, &rect.h);
        Helper::SDL_QueryTexture(imageTexture, &rect.w, &rect.h);
        rect.x = info.x;
        rect.y = info.y;
        SDL_FPoint pivot = { static_cast<float>(info.pivotX*rect.w), static_cast<float>(info.pivotY*rect.h)};
        
        SDL_SetTextureColorMod(imageTexture, info.r, info.g, info.b);
        SDL_SetTextureAlphaMod(imageTexture, info.a);
        
        Helper::SDL_RenderCopyEx(0, "hold", Renderer::renderer, imageTexture, NULL, &rect, 0, &pivot, SDL_FLIP_NONE);
        
        SDL_SetTextureColorMod(imageTexture, 255, 255, 255);
        SDL_SetTextureAlphaMod(imageTexture, 255);
    }
    
    drawRequestUIQueue.clear();
}

void ImageDB::renderPixels() {
    while (!drawPointQueue.empty()) {
        PointInfo info = drawPointQueue.front();
        drawPointQueue.pop();
        SDL_SetRenderDrawColor(Renderer::renderer, info.r, info.g, info.b, info.a);
        SDL_RenderDrawPointF(Renderer::renderer, info.x, info.y);
        Renderer::resetColor();
    }
}

SDL_Texture* ImageDB::getTexture(std::string image) {
    if (images.find(image) == images.end()) {
        addImage(image);
    }
    return images[image];
}

void ImageDB::DrawUI(std::string image, float x, float y) {
    drawRequestUIQueue.push_back(ImageInfo{image,x,y,0,1.0f,1.0f,0.5f,0.5f,255,255,255,255,0});
    
}

void ImageDB::DrawUIEx(std::string image, float x, float y, float r, float g, float b, float a, float sort) {
    drawRequestUIQueue.push_back(ImageInfo{ image,x,y,0,1.0f,1.0f,0.5f,0.5f,static_cast<int>(r),static_cast<int>(g),static_cast<int>(b),static_cast<int>(a),static_cast<int>(sort) });
}

void ImageDB::Draw(std::string image, float x, float y) {
    drawRequestQueue.push_back(ImageInfo{ image,x,y,0,1.0f,1.0f,0.5f,0.5f,255,255,255,255,0 });
}

void ImageDB::DrawEx(std::string image, float x, float y, float rotation, float scalex, float scaley, float pivotx, float pivoty, float r, float g, float b, float a, float sort) {
    drawRequestQueue.push_back(ImageInfo{ image,x,y,static_cast<int>(rotation),scalex,scaley,pivotx,pivoty,static_cast<int>(r),static_cast<int>(g),static_cast<int>(b),static_cast<int>(a),static_cast<int>(sort) });
}

void ImageDB::DrawPixel(float x, float y, float r, float g, float b, float a) {
    drawPointQueue.push(PointInfo{ x,y,static_cast<int>(r),static_cast<int>(g),static_cast<int>(b),static_cast<int>(a) });
}
