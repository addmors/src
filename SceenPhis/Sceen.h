#pragma once
#include <vector>
#include <memory>
#include "../Body/Body.h"
#include "../Manifold/Manifold.h"
#include "../Joint/Joint.h"
#include "../PicingTexture.h"
#include "../picking_technique.h"

using namespace std;

// Вызываем функцию из таблицы с одномерной виртуальной диспатчеризацией
class Scene
{
public:
	Scene(float dt, int iterations) :m_dt(dt), m_iterations(iterations){};
	void Step(void);
	void Add(Body *shape);
	Body* Add(Shape *, int,  int);
	void AddJoint(Joint& joint);
	std::vector<Body*> bodies;
	std::vector<Joint> joints;
	std::vector<Manifold> contacts;
	float m_dt;
	int m_iterations;
};
