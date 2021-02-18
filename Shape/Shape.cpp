#include "Shape.h"

void Circle::ComputeMass(float density)
{
	mass = PI * radius * radius * density;
	invesMass = (mass) ? 1.0f / mass : 0.0f;
	Inertial = mass * radius * radius;
	inversInertial = (Inertial) ? 1.0f / Inertial : 0.0f;
	length = radius;
}

void Polygon::ComputeMass(float  density)
{
	// Calculate centroid and moment of interia
	glm::vec2  c(0.0f, 0.0f); // centroid
	float  area = 0.0f;
	float  I = 0.0f;
	for (unsigned int i = 0; i < count_vertex; ++i)
	{
		// Triangle vertices, third vertex implied as (0, 0)
		glm::vec2 p1(vertices.at(i).Pos);
		unsigned int  i2 = i + 1 < count_vertex ? i + 1 : 0;
		glm::vec2 p2(vertices.at(i2).Pos);

		float D = CrossMy(p1, p2);
		float triangleArea = 0.5f * D;

		area += triangleArea;

		// Use area to weight the centroid average, not just vertex position
		c += triangleArea * (p1 + p2)/(float)3;

		float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
		float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
		I += (0.5f *triangleArea / 3) * (intx2 + inty2);
	}

	c *= 1.0f / area;

	// Translate vertices to centroid (make the centroid (0, 0)
	// for the polygon in model space)
	// Not really necessary, but I like doing this anyway
	for (unsigned int i = 0; i < count_vertex; ++i)
		vertices.at(i).Pos -= c;
	length = 0;
	for (unsigned int i = 0; i < count_vertex; ++i) {
		float a = glm::length(vertices[i].Pos);
		if (length < a) length = a;
	}
	setupMesh();
	mass = density * area;
	invesMass = (mass) ? 1.0f / mass: 0.0f;
	Inertial = I * density;
	inversInertial = Inertial ? 1.0f / Inertial : 0.0f;
}

void Polygon::SetBox(float hw, float hh)
{
	count_vertex = 4;
	vertices.clear();
	vertices.push_back({ {0,0}, {0.5,0.5}});
	vertices.push_back({ { -hw, -hh },{0,0}});
	vertices.push_back({ { hw, -hh },{0,1}});
	vertices.push_back({ { hw, hh },{1,1}});
	vertices.push_back({ { -hw, hh },{1,0}});
	normals.clear();
	normals.push_back({ 0.0f, -1.0f });
	normals.push_back({ 1.0f, 0.0f } );
	normals.push_back({ 0.0f, 1.0f } );
	normals.push_back({ -1.0f, 0.0f });
	Generate_indices();
}

void Polygon::Generate_indices() {
	indices.clear();
	for (unsigned int i = 0; i < count_vertex-2; i++) {
		this->indices.push_back(0);
		this->indices.push_back(i + 1);
		this->indices.push_back(i + 2);
	}
}

void Shape::Delete() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

}
Circle::Circle(float r, float x, float y) :radius(r){
	this->vertices.push_back({ {0,0}, {0.5,0.5} });
	this->vertices.push_back({{r,0},{1, 0.5}});
	for (int ii = 2; ii < 40; ii++) {
		this->indices.push_back(0);
		float theta = 2.0f * 3.1415926f * float(ii) / float(40);
		float xx = r * cosf(theta);//calculate the x component
		float yy = r * sinf(theta);//calculate the y component
		this->vertices.push_back({ {xx,yy}, {(xx/float(r)+1)/2, (yy/float(r)+1)/2}});
		this->indices.push_back(ii-1);
		this->indices.push_back(ii);
	}
	this->indices.push_back(0);
	this->indices.push_back(39);
	this->indices.push_back(1);
	setupMesh();
	vertices.clear();
}

Polygon::Polygon(glm::vec2 vecs[], int count_vert, glm::vec2 Pos, glm::vec3 color)
{

	this->vertices.push_back({ Pos, {0.5,0.5} });
	for (int i = 0; i < count_vert; i++) {
		this->vertices.push_back({ vecs[i],{0,0}});
		this->indices.push_back(0);
		this->indices.push_back(i + 1);
		this->indices.push_back(i + 2);
	};
	this->indices.push_back(0);
	this->indices.push_back(count_vert);
	this->indices.push_back(1);

	for (int i = 0; i < count_vert; i++) {
		glm::vec2 normal = glm::normalize( vertices.at(i + 1).Pos - vertices.at(i).Pos);
		this->normals.push_back({ normal.y, -normal.x });
	}
	glm::vec2 normal = glm::normalize(vertices.at(0).Pos - vertices.at(count_vert).Pos);
	this->normals.push_back({ normal.y, -normal.x });
	count_vertex = count_vert;

	setupMesh();
	vertices.erase(vertices.begin());
}

void Shape::setupMesh()
{
	int w, h;
	unsigned char* image = SOIL_load_image("wood.png", &w, &h, 0, SOIL_LOAD_RGB);
	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vert), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (void*)offsetof(Vert, TexCoord));
	glBindVertexArray(0);
}

void Shape::Draw() const  {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


glm::vec2 Polygon::GetSupport(glm::vec2& dir) {
	float LongestProg = -10000;
	glm::vec2 bestvect;

	for (unsigned int  i = 0; i < count_vertex; ++i)
	{
		glm::vec2 v = vertices.at(i).Pos;
		float projection = glm::dot(v, dir);
		if (projection > LongestProg)
		{
			bestvect = v;
			LongestProg = projection;}
	}
	return bestvect;
};


void Shape::SetOrient(float radius)
{
		orient = radius;
		float c = std::cos(radius);
		float s = std::sin(radius);
		u = {c,s,-s,c};
}
