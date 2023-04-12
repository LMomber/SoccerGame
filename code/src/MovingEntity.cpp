#include "MovingEntity.h"

void MovingEntity::SetVelocity(const Tmpl8::vec2 velocity)
{
    this->velocity = velocity;
}

void MovingEntity::SetGravity(const Tmpl8::vec2 gravity)
{
    this->gravity = gravity;
}

void MovingEntity::SetFrameFloat(const float frameFloat)
{
	this->frameFloat = frameFloat;
}
