#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include "../Engine/Scene.h"
#include "../Engine/ECS/SoundComponent.h"
#include "../Engine/Vec2.h"
#include "Ball.h"

class BreakoutScene : public Scene {
public:
  BreakoutScene(Window& ParentWindow)
    : Scene{ParentWindow} {
      Load(1);
    }

  void HandleEvent(const SDL_Event& E) override {
    std::cout << "Blocks Remining " << BlocksRemaining << "\n";
    std::cout << "BALLS " << Balls << "\n";

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
    else if (E.type == ADD_BALL) {
      Vec2 Position = {
        static_cast<float>(reinterpret_cast<uintptr_t>(E.user.data1)),
        static_cast<float>(reinterpret_cast<uintptr_t>(E.user.data2))
      };
      Entities.emplace_back(std::make_unique<Ball>(
        *this, false, Entities.size(), Position
      ));
      Balls++;
    } else if (E.type == REMOVE_BALL) {
      Uint8 Index = static_cast<int>(reinterpret_cast<uintptr_t>(E.user.data1));
      RemoveEntity(Index);
      Balls--;
    } else if (E.type == LAUNCH_LEVEL) {
      Balls = 0;
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

  void Render(SDL_Surface* Surface, float DeltaTime) {
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
    Scene::Render(Surface, DeltaTime);
  }

  void Tick(float DeltaTime) override {
    if (BlocksRemaining == 0 && !WinSound->IsPlaying()) {
      SDL_Event WonEvent{};
      WonEvent.type = UserEvents::COMPLETE_LEVEL;
      SDL_PushEvent(&WonEvent);
    }

    Scene::Tick(DeltaTime);
  }

  int GetBalls() const { return Balls; }

private:
  int Balls{0};
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
