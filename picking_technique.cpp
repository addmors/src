
#include "picking_technique.h"
#include<GL/glew.h>
#include<GLFW/glfw3.h>

void PickingTechnique::DrawStartCB(unsigned int DrawIndex)
{
	glUniform1ui(m_drawIndexLocation, DrawIndex);
};

void PickingTechnique::SetObjectIndex(unsigned int ObjectIndex)
{
	glUniform1ui(m_objectIndexLocation, ObjectIndex);
}
