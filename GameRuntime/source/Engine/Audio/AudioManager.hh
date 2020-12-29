#pragma once

#include <map>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    void PlayMusic(const std::string& id);
    bool LoadMusicTrack(const std::string& id, const std::string& filepath);
    bool MusicPlaying();
private:
    uint32_t mMusicAudioDevice;
};