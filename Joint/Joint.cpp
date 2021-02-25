#include"../Precompiled.h"

void Joint::ApplyImpulse() {
	 
	 glm::vec2 ra = A->shape->u*dot_In_A +A->position + A->velocity
		 + CrossMy(A->angularVelocity, A->shape->u*dot_In_A);

	 glm::vec2 rb = B->shape->u*dot_In_B +B->position + B->velocity
		 + CrossMy(B->angularVelocity, B->shape->u*dot_In_B);

	 rv = rb-ra;
	 if (rv.x == 0 && rv.y == 0) return;

	 float len =glm::length(rb - ra)*40;
	 glm::vec2 normal = glm::normalize(rv);
	 glm::vec2 impulse = normal *len;

	 A->velocity += A->invesMass * impulse;
	 A->angularVelocity += A->inversInertial * CrossMy(A->shape->u*dot_In_A, impulse);
	 B->velocity += B->invesMass * -impulse;
	 B->angularVelocity += B->inversInertial * CrossMy(B->shape->u*dot_In_B, -impulse);
	 //A->ApplyImpulse(impulse, A->u*dot_In_A);
	 //B->ApplyImpulse(-impulse, B->u*dot_In_B);

}