#include "Editor/ActorTooltip.h"
#include "Editor/Scene.h"
#include "Config.h"

using namespace Editor;

ActorTooltip::ActorTooltip(Scene& ParentScene)
  : ParentScene{ParentScene} {
  SDLWindow = SDL_CreatePopupWindow(
    ParentScene.GetWindow().GetRaw(),
    0, 0, 100, 100,
    SDL_WINDOW_HIDDEN
    | SDL_WINDOW_TOOLTIP
    | SDL_WINDOW_BORDERLESS
    | SDL_WINDOW_ALWAYS_ON_TOP
    | SDL_WINDOW_NOT_FOCUSABLE
  );
  CheckSDLError("Creating Tooltip Window");

  DenyCursor = SDL_CreateSystemCursor(
    SDL_SYSTEM_CURSOR_NOT_ALLOWED
  );
  CheckSDLError("Creating DenyCursor");
}

ActorTooltip::~ActorTooltip() {
  if (!SDL_WasInit(SDL_INIT_VIDEO)) return;
  if (SDLWindow) {
    SDL_DestroyWindow(SDLWindow);
  }
  if (DenyCursor) {
    SDL_DestroyCursor(DenyCursor);
  }
}

void ActorTooltip::Render() {
  if (!isVisible) return;
  DragActor->GetArt().Render(
    GetSurface(),
    SDL_Rect{
      0, 0,
      DragActor->GetRect().w,
      DragActor->GetRect().h
    }
  );

  SDL_UpdateWindowSurface(SDLWindow);
}

void ActorTooltip::Tick(float DeltaTime) {
  if (!isVisible) return;

  auto Buttons{
    SDL_GetGlobalMouseState(nullptr, nullptr)
  };
  if (!(Buttons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT))) {
    SetIsVisible(false);
    ParentScene.GetLevel().HandleDrop(DragActor);
  } else {
    PositionWindow();
  }
}

void ActorTooltip::PositionWindow() {
  float x, y;
  SDL_GetMouseState(&x, &y);

  auto [DragOffsetX, DragOffsetY]{
    DragActor->GetDragOffset()
  };

  // Snap when over level
  if (ParentScene.GetLevel().HasMouseFocus()) {
    SDL_SetWindowOpacity(SDLWindow, 1.0f);
    SDL_SetCursor(SDL_GetDefaultCursor());

    auto [GridX, GridY]{
      ParentScene.GetLevel().SnapToGridPosition(
        int(x), int(y)
      )
    };

    SDL_SetWindowPosition(
      SDLWindow, GridX, GridY
    );
  } else {
    SDL_SetWindowPosition(
      SDLWindow,
      int(x) - DragOffsetX,
      int(y) - DragOffsetY
    );
    SDL_SetWindowOpacity(SDLWindow, 0.5f);
    SDL_SetCursor(DenyCursor);
  }
}

void ActorTooltip::HandleEvent(
  const SDL_Event& E) {
  using namespace UserEvents;
  if (E.type == ACTOR_DRAG) {
    DragActor = static_cast<Actor*>(
      E.user.data1
    );
    SDL_SetWindowSize(
      SDLWindow,
      DragActor->GetRect().w,
      DragActor->GetRect().h
    );
    SetIsVisible(true);
  }
}

void ActorTooltip::SetIsVisible(bool Visible) {
  isVisible = Visible;
  if (isVisible) {
    SDL_ShowWindow(SDLWindow);
  } else {
    SDL_HideWindow(SDLWindow);
    SDL_SetCursor(SDL_GetDefaultCursor());
    SDL_SetWindowOpacity(SDLWindow, 1.0f);
  }
}
