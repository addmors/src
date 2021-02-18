#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<vector>
#include <algorithm>
#include<memory>
#include "../Math.h"
using namespace std;



inline bool BiasGreaterThan(float a, float b)
{
	const float k_biasRelative = 0.95f;
	const float k_biasAbsolute = 0.01f;
	return a >= b * k_biasRelative + a * k_biasAbsolute;
}
//enum class typeMaterial {
//	Wood,
//	Steel,
//	Glass
//	
//};


//struct Material {
//	const float restitution;
//	const float friction;
//	Material(typeMaterial type);
//};


class Body {
public:
	Body() :
		staticFriction(0.5f),
		dynamicFriction(0.5f),
		restitution(0.4f)
		{};
	Body(float s): mass(s){};
	~Body() {};
	//Применение силы
	void ApplyForce(const glm::vec2& f) { force += f; };
	void ApplyImpulse(const glm::vec2& impulse, const glm::vec2& contactVector);

	//Для неподвижного обьекта масса бесконечна!!!
	void SetStatic(void)
	{
		Inertial = 0.0f;
		inversInertial = 0.0f;
		mass = 0.0f;
		invesMass = 0.0f;
	}
	float angularVelocity;
	float staticFriction;
	float dynamicFriction;
	float restitution;
	glm::vec2 velocity;
	float torque;
	glm::vec2 force;
	// Set by shape
	float Inertial;  // moment of inertia
	float inversInertial; // inverse inertia
	float mass;  // mass
	float invesMass; // inverse masee
};