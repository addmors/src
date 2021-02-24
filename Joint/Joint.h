#pragma once
#include "../Body/Body.h"
#include "Math.h"

class Shape;

class Joint
{
public:
	Joint(Body* a, Body* b, glm::vec2 dot_a, glm::vec2 dot_b): A(a),B(b), dot_In_A(dot_a), dot_In_B(dot_b)
	{
		A->joint = b;
		B->joint = a;};
	Body* A;
	Body* B;
	glm::vec2 dot_In_A;
	glm::vec2 dot_In_B;
	glm::vec2 rv;
	glm::vec2 normal;
	void ApplyImpulse();
	void PositionalCorrection()
	{
		if (rv.x == 0 && rv.y == 0) return;
		glm::vec2 correction = (K_SLOP/(A->invesMass + B->invesMass)) * glm::normalize(rv)*PERCENT;
		A->position -= correction * A->invesMass;
		B->position += correction * B->invesMass;
	}
};

