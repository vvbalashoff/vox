// SoundManager.h
//
// Zach Elko
// 2010
//
// A simple sound manager for SDL.
//
#ifndef _SOUNDMANAGER_H
#define	_SOUNDMANAGER_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <string>
#include <iostream>

namespace soundmanager
{

   class SoundManager
   {
   public:

      // Singleton pattern

      static SoundManager* getInstance()
      {
         if (instance == 0)
         {
            instance = new SoundManager;
            SoundManager::initAudioDevice();
         }
         return instance;
      }

      void playMusic(const std::string& fileName);
      void pauseMusic();
      void stopMusic();
      void playSound(const std::string& fileName) const;
      void setVolume(float v);

      inline bool isPaused() const
      {
         return currentState == PAUSED;
      }

      inline bool isStopped() const
      {
         return currentState == STOPPED;
      }

      inline bool isPlaying() const
      {
         return currentState == PLAYING;
      }

      inline bool inErrorState() const
      {
         return currentState == ERROR;
      }

   private:

      static SoundManager* instance;

      enum AudioState
      {
         ERROR = 0,
         WAITING,
         PAUSED,
         STOPPED,
         PLAYING
      };
      static AudioState currentState;

      static void initAudioDevice()
      {
         if (SDL_Init(SDL_INIT_AUDIO) != -1)
         {
            if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
            {
               std::cerr << "Error initializing audio device...\n";
               currentState = ERROR;
            }
            else
            {
               currentState = WAITING;
            }
         }
         else
         {
            std::cerr << "Error initializing SDL audio subsystem...\n";
            currentState = ERROR;
         }
      }

      // All of these are private due to the Singleton pattern

      SoundManager()
      {
      }

      SoundManager(const SoundManager&)
      {
      }

      SoundManager & operator=(const SoundManager&)
      {
          return *this;
      }

      ~SoundManager()
      {
         Mix_CloseAudio();
      }
   };

}
#endif
