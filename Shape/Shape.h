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


static unsigned int VAO_circle, VBO_circle, EBO_circle;
static GLuint texture1_c;
static std::vector<Vert> vertices_c;
static std::vector<unsigned int> indices_c;


class Shape
{
public:
	Shape() {};
	virtual void ComputeMass(float density) = 0;
	virtual void Draw() const;
	virtual void Delete();
	virtual void setupMesh();
	void Initialize( void )
  {
    ComputeMass( 1.0f );
  }
	virtual Type GetType() const = 0;
	void SetOrient(float radius);
	glm::vec3 color;
	glm::mat2 orientation;
	glm::mat2 u;	
	Body *body;
	//Указатели на буферы для рисования 	//Кол-во точек;
	unsigned int VAO, VBO, EBO, count_vertex;
	//Вектор точек;
	std::vector<Vert> vertices;
	GLuint texture1;
	//Инексы вершин треугольников
	std::vector<unsigned int> indices;
	glm::vec3 mScale = glm::vec3(1.f, 1.f, 1.f);
};

class Polygon : public virtual Shape {
	public: 
	//Инициализаторы
	Polygon(float x, float y);
	Polygon(glm::vec2 vecs[], int count_vert, glm::vec2 Pos, glm::vec3 color);
	void SetBox(float hh, float hw);

	//Гетеры
	Type GetType() const override { return Type::typePolygon; };
	glm::vec2 GetSupport(glm::vec2& dir);
	static unsigned int VAO_cube, VBO_cube, EBO_cube;
	void ComputeMass(float density) override;
	//Для рисования
	std::vector<glm::vec2> normals;
	
private:
	//Текстуры TODO
	void Generate_indices();
};

class Circle : public virtual Shape {
public:	
	Circle(float r);
	static void Init();
	void ComputeMass(float density) override;
	static void Delete();
	void Draw() const override;
	Type GetType() const override { return Type::typeCircle; };
	static void setupMeshCircle();
	float radius;
};


//class ComplexShape : public Body {
//
//};