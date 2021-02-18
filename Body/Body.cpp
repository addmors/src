#include "Body.h"


void Body::ApplyImpulse(const glm::vec2& impulse, const glm::vec2& contactVector)
{
	velocity += invesMass * impulse;
	angularVelocity += inversInertial * CrossMy(contactVector , impulse);
}

