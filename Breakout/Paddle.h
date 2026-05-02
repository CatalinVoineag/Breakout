#pragma once
#include "../Engine/ECS/Entity.h"
#include "../Engine/ECS/TransformComponent.h"
#include "../Engine/ECS/ImageComponent.h"
#include "../Engine/ECS/CollisionComponent.h"
#include "../Engine/ECS/InputComponent.h"
#include "../Engine/ECS/PhysicsComponent.h"
#include "BreakoutScene.h"
#include "../Config.h"

class Paddle : public Entity {
  public:
    Paddle(BreakoutScene& Scene) : Entity{Scene} {
      Transform = AddComponent<TransformComponent>();
      Transform->SetPosition({
        5.0f * Scene.PIXELS_PER_METER,
        7.2f * Scene.PIXELS_PER_METER
      });

      Input = AddComponent<InputComponent>();
      Input->BindKeyHeld(SDLK_LEFT, CreateMoveLeftCommand);
      Input->BindKeyHeld(SDLK_RIGHT, CreateMoveRightCommand);
      Input->UnbindKey(SDLK_SPACE);

      ImageComponent* Paddle = AddComponent<ImageComponent>("Assets/Paddle.png");

      float CollisionWidth{Paddle->GetWidth()};
      float CollisionHeight{Paddle->GetHeight()};

      Physics = AddComponent<PhysicsComponent>();
      Physics->SetGravity({0, 0});
      Physics->ConstrainHorizontalMovement(
        0,
        Scene.GetWidth() - CollisionWidth
      );

      Collision = AddComponent<CollisionComponent>();
      Collision->SetSize(
        CollisionWidth,
        CollisionHeight
      );
    }

    void Tick(float DeltaTime) override {
      Entity::Tick(DeltaTime);
      Physics->SetVelocity({0, 0});
    }

  private:
    TransformComponent* Transform{nullptr};
    InputComponent* Input{nullptr};
    PhysicsComponent* Physics{nullptr};
    CollisionComponent* Collision{nullptr};

    static CommandPtr CreateMoveLeftCommand() {
      using namespace Config::Breakout;
      return std::make_unique<MovementCommand>(
        Vec2{-PADDLE_SPEED * Scene::PIXELS_PER_METER, 0.0}
      );
    }
    static CommandPtr CreateMoveRightCommand() {
      using namespace Config::Breakout;
      return std::make_unique<MovementCommand>(
        Vec2{PADDLE_SPEED * Scene::PIXELS_PER_METER, 0.0}
      );
    }
};
