#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include "../Config.h"

namespace Editor{
class Image {
 public:
  Image() = default;
  Image(const std::string& Path)
  : ImageSurface{IMG_Load(Path.c_str())
  } {
    CheckSDLError("Loading Image");
  }

  void Render(
    SDL_Surface* Surface, SDL_Rect Rect
  ) const {
    SDL_BlitSurfaceScaled(
      ImageSurface, nullptr, Surface, &Rect,
      SDL_SCALEMODE_LINEAR
    );
  }

  Image(Image&& Other) noexcept
  : ImageSurface(Other.ImageSurface) {
    Other.ImageSurface = nullptr;
  }

  ~Image() {
    if (ImageSurface) {
      SDL_DestroySurface(ImageSurface);
    }
  }

  Image(const Image&) = delete;
  Image& operator=(const Image&) = delete;

 private:
  SDL_Surface* ImageSurface{nullptr};
};
}
