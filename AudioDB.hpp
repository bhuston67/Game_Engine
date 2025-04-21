//
//  AudioDB.hpp
//  game_engine
//
//  Created by Ben Huston on 2/5/25.
//

#ifndef AudioDB_hpp
#define AudioDB_hpp

#include <stdio.h>
#include <string>
#include <filesystem>
#include <unordered_map>

#include "SDL_mixer.h"
#include "rapidjson/document.h"
#include "EngineUtil.h"
#include "AudioHelper.h"

class AudioDB {
public:
    
    
    static inline std::unordered_map<std::string, Mix_Chunk*> audios;
    static inline bool hasScoreSFX;
    
    static void Init(rapidjson::Document &document);
    
    static void addAudio(std::string audioName);
    
    static void playChannel(int channel, const std::string & audio, bool loop);
    
    static void haltChannel(int channel);
    
    static void setVolume(int channel, int volume);
    
    
};

#endif /* AudioDB_hpp */
