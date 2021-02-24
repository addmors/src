#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<vector>
#include <algorithm>
#include<memory>
#include "../Math.h"
using namespace std;


class Shape;

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
	Body(Shape *, int,  int);
	Body(float s): mass(s){};
	~Body() {};
	//Применение силы
	void ApplyForce(const glm::vec2& f) { force += f; };
	void ApplyImpulse(const glm::vec2& impulse, const glm::vec2& contactVector);
	void SetOrient(float radians);
	//Для неподвижного обьекта масса бесконечна!!!
	void SetStatic(void)
	{
		Inertial = 0.0f;
		inversInertial = 0.0f;
		mass = 0.0f;
		invesMass = 0.0f;
	}
	Shape *shape;
	float angularVelocity;
	float staticFriction;
	float dynamicFriction;
	float restitution;
	glm::vec2 position;
	glm::vec2 velocity;
	float length;
	float orient;
	Body* joint;
	float torque;
	glm::vec2 force;
	// Set by shape
	float Inertial;  // moment of inertia
	float inversInertial; // inverse inertia
	float mass;  // mass
	float invesMass; // inverse masee
};