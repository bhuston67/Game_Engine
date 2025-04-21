//
//  AudioDB.cpp
//  game_engine
//
//  Created by Ben Huston on 2/5/25.
//

#include "AudioDB.hpp"


void AudioDB::Init(rapidjson::Document &document) {
    AudioHelper::Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
    AudioHelper::Mix_AllocateChannels(50);
//    hasScoreSFX = false;
//    auto it = document.FindMember("intro_bgm");
//    if (it != document.MemberEnd()) {
//        std::string audioName = it->value.GetString();
//        std::string audioPath = "resources/audio/" + audioName + ".wav";
//        if (!std::filesystem::exists(audioPath)) {
//            audioPath = "resources/audio/" + audioName + ".ogg";
//            if (!std::filesystem::exists(audioPath)) {
//                std::cout << "error: failed to play audio clip " << audioName;
//                exit(0);
//            }
//        }
//        audios["bgm"] = AudioHelper::Mix_LoadWAV(audioPath.c_str());
//        AudioHelper::Mix_PlayChannel(0, audios["bgm"], -1);
        
    //}

}


void AudioDB::addAudio(std::string audioName) {
        std::string audioPath = "resources/audio/" + audioName + ".wav";
        if (!std::filesystem::exists(audioPath)) {
            audioPath = "resources/audio/" + audioName + ".ogg";
            if (!std::filesystem::exists(audioPath)) {
                std::cout << "error: failed to play audio clip " << audioName;
                exit(0);
            }
        }
        audios[audioName] = AudioHelper::Mix_LoadWAV(audioPath.c_str());
}

void AudioDB::playChannel(int channel, const std::string &audio, bool loop) {
    if (audios.find(audio) == audios.end()) {
        addAudio(audio);
    }
    if (loop) {
        AudioHelper::Mix_PlayChannel(channel, audios[audio], -1);
    }
    else {
        AudioHelper::Mix_PlayChannel(channel, audios[audio], 0);
    }
}

void AudioDB::haltChannel(int channel) {
    AudioHelper::Mix_HaltChannel(channel);
}

void AudioDB::setVolume(int channel, int volume) {
    AudioHelper::Mix_Volume(channel, volume);
}
