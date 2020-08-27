#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>

namespace Bengine{

class SoundEffect{
public:
    friend class AudioEngine;
    SoundEffect();
    SoundEffect(Mix_Chunk* chunk, int volumeVal = MIX_MAX_VOLUME / 2);

    ///Plays the effect file
    ///@param loops: if loops == -1, loops forever.
    ///Otherwise, plays it loops+1 times.
    void play(int loops = 0);
    void setVolume(int volumeVal); ///< @param volumeVal ranges from 0-128

private:
    Mix_Chunk* m_chunk = nullptr;
    //int m_Volume = MIX_MAX_VOLUME;

};

class Music{
public:
    friend class AudioEngine;

    ///Plays the audio file
    ///@param loops: if loops == -1, loops forever.
    ///Otherwise, plays it loops times.
    void play(int loops = 1);

    ///These functions will affect whatever music is being played.
    ///Only one music file can be played at a time
    static void pause();
    static void stop();
    static void resume();

private:
    Mix_Music* m_music = nullptr;

};

class AudioEngine{
    public:
        AudioEngine();
        virtual ~AudioEngine();

        void init();
        void destroy();

        SoundEffect loadSoundEffect(const std::string& filePath, int volumeVal = MIX_MAX_VOLUME / 2);
        Music loadMusic(const std::string& filePath);

    protected:

    private:

        std::map<std::string, Mix_Chunk*> m_effectMap;
        std::map<std::string, Mix_Music*> m_musicMap;
        bool m_isInitialized = false;
};

}
#endif // AUDIOENGINE_H
