#include "Scene3.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"
#include "Application.h"

Scene3::Scene3()
{
}

Scene3::~Scene3()
{
}

void Scene3::Init()
{

	rotateAngle = 120;
	translateX = 0;
	scaleAll = 6;

	glClearColor(1.0f, 0.73f, 0.0f, 0.0f);

	// Generate default VAO
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);

	// an array of 3 vectors which represents 3 vertices
	static GLfloat vertex_buffer_data[102] = {
		0.0f, 0.0f, 0.0f,
	};

	vertex_buffer_data[99] = 0.0f;
	vertex_buffer_data[100] = 0.0f;
	vertex_buffer_data[101] = 0.0f;

	for (int i = 1; i < 99; i++) {
		float angle = Math::DegreeToRadian((360.0f / 32.0f) * i);
		float x = cos(angle);
		float y = sin(angle);

		vertex_buffer_data[i * 3] = x;
		vertex_buffer_data[i * 3 + 1] = y;
		vertex_buffer_data[i * 3 + 2] = 0.0f;
	}

	// An array of 3 vectors which represents the colors of the 3 vertices
	static const GLfloat color_buffer_data[] = {
		0.0f, 0.0f, 0.0f,
	};

	static GLfloat color_buffer_data_red[102] = {};
	for (int i = 0; i < 34; i++) {
		color_buffer_data_red[i * 3] = 1.0f;
		color_buffer_data_red[i * 3 + 1] = 0.0f;
		color_buffer_data_red[i * 3 + 2] = 0.0f;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_red), color_buffer_data_red, GL_STATIC_DRAW);

	/*static const GLfloat vertex_buffer_data2[] = {
		-0.5f, 1.0f, 0.0f,
		0.5f, 1.0f, 0.0f,
		0.5f, -1.0f, 0.0f,
		0.5f, -1.0f, 0.0f,
		-0.5f, -1.0f, 0.0f,
		-0.5f, 1.0f, 0.0f,
	};

	static const GLfloat color_buffer_data2[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.2f, 0.0f,
		1.0f, 0.4f, 0.0f,
		1.0f, 0.6f, 0.0f,
		1.0f, 0.8f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data2), vertex_buffer_data2, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data2), color_buffer_data2, GL_STATIC_DRAW);*/

	m_programID = LoadShaders(
		"Shader//TransformVertexShader.vertexshader",
		"Shader//SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Use our shader
	glUseProgram(m_programID);

}

void Scene3::Update(double dt) {
	translateX -= (float)(20 * dt);

	if (translateX < -20)
		translateX = 20;
}

void Scene3::Render() {

	Mtx44 translate, rotate, scale;
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
	Mtx44 MVP;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors

	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();
	view.SetToIdentity(); //no need camera for now, set it at World's origin
	projection.SetToOrtho(-10, 10, -10, 10, -10, 10); //Our world is a cube defined by these boundaries

	// circle 1
	scale.SetToScale(1.5, 2, 2);
	rotate.SetToRotation(10, 0, 0, 1);
	translate.SetToTranslation(-4 + translateX, 1, 0);
	model = translate * rotate * scale; //scale, followed by rotate, then lastly translate
	MVP = projection * view * model; // Remember, matrix multiplication is the other way around
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 99); // Starting from vertex 0; 3 vertices = 1 triangle

	// circle 2
	scale.SetToScale(1.5, 2, 2);
	rotate.SetToRotation(10, 0, 0, 1);
	translate.SetToTranslation(3 + translateX, 3, 0);
	model = translate * rotate * scale; //scale, followed by rotate, then lastly translate
	MVP = projection * view * model; // Remember, matrix multiplication is the other way around
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 34); // Starting from vertex 0; 3 vertices = 1 triangle

	// circle 3
	scale.SetToScale(5, 4, 2);
	rotate.SetToRotation(186, 0, 0, 1);
	translate.SetToTranslation(0.5 + translateX, -1, 0);
	model = translate * rotate * scale; //scale, followed by rotate, then lastly translate
	MVP = projection * view * model; // Remember, matrix multiplication is the other way around
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 18); // Starting from vertex 0; 3 vertices = 1 triangle

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

}

void Scene3::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

}
