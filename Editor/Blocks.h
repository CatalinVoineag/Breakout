#pragma once
#include "Editor/Actor.h"

#define DECLARE_BLOCK_TYPE(BlockName)               \
public:                                             \
  static constexpr int WIDTH{50};                   \
  static constexpr int HEIGHT{25};                  \
  BlockName(Scene& ParentScene, SDL_Rect Rect);     \
  std::unique_ptr<Actor> Clone() const override {   \
    return std::make_unique<BlockName>(*this);      \
  }                                                 \
  Config::ActorType GetActorType() const override { \
    return Config::ActorType::BlockName;            \
  }
  
#define DEFAULT_BLOCK_CONSTRUCT(BlockName) \
public:                                    \
  static std::unique_ptr<Actor> Construct( \
    SDL_IOStream* Handle,                  \
    Scene& ParentScene                     \
  ) {                                      \
    return std::make_unique<BlockName>(    \
      ParentScene,                         \
      GeneratePositionRectangle(           \
        Handle, WIDTH, HEIGHT              \
      )                                    \
    );                                     \
  }

namespace Editor{
class BlueBlock : public Actor {
public:
  DECLARE_BLOCK_TYPE(BlueBlock)
  DEFAULT_BLOCK_CONSTRUCT(BlueBlock)
};

class CyanBlock : public Actor {
public:
  DECLARE_BLOCK_TYPE(CyanBlock)
  DEFAULT_BLOCK_CONSTRUCT(CyanBlock)
};

class OrangeBlock : public Actor {
public:
  DECLARE_BLOCK_TYPE(OrangeBlock)
  DEFAULT_BLOCK_CONSTRUCT(OrangeBlock)
};

class RedBlock : public Actor {
public:
  DECLARE_BLOCK_TYPE(RedBlock)
  DEFAULT_BLOCK_CONSTRUCT(RedBlock)
};

class YellowBlock : public Actor {
public:
  DECLARE_BLOCK_TYPE(YellowBlock)
  DEFAULT_BLOCK_CONSTRUCT(YellowBlock)
};

class GreenBlock : public Actor {
public:
  DECLARE_BLOCK_TYPE(GreenBlock)

  Sint16 SomeNumber{32};
  std::vector<Sint32> SomeArray{1, 2, 3};

  void Serialize(SDL_IOStream* Handle) const override {
    Actor::Serialize(Handle);
    SDL_WriteU16LE(Handle, SomeNumber);
    SDL_WriteU32LE(Handle, Uint32(SomeArray.size()));
    for (Sint32 Num : SomeArray) {
      SDL_WriteU32LE(Handle, Num);
    }
  }

  static std::unique_ptr<Actor> Construct(
    SDL_IOStream* Handle,
    Scene& ParentScene
  ) {
    auto NewActor{std::make_unique<GreenBlock>(
      ParentScene,
      GeneratePositionRectangle(
        Handle, WIDTH, HEIGHT
      )
    )};

    Uint16 TempShort{0};
    SDL_ReadU16LE(Handle, &TempShort);
    NewActor->SomeNumber = static_cast<Sint16>(TempShort);

    Uint32 ArraySize{0};
    SDL_ReadU32LE(Handle, &ArraySize);
    NewActor->SomeArray.resize(ArraySize);
    for (Uint32 i{0}; i < ArraySize; ++i) {
      Uint32 TempInt{0};
      SDL_ReadU32LE(Handle, &TempInt);
      NewActor->SomeArray[i] = static_cast<Sint32>(TempInt);
    }
    return NewActor;
  }
};

class StrongYellowBlock : public Actor {
public:
  DECLARE_BLOCK_TYPE(StrongYellowBlock)
  DEFAULT_BLOCK_CONSTRUCT(StrongYellowBlock)
};

class StrongRedBlock : public Actor {
public:
  DECLARE_BLOCK_TYPE(StrongRedBlock)
  DEFAULT_BLOCK_CONSTRUCT(StrongRedBlock)
};

class CrackedYellowBlock : public Actor {
public:
  DECLARE_BLOCK_TYPE(CrackedYellowBlock)
  DEFAULT_BLOCK_CONSTRUCT(CrackedYellowBlock)
};

class CrackedRedBlock : public Actor {
public:
  DECLARE_BLOCK_TYPE(CrackedRedBlock)
  DEFAULT_BLOCK_CONSTRUCT(CrackedRedBlock)
};

class TNTRedBlock : public Actor {
public:
  DECLARE_BLOCK_TYPE(TNTRedBlock)
  DEFAULT_BLOCK_CONSTRUCT(TNTRedBlock)
};

class TNTBlueBlock : public Actor {
public:
  DECLARE_BLOCK_TYPE(TNTBlueBlock)
  DEFAULT_BLOCK_CONSTRUCT(TNTBlueBlock)
};
}
