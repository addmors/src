#include "SceenPhis\Sceen.h"
#include "Window\Window.h"

float delt = (float)1 / (float)60;

int main()
{
	Window win;

	Scene scene(delt,15);

	Polygon* polygon = new Polygon(1, 1);	
	Polygon* polygon2 = new Polygon(40, 2);
	Polygon* polygon3 = new Polygon(4, 40);
	Polygon* polygon4 = new Polygon(10, 10);
	Circle* circle = new Circle(20, 2, 2);
	Joint j(polygon2, polygon3, { 30,0 }, { 0,3 });

	circle->position = { 50, 200 };
	circle->ComputeMass(2);

	polygon->position = { 100,-50};
	polygon2->position = {0,30};
	polygon3->position = {14,27};
	polygon4->position = { 30,100};
	polygon->ComputeMass(1);
	polygon->SetStatic();
	polygon2->ComputeMass(1);
	polygon3->ComputeMass(1);
	polygon4->ComputeMass(1);
	polygon3->SetStatic();
	scene.Add(polygon);
	scene.Add(polygon2);
	scene.Add(polygon3);
	scene.Add(polygon4);
	scene.Add(circle);
	scene.AddJoint(j);

	
	win.MainLoop(scene);
	win.Terminate();

	polygon->Delete();
	polygon2->Delete();
	polygon3->Delete();
	polygon4->Delete();
	circle->Delete();

	delete polygon;
	delete polygon2;
	delete polygon3;
	delete polygon4;
		delete circle;
	

	return 0;
}