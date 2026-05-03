#pragma once
#include <format>
#include <iostream>
#include <unordered_map>
#include <SDL3/SDL.h>
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/TransformComponent.h"
#include "Engine/ECS/ImageComponent.h"
#include "Engine/ECS/CollisionComponent.h"
#include "../Config.h"

class Block : public Entity {
  public:
    Block(SDL_IOStream* Handle, BreakoutScene& Scene) : Entity{Scene} {
      Uint8 Type{0};
      SDL_ReadU8(Handle, &Type);
      Uint8 GridRow{0};
      SDL_ReadU8(Handle, &GridRow);
      Uint8 GridCol{0};
      SDL_ReadU8(Handle, &GridCol);

      std::cout << std::format(
        "Loaded type {}, block (Row={}, Col={})\n",
        Type, GridRow, GridCol
      );

      if (static_cast<Config::ActorType>(Type) == Config::ActorType::GreenBlock) {
        SDL_SeekIO(Handle, 2, SDL_IO_SEEK_CUR);
        Uint32 ArraySize{0};
        SDL_ReadU32LE(Handle, &ArraySize);
        SDL_SeekIO(
          Handle, ArraySize * 4, SDL_IO_SEEK_CUR
        );
      }

      float Width{50.f};
      float Height {25.f};
      Transform = AddComponent<TransformComponent>();
      Transform->SetPosition(Vec2{GridCol * Width, GridRow * Height});

      Collision = AddComponent<CollisionComponent>();
      Collision->SetSize(Width, Height);

      Image = AddComponent<ImageComponent>(
        Images[static_cast<Config::ActorType>(Type)]
      );
      Image->SetWidth(Width);
      Image->SetHeight(Height);

      HitSound = AddComponent<SoundComponent>("Assets/brick_collision.wav");
    }

    void HandleCollision(Entity& Other) override {
      if (dynamic_cast<Ball*>(&Other)) {
        HitSound->Play();
        Image->SetIsEnabled(false);
        Collision->SetIsEnabled(false);
        SDL_Event E{};
        E.type = UserEvents::BLOCK_DESTROYED;
        SDL_PushEvent(&E);
      }
    }
  private:
    TransformComponent* Transform{nullptr};
    ImageComponent* Image{nullptr};
    CollisionComponent* Collision{nullptr};
    SoundComponent* HitSound{nullptr};
    using ImageMap = std::unordered_map<Config::ActorType, std::string>;
    using enum Config::ActorType;
    inline static ImageMap Images {
      {Actor, ""},
      {BlueBlock, "Assets/Brick_Blue_A.png"},
      {GreenBlock, "Assets/Brick_Green_A.png"},
      {CyanBlock, "Assets/Brick_Cyan_A.png"},
      {OrangeBlock, "Assets/Brick_Orange_A.png"},
      {RedBlock, "Assets/Brick_Red_A.png"},
      {YellowBlock, "Assets/Brick_Yellow_A.png"},
    };
};
