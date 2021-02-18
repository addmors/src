#pragma once
#include <vector>
#include <memory>
#include "../Shape/Shape.h"
#include "../Manifold/Manifold.h"
#include "../Joint/Joint.h"
#include "../PicingTexture.h"
#include "../picking_technique.h"

using namespace std;

// �������� ������� �� ������� � ���������� ����������� ����������������
class Scene
{
public:
	Scene(float dt, int iterations) :m_dt(dt), m_iterations(iterations){};
	void Step(void);
	void Add(Shape *shape);
	void AddJoint(Joint& joint);
	std::vector<Shape*> shapes;
	std::vector<Joint> joints;
	std::vector<Manifold> contacts;
	float m_dt;
	int m_iterations;
};
