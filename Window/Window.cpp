#include "Window.h"


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouse.x = xpos;
	mouse.y = ypos;
};
void mouseKey(GLFWwindow * window, int button, int action, int mode)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == 1) {
		mouse.is_presed = true;
	}
	else mouse.is_presed = false;
	glfwGetCursorPos(window, &mouse.x, &mouse.y);
};

Window::Window() : projectionMatrix(glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 200.0f))
{
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(-1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//Минорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера
	window = glfwCreateWindow(800, 600, "MyPhiziks", nullptr/*glfwGetPrimaryMonitor()*/, nullptr);
	glfwSetMouseButtonCallback(window, mouseKey);
	glfwSetCursorPosCallback(window, mouse_callback);
	if (window == nullptr)
	{
		fprintf(stderr, "Failed to create GLFW window");
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW");
		exit(-1);
	}
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	m_pickingTexture.Init(800, 600);
	shader = new Shader("shader/shadowshader.vs", "shader/shadowshader.fs");
	picing = new Shader("shader/picking_technique.vs", "shader/picking_technique.fs");
	red = new Shader("shader/red.vs", "shader/red.fs");
}

void Window::Picing(vector<Shape*> &shapes) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	m_pickingTexture.EnableWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	picing->Use();
	picing->SetMat4("projection", projectionMatrix);

	for (unsigned int i = 0; i < shapes.size(); i++) {
		glm::mat4 model(glm::translate(glm::mat4(1), { shapes[i]->position,0 }));
		model = projectionMatrix * glm::rotate(model, shapes[i]->orient, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, shapes[i]->mScale);
		m_pickingEffect.SetObjectIndex(i);
		picing->SetMat4("model", model);
		m_pickingEffect.DrawStartCB(i);
		m_pickingEffect.SetObjectIndex(i);
		shapes[i]->Draw();
	}

	m_pickingTexture.DisableWriting();
};
void Window::Render(vector<Shape*> &shapes)
{
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	shader->Use();
	shader->SetMat4("projection", projectionMatrix);
	for (int i = 0; i < shapes.size(); ++i)
	{
		glm::mat4 model(glm::translate(glm::mat4(1), { shapes[i]->position,0 }));
		model = projectionMatrix * glm::rotate(model, shapes[i]->orient, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, shapes[i]->mScale);
		shader->SetMat4("model", model);
		shapes[i]->Draw();
	}
	if (mouse.is_presed) {

		PickingTexture::PixelInfo Pixel = m_pickingTexture.ReadPixel((unsigned int)mouse.x, 600 - (unsigned int)mouse.y);
		std::cout << Pixel.PrimID << " " << mouse.x << " " << mouse.y << endl;
		if (Pixel.PrimID != 0) {
			red->Use();
			red->SetMat4("projection", projectionMatrix);
			glm::mat4 model(glm::translate(glm::mat4(1), { shapes[Pixel.DrawID]->position, 0 }));
			model = glm::scale(model, shapes[Pixel.DrawID]->mScale);
			model = projectionMatrix * glm::rotate(model, shapes[Pixel.DrawID]->orient, glm::vec3(0.0f, 0.0f, 1.0f));
			red->SetMat4("model", model);
			shapes[Pixel.DrawID]->Draw();
			shapes[Pixel.DrawID]->velocity = { 0,0 };
			shapes[Pixel.DrawID]->angularVelocity += 0.2f;
			glm::vec4 pos = glm::inverse(projectionMatrix)*glm::vec4((GLfloat)(mouse.x - 400) / 400, (GLfloat)(300 - mouse.y) / 300, -1, 190);
			shapes[Pixel.DrawID]->position = { pos.x * 190,pos.y * 190 };

		}
	}
}


int Window::MainLoop(Scene& scene) {
	shader->Use();
	shader->SetInt("screenTexture", 0);
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.f, 0.1f, 50.0f);
	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();
		float current = (float)glfwGetTime();
		picing->Use();
		Picing(scene.shapes);
		scene.Step();
		shader->Use();
		Render(scene.shapes);

		glfwSwapBuffers(window);
	}
	return 0;
};

