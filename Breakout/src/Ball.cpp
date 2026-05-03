#include "../Ball.h"
#include "../../Engine/Vec2.h"
#include "../Paddle.h"

void Ball::HandleCollision(Entity& Other) {
  if (dynamic_cast<Paddle*>(&Other)) {
    return;
  }
  Sound->Play();
  SDL_FRect Intersection;
  Collision->GetCollisionRectangle(
    *Other.GetComponent<CollisionComponent>(),
    &Intersection
  );

  if (!(Physics && Transform)) return;

  // Determine collision axis based on intersection shape
  bool IsVertical{
    Intersection.w > Intersection.h
  };

  // 1. Push ball out of the object to prevent sticking
  Vec2 CurrentPos{Transform->GetPosition()};
  if (IsVertical) {
    if (Physics->GetVelocity().y < 0)
      CurrentPos.y += Intersection.h;
    else
      CurrentPos.y -= Intersection.h;
  } else {
    if (Physics->GetVelocity().x > 0)
      CurrentPos.x -= Intersection.w;
    else
      CurrentPos.x += Intersection.w;
  }
  Transform->SetPosition(CurrentPos);

  TransformComponent* OtherTransform{
    Other.GetComponent<TransformComponent>()
  };

  // Calculate relative position to determine the normal
  Vec2 RelativePosition{
    Transform->GetPosition() - OtherTransform->GetPosition()
  };

  Vec2 Normal {
    IsVertical ? Vec2{0.0f, (RelativePosition.y > 0) ? 1.0f : -1.0f}
    : Vec2{(RelativePosition.x > 0) ? 1.0f : -1.0f, 0.0f}
  };

  Vec2 Velocity{Physics->GetVelocity()};
  // Calculate dot product
  float DotProduct {
    Velocity.x * Normal.x + Velocity.y * Normal.y
  };

  // Only bounce if moving towards the surface
  if (DotProduct < 0) {
    Physics->SetVelocity(
      Velocity - (2 * DotProduct * Normal)
    );
  }
}
