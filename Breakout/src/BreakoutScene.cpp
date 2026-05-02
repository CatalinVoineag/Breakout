#include <format>
#include <iostream>
#include "../BreakoutScene.h"
#include "../Ball.h"
#include "../Wall.h"
#include "../Paddle.h"
#include "../Block.h"

void BreakoutScene::Load(int Level) {
  Entities.clear();
  Entities.emplace_back(std::make_unique<Ball>(*this));

  using enum WallPosition;
  Entities.emplace_back(std::make_unique<Wall>(Top, *this));
  Entities.emplace_back(std::make_unique<Wall>(Left, *this));
  Entities.emplace_back(std::make_unique<Wall>(Bottom, *this));
  Entities.emplace_back(std::make_unique<Wall>(Right, *this));
  Entities.emplace_back(std::make_unique<Paddle>(*this));

  std::string FileName{std::format("Assets/Level{}.bin", Level)};
  SDL_IOStream* Handle{
    SDL_IOFromFile(FileName.c_str(), "rb")
  };

  if (!Handle) {
    CheckSDLError("Loading Level");
    return;
  }

  Uint8 FileVersion{0};
  SDL_ReadU8(Handle, &FileVersion);

  Uint8 GridWidth{0};
  SDL_ReadU8(Handle, &GridWidth);

  Uint8 GridHeight{0};
  SDL_ReadU8(Handle, &GridHeight);

  Uint32 BlockCount{0};
  SDL_ReadU32LE(Handle, &BlockCount);

  std::cout << std::format(
    "Loading a version "
    "{} level ({}x{}) with {} blocks\n",
    FileVersion, GridWidth, GridHeight, BlockCount
  );

  for (size_t i{0}; i < BlockCount; i++) {
    Entities.emplace_back(
      std::make_unique<Block>(Handle, *this)
    );
  }
  SDL_CloseIO(Handle);
  BlocksRemaining = BlockCount;
  LoadedLevel = Level;
  SetState(GameState::InProgress);
}
