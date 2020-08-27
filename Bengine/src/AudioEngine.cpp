#include "AudioEngine.h"

#include "BengineErrors.h"
#include <iostream>

namespace Bengine{

SoundEffect::SoundEffect(){}

SoundEffect::SoundEffect(Mix_Chunk* chunk, int volumeVal){
    m_chunk = chunk;
    if(volumeVal < 0){
        std::cout << "Negative Volume Value passed to Sound Effect.\n";
    }
    Mix_VolumeChunk(m_chunk, volumeVal);
}

void SoundEffect::play(int loops /* = 0 */ ){
    if( Mix_PlayChannel(-1, m_chunk, loops) == -1 ){
        if( Mix_PlayChannel(0, m_chunk, loops) == -1 ){
            fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
        }
    }
}

void SoundEffect::setVolume(int volumeVal){
    if(volumeVal < 0){
        std::cout << "Negative Volume Value passed to Sound Effect\n.";
    }
    if(m_chunk != nullptr){
        Mix_VolumeChunk(m_chunk, volumeVal);
    }else{
        fatalError("Tried to set volume to uninitialized sound effect.\n");
    }
}

void Music::play(int loops /* = 1 */ ){
    if( Mix_PlayMusic(m_music, loops) == -1 ){
        fatalError("Mix_PlayMusic error: " + std::string(Mix_GetError()));
    }
}

void Music::pause(){
    Mix_PausedMusic();
}

void Music::stop(){
    Mix_HaltMusic();
}

void Music::resume(){
    Mix_ResumeMusic();
}

AudioEngine::AudioEngine(){}

AudioEngine::~AudioEngine(){
    destroy();
}

void AudioEngine::init(){//Must be called after SDL has been initialized

    if(m_isInitialized){
        fatalError("Tried to initialize Audio Engine twice.");
    }

    ///Parameter can be a bitwise operation of flags MIX_INIT_FAC, MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
    if( Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1 ){
        fatalError("Mix_Init error: " + std::string(Mix_GetError()));
    }
    if( Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1 ){
        fatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
    }
    m_isInitialized = true;
}

void AudioEngine::destroy(){
    if(m_isInitialized){
        m_isInitialized = false;

        for(auto& it : m_effectMap){
            Mix_FreeChunk(it.second);
        }

        for(auto& it : m_musicMap){
            Mix_FreeMusic(it.second);
        }

        m_effectMap.clear();
        m_musicMap.clear();

        Mix_CloseAudio();
        Mix_Quit();
    }
}

SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath, int volumeVal /*= MIX_MAX_VOLUME / 2 */ ){
    //Search for the audio file
    auto it = m_effectMap.find(filePath);

    //SoundEffect effect;

    if(it == m_effectMap.end()){
        ///Failed to find, must load
        Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
        if(chunk == nullptr){///Error check
            fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
        }

        m_effectMap[filePath] = chunk;
        //effect.m_chunk = chunk;
        return SoundEffect(chunk, volumeVal);

    }else{
        //effect.m_chunk = it->second;
        return SoundEffect(it->second, volumeVal);
    }

    //return effect;
}

Music AudioEngine::loadMusic(const std::string& filePath){
    //Search for the audio file
    auto it = m_musicMap.find(filePath);

    Music music;

    if(it == m_musicMap.end()){
        //Failed to find, must load
        Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
        if(mixMusic == nullptr){//Error check
            fatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
        }

        m_musicMap[filePath] = mixMusic;
        music.m_music = mixMusic;

    }else{
        music.m_music = it->second;
    }

    return music;
}

}
