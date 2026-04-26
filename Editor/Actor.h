#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include "Image.h"
#include "../Config.h"

enum class ActorLocation { Level, Menu };

namespace Editor {
class Scene;
class Actor {
 public:
  Actor(
    Scene& ParentScene,
    const SDL_Rect& Rect,
    Image& Image
  ) : ParentScene{ParentScene},
      Rect{Rect},
      Art{Image}
  {}

  bool HasMouseFocus() const;
  virtual bool HandleEvent(const SDL_Event& E);

  void Tick(float DeltaTime) {}

  void Render(SDL_Surface* Surface) {
    if (isVisible) {
      Art.Render(Surface, Rect);
    }
  }

  const SDL_Rect& GetRect() const {
    return Rect;
  }

  const SDL_Point& GetDragOffset() const {
    return DragOffset;
  }

  const Image& GetArt() const {
    return Art;
  }

  SDL_Point GetPosition() const {
    return {Rect.x, Rect.y};
  }

  void SetPosition(int x, int y) {
    Rect.x = x;
    Rect.y = y;
  }

  virtual std::unique_ptr<Actor> Clone() const {
    return std::make_unique<Actor>(*this);
  }

  ActorLocation GetLocation() const {
    return Location;
  }

  void SetLocation(ActorLocation NewLocation) {
    Location = NewLocation;
  }

  bool GetIsVisible() const {
    return isVisible;
  }

  void SetIsVisible(bool NewVisibility) {
    isVisible = NewVisibility;
  }

  virtual Config::ActorType GetActorType() const {
    return Config::ActorType::Actor;
  }

  virtual void Serialize(SDL_IOStream* Handle) const {
    using namespace Config::Editor;
    Uint8 ActorType{static_cast<Uint8>(GetActorType())};
    SDL_WriteU8(Handle, ActorType);

    Uint8 GridRow = Uint8((Rect.y / VERTICAL_GRID_SNAP));
    SDL_WriteU8(Handle, GridRow);

    Uint8 GridCol = Uint8((Rect.x / HORIZONTAL_GRID_SNAP));
    SDL_WriteU8(Handle, GridCol);
  }

  static SDL_Rect GeneratePositionRectangle(
    SDL_IOStream* Handle, int Width, int Height
  ) {
    using namespace Config::Editor;
    Uint8 GridRow{0}, GridCol{0};
    SDL_ReadU8(Handle, &GridRow);
    SDL_ReadU8(Handle, &GridCol);
    SDL_Rect R{0, 0, Width, Height};
    R.x = GridCol * HORIZONTAL_GRID_SNAP;
    R.y = GridRow * VERTICAL_GRID_SNAP;
    return R;
  }

protected:
  Scene& ParentScene;
  SDL_Rect Rect;
  Image& Art;
  SDL_Point DragOffset{0, 0};
  ActorLocation Location{ActorLocation::Menu};
  bool isVisible{true};
};
}
