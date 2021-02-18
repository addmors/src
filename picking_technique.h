#pragma once
#define GLEW_STATIC

#include "render_callbacks.h"
#include<GL/glew.h>
#include<GLFW/glfw3.h>
class PickingTechnique : public IRenderCallbacks
{
public:
	void SetObjectIndex(unsigned int ObjectIndex);
	void DrawStartCB(unsigned int DrawIndex);
private:     
	GLuint m_WVPLocation;
	GLuint m_drawIndexLocation;
	GLuint m_objectIndexLocation;
};

