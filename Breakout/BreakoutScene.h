#pragma once
#include <SDL3/SDL.h>
#include "../Engine/Scene.h"
#include "../Engine/ECS/SoundComponent.h"

class BreakoutScene : public Scene {
public:
  BreakoutScene(Window& ParentWindow)
    : Scene{ParentWindow} {
      Load(1);
    }

  void HandleEvent(const SDL_Event& E) override {
    Scene::HandleEvent(E);
    using namespace UserEvents;
    if (E.type == BLOCK_DESTROYED) {
      --BlocksRemaining;
      if (BlocksRemaining == 0) {
        WinSound->Play();
        SDL_Event WonEvent{};
        WonEvent.type = UserEvents::GAME_WON;
        SDL_PushEvent(&WonEvent);
      }
    }

#ifdef ENABLE_CHEATS
    else if (E.type == SDL_EVENT_KEY_DOWN && E.key.key == SDLK_C) {
      CompleteLevel();
    }
#endif
    else if (E.type == LAUNCH_LEVEL) {
      Load(E.user.code);
    } else if (E.type == GAME_WON) {
      SetState(GameState::Won);
    } else if (E.type == COMPLETE_LEVEL) {
      CompleteLevel();
    } else if (E.type == GAME_LOST) {
      SetState(GameState::Lost);
    } else if (E.type == SDL_EVENT_KEY_DOWN && E.key.key == SDLK_R) {
      SetState(GameState::InProgress);
      Load(1);
    }
  }

  void Render(SDL_Surface* Surface) {
    const auto* Fmt{SDL_GetPixelFormatDetails(Surface->format)};

    if (GetState() == GameState::Won) {
      SDL_FillSurfaceRect(
        Surface, nullptr,
        SDL_MapRGB(Fmt, nullptr, 20, 50, 20)
      );
    } else if (GetState() == GameState::Lost) {
      SDL_FillSurfaceRect(
        Surface, nullptr,
        SDL_MapRGB(Fmt, nullptr, 50, 0, 0)
      );
    }
    Scene::Render(Surface);
  }

  void Tick(float DeltaTime) override {
    if (BlocksRemaining == 0 && !WinSound->IsPlaying()) {
      SDL_Event WonEvent{};
      WonEvent.type = UserEvents::COMPLETE_LEVEL;
      SDL_PushEvent(&WonEvent);
    }

    Scene::Tick(DeltaTime);
  }

private:
  int BlocksRemaining{0};
  int LoadedLevel{1};
  void Load(int Level);
  SoundComponent* WinSound{nullptr};
  std::unique_ptr<Entity> SoundEntity;

  void CompleteLevel() {
    if (LoadedLevel == 3) {
      SDL_Event WonEvent{};
      WonEvent.type = UserEvents::GAME_WON;
      SDL_PushEvent(&WonEvent);
    } else {
      Load(LoadedLevel + 1);
    }
  }
};
