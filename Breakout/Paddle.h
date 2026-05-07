#pragma once
#include "../Engine/ECS/Entity.h"
#include "../Engine/ECS/TransformComponent.h"
#include "../Engine/ECS/ImageComponent.h"
#include "../Engine/ECS/CollisionComponent.h"
#include "../Engine/ECS/InputComponent.h"
#include "../Engine/ECS/PhysicsComponent.h"
#include "../Engine/ECS/AnimationComponent.h"
#include "BreakoutScene.h"
#include "Breakout/Ball.h"
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
      // ExplosionAnimation = AddComponent<AnimationComponent>(
      //   "Assets/explosion.png", 13
      // );

      SetIsPaused(true);
    }

    void Tick(float DeltaTime) override {
      Entity::Tick(DeltaTime);
      Physics->SetVelocity({0, 0});
    }

    void HandleCollision(Entity& Other) override {
      if (Ball* Ptr{dynamic_cast<Ball*>(&Other)}) {
        HandleBallCollision(Ptr);
        Collision->SetIsEnabled(false);
        if (TimerID != 0) {
          SDL_RemoveTimer(TimerID);
        }
        TimerID = SDL_AddTimer(500, &Paddle::EnableCollision, this);
      } 
    }

    void HandleEvent(const SDL_Event& E) override {
      if (
        E.type == SDL_EVENT_KEY_DOWN &&
        E.key.key == SDLK_SPACE &&
        GetScene().GetState() == GameState::InProgress
      ) {
        SetIsPaused(false);
      } else if (
        E.type == UserEvents::GAME_WON ||
        E.type == UserEvents::GAME_LOST
      ) {
        SetIsPaused(true);
      } else if (E.type == UserEvents::BLOCK_DESTROYED) {
        // ExplosionAnimation->Play=true;
      }
    }

    ~Paddle() {
      if (TimerID) {
        SDL_RemoveTimer(TimerID);
      }
    }
    Paddle& operator=(const Paddle& Other) = delete;
    Paddle(const Paddle& Other) = delete;

  private:
    TransformComponent* Transform{nullptr};
    InputComponent* Input{nullptr};
    PhysicsComponent* Physics{nullptr};
    CollisionComponent* Collision{nullptr};
    AnimationComponent* ExplosionAnimation{nullptr};

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

    void HandleBallCollision(Ball* BallPtr) {
      Vec2 PaddlePos{Collision->GetCenter()};
      float PaddleWidth{Collision->GetSize().x};

      CollisionComponent* BallCollision{
        BallPtr->GetComponent<CollisionComponent>()
      };

      Vec2 BallPos{BallCollision->GetCenter()};
      float BallWidth{BallCollision->GetSize().x};

      // Where on the paddle the ball hit from
      // -1.0 (left edge) to 1.0 (right edge)
      float HitOffset{
        (BallPos.x - PaddlePos.x) / (PaddleWidth + BallWidth) / 2
      };

      float Strength{10.0};
      Vec2 Direction{HitOffset * Strength, -1.0f};

      PhysicsComponent* BallPhysics{
        BallPtr->GetComponent<PhysicsComponent>()
      };

      BallPhysics->SetVelocity(
        Direction.Normalize() *
        BallPhysics->GetVelocity().GetLength()
      );
    } 

    SDL_TimerID TimerID{0};
    static Uint32 EnableCollision(void* Entity, SDL_TimerID, Uint32) {
      Paddle* Target{static_cast<Paddle*>(Entity)};
      if (Target && Target->Collision) {
        Target->Collision->SetIsEnabled(true);
      }

      return 0;
    }

    void SetIsPaused(bool isPaused) {
      Input->SetIsEnabled(!isPaused);
      Physics->SetIsEnabled(!isPaused);
    }
};
