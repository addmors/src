#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>


#define K_SLOP 0.1f  // Penetration allowance
#define PERCENT 0.25f  // Penetration percentage to correct
#define EPSILON  0.0001f
#define  PI 3.141592741f 
static const float gravityScale = 5.0f;
const glm::vec2 gravity(0, -10.0f * gravityScale);
static const float dt = 1/(float)60;


inline glm::vec2 CrossMy(const glm::vec2& v, float a)
{return glm::vec2(a * v.y, -a * v.x);}

inline glm::vec2 CrossMy(float a, const glm::vec2 v)
{return glm::vec2(-a * v.y, a * v.x);}

inline float CrossMy(const glm::vec2& a, const glm::vec2& b)
{return a.x * b.y - a.y * b.x;}

inline bool Equal(float a, float b)
{return std::abs(a - b) <= EPSILON;}

inline float DistSqr(const glm::vec2& a, const glm::vec2& b)
{
	glm::vec2 c = a - b;
	return glm::dot(c, c);
}

inline float Clamp(float min, float max, float a)
{
	if (a < min) return min;
	if (a > max) return max;
	return a;
}