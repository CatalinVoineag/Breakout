#pragma once
#include <optional>
#include "../Engine/ECS/Entity.h"
#include "../Engine/ECS/ImageComponent.h"
#include "../Engine/ECS/SoundComponent.h"
#include "../Engine/ECS/CollisionComponent.h"
#include "../Engine/ECS/TransformComponent.h"
#include "../Engine/ECS/PhysicsComponent.h"
#include "../Config.h"
#include "../Engine/Vec2.h"

class BreakoutScene;

class Ball : public Entity {
  public:
    Ball(
      BreakoutScene& Scene,
      bool Paused,
      int Index,
      std::optional<Vec2> Position
    );

    void HandleEvent(const SDL_Event& E) override;
    void HandleCollision(Entity& Other) override;
    int GetEntityIndex() const { return EntityIndex; } 

  private:
    TransformComponent* Transform;
    ImageComponent* Image;
    CollisionComponent* Collision;
    PhysicsComponent* Physics;
    SoundComponent* Sound;
    int EntityIndex;

    void SetIsPaused(bool isPaused) {
      Physics->SetIsEnabled(!isPaused);
      Collision->SetIsEnabled(!isPaused);
    }
};
