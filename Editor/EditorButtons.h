#pragma once
#include "Button.h"

namespace Editor{
using namespace Config::Editor;
class LoadLevelButton : public Button {
public:
  constexpr static int WIDTH{40};
  LoadLevelButton(Scene& ParentScene, int Level)
  : Level{Level},
    Button {
      ParentScene,
      std::to_string(Level),
      {
        PADDING * Level + WIDTH * (Level-1),
        LEVEL_HEIGHT + PADDING,
        WIDTH, 30
      }
    }
  {
    if (Level == 1) {
      SetState(ButtonState::Active);
    }
  }

  void HandleEvent(const SDL_Event& E) {
    Button::HandleEvent(E);
    if (E.type == UserEvents::LOAD_LEVEL) {
      if (E.user.code == Level) {
        SetState(ButtonState::Active);
      } else {
        SetState(ButtonState::Normal);
      }
    }
  }

  void HandleLeftClick() override {
    SDL_Event E;
    E.type = UserEvents::LOAD_LEVEL;
    E.user.code = Level;
    SDL_PushEvent(&E);
  }

private:
  int Level;
};

class PlayLevelButton : public Button {
public:
  constexpr static int WIDTH{70};
  PlayLevelButton(Scene& ParentScene)
  : Button {
      ParentScene,
      "PLAY",
      {
        LEVEL_WIDTH - WIDTH - PADDING,
        LEVEL_HEIGHT + PADDING,
        WIDTH, 30
      }
    }
  {}

  void HandleLeftClick() override {
    using namespace UserEvents;
    SDL_Event E;
    E.type = SAVE_AND_PLAY_LEVEL;
    SDL_PushEvent(&E);
  }
};

class SaveLevelButton : public Button {
public:
  constexpr static int WIDTH{70};
  SaveLevelButton(Scene& ParentScene)
  : Button {
      ParentScene,
      "SAVE",
      {
        LEVEL_WIDTH - WIDTH
          - PlayLevelButton::WIDTH - PADDING * 2,
        LEVEL_HEIGHT + PADDING,
        WIDTH, 30
      }
    }
  {
    SetState(ButtonState::Disabled);
  }

  void HandleEvent(const SDL_Event& E) {
    Button::HandleEvent(E);
    using namespace UserEvents;
    if (E.type == LEVEL_EDIT) {
      SetState(ButtonState::Normal);
    } else if (
      E.type == LOAD_LEVEL ||
      E.type == SAVE_LEVEL ||
      E.type == SAVE_AND_PLAY_LEVEL
    ) {
      SetState(ButtonState::Disabled);
    }
  }

  void HandleLeftClick() override {
    SDL_Event E;
    E.type = UserEvents::SAVE_LEVEL;
    SDL_PushEvent(&E);
  }
};
}
