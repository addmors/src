#include "Precompiled.h"
float delt = (float)1 / (float)60;



int main()
{
	Window win;
	Scene scene(delt,15);

	Circle::Init();
	
	Polygon* polygon = new Polygon(150, 50);
	//Polygon* polygon2 = new Polygon(40, 2);
	//Polygon* polygon3 = new Polygon(4, 40);
	//Polygon* polygon4 = new Polygon(10, 10);
	Circle* circle = new Circle(10);
	Circle* circle2 = new Circle(20);

	//Joint j(polygon2, polygon3, { 30,0 }, { 0,3 });

	//polygon2->position = {0,30};
	//polygon3->position = {14,27};
	//polygon4->position = { 30,100};
	//polygon2->ComputeMass(1);
	//polygon3->ComputeMass(1);
	//polygon4->ComputeMass(1);
	//polygon3->SetStatic();
	Body* b = scene.Add(polygon,0,-100);
	b->SetStatic();
	win.AddShape(b->shape);

	//scene.Add(polygon2);
	//scene.Add(polygon3);
	//scene.Add(polygon4);
	b = scene.Add(circle,22,30);
	win.AddShape(b->shape);

	b = scene.Add(circle, 22, 30);
	win.AddShape(b->shape);

	b = scene.Add(circle, 22, 30);
	win.AddShape(b->shape);

	b = scene.Add(circle, 22, 30);
	win.AddShape(b->shape);

	b = scene.Add(circle, 22, 30);
	win.AddShape(b->shape);

	b = scene.Add(circle, 22, 30);
	win.AddShape(b->shape);
	b = scene.Add(circle, 40, 40);
	win.AddShape(b->shape);

	b = scene.Add(circle, 50, 50);
	win.AddShape(b->shape);
	b =scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);

	b = scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);

	b = scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);

	b = scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);

	b = scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);

	b = scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);

	b = scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);

	b = scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);

	b = scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);

	b = scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);

	b = scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);

	b = scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);

	b = scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);
	b = scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);
	b = scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);
	b = scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);
	b = scene.Add(circle2, 20, 0);
	win.AddShape(b->shape);
	//scene.AddJoint(j);

	
	win.MainLoop(scene);

	polygon->Delete();
	//polygon2->Delete();
	//polygon3->Delete();
	//polygon4->Delete();
	Circle::Delete();

	delete polygon;
	//delete polygon2;
	//delete polygon3;
	//delete polygon4;
	delete circle;
	delete circle2;


	return 0;
}