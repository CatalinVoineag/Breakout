#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Actor.h"
#include "Blocks.h"
#include "../Config.h"

namespace Editor {
class Scene;
using ActorPtr = std::unique_ptr<Actor>;
using ActorPtrs = std::vector<ActorPtr>;

class ActorMenu {
 public:
  ActorMenu(Scene& ParentScene)
  : ParentScene{ParentScene}
  {
    using namespace Config::Editor;
    Actors.emplace_back(
      std::make_unique<BlueBlock>(
        GetScene(),
        SDL_Rect{
          ACTOR_MENU_POSITION_X + PADDING,
          PADDING,
          0, 0
        }
      )
    );

    Actors.emplace_back(
      std::make_unique<GreenBlock>(
        GetScene(),
        SDL_Rect{
          ACTOR_MENU_POSITION_X + PADDING,
          BlueBlock::HEIGHT + PADDING * 2,
          0, 0
        }
      )
    );

    Actors.emplace_back(
      std::make_unique<RedBlock>(
        GetScene(),
        SDL_Rect{
          ACTOR_MENU_POSITION_X + PADDING,
          BlueBlock::HEIGHT * 2 + PADDING * 3,
          0, 0
        }
      )
    );

    Actors.emplace_back(
      std::make_unique<OrangeBlock>(
        GetScene(),
        SDL_Rect{
          ACTOR_MENU_POSITION_X + PADDING,
          BlueBlock::HEIGHT * 3 + PADDING * 4,
          0, 0
        }
      )
    );

    Actors.emplace_back(
      std::make_unique<YellowBlock>(
        GetScene(),
        SDL_Rect{
          ACTOR_MENU_POSITION_X + PADDING,
          BlueBlock::HEIGHT * 4 + PADDING * 5,
          0, 0
        }
      )
    );

    Actors.emplace_back(
      std::make_unique<StrongYellowBlock>(
        GetScene(),
        SDL_Rect{
          ACTOR_MENU_POSITION_X + PADDING,
          BlueBlock::HEIGHT * 5 + PADDING * 6,
          0, 0
        }
      )
    );

    Actors.emplace_back(
      std::make_unique<StrongRedBlock>(
        GetScene(),
        SDL_Rect{
          ACTOR_MENU_POSITION_X + PADDING,
          BlueBlock::HEIGHT * 6 + PADDING * 7,
          0, 0
        }
      )
    );

    Actors.emplace_back(
      std::make_unique<TNTRedBlock>(
        GetScene(),
        SDL_Rect{
          ACTOR_MENU_POSITION_X + PADDING,
          BlueBlock::HEIGHT * 7 + PADDING * 8,
          0, 0
        }
      )
    );

    Actors.emplace_back(
      std::make_unique<TNTBlueBlock>(
        GetScene(),
        SDL_Rect{
          ACTOR_MENU_POSITION_X + PADDING,
          BlueBlock::HEIGHT * 8 + PADDING * 8,
          0, 0
        }
      )
    );

    Actors.emplace_back(
      std::make_unique<BallGreenBlock>(
        GetScene(),
        SDL_Rect{
          ACTOR_MENU_POSITION_X + PADDING,
          BlueBlock::HEIGHT * 9 + PADDING * 9,
          0, 0
        }
      )
    );
  }

  void HandleEvent(const SDL_Event& E) {
    for (ActorPtr& A : Actors) {
      A->HandleEvent(E);
    }
  }

  void Tick(float DeltaTime) {
    for (ActorPtr& A : Actors) {
      A->Tick(DeltaTime);
    }
  }

  void Render(SDL_Surface* Surface) {
    using namespace Config::Editor;
    auto [r, g, b, a]{ACTOR_MENU_BACKGROUND};

    const auto* Fmt{
      SDL_GetPixelFormatDetails(Surface->format)
    };

    SDL_FillSurfaceRect(
      Surface,
      &Rect,
      SDL_MapRGB(Fmt, nullptr, r, g, b)
    );

    for (ActorPtr& A : Actors) {
      A->Render(Surface);
    }
  }

  Scene& GetScene() const {
    return ParentScene;
  }

 private:
  Scene& ParentScene;
  ActorPtrs Actors;
  SDL_Rect Rect{
    Config::Editor::ACTOR_MENU_POSITION_X, 0,
    Config::Editor::ACTOR_MENU_WIDTH,
    Config::Editor::WINDOW_HEIGHT
  };
};
}
