#pragma once
#include "../Engine/ECS/Entity.h"
#include "../Engine/Scene.h"
#include "../Engine/ECS/TransformComponent.h"
#include "../Engine/ECS/CollisionComponent.h"

enum class WallPosition {
  Top, Bottom, Left, Right
};

class Wall : public Entity {
 public:
  Wall(WallPosition Position, Scene& Scene)
    : Entity{Scene}, Position{Position} {
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

  private:
    WallPosition Position;
};
