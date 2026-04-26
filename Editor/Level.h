#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include <vector>
#include "Actor.h"

namespace Editor {
class Scene;
using ActorPtr = std::unique_ptr<Actor>;
using ActorPtrs = std::vector<ActorPtr>;

class Level {
 public:
  Level(Scene& ParentScene)
  : ParentScene{ParentScene} {
    Load();
  }

  void HandleEvent(const SDL_Event& E);
  void HandleDrop(Actor* DragActor);
  void Tick(float DeltaTime);
  void Render(SDL_Surface* Surface);
  bool HasMouseFocus() const;
  void AddToLevel(ActorPtr NewActor);
  SDL_Point SnapToGridPosition(int x, int y);
  void DeleteAtPosition(
    int x, int y, const Actor* Unless
  );
  void Load();
  void Save();
  void SaveAndPlay();

 private:
  Scene& ParentScene;
  ActorPtrs Actors;
  Actor* SelectedActor{nullptr};
  int LoadedLevel{1};

  SDL_Rect Rect{
    0, 0,
    Config::Editor::LEVEL_WIDTH,
    Config::Editor::LEVEL_HEIGHT
  };
};
}
