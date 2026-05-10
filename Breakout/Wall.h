#pragma once
#include "../Engine/ECS/Entity.h"
#include "../Engine/Scene.h"
#include "../Engine/ECS/TransformComponent.h"
#include "../Engine/ECS/CollisionComponent.h"
#include "Ball.h"
#include "BreakoutScene.h"

enum class WallPosition {
  Top, Bottom, Left, Right
};

class Wall : public Entity {
 public:
  Wall(WallPosition Position, Scene& Scene, int Index)
    : Entity{Scene}, Position{Position}, EntityIndex{Index} {
      TransformComponent* Transform{
        AddComponent<TransformComponent>()
      };

      CollisionComponent* Collision{
        AddComponent<CollisionComponent>()
      };

      float Height{Scene.GetHeight()};
      float Width{Scene.GetWidth()};
      float Thickness{1.0f * Scene::PIXELS_PER_METER};

      using enum WallPosition;
      if (Position == Top) {
        Transform->SetPosition({0, -Thickness});
        Collision->SetSize(Width, Thickness);
      } else if (Position == Bottom) {
        Transform->SetPosition({0, Height});
        Collision->SetSize(Width, Thickness);
      } else if (Position == Left) {
        Transform->SetPosition({-Thickness, 0});
        Collision->SetSize(Thickness, Height);
      } else if (Position == Right) {
        Transform->SetPosition({Width, 0});
        Collision->SetSize(Thickness, Height);
      }
    }

  void HandleCollision(Entity& Other) override {
    auto& GameScene = static_cast<BreakoutScene&>(GetScene());
    Ball* BallPtr = dynamic_cast<Ball*>(&Other);

    if (BallPtr && Position == WallPosition::Bottom) {
      SDL_Event E{};

      if (GameScene.GetBalls() > 1) {
        E.type = UserEvents::REMOVE_BALL;
        E.user.data1 = reinterpret_cast<void*>(static_cast<uintptr_t>(BallPtr->GetEntityIndex()));
      } else {
        E.type = UserEvents::GAME_LOST;
      }
      SDL_PushEvent(&E);
    }
  }

  int GetEntityIndex() const { return EntityIndex; } 

  private:
    WallPosition Position;
    int EntityIndex;
};
