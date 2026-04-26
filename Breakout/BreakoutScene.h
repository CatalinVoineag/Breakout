#pragma once
#include <SDL3/SDL.h>
#include "../Engine/Scene.h"

class BreakoutScene : public Scene {
public:
  BreakoutScene(Window& ParentWindow)
    : Scene{ParentWindow} {
      Load(1);
    }

private:
  void Load(int Level);
};
