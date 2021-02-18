#pragma once
#include "../Body/Body.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <vector>
#include"../shader/Shad.h"
#include <string>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <SOIL.h>

#define MAX_VERTEX 10

struct Vert {
	glm::vec2 Pos;
	glm::vec2 TexCoord;
};

enum Type {
	typeCircle,
	typePolygon,
	typeCount
};


class Shape : public Body
{
public:
	Shape() : joint(nullptr) {};
	virtual void ComputeMass(float density) = 0;
	void Draw() const;
	void Delete();
	void setupMesh();
	virtual Type GetType() const = 0;
	void SetOrient(float radius);
	Shape* joint;
	glm::vec3 color;
	glm::vec2 position;
	glm::mat2 orientation;
	float orient;
	float length;
	glm::mat2 u;	
	//Указатели на буферы для рисования 	//Кол-во точек;
	unsigned int VAO, VBO, EBO, count_vertex;
	//Вектор точек;
	std::vector<Vert> vertices;
	GLuint texture1;
	//Инексы вершин треугольников
	std::vector<unsigned int> indices;
};

class Polygon : public Shape {
	public: 
	//Инициализаторы
	Polygon(float x, float y)
	{	SetBox(x, y); 
		setupMesh();
		vertices.erase(vertices.begin());
	};
	Polygon(glm::vec2 vecs[], int count_vert, glm::vec2 Pos, glm::vec3 color);
	void SetBox(float hh, float hw);

	//Гетеры
	Type GetType() const override { return Type::typePolygon; };
	glm::vec2 GetSupport(glm::vec2& dir);

	void ComputeMass(float density) override;
	//Для рисования
	std::vector<glm::vec2> normals;
private:
	//Текстуры TODO
	void Generate_indices();
};
class Circle : public Shape {
public:	
	Circle(float r, float x, float y);
	void ComputeMass(float density) override;
	Type GetType() const override { return Type::typeCircle; };
	float radius;
};