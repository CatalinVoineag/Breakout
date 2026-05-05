#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <optional>
#include <SDL3/SDL.h>
#include "Component.h"
#include "Config.h"
#include "../AssetManager.h"

// Put this somewhere else?
enum class AnimationScalingMode {
  None, Fill, Contain, Cover
};

class AnimationComponent : public Component {
  public:
    using Component::Component;
    AnimationComponent(Entity* Owner, const std::string &FilePath, int SpriteFrames);

    void Render(SDL_Surface* Surface, float DeltaTime) override;

    int GetSurfaceWidth() const;
    int GetSurfaceHeight() const;
    float GetWidth() const;
    float GetHeight() const;
    // void SetScalingMode(ScalingMode Mode);

    bool Play{false};

  private:
  std::string AnimationFilePath;
  std::shared_ptr<SDL_Surface> AnimationSurface{nullptr};
  std::optional<float> Width{std::nullopt};
  std::optional<float> Height{std::nullopt};
  AnimationScalingMode ScaleMode{AnimationScalingMode::Fill};
  int SpriteSheetIndex{0};
  int SpriteFrames{0};
  float AnimationTime{.1f};
  float DeltaTimeCounter{0};
};
