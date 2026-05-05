#include <SDL3/SDL.h>
#include <iostream>
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/AnimationComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/Utilities.h"

namespace{
  struct BlitInfo {
    SDL_Rect SourceRect;
    SDL_Rect DestRect;
  };

  BlitInfo CalculateBlitInfo(
    AnimationScalingMode Mode,
    int SurfaceW, int SurfaceH,
    float TargetX, float TargetY,
    float TargetW, float TargetH
  ) {
    BlitInfo Info;

    Info.SourceRect = {
      0, 0, SurfaceW, SurfaceH
    };

    if (Mode == AnimationScalingMode::None) {
      Info.DestRect = Utilities::Round({
        TargetX,
        TargetY,
        static_cast<float>(SurfaceW),
        static_cast<float>(SurfaceH)
      });
      return Info;
    }

    if (Mode == AnimationScalingMode::Fill) {
      Info.DestRect = Utilities::Round({
        TargetX, TargetY, TargetW, TargetH
      });
      return Info;
    }

    if (Mode == AnimationScalingMode::Contain) {
      float Scale{
        std::min(TargetW / SurfaceW,
                 TargetH / SurfaceH)};
      Info.DestRect =
        Utilities::Round({
          TargetX, TargetY,
          SurfaceW * Scale,
          SurfaceH * Scale
        });

      return Info;
    }

    if (Mode == AnimationScalingMode::Cover) {
      float Scale{
        std::max(TargetW / SurfaceW,
                 TargetH / SurfaceH)};
      if (Scale * SurfaceW > TargetW) {
        float ClipW{TargetW / Scale};
        Info.SourceRect =
          Utilities::Round({
            0, 0, ClipW,
            static_cast<float>(SurfaceH)});
      } else if (Scale * SurfaceH > TargetH) {
        float ClipH{TargetH / Scale};
        Info.SourceRect =
          Utilities::Round({
            0, 0, static_cast<float>(SurfaceW),
            ClipH});
      }
      Info.DestRect = Utilities::Round({
        TargetX, TargetY, TargetW, TargetH});
      return Info;
    }

    std::cerr <<
      "Error: Unknown Scaling Mode\n";
    return Info;
  }
}

AnimationComponent::AnimationComponent(
  Entity* Owner,
  const std::string& FilePath,
  int SpriteFrames
  ) :
  Component(Owner),
  AnimationFilePath(FilePath),
  SpriteFrames{SpriteFrames} {

  AnimationSurface = GetAssetManager()
    .LoadSurface(AnimationFilePath);
}

void AnimationComponent::Render(SDL_Surface* Surface, float DeltaTime) {
  if (!AnimationSurface || !Play) return;

  DeltaTimeCounter += DeltaTime;

  if (DeltaTimeCounter >= AnimationTime) {
    DeltaTimeCounter = 0;
    if (SpriteSheetIndex == SpriteFrames) {
      Play = false;
    } else {
      SpriteSheetIndex++;
    }
  }

    // auto [TargetX, TargetY]{GetOwnerPosition() + Offset};
    auto [TargetX, TargetY]{GetOwnerPosition()};
    float TargetW{GetWidth() * GetOwnerScale()};
    float TargetH{GetHeight() * GetOwnerScale()};

    int SurfaceW{GetSurfaceWidth()};
    int SurfaceH{GetSurfaceHeight()};

    BlitInfo Info{
      CalculateBlitInfo(
          ScaleMode,
          SurfaceW, SurfaceH,
          TargetX, TargetY,
          TargetW, TargetH
          )};

    SDL_Rect rect {SpriteSheetIndex * 128, 0, 128, 128};
    SDL_Rect DestRect {TargetX, TargetY, 128, 128};

    if (!SDL_BlitSurfaceScaled(
          AnimationSurface.get(),
          // &Info.SourceRect,
          &rect,
          Surface,
          &DestRect,
          SDL_SCALEMODE_LINEAR
          )) {
      std::cerr << "Error: Blit failed: "
        << SDL_GetError() << '\n';
    }

}

int AnimationComponent::GetSurfaceWidth() const {
  if (!AnimationSurface) {
    std::cerr << "Warning: Attempted to get "
      "width from null AnimationSurface.\n";
    return 0;
  }
  return AnimationSurface->w;
}

int AnimationComponent::GetSurfaceHeight() const {
  if (!AnimationSurface) {
    std::cerr << "Warning: Attempted to get "
      "height from null AnimationSurface.\n";
    return 0;
  }
  return AnimationSurface->h;
}

float AnimationComponent::GetWidth() const {
  // If Width has a value, return it.
  // Otherwise, return surface width.
  return Width.value_or(GetSurfaceWidth());
}

float AnimationComponent::GetHeight() const {
  // If Height has a value, return it.
  // Otherwise, return surface height.
  return Height.value_or(GetSurfaceHeight());
}
