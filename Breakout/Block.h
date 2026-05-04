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
      SDL_ReadU8(Handle, &Type);
      Uint8 GridRow{0};
      SDL_ReadU8(Handle, &GridRow);
      Uint8 GridCol{0};
      SDL_ReadU8(Handle, &GridCol);

      // std::cout << std::format(
      //   "Loaded type {}, block (Row={}, Col={})\n",
      //   Type, GridRow, GridCol
      // );

      if (static_cast<Config::ActorType>(Type) == Config::ActorType::GreenBlock) {
        SDL_SeekIO(Handle, 2, SDL_IO_SEEK_CUR);
        Uint32 ArraySize{0};
        SDL_ReadU32LE(Handle, &ArraySize);
        SDL_SeekIO(
          Handle, ArraySize * 4, SDL_IO_SEEK_CUR
        );
      }

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

        if (IsStrong() && !Damaged) {
          DamageBlock();
        } else {
          Image->SetIsEnabled(false);
          Collision->SetIsEnabled(false);
          Image->SetIsEnabled(false);
          SDL_Event E{};
          E.type = UserEvents::BLOCK_DESTROYED;
          SDL_PushEvent(&E);
        }
      }
    }

    bool IsStrong() {
      return static_cast<Config::ActorType>(Type) == Config::ActorType::StrongYellowBlock ||
        static_cast<Config::ActorType>(Type) == Config::ActorType::StrongRedBlock;
    }

    void DamageBlock() {
      if (static_cast<Config::ActorType>(Type) == Config::ActorType::StrongYellowBlock) {
        Image->LoadNewImage(Images[CrackedYellowBlock]);
      } else if (static_cast<Config::ActorType>(Type) == Config::ActorType::StrongRedBlock) {
        Image->LoadNewImage(Images[CrackedRedBlock]);
      }
      Damaged = true;
    }
  private:
    Uint8 Type{0};
    float Width{50.f};
    float Height {25.f};
    bool Damaged {false};
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
      {StrongYellowBlock, "Assets/Strong_Brick_Yellow.png"},
      {StrongRedBlock, "Assets/Strong_Brick_Red.png"},
      {CrackedYellowBlock, "Assets/Cracked_Brick_Yellow.png"},
      {CrackedRedBlock, "Assets/Cracked_Brick_Red.png"},
      {TNTRedBlock, "Assets/TNT_Brick_Red.png"},
      {TNTBlueBlock, "Assets/TNT_Brick_Blue.png"},
    };
};
