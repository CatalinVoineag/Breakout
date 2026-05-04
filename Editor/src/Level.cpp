#include <ranges>
#include <format>
#include <functional>
#include <unordered_map>
#include "Editor/Level.h"
#include "Editor/Scene.h"

using namespace Editor;

void Level::HandleEvent(const SDL_Event& E) {
  using namespace std::views;
  using namespace UserEvents;
  for (ActorPtr& A : reverse(Actors)) {
    if (A->HandleEvent(E)) {
      break;
    }
  }

  if (
    E.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
    E.button.button == SDL_BUTTON_LEFT
  ) {
    SelectedActor = nullptr;
  } else if (
    E.type == SDL_EVENT_KEY_DOWN &&
    E.key.key == SDLK_DELETE &&
    SelectedActor
  ) {
    std::erase_if(Actors,
      [&](ActorPtr& Actor){
        return Actor.get() == SelectedActor;
      }
    );

    SelectedActor = nullptr;
  } else if (E.type == LOAD_LEVEL) {
    LoadedLevel = E.user.code;
    Load();
  } else if (E.type == SAVE_LEVEL) {
    Save();
  } else if (E.type == SAVE_AND_PLAY_LEVEL) {
    SaveAndPlay();
  }
}

void Level::Tick(float DeltaTime) {
  for (ActorPtr& A : Actors) {
    A->Tick(DeltaTime);
  }
}

void Level::Render(SDL_Surface* Surface) {
  auto [r, g, b, a]{
    Config::Editor::LEVEL_BACKGROUND
  };

  const auto* Fmt = SDL_GetPixelFormatDetails(
    Surface->format);

  SDL_FillSurfaceRect(Surface, &Rect, SDL_MapRGB(
    Fmt, nullptr, r, g, b
  ));

  for (ActorPtr& A : Actors) {
    if (SelectedActor &&
      A.get() == SelectedActor &&
      SelectedActor->GetIsVisible()
    ) {
      auto [x, y, w, h]{
        SelectedActor->GetRect()
      };
      SDL_Rect Rect{x - 1, y - 1, w + 2, h + 2};
      SDL_FillSurfaceRect(Surface, &Rect, SDL_MapRGB(
        Fmt, nullptr, 255, 255, 255)
      );
    }
    A->Render(Surface);
  }
}

void Level::HandleDrop(Actor* DragActor) {
  DragActor->SetIsVisible(true);
  if (!HasMouseFocus()) {
    return;
  }

  float MouseX, MouseY;
  SDL_GetMouseState(&MouseX, &MouseY);
  auto [GridX, GridY]{
    SnapToGridPosition(int(MouseX), int(MouseY))
  };
  DeleteAtPosition(GridX, GridY, DragActor);

  using enum ActorLocation;
  if (DragActor->GetLocation() == Menu) {
    ActorPtr NewActor{DragActor->Clone()};
    NewActor->SetPosition(GridX, GridY);
    SelectedActor = NewActor.get();
    AddToLevel(std::move(NewActor));
  } else {
    DragActor->SetPosition(GridX, GridY);
    SelectedActor = DragActor;
  }
  SDL_Event E;
  E.type = UserEvents::LEVEL_EDIT;
  SDL_PushEvent(&E);
}

void Level::AddToLevel(ActorPtr NewActor) {
  NewActor->SetLocation(ActorLocation::Level);
  Actors.push_back(std::move(NewActor));
}

bool Level::HasMouseFocus() const {
  if (!ParentScene.HasMouseFocus()) {
    return false;
  }
  float x, y;
  SDL_GetMouseState(&x, &y);

  if (
    x < 0 ||
    y < 0 ||
    x >= Config::Editor::LEVEL_WIDTH ||
    y >= Config::Editor::LEVEL_HEIGHT
  ) {
    return false;
  }
  return true;
}

SDL_Point Level::SnapToGridPosition(
  int x, int y
) {
  using namespace Config::Editor;
  int sx{HORIZONTAL_GRID_SNAP};
  int sy{VERTICAL_GRID_SNAP};
  return {
    (x / sx) * sx,
    (y / sy) * sy,
  };
}

void Level::DeleteAtPosition(
  int x, int y, const Actor* Unless
) {
  for (size_t i{0}; i < Actors.size(); ++i) {
    auto [ax, ay]{Actors[i]->GetPosition()};
    if (ax == x && ay == y
        && Actors[i].get() != Unless
    ) {
      Actors.erase(Actors.begin() + i);
      break;
    }
  }
}

namespace{
using namespace Config;
using ActorLoaderFunc = std::function<
  std::unique_ptr<Actor>(SDL_IOStream*, Scene&)>;

using enum ActorType;
std::unordered_map<ActorType, ActorLoaderFunc> ActorLoaders{
  {Actor, {}},
  {BlueBlock, BlueBlock::Construct},
  {GreenBlock, GreenBlock::Construct},
  {CyanBlock, CyanBlock::Construct},
  {OrangeBlock, OrangeBlock::Construct},
  {RedBlock, RedBlock::Construct},
  {YellowBlock, YellowBlock::Construct},
  {StrongYellowBlock, StrongYellowBlock::Construct},
  {StrongRedBlock, StrongRedBlock::Construct},
  {CrackedYellowBlock, CrackedYellowBlock::Construct},
  {CrackedRedBlock, CrackedRedBlock::Construct},
  {TNTRedBlock, TNTRedBlock::Construct},
  {TNTBlueBlock, TNTBlueBlock::Construct}};
}

void Level::Load() {
  Actors.clear();
  std::string FileName{
    std::format(
      "Assets/Level{}.bin", LoadedLevel
    )
  };
  SDL_IOStream* Handle{SDL_IOFromFile(
    FileName.c_str(), "rb"
  )};
  if (!Handle) {
    CheckSDLError("Loading Level");
    return;
  }
  Uint8 FileVersion{0};
  SDL_ReadU8(Handle, &FileVersion);
  if (FileVersion != VERSION) {
    // This file is from a different version of
    // our software - react as needed
  }
  Uint8 GridWidth{0};
  SDL_ReadU8(Handle, &GridWidth);
  Uint8 GridHeight{0};
  SDL_ReadU8(Handle, &GridHeight);

  Uint32 ActorCount{0};
  SDL_ReadU32LE(Handle, &ActorCount);

  // std::cout << std::format( "Loading a version "
  //   "{} level ({}x{}) with {} actors\n",
  //   FileVersion, GridWidth,
  //   GridHeight, ActorCount
  // );

  for (size_t i{0}; i < ActorCount; ++i) {
    Uint8 Type{0};
    SDL_ReadU8(Handle, &Type);

    auto Loader{
      ActorLoaders[static_cast<ActorType>(Type)]
    };
    if (Loader) {
      ActorPtr NewActor{Loader(Handle, ParentScene)};
      NewActor->SetLocation(ActorLocation::Level);
      AddToLevel(std::move(NewActor));
    } else {
      std::cout << "Error: No Loader for Actor "
        "Type " << (int)Type << '\n';
    }
  }

  SDL_CloseIO(Handle);
}

void Level::Save() {
  std::string FileName{
    std::format(
      "Assets/Level{}.bin", LoadedLevel
    )
  };
  SDL_IOStream* Handle{SDL_IOFromFile(
    FileName.c_str(), "wb")
  };
  if (!Handle) {
    CheckSDLError("Saving Level");
    Handle = SDL_IOFromFile(
      FileName.c_str(), "w+b"
    );
  }

  using namespace Config::Editor;
  SDL_WriteU8(Handle, VERSION);
  SDL_WriteU8(Handle, GRID_WIDTH);
  SDL_WriteU8(Handle, GRID_HEIGHT);

  SDL_WriteU32LE(Handle, (Uint32)Actors.size());
  for (const ActorPtr& A : Actors) {
    A->Serialize(Handle);
  }

  SDL_CloseIO(Handle);
}

void Level::SaveAndPlay() {
  Save();
  SDL_Event E;
  E.type = UserEvents::LAUNCH_LEVEL;
  E.user.code = LoadedLevel;
  SDL_PushEvent(&E);
}
