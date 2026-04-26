#pragma once
#include <SDL3/SDL.h>
#include "Actor.h"

namespace Editor{
class Scene;

class ActorTooltip {
public:
  ActorTooltip(Scene& ParentScene);
  ~ActorTooltip();
  ActorTooltip(const ActorTooltip&) = delete;
  ActorTooltip& operator=(const ActorTooltip&) = delete;
  void Render();
  void Tick(float DeltaTime);
  void PositionWindow();
  void HandleEvent(const SDL_Event& E);
  void SetIsVisible(bool NewVisibility);

  SDL_Surface* GetSurface() const {
    return SDL_GetWindowSurface(SDLWindow);
  }

private:
  bool isVisible{false};
  SDL_Window* SDLWindow{nullptr};
  Actor* DragActor{nullptr};
  Scene& ParentScene;
  SDL_Cursor* DenyCursor{nullptr};
};
}
