// SoundManager.cpp
//
// Zach Elko
// 2010
//
// A simple sound manager for SDL.
//
#include "SoundManager.h"
#include "SDL/SDL_mixer.h"
#include <string>

using namespace soundmanager;

// Initialize our static variables
SoundManager* SoundManager::instance = 0;
SoundManager::AudioState SoundManager::currentState = ERROR;

void SoundManager::playMusic(const std::string& fileName)
{
   if (currentState != ERROR)
   {
      // If no music is playing, play it
      if (Mix_PlayingMusic() == 0)
      {
         // Load music
         Mix_Music* music = Mix_LoadMUS(fileName.c_str());

         if (music == NULL)
         {
            // Display a debug error, but remain in the state we were in
            // since nothing has changed.
            std::cerr << "Error loading music file: " << fileName << std::endl;
         }
         else
         {
            //Play music
            Mix_PlayMusic(music, -1);
            currentState = PLAYING;
         }
      }
      else
      {
         // If music is playing, pause it
         this->pauseMusic();
      }
   }
}

void SoundManager::pauseMusic()
{
   if (currentState != ERROR)
   {
      // If music is playing, handle the pause request
      if (Mix_PlayingMusic() == 1)
      {
         if (Mix_PausedMusic() == 1)
         {
            // If we receive a pause request and the music is already paused, resume it.
            Mix_ResumeMusic();
            currentState = PLAYING;
         }
         else
         {
            // Otherwise, pause the music
            Mix_PauseMusic();
            currentState = PAUSED;
         }
      }
   }
}

void SoundManager::stopMusic()
{
   if (currentState != ERROR)
   {
      Mix_HaltMusic();
      currentState = STOPPED;
   }
}

void SoundManager::playSound(const std::string& fileName) const
{
   if (currentState != ERROR)
   {
      // TODO: Alter this to work for mp3 as well
      Mix_Chunk* fx = Mix_LoadWAV(fileName.c_str());

      if (fx == NULL)
      {
         std::cerr << "Error loading music file: " << fileName << std::endl;
      }
      else
      {
         Mix_PlayChannel(-1, fx, 0);
      }
   }
}

void SoundManager::setVolume(float v)
{
    Mix_Volume(-1, (int)v);
}
