#pragma once
#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <iostream>
#include "../shader/Shad.h"
#include "../SceenPhis/Sceen.h"
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
	void Terminate() { glfwTerminate(); delete shader; delete picing; delete red; };
	void Render(vector<Shape*> &shapes);
	void Picing(vector<Shape*> &shapes);
	int MainLoop(Scene&);
private:
	GLFWwindow* window;
	Shader *shader, *picing, *red;
	int width, height;
	glm::mat4 projectionMatrix;
	PickingTexture m_pickingTexture;
	PickingTechnique m_pickingEffect;
};
