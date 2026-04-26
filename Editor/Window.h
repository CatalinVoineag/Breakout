#pragma once
#include <SDL3/SDL.h>
#include "../Config.h"

namespace Editor{
class Window {
public:
  Window() {
    SDLWindow = SDL_CreateWindow(
      Config::Editor::WINDOW_TITLE.c_str(),
      Config::Editor::WINDOW_WIDTH,
      Config::Editor::WINDOW_HEIGHT,
      0
    );
    CheckSDLError("Creating Editor Window");
  }

  ~Window() {
    if (SDLWindow && SDL_WasInit(SDL_INIT_VIDEO)) {
      SDL_DestroyWindow(SDLWindow);
    }
  }

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  void Render() {
    auto [r, g, b, a]{
      Config::Editor::WINDOW_BACKGROUND
    };

    const auto* Fmt{SDL_GetPixelFormatDetails(
      GetSurface()->format
    )};

    SDL_FillSurfaceRect(
      GetSurface(), nullptr,
      SDL_MapRGB(Fmt, nullptr, r, g, b)
    );
  }

  void Update() {
    SDL_UpdateWindowSurface(SDLWindow);
  }

  SDL_Surface* GetSurface() const {
    return SDL_GetWindowSurface(SDLWindow);
  }

  bool HasMouseFocus() const {
    return SDL_GetMouseFocus() == SDLWindow;
  }

  SDL_Point GetPosition() const {
    int x, y;
    SDL_GetWindowPosition(SDLWindow, &x, &y);
    return {x, y};
  }

  SDL_Point GetSize() const {
    int x, y;
    SDL_GetWindowSize(SDLWindow, &x, &y);
    return {x, y};
  }

  SDL_Window* GetRaw() const {
    return SDLWindow;
  }

private:
  SDL_Window* SDLWindow{nullptr};
};
}
