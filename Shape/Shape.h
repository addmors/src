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


static unsigned int *VAO_circle = new unsigned int(0), *VBO_circle =  new unsigned int(0), *EBO_circle = new unsigned int(0);
static GLuint texture1_c;
static std::vector<Vert> vertices_c;
static std::vector<unsigned int> indices_c;


class Shape
{
public:
	Shape() = default;
	~Shape() { delete VAO; delete VBO; delete EBO; };
	virtual void ComputeMass(float density) = 0;
	virtual Shape* Clone() const = 0;
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
	float area;
	Body *body;
	//Указатели на буферы для рисования 	//Кол-во точек;
	unsigned int* VAO = new unsigned int(0), *VBO = new unsigned int(0), *EBO = new unsigned int(0);
	unsigned int count_vertex;
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
	Polygon()  = default;
	Shape *Clone(void) const
	{
		Polygon *poly = new Polygon();
		poly->u = u;
		for (unsigned i = 0; i < count_vertex; ++i)
		{
			poly->vertices.push_back(vertices[i]);
			poly->normals.push_back(normals[i]);
		}
		std::copy(indices.begin(), indices.end(),
			std::back_inserter(poly->indices));
		poly->texture1 = texture1;
		poly->VAO = VAO;
		poly->VBO = VBO;
		poly->EBO = EBO;
		poly->count_vertex = count_vertex;
		return poly;
	}
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
	Shape *Clone(void) const
	{
		return new Circle(radius);
	};
	static void Init();
	void ComputeMass(float density) override;
	static void Delete();
	void Draw() const override;
	Type GetType() const override { return Type::typeCircle; };
	static void setupMeshCircle();
	float radius;
};


struct DotShape { glm::vec2 localpos; Shape *shape; };

class ComplexShape: public Shape{
public:
	ComplexShape() = default;
	void AddShape(Shape* shape, glm::vec2 localpos);
	//Temp
	Type GetType() const override { return Type::typePolygon; };
	//Temp
	void Draw() const override;
	void Delete() override;
	void ComputeMass(float desteny) override;

	vector <DotShape> dotShape;
	Body* body;

};