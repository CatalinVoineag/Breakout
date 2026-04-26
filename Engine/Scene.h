#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <memory>
#include "ECS/Entity.h"
#include "AssetManager.h"
#include "Window.h"
#include "../Config.h"

using EntityPtr = std::unique_ptr<Entity>;
using EntityPtrs = std::vector<EntityPtr>;

class Scene {
public:
  Scene(Window& ParentWindow);

  virtual void HandleEvent(const SDL_Event& E);
  virtual void Tick(float DeltaTime);
  virtual void Render(SDL_Surface* Surface);

  AssetManager& GetAssetManager();
  Window& GetWindow() const;

  float GetWidth() const;
  float GetHeight() const;

#ifdef DRAW_DEBUG_HELPERS
  SDL_Surface* Trajectories{nullptr};
#endif

  static inline int PIXELS_PER_METER{50};
  static inline Vec2 GRAVITY{
    0, 9.8f * PIXELS_PER_METER
  };

protected:
  EntityPtrs Entities;
  Window& ParentWindow;
  AssetManager Assets;

private:
  void CheckCollisions();
};
