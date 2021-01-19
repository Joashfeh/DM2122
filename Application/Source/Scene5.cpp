#include "Scene5.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"

Scene5::Scene5()
{
}

Scene5::~Scene5()
{
}

void Scene5::Init()
{
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	camera.Init(Vector3(4, 3, 3), Vector3(0, 0, 0), Vector3(0, 1, 0));

	rotateAngle = 0;
	translateX = 0;
	scaleAll = 6;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Generate default VAO
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("Quad", Color(1, 0, 0));
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Cube", Color(1, 0, 0));
	meshList[GEO_RING] = MeshBuilder::GenerateRing("Ring", Color(0.88, 0.77, 0.6), 100);
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("Circle", Color(0.88, 0.77, 0.6), 12);
	meshList[GEO_SPHERE1] = MeshBuilder::GenerateSphere("Sun", Color(1, 0.25, 0), 16, 16);
	meshList[GEO_SPHERE2] = MeshBuilder::GenerateSphere("Moon", Color(0.75, 0.75, 0.75), 16, 16);
	meshList[GEO_SPHERE3] = MeshBuilder::GenerateSphere("Sphere", Color(0.88, 0.77, 0.6), 16, 16);
	meshList[GEO_SPHERE4] = MeshBuilder::GenerateSphere("Sphere", Color(0.50, 0.77, 0.88), 16, 16);
	

	m_programID = LoadShaders("Shader//TransformVertexShader.vertexshader", "Shader//SimpleFragmentShader.fragmentshader");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	glEnable(GL_DEPTH_TEST);
	glUseProgram(m_programID);



}

void Scene5::Update(double dt) {

	camera.Update(dt);
		
	rotateAngle += 0.5f;

	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);

	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);

	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode

	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
}

void Scene5::Render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y,
		camera.position.z, camera.target.x, camera.target.y,
		camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_AXES]->Render();

	// Sun
	modelStack.PushMatrix();

	// Planet 1
	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle, 0, 0.3, 0);
	modelStack.Translate(2.5, 0, 0);

	// Moon
	modelStack.PushMatrix();
	modelStack.Rotate(-rotateAngle * 4, 0, 1, 0);
	modelStack.Translate(0.3, 0, 0.3);
	modelStack.Scale(0.1, 0.1, 0.1);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_SPHERE2]->Render();
	modelStack.PopMatrix();

	modelStack.Scale(0.3, 0.3, 0.3);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_SPHERE3]->Render();
	modelStack.PopMatrix();

	// Planet 2
	modelStack.PushMatrix();
	modelStack.Rotate(-rotateAngle * 1.5, 0, 0.3, 0);
	modelStack.Translate(1.5, 0, 0);

	// Moon
	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle * 4, 0.2, 1, 0);
	modelStack.Translate(0.3, 0, 0.3);
	modelStack.Scale(0.05, 0.05, 0.05);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_SPHERE2]->Render();
	modelStack.PopMatrix();

	modelStack.Scale(0.2, 0.2, 0.2);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_SPHERE4]->Render();
	modelStack.PopMatrix();

	modelStack.Rotate(rotateAngle, 0, 0.3, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_SPHERE1]->Render();

	// Planet 3
	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle * 0.25, 0, 0.5, 0);
	modelStack.Translate(4, 0, 0);

	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle, 0.5, 0.3, 0.2);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_RING]->Render();
	modelStack.PopMatrix();

	modelStack.Scale(0.5, 0.5, 0.5);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_SPHERE2]->Render();
	modelStack.PopMatrix();

	modelStack.PopMatrix();

}

void Scene5::Exit()
{
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

}
