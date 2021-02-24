#include "../Precompiled.h"


Body::Body(Shape *_shape,  int x,  int y) :
	staticFriction(0.5f),
	dynamicFriction(0.5f),
	restitution(0.4f),
	shape(_shape),
	position({x,y})
{
	shape->body = this;
	shape->Initialize();
}

void Body::ApplyImpulse(const glm::vec2& impulse, const glm::vec2& contactVector)
{
	velocity += invesMass * impulse;
	angularVelocity += inversInertial * CrossMy(contactVector , impulse);
}

void Body::SetOrient(float radians)
{
	orient = radians;
	shape->SetOrient(radians);
}