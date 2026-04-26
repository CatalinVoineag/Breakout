#pragma once
#include "../Engine/ECS/Entity.h"
#include "../Engine/ECS/ImageComponent.h"
#include "../Engine/ECS/CollisionComponent.h"
#include "../Engine/ECS/TransformComponent.h"
#include "../Engine/ECS/PhysicsComponent.h"
#include "BreakoutScene.h"
#include "../Config.h"
#include "../Engine/Vec2.h"

class Ball : public Entity {
  public:
    Ball(BreakoutScene& Scene) : Entity{Scene} {
      Transform = AddComponent<TransformComponent>();
      Transform->SetPosition({
        6.f * Scene::PIXELS_PER_METER,
        6.f * Scene::PIXELS_PER_METER
      });
      Transform->SetScale(0.05f);

      Physics = AddComponent<PhysicsComponent>();
      Physics->SetGravity({0.f, 0.f});
      Physics->SetVelocity(
        Vec2{1.f, 2.f}.Normalize() 
        * Config::Breakout::BALL_SPEED
        * Scene::PIXELS_PER_METER
      );

      Image = AddComponent<ImageComponent>("Assets/Grey.png");

      Collision = AddComponent<CollisionComponent>();
      Collision->SetSize(
        Image->GetWidth(),
        Image->GetHeight()
      );
    }

    void HandleCollision(Entity& Other) override;

  private:
    TransformComponent* Transform;
    ImageComponent* Image;
    CollisionComponent* Collision;
    PhysicsComponent* Physics;
};
