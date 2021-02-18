#pragma once
#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <stdio.h>
#include <string>
#include <iostream>


class PickingTexture
{
public:
	PickingTexture();

	~PickingTexture();

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

	void EnableWriting();

	void DisableWriting();

	struct PixelInfo {
		unsigned int ObjectID;
		unsigned int DrawID;
		unsigned int PrimID;

		PixelInfo()
		{
			ObjectID = 0;
			DrawID = 0;
			PrimID = 0;
		}
	};

	PixelInfo ReadPixel(unsigned int x, unsigned int y);

private:
	GLuint m_fbo;
	GLuint m_pickingTexture;
	GLuint m_depthTexture;
};

