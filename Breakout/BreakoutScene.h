#pragma once
#include <SDL3/SDL.h>
#include "../Engine/Scene.h"

class BreakoutScene : public Scene {
public:
  BreakoutScene(Window& ParentWindow)
    : Scene{ParentWindow} {}
};
