#include "MeshBuilder.h"
#include "Vertex.h"
#include <vector>
#include <GL\glew.h>
#include "MyMath.h"
#include "Mtx44.h"
#include "LoadOBJ.h"

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	//x-axis
	v.pos.Set(-lengthX, 0, 0);	v.color.Set(1, 0, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX, 0, 0);	v.color.Set(1, 0, 0);	vertex_buffer_data.push_back(v);
	//y-axis
	v.pos.Set(0, -lengthY, 0);	v.color.Set(0, 1, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(0, lengthY, 0);	v.color.Set(0, 1, 0);	vertex_buffer_data.push_back(v);
	//z-axis
	v.pos.Set(0, 0, -lengthZ);	v.color.Set(0, 0, 1);	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, lengthZ);	v.color.Set(0, 0, 1);	vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float size)
{
	Vertex v;
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	v.pos.Set(0.5f * size, 0.5f * size, 0.f * size); v.normal.Set(0, 0, 1); v.texCoord.Set(1, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * size, 0.5f * size, 0.f * size); v.normal.Set(0, 0, 1); v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * size, -0.5f * size, 0.f * size); v.normal.Set(0, 0, 1); v.texCoord.Set(0, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * size, -0.5f * size, 0.f * size); v.normal.Set(0, 0, 1); v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();	
	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string& meshName, Color color, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	v.color = color;
	//front face-okay				
	v.pos.Set(length, -1 * length, length); //4 
	v.normal.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(length, length, length); //2
	v.normal.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-1 * length, length, length); //1
	v.normal.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	v.pos.Set(length, -1 * length, length); //4 
	v.normal.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-1 * length, length, length); //1
	v.normal.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-1 * length, -1 * length, length); //3
	v.normal.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	//back face	- okay
	v.pos.Set(-1 * length, length, -1 * length); //5
	v.normal.Set(0, 0, -1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(length, length, -1 * length); //6
	v.normal.Set(0, 0, -1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(length, -1 * length, -1 * length);// 8
	v.normal.Set(0, 0, -1);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-1 * length, -1 * length, -1 * length); //7
	v.normal.Set(0, 0, -1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-1 * length, length, -1 * length); //5
	v.normal.Set(0, 0, -1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(length, -1 * length, -1 * length);//8
	v.normal.Set(0, 0, -1);
	vertex_buffer_data.push_back(v);

	//right face	- okay			
	v.pos.Set(length, -1 * length, -1 * length);// 8
	v.normal.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(length, length, -1 * length); //6
	v.normal.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(length, length, length); //2
	v.normal.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(length, -1 * length, -1 * length);// 8
	v.normal.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(length, length, length); //2
	v.normal.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(length, -1 * length, length); //4 
	v.normal.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);

	//left face - okay		
	v.pos.Set(-1 * length, length, length); //1
	v.normal.Set(-1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-1 * length, length, -1 * length); //5
	v.normal.Set(-1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-1 * length, -1 * length, -1 * length); //7
	v.normal.Set(-1, 0, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-1 * length, -1 * length, length); //3
	v.normal.Set(-1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-1 * length, length, length); //1
	v.normal.Set(-1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-1 * length, -1 * length, -1 * length); //7
	v.normal.Set(-1, 0, 0);
	vertex_buffer_data.push_back(v);

	//top face				
	v.pos.Set(length, length, -1 * length); //6
	v.normal.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-1 * length, length, -1 * length); //5
	v.normal.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-1 * length, length, length); //1
	v.normal.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(length, length, -1 * length); //6
	v.normal.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-1 * length, length, length); //1
	v.normal.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(length, length, length); //2
	v.normal.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);

	//bottom face				
	v.pos.Set(-1 * length, -1 * length, length); //3
	v.normal.Set(0, -1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-1 * length, -1 * length, -1 * length); //7
	v.normal.Set(0, -1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(length, -1 * length, -1 * length);// 8
	v.normal.Set(0, -1, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(length, -1 * length, length); //4 
	v.normal.Set(0, -1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-1 * length, -1 * length, length); //3
	v.normal.Set(0, -1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(length, -1 * length, -1 * length);// 8
	v.normal.Set(0, -1, 0);
	vertex_buffer_data.push_back(v);


	for (unsigned i = 0; i < 36; ++i)
	{
		index_buffer_data.push_back(i);
	}

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateCircle(const std::string& meshName, Color color, unsigned numSlice, float size) {

	Vertex v;
	v.color = color;
	v.normal.Set(0, 1, 0);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerSlice = 360.f / numSlice;

	v.pos.Set(0, 0, 0);
	v.normal.Set(0, -1, 0);
	vertex_buffer_data.push_back(v);
	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		float theta = slice * degreePerSlice;
		v.pos.Set(size * cos(Math::DegreeToRadian(-theta)), 0, size * sin(Math::DegreeToRadian(-theta)));
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		index_buffer_data.push_back(slice + 1);
		index_buffer_data.push_back(0);
	}


	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

float sphereX(float p, float t) {
	return cos(Math::DegreeToRadian(p)) * cos(Math::DegreeToRadian(t));
}

float sphereY(float p, float t) {
	return sin(Math::DegreeToRadian(p));
}

float sphereZ(float p, float t) {
	return cos(Math::DegreeToRadian(p)) * sin(Math::DegreeToRadian(t));
}

Mesh* MeshBuilder::GenerateSphere(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
	Vertex v;
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerStack = 180.f / numStack;
	float degreePerSlice = 360.f / numSlice;

	for (unsigned stack = 0; stack < numStack + 1; ++stack) //stack //replace with 180 for sphere
	{
		float phi = -90.f + stack * degreePerStack;
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
		{
			float theta = slice * degreePerSlice;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));		
			v.texCoord.Set(-float(slice) / float(numSlice), float(stack) / float(numStack));
			vertex_buffer_data.push_back(v);

			
		}
	}
	for (unsigned stack = 0; stack < numStack; ++stack)
	{
		for (unsigned slice = 0; slice < numSlice + 1; ++slice)
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateHemisphere(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
	Vertex v;
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerStack = 180.f / numStack;
	float degreePerSlice = 360.f / numSlice;

	for (unsigned stack = 0; stack < numStack / 2 + 1; ++stack) //stack //replace with 180 for sphere
	{
		float phi = -90.f + stack * degreePerStack;
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
		{
			float theta = slice * degreePerSlice;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
			vertex_buffer_data.push_back(v);
		}
	}

	for (unsigned stack = 0; stack < numStack / 2; ++stack)
	{
		for (unsigned slice = 0; slice < numSlice + 1; ++slice)
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}

	unsigned circleStartIndex = vertex_buffer_data.size();
	v.pos.Set(0, 0, 0); 
	v.normal.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		float theta = slice * degreePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		index_buffer_data.push_back(circleStartIndex + slice + 1);
		index_buffer_data.push_back(circleStartIndex + 0);
	}

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateRing(const std::string& meshName, Color color, unsigned numSlice, float innerRad, float outerRad) {
	Vertex v;

	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	int index = 0;

	for (float theta = 0; theta <= 360.f; theta += 1.f) {
		float Xtheta = float(cos(Math::DegreeToRadian(theta)));
		float Ztheta = float(sin(Math::DegreeToRadian(theta)));

		v.pos.Set(outerRad * Xtheta, 0, outerRad * Ztheta);
		v.color = color;
		vertex_buffer_data.push_back(v);

		index_buffer_data.push_back(index++);

		v.pos.Set(innerRad * Xtheta, 0, innerRad * Ztheta);
		v.color = color;
		vertex_buffer_data.push_back(v);

		index_buffer_data.push_back(index++);
	}


	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->mode = Mesh::DRAW_MODE::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateCylinder(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius, float height) {
	Vertex v;
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerSlice = 360.f / numSlice;

	v.pos.Set(0, -0.5f * height, 0);
	v.normal.Set(0, -1, 0);
	vertex_buffer_data.push_back(v);
	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		float theta = slice * degreePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(-theta)), -0.5f * height, radius * sin(Math::DegreeToRadian(-theta)));
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		index_buffer_data.push_back(slice + 1);
		index_buffer_data.push_back(0);
	}

	unsigned cylinderStartIndex = vertex_buffer_data.size();
	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		float theta = slice * degreePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), -0.5f * height, radius * sin(Math::DegreeToRadian(theta)));
		v.normal.Set(cos(Math::DegreeToRadian(theta)), 0, sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);

		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0.5f * height, radius * sin(Math::DegreeToRadian(theta)));
		v.normal.Set(cos(Math::DegreeToRadian(theta)), 0, sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
	}

	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		index_buffer_data.push_back(cylinderStartIndex + 2 * slice + 0);
		index_buffer_data.push_back(cylinderStartIndex + 2 * slice + 1);
	}

	unsigned circleStartIndex = vertex_buffer_data.size();
	v.pos.Set(0, 0.5f * height, 0);
	v.normal.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		float theta = slice * degreePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0.5f * height, radius * sin(Math::DegreeToRadian(theta)));
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		index_buffer_data.push_back(circleStartIndex + slice + 1);
		index_buffer_data.push_back(circleStartIndex + 0);
	}

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->mode = Mesh::DRAW_MODE::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateCone(const std::string& meshName, Color color, unsigned numSlice, float radius, float height) {
	
	Vertex v;
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;


	float degreePerSlice = 360.f / numSlice;

	v.pos.Set(0, -0.5f * height, 0);
	v.normal.Set(0, -1, 0);
	vertex_buffer_data.push_back(v);
	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		float theta = slice * degreePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(-theta)), -0.5f * height, radius * sin(Math::DegreeToRadian(-theta)));
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
	}

	index_buffer_data.push_back(0);
	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		index_buffer_data.push_back(slice + 1);
	}

	unsigned coneStartIndex = vertex_buffer_data.size();
	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		float theta = slice * degreePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), -0.5f * height, radius * sin(Math::DegreeToRadian(theta)));
		v.normal.Set(height * cos(Math::DegreeToRadian(theta)), radius, height * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, 0.5f * height, 0);
		v.normal.Set(height * cos(Math::DegreeToRadian(theta)), radius, height * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
	}

	for (unsigned slice = 0; slice < (numSlice * 2) + 1; ++slice) {
		index_buffer_data.push_back(coneStartIndex + slice);
	}

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->mode = Mesh::DRAW_MODE::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateTorus(const std::string& meshName, Color color, unsigned numstack, unsigned numslice, float outerR, float innerR)
{
    Vertex v;
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;
    float degreePerStack = Math::DegreeToRadian(360.f / numstack);
    float degreePerSlice = Math::DegreeToRadian(360.f / numslice);

    float x1, z1; // y1 always 0
    float x2, y2, z2;
    v.color = color;

    for (unsigned stack = 0; stack < (numstack+1); stack++) {

        for (unsigned slice = 0; slice < numslice+1; slice++) {
            z1 = outerR * cos(stack * degreePerStack);
            x1 = outerR * sin(stack *degreePerStack);
            z2 = (outerR +innerR* cos(slice *degreePerSlice)) * cos(stack * degreePerStack);
            y2 = innerR * sin(slice * degreePerStack);
            x2 = (outerR + innerR * cos(slice * degreePerSlice)) * sin(stack * degreePerStack);
            v.pos.Set(x2, y2, z2); 
            
            v.normal.Set(x2 - x1, y2, z2 - z1);
            vertex_buffer_data.push_back(v);
        }
    }

    for (unsigned stack = 0; stack < numstack; stack++) {
		for (unsigned slice = 0; slice < numslice + 1; slice++) {
			index_buffer_data.push_back((numslice + 1) * stack + slice + 0);
            index_buffer_data.push_back((numslice + 1) * (stack+1) + slice + 0);
        }
    }
    
    Mesh* mesh = new Mesh(meshName);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
    mesh->mode = Mesh::DRAW_MODE::DRAW_TRIANGLE_STRIP;
    mesh->indexSize = index_buffer_data.size();
    return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string& meshName, const std::string& file_path) {
	//Read vertices, texcoords & normals from OBJ
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if (!success)
		return NULL;
	//Index the vertices, texcoords & normals properly
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);
	
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_MODE::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol) {
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float width = 1.f / numCol;
	float height = 1.f / numRow;
	unsigned offset = 0;
	for (unsigned row = 0; row < numRow; ++row)
	{
		for (unsigned col = 0; col < numCol; ++col)
		{
			v.pos.Set(0.5, 0.5, 0); v.texCoord.Set(width * (col + 1), height * (numRow - row)), v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);
			v.pos.Set(-0.5, 0.5, 0); v.texCoord.Set(width * (col), height * (numRow - row)), v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);
			v.pos.Set(-0.5, -0.5, 0); v.texCoord.Set(width * (col), height * (numRow - row - 1)), v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);
			v.pos.Set(0.5, -0.5, 0); v.texCoord.Set(width * (col + 1), height * (numRow - row - 1)), v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);

			//tri1
			index_buffer_data.push_back(0 + offset);
			index_buffer_data.push_back(1 + offset);
			index_buffer_data.push_back(2 + offset);
			//tri2
			index_buffer_data.push_back(0 + offset);
			index_buffer_data.push_back(2 + offset);
			index_buffer_data.push_back(3 + offset);

			offset += 4;
		}
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;

}
