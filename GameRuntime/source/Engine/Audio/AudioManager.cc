#include <pch.hh>
#include "AudioManager.hh"


AudioManager::AudioManager() {}

AudioManager::~AudioManager() {}

void AudioManager::PlayMusic(const std::string& id) {}

bool AudioManager::LoadMusicTrack(const std::string& id, const std::string& filepath) { return false; }

bool AudioManager::MusicPlaying() { return false; }


// AudioManager::~AudioManager() {
//     if (MusicPlaying()) {
//         SDL_CloseAudioDevice(mMusicAudioDevice);
//     }
//     for (auto track : mMusicTracks) {
//         delete track.second;
//     }
//     mMusicTracks.clear();
// }


// void AudioManager::PlayMusic(const std::string& id) {
//     auto mit = mMusicTracks.find(id);
//     if (mit == mMusicTracks.end()) {
//         GAME_ERROR("Attempted to play non-existing track! {}", id);
//     }

//     if (SDL_GetAudioDeviceStatus(mMusicAudioDevice) == SDL_AUDIO_STOPPED) {
//         mMusicAudioDevice = SDL_OpenAudioDevice(NULL, 0, mit->second->GetSpec(), NULL, 0);
//         SDL_PauseAudioDevice(mMusicAudioDevice, 0);
//     } else {
//         SDL_CloseAudioDevice(mMusicAudioDevice);
//         PlayMusic(id);
//     }
// }

// bool AudioManager::LoadMusicTrack(const std::string& id, const std::string& filepath) {
//     auto mit = mMusicTracks.find(id);
//     if (mit == mMusicTracks.end()) {
//         MusicTrack* track = new MusicTrack();
//         bool r = track->LoadTrack(filepath);
//         if (r) {
//             mMusicTracks[id] = track;
//             return true;
//         } else {
//             return false;
//         }
//     } else {
//         return false;
//     }
// }


// bool AudioManager::MusicPlaying() {
//     switch(SDL_GetAudioDeviceStatus(mMusicAudioDevice)) {
//         case SDL_AUDIO_STOPPED: return false;
//         case SDL_AUDIO_PLAYING: return true;
//         case SDL_AUDIO_PAUSED: return true;
//         default: return false;
//     }
// }
