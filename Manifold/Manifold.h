#pragma once
#include "../Collision/Collision.h"
#include "../Shape/Shape.h"


struct Manifold
{
public:
	Manifold(Body *a, Body *b)
		: A(a), B(b),
		e(std::min(a->restitution, b->restitution)),
		df(std::sqrt(a->staticFriction * b->staticFriction)),
		sf(std::sqrt(a->dynamicFriction * b->dynamicFriction)){};
	Body* A;
	Body* B;
	int contact_count; // Number of contacts that occured during collision
	float e;               // Mixed restitution
	float df;              // Mixed dynamic friction
	float sf;              // Mixed static friction
	void Solve();
	void ApplyImpulse();
	void Initialize();
	void PositionalCorrection();
	void InfiniteMassCorrection();
	float penetration;     // Depth of penetration from collision
	glm::vec2 normal;          // From A to B
	glm::vec2 contacts[2];     // Points of contact during collision
};

