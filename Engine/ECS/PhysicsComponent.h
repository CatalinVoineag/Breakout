#pragma once
#include "Component.h"
#include "../Vec2.h"

class PhysicsComponent : public Component {
 public:
  using Component::Component;
  void Initialize() override;

  void Tick(float DeltaTime) override;
  void DrawDebugHelpers(SDL_Surface* Surface) override;

  void ApplyForce(const Vec2& Force);
  void ApplyImpulse(const Vec2& Impulse);

  Vec2 GetVelocity() const { return Velocity; }
  void SetVelocity(const Vec2& NewVelocity) {
    Velocity = NewVelocity;
  }

  float GetMass() const { return Mass; }
  void SetMass(float NewMass);

 private:
  Vec2 Velocity{0.0, 0.0}; // m/s
  Vec2 Acceleration{0.0, 0.0}; // m/s^2
  float Mass{1.0}; // kg
};
