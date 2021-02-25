#pragma once
#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <iostream>
#include "../shader/Shad.h"
#include "../SceenPhis/Sceen.h"
#include "../Time.h"


static struct {
	bool is_presed;
	double x;
	double y;
} mouse;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouseKey(GLFWwindow * window, int button, int action, int mode);


class Window 
{
public:
	Window();
	~Window() { glfwTerminate(); delete shader; delete picing; delete red; };
	void Render();
	void Picing();
	int MainLoop(Scene&);
	void AddShape(Shape *shape) { shapes.push_back(shape);};
private:
	glm::mat4 projectionMatrix;
	Clock clock;
	Shader *shader, *picing, *red;
	vector<Shape*> shapes;
 	float accumulator = 0;
	GLFWwindow* window;
	int width, height;
	PickingTexture m_pickingTexture;
	PickingTechnique m_pickingEffect;
};

