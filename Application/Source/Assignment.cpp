#include "Assignment.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"

int frames;

Assignment::Assignment()
{
}

Assignment::~Assignment()
{
}

void Assignment::Init()
{
	m_programID = LoadShaders("Shader//Shading.vertexshader", "Shader//LightSource.fragmentshader");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(0, 15, 20);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.0001f;
	light[0].kQ = 0.00001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 6.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	light[1].type = Light::LIGHT_DIRECTIONAL;
	light[1].position.Set(0, 10, 100);
	light[1].color.Set(1, 1, 1);
	light[1].power = 1;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(2.f, 1.f, 0.f);

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 1000, 1000, 1000);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Overall", Color(1, 1, 1), 48, 48);
	meshList[GEO_SPHERE]->material.kShininess = 1.f;

	meshList[GEO_HEMISPHERE] = MeshBuilder::GenerateHemisphere("Hemisphere", Color(1, 1, 1), 48, 48);
	meshList[GEO_HEMISPHERE]->material.kShininess = 1.f;
	meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("Cylinder", Color(1, 1, 1), 48, 48, 1, 2);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Cube", Color(1, 1, 1));

	meshList[GEO_TORUS] = MeshBuilder::GenerateTorus("Torus", Color(1, 1, 1), 48, 48, 1, 0.2);
	meshList[GEO_TORUS]->material.kAmbient.Set(0.f, 0.f, 0.1f);
	meshList[GEO_TORUS]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
	meshList[GEO_TORUS]->material.kSpecular.Set(0.f, 0.f, 0.3f);
	meshList[GEO_TORUS]->material.kShininess = 1.f;

	meshList[GEO_TORUS2] = MeshBuilder::GenerateTorus("Torus", Color(1, 1, 1), 48, 48, 1, 0.5);
	meshList[GEO_TORUS2]->material.kAmbient.Set(0.f, 0.f, 0.1f);
	meshList[GEO_TORUS2]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
	meshList[GEO_TORUS2]->material.kSpecular.Set(0.f, 0.f, 0.3f);
	meshList[GEO_TORUS2]->material.kShininess = 1.f;

	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("Circle", Color(1, 1, 1), 48);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("Light ball", Color(1, 1,1), 48, 48);
	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("Quad", Color(0.486, 0.988, 0.000));
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("Quad", Color(1, 1, 1));


	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	camera.Init(Vector3(MarioPos.x + 0, MarioPos.y + 3, MarioPos.z - 10), Vector3(MarioPos.x, MarioPos.y + 2.5, MarioPos.z), Vector3(0, 1, 0));

	MarioPos.Set(0, 0, 0);

	bodyDirectionAngle = 0;
	bodySize = 1;
	bodyAngle = 0;
	headAngle = 0;

	frames = 0;

	// Right Arm
	rightShoulderAngle = 0;
	leftShoulderAngle = 0;

	// Left Arm
	rightElbowAngle = 0;
	leftElbowAngle = 0;

	// Right Leg
	rightHipAngle = 0;
	leftHipAngle = 0;

	// Left Leg
	rightKneeAngle = 0;
	leftKneeAngle = 0;

	running = false;
	jump = false;
	scaling = false;

	time = 0;
	translateX = 0;
	translateY = 0;
	translateZ = 0;

	glClearColor(0.529f, 0.808f, 0.922f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	// Generate default VAO
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	glUseProgram(m_programID);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);



}

void Assignment::Update(double dt) {

	camera.Update(dt);
	const float LSPEED = 10.f;

	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);

	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);

	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode

	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	if (Application::IsKeyPressed('5')) {
		light[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	else if (Application::IsKeyPressed('6')) {
		light[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	else if (Application::IsKeyPressed('7'))
	{
		light[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}

	if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);

	// Movement Control
	{
		if (Application::IsKeyPressed(VK_UP))
		{
			bodyDirectionAngle = 0;
			running = true;
		}
		else if (Application::IsKeyPressed(VK_DOWN))
		{
			bodyDirectionAngle = 180;
			running = true;
		}
		else if (Application::IsKeyPressed(VK_LEFT))
		{
			bodyDirectionAngle = 90;
			running = true;
		}
		else if (Application::IsKeyPressed(VK_RIGHT))
		{
			bodyDirectionAngle = 270;
			running = true;
		}
		else if (Application::IsKeyReleased(VK_UP) && Application::IsKeyReleased(VK_DOWN) && Application::IsKeyReleased(VK_LEFT) && Application::IsKeyReleased(VK_RIGHT))
			running = false;
	}

	// Scale Control
	if (Application::IsKeyPressed('Q'))
		scaling = true;

	// Jump Control
	if (Application::IsKeyPressed(VK_SPACE))
		jump = true;

	// Function Updates
	{
		if (running && !jump)
			ModelRun(dt);

		if (scaling)
			ModelScale(dt);

		if (jump)
			ModelJump(dt);

		else if (!running && !jump) {
			ResetAnimation();
			time = 0;
		}
	}

	// Reset
	if (Application::IsKeyPressed('R'))
		this->Init();

}

void Assignment::Render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y,
		camera.position.z, camera.target.x, camera.target.y,
		camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	RenderMesh(meshList[GEO_AXES], false);
	RenderFloor();
	RenderMario();
	modelStack.PushMatrix();
	modelStack.Translate(1, 0, 1);
	RenderFireFlower();
	modelStack.PopMatrix();
	
	// Blocks
	{
		modelStack.PushMatrix();
		modelStack.Translate(35, 20, 5);
		RenderBlocks();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(14, 20, 6);
		RenderBlocks();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(4, 20, 3);
		RenderBlocks();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-16, 20, 5);
		RenderBlocks();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-2, 20, -25);
		RenderBlocks();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(20, 20, -6);
		RenderBlocks();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-4, 20, -10);
		RenderBlocks();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-50, 20, 20);
		RenderBlocks();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(30, 20, 30);
		RenderBlocks();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 20, 20);
		RenderBlocks();
		modelStack.PopMatrix();
	}

	// Lightball
	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	modelStack.Scale(0.1, 0.1, 0.1);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);

}

void Assignment::Exit()
{
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

}

void Assignment::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	mesh->Render();
}

void Assignment::RenderFloor() {
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[GEO_FLOOR], true);
	modelStack.PopMatrix();
}

void Assignment::RenderMario() {


	// Body
	modelStack.PushMatrix();
	modelStack.Translate(MarioPos.x, MarioPos.y, MarioPos.z);
	modelStack.Rotate(bodyDirectionAngle, 0, 1, 0);
	modelStack.Scale(bodySize, bodySize, bodySize);
	modelStack.Translate(0, 2.4, 0);
	modelStack.Rotate(bodyAngle, 1, 0, 0);
	{
		// Bottom Hemisphere
		modelStack.PushMatrix();
		modelStack.Scale(1, 1, 0.8);
		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.f, 0.f, 0.1f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.f, 0.f, 0.3f);
		RenderMesh(meshList[GEO_HEMISPHERE], true);

		// Top Hemisphere
		modelStack.PushMatrix();
		modelStack.Translate(0, 0.4, 0);
		modelStack.Rotate(180, 1, 0, 0);
		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.1f, 0.f, 0.f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.3f, 0.f, 0.f);
		RenderMesh(meshList[GEO_HEMISPHERE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0.3, 0);

		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0.3, 0);

			{
				modelStack.PushMatrix();
				modelStack.Translate(-0.4, 0, 0);
				modelStack.Rotate(90, 0, 0, 1);
				modelStack.Scale(0.7, 0.7, 0.75);
				meshList[GEO_TORUS]->material.kAmbient.Set(0.f, 0.f, 0.1f);
				meshList[GEO_TORUS]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
				meshList[GEO_TORUS]->material.kSpecular.Set(0.f, 0.f, 0.3f);
				RenderMesh(meshList[GEO_TORUS], true);
				modelStack.PopMatrix();
			}

			{
				modelStack.PushMatrix();
				modelStack.Translate(0.4, 0, 0);
				modelStack.Rotate(90, 0, 0, 1);
				modelStack.Scale(0.7, 0.7, 0.75);
				meshList[GEO_TORUS]->material.kAmbient.Set(0.f, 0.f, 0.1f);
				meshList[GEO_TORUS]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
				meshList[GEO_TORUS]->material.kSpecular.Set(0.f, 0.f, 0.3f);
				RenderMesh(meshList[GEO_TORUS], true);
				modelStack.PopMatrix();
			}

			modelStack.Rotate(180, 1, 0, 0);
			modelStack.Scale(1, 0.3, 1);
			meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.f, 0.f, 0.1f);
			meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
			meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.f, 0.f, 0.3f);
			RenderMesh(meshList[GEO_HEMISPHERE], true);
			modelStack.PopMatrix();
		}

		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Scale(1, 0.3, 1);
		meshList[GEO_CYLINDER]->material.kAmbient.Set(0.f, 0.f, 0.1f);
		meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
		meshList[GEO_CYLINDER]->material.kSpecular.Set(0.f, 0.f, 0.3f);
		RenderMesh(meshList[GEO_CYLINDER], true);
		modelStack.PopMatrix();

		// Buttons
		modelStack.PushMatrix();
		modelStack.Rotate(-25, 0, 1, 0);
		modelStack.Translate(0, 0.5, 1);
		modelStack.Rotate(-14, 1, 0, 0);
		modelStack.Scale(0.15, 0.15, 0.05);
		meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.6f, 0.f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.6f, 0.6f, 0.f);
		RenderMesh(meshList[GEO_SPHERE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Rotate(25, 0, 1, 0);
		modelStack.Translate(0, 0.5, 1);
		modelStack.Rotate(-14, 1, 0, 0);
		modelStack.Scale(0.15, 0.15, 0.05);
		meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.6f, 0.f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.6f, 0.6f, 0.f);
		RenderMesh(meshList[GEO_SPHERE], true);
		modelStack.PopMatrix();

	}

	RenderRightLeg();
	RenderLeftLeg();
	RenderRightArm();
	RenderLeftArm();
	RenderHead();
	modelStack.PopMatrix();
}

void Assignment::RenderLeftArm()
{
	// Left Arm
	{
		// Shoulder
		modelStack.PushMatrix();
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Translate(-0.7, 1, 0);
		modelStack.Scale(0.275, 0.275, 0.34375);

		modelStack.PushMatrix();
		modelStack.Rotate(leftShoulderAngle, 1, 0, 0);
		modelStack.Rotate(50, 0, 0, 1);

		{
			// Upper Arm
			modelStack.PushMatrix();
			modelStack.Rotate(90, 0, 0, 1);
			modelStack.Translate(0, 1, 0);

			{
				// Elbow
				modelStack.PushMatrix();
				modelStack.Translate(0, 1, 0);
				modelStack.Rotate(leftElbowAngle, 1, 0, 0);

				{
					// Lower Arm
					modelStack.PushMatrix();
					modelStack.Translate(0, 1, 0);

					{
						// Hand
						modelStack.PushMatrix();
						modelStack.Translate(0, 1, 0);
						{
							// Fist
							modelStack.PushMatrix();
							modelStack.Translate(0, 1, 0);
							modelStack.Scale(1.25, 1.25, 1.25);
							meshList[GEO_SPHERE]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
							meshList[GEO_SPHERE]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
							meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
							RenderMesh(meshList[GEO_SPHERE], true);
							modelStack.PopMatrix();

						}

						meshList[GEO_TORUS]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
						meshList[GEO_TORUS]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
						meshList[GEO_TORUS]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
						RenderMesh(meshList[GEO_TORUS], true);
						modelStack.PopMatrix();
					}

					meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.f, 0.f);
					meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
					meshList[GEO_CYLINDER]->material.kSpecular.Set(0.3f, 0.f, 0.f);
					RenderMesh(meshList[GEO_CYLINDER], true);
					modelStack.PopMatrix();
				}

				meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.f, 0.f);
				meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
				meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.f, 0.f);
				RenderMesh(meshList[GEO_SPHERE], true);
				modelStack.PopMatrix();
			}

			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.f, 0.f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.3f, 0.f, 0.f);
			RenderMesh(meshList[GEO_CYLINDER], true);
			modelStack.PopMatrix();
		}

		meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.f, 0.f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.f, 0.f);
		RenderMesh(meshList[GEO_SPHERE], true);

		modelStack.PopMatrix();
		modelStack.PopMatrix();

	}
	modelStack.PopMatrix();
}

void Assignment::RenderRightLeg()
{
	// Right Leg
	{
		// Hip Joint
		modelStack.PushMatrix();
		modelStack.Translate(-0.5, -0.5, 0);
		modelStack.Scale(0.4, 0.4, 0.5);
		modelStack.Rotate(rightHipAngle, 1, 0, 0);


		{
			// Upper Leg
			modelStack.PushMatrix();
			modelStack.Translate(0, -1, 0);

			{
				// Knee
				modelStack.PushMatrix();
				modelStack.Translate(0, -1, 0);
				modelStack.Rotate(rightKneeAngle, 1, 0, 0);

				{
					// Shin
					modelStack.PushMatrix();
					modelStack.Translate(0, -1, 0);

					{
						// Shoe
						modelStack.PushMatrix();
						modelStack.Translate(0, -1.4, 0);

						{
							modelStack.PushMatrix();
							modelStack.Translate(0, 0, 0.75);

							{
								modelStack.PushMatrix();
								modelStack.Translate(0, 0, 0.75);

								{
									modelStack.PushMatrix();
									modelStack.Translate(0, 0.4, 0);
									modelStack.Rotate(180, 1, 0, 0);
									modelStack.Scale(1, 0.5, 1);
									meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
									meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
									meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
									RenderMesh(meshList[GEO_HEMISPHERE], true);
									modelStack.PopMatrix();
								}

								modelStack.Scale(1, 0.4, 1);
								meshList[GEO_CYLINDER]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
								meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
								meshList[GEO_CYLINDER]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
								RenderMesh(meshList[GEO_CYLINDER], true);
								modelStack.PopMatrix();

								modelStack.PushMatrix();
								modelStack.Translate(0, 0.4, 0.2);
								modelStack.Rotate(90, 1, 0, 0);
								modelStack.Scale(1, 0.5, 0.5);
								meshList[GEO_CYLINDER]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
								meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
								meshList[GEO_CYLINDER]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
								RenderMesh(meshList[GEO_CYLINDER], true);
								modelStack.PopMatrix();

							}

							modelStack.Scale(1, 0.4, 0.625);
							meshList[GEO_CUBE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
							meshList[GEO_CUBE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
							meshList[GEO_CUBE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
							RenderMesh(meshList[GEO_CUBE], true);
							modelStack.PopMatrix();


						}

						modelStack.Scale(1, 0.4, 1);
						meshList[GEO_CYLINDER]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
						meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
						meshList[GEO_CYLINDER]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
						RenderMesh(meshList[GEO_CYLINDER], true);
						modelStack.PopMatrix();

					}

					meshList[GEO_CYLINDER]->material.kAmbient.Set(0.f, 0.f, 0.1f);
					meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
					meshList[GEO_CYLINDER]->material.kSpecular.Set(0.f, 0.f, 0.3f);
					RenderMesh(meshList[GEO_CYLINDER], true);
					modelStack.PopMatrix();
				}

				meshList[GEO_SPHERE]->material.kAmbient.Set(0.f, 0.f, 0.1f);
				meshList[GEO_SPHERE]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
				meshList[GEO_SPHERE]->material.kSpecular.Set(0.f, 0.f, 0.3f);
				RenderMesh(meshList[GEO_SPHERE], true);
				modelStack.PopMatrix();
			}

			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.f, 0.f, 0.1f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.f, 0.f, 0.3f);
			RenderMesh(meshList[GEO_CYLINDER], true);
			modelStack.PopMatrix();

		}

		meshList[GEO_SPHERE]->material.kAmbient.Set(0.f, 0.f, 0.1f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.f, 0.f, 0.3f);
		RenderMesh(meshList[GEO_SPHERE], true);
		modelStack.PopMatrix();

	}
}

void Assignment::RenderLeftLeg()
{

	// Right Leg
	{
		// Hip Joint
		modelStack.PushMatrix();
		modelStack.Translate(0.5, -0.5, 0);
		modelStack.Scale(0.4, 0.4, 0.5);
		modelStack.Rotate(leftHipAngle, 1, 0, 0);

		{
			// Upper Leg
			modelStack.PushMatrix();
			modelStack.Translate(0, -1, 0);

			{
				// Knee
				modelStack.PushMatrix();
				modelStack.Translate(0, -1, 0);
				modelStack.Rotate(leftKneeAngle, 1, 0, 0);

				{
					// Shin
					modelStack.PushMatrix();
					modelStack.Translate(0, -1, 0);

					{
						// Shoe
						modelStack.PushMatrix();
						modelStack.Translate(0, -1.4, 0);

						{
							modelStack.PushMatrix();
							modelStack.Translate(0, 0, 0.75);

							{
								modelStack.PushMatrix();
								modelStack.Translate(0, 0, 0.75);

								{
									modelStack.PushMatrix();
									modelStack.Translate(0, 0.4, 0);
									modelStack.Rotate(180, 1, 0, 0);
									modelStack.Scale(1, 0.5, 1);
									meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
									meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
									meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
									RenderMesh(meshList[GEO_HEMISPHERE], true);
									modelStack.PopMatrix();
								}

								modelStack.Scale(1, 0.4, 1);
								meshList[GEO_CYLINDER]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
								meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
								meshList[GEO_CYLINDER]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
								RenderMesh(meshList[GEO_CYLINDER], true);
								modelStack.PopMatrix();

								modelStack.PushMatrix();
								modelStack.Translate(0, 0.4, 0.2);
								modelStack.Rotate(90, 1, 0, 0);
								modelStack.Scale(1, 0.5, 0.5);
								meshList[GEO_CYLINDER]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
								meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
								meshList[GEO_CYLINDER]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
								RenderMesh(meshList[GEO_CYLINDER], true);
								modelStack.PopMatrix();

							}

							modelStack.Scale(1, 0.4, 0.625);
							meshList[GEO_CUBE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
							meshList[GEO_CUBE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
							meshList[GEO_CUBE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
							RenderMesh(meshList[GEO_CUBE], true);
							modelStack.PopMatrix();


						}

						modelStack.Scale(1, 0.4, 1);
						meshList[GEO_CYLINDER]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
						meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
						meshList[GEO_CYLINDER]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
						RenderMesh(meshList[GEO_CYLINDER], true);
						modelStack.PopMatrix();

					}

					meshList[GEO_CYLINDER]->material.kAmbient.Set(0.f, 0.f, 0.1f);
					meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
					meshList[GEO_CYLINDER]->material.kSpecular.Set(0.f, 0.f, 0.3f);
					RenderMesh(meshList[GEO_CYLINDER], true);
					modelStack.PopMatrix();
				}

				meshList[GEO_SPHERE]->material.kAmbient.Set(0.f, 0.f, 0.1f);
				meshList[GEO_SPHERE]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
				meshList[GEO_SPHERE]->material.kSpecular.Set(0.f, 0.f, 0.3f);
				RenderMesh(meshList[GEO_SPHERE], true);
				modelStack.PopMatrix();
			}

			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.f, 0.f, 0.1f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.f, 0.f, 0.3f);
			RenderMesh(meshList[GEO_CYLINDER], true);
			modelStack.PopMatrix();

		}

		meshList[GEO_SPHERE]->material.kAmbient.Set(0.f, 0.f, 0.1f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.f, 0.f, 0.3f);
		RenderMesh(meshList[GEO_SPHERE], true);
		modelStack.PopMatrix();

	}
}

void Assignment::RenderRightArm() 
{
	// Right Arm
	{
		// Shoulder
		modelStack.PushMatrix();
		modelStack.Translate(-0.7, 1, 0);
		modelStack.Scale(0.275, 0.275, 0.34375);

		modelStack.PushMatrix();
		modelStack.Rotate(rightShoulderAngle, 1, 0, 0);
		modelStack.Rotate(50, 0, 0, 1);

		{
			// Upper Arm
			modelStack.PushMatrix();
			modelStack.Rotate(90, 0, 0, 1);
			modelStack.Translate(0, 1, 0);

			{
				// Elbow
				modelStack.PushMatrix();
				modelStack.Translate(0, 1, 0);
				
				modelStack.Rotate(rightElbowAngle, 1, 0, 0);

				{
					// Lower Arm
					modelStack.PushMatrix();
					modelStack.Translate(0, 1, 0);

					{
						// Hand
						modelStack.PushMatrix();
						modelStack.Translate(0, 1, 0);
						{
							// Fist
							modelStack.PushMatrix();
							modelStack.Translate(0, 1, 0);
							modelStack.Scale(1.25, 1.25, 1.25);
							meshList[GEO_SPHERE]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
							meshList[GEO_SPHERE]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
							meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
							RenderMesh(meshList[GEO_SPHERE], true);
							modelStack.PopMatrix();

						}

						meshList[GEO_TORUS]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
						meshList[GEO_TORUS]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
						meshList[GEO_TORUS]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
						RenderMesh(meshList[GEO_TORUS], true);
						modelStack.PopMatrix();
					}

					meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.f, 0.f);
					meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
					meshList[GEO_CYLINDER]->material.kSpecular.Set(0.3f, 0.f, 0.f);
					RenderMesh(meshList[GEO_CYLINDER], true);
					modelStack.PopMatrix();
				}

				meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.f, 0.f);
				meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
				meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.f, 0.f);
				RenderMesh(meshList[GEO_SPHERE], true);
				modelStack.PopMatrix();
			}

			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.f, 0.f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.3f, 0.f, 0.f);
			RenderMesh(meshList[GEO_CYLINDER], true);
			modelStack.PopMatrix();
		}

		meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.f, 0.f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.f, 0.f);
		RenderMesh(meshList[GEO_SPHERE], true);

		modelStack.PopMatrix();
		modelStack.PopMatrix();

	}
}

void Assignment::RenderHead() {
	// Head
	modelStack.PushMatrix();
	modelStack.Translate(0, 2.1, -0.1);
	modelStack.Rotate(headAngle, 1, 0, 0);

	// Chin
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, -0.5, 0.45);
		modelStack.Scale(0.4, 0.4, 0.4);
		meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.0894f, 0.071f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.5364f, 0.426f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.268f, 0.213f);
		RenderMesh(meshList[GEO_SPHERE], true);
		modelStack.PopMatrix();
	}

	// Chin Torus part
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, -0.3, 0);
		modelStack.Rotate(10, 1, 0, 0);
		modelStack.Scale(0.6, 0.8, 0.6);
		meshList[GEO_TORUS2]->material.kAmbient.Set(0.1f, 0.0894f, 0.071f);
		meshList[GEO_TORUS2]->material.kDiffuse.Set(0.6f, 0.5364f, 0.426f);
		meshList[GEO_TORUS2]->material.kSpecular.Set(0.3f, 0.268f, 0.213f);
		RenderMesh(meshList[GEO_TORUS2], true);
		modelStack.PopMatrix();
	}

	// Hair
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0.03, -0.1);
		modelStack.Scale(0.9, 0.9, 0.95);
		meshList[GEO_SPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_SPHERE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Rotate(70, 0, 1, 0);
		modelStack.Rotate(-10, 1, 0, 0);
		modelStack.Translate(0, 0, 0.85);

		{
			modelStack.PushMatrix();
			modelStack.Translate(0.2, 0.3, 0);
			modelStack.Scale(0.2, 0.2, 0.1);
			meshList[GEO_SPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
			meshList[GEO_SPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
			meshList[GEO_SPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();
		}

		modelStack.Scale(0.15, 0.4, 0.1);
		meshList[GEO_SPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_SPHERE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Rotate(-70, 0, 1, 0);
		modelStack.Rotate(-10, 1, 0, 0);
		modelStack.Translate(0, 0, 0.85);

		{
			modelStack.PushMatrix();
			modelStack.Translate(-0.2, 0.3, 0);
			modelStack.Scale(0.2, 0.2, 0.1);
			meshList[GEO_SPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
			meshList[GEO_SPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
			meshList[GEO_SPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();
		}

		modelStack.Scale(0.15, 0.4, 0.1);
		meshList[GEO_SPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_SPHERE], true);
		modelStack.PopMatrix();
	}

	// Nose
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, -0.2, 1);
		modelStack.Scale(0.3, 0.22, 0.22);
		meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.0894f, 0.071f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.5364f, 0.426f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.268f, 0.213f);
		RenderMesh(meshList[GEO_SPHERE], true);
		modelStack.PopMatrix();
	}

	// Eye
	{	
		modelStack.PushMatrix();
		modelStack.Rotate(22.5, 0, 1, 0);
		modelStack.Rotate(-7, 1, 0, 0);
		modelStack.Translate(0, 0, 0.875);

		{
			modelStack.PushMatrix();
			modelStack.Translate(0.02, 0.175, -0.02);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(0.2, 0.2, 0.2);
			meshList[GEO_CIRCLE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
			meshList[GEO_CIRCLE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
			meshList[GEO_CIRCLE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
			RenderMesh(meshList[GEO_CIRCLE], true);
			modelStack.PopMatrix();
		}

		modelStack.Scale(0.175, 0.225, 0.05);

		{
			modelStack.PushMatrix();
			modelStack.Translate(-0.2, -0.05, 0.2);
			modelStack.Scale(0.8, 0.8, 1);

			{
				modelStack.PushMatrix();
				modelStack.Translate(-0.2, -0.05, 0.2);
				modelStack.Scale(0.8, 0.8, 0.9);
				meshList[GEO_SPHERE]->material.kAmbient.Set(0.05f, 0.05f, 0.05f);
				meshList[GEO_SPHERE]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
				meshList[GEO_SPHERE]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
				meshList[GEO_SPHERE]->material.kShininess = 200.f;
				RenderMesh(meshList[GEO_SPHERE], true);
				modelStack.PopMatrix();
			}

			meshList[GEO_SPHERE]->material.kShininess = 1.f;
			meshList[GEO_SPHERE]->material.kAmbient.Set(0.f, 0.079f, 0.1f);
			meshList[GEO_SPHERE]->material.kDiffuse.Set(0.f, 0.474f, 0.6f);
			meshList[GEO_SPHERE]->material.kSpecular.Set(0.f, 0.247f, 0.3f);
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();
		}

		meshList[GEO_SPHERE]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		RenderMesh(meshList[GEO_SPHERE], true);
		modelStack.PopMatrix();
	}

	// Eye
	{
		modelStack.PushMatrix();
		modelStack.Rotate(-22.5, 0, 1, 0);
		modelStack.Rotate(-7, 1, 0, 0);
		modelStack.Translate(0, 0, 0.875);

		{
			modelStack.PushMatrix();
			modelStack.Translate(-0.02, 0.175, -0.02);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(0.2, 0.2, 0.2);
			meshList[GEO_CIRCLE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
			meshList[GEO_CIRCLE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
			meshList[GEO_CIRCLE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
			RenderMesh(meshList[GEO_CIRCLE], true);
			modelStack.PopMatrix();
		}

		modelStack.Scale(0.175, 0.225, 0.05);

		{
			modelStack.PushMatrix();
			modelStack.Translate(0.2, -0.05, 0.2);
			modelStack.Scale(0.8, 0.8, 1);

			{
				modelStack.PushMatrix();
				modelStack.Translate(0.2, -0.05, 0.2);
				modelStack.Scale(0.8, 0.8, 0.9);
				meshList[GEO_SPHERE]->material.kAmbient.Set(0.05f, 0.05f, 0.05f);
				meshList[GEO_SPHERE]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
				meshList[GEO_SPHERE]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
				meshList[GEO_SPHERE]->material.kShininess = 200.f;
				RenderMesh(meshList[GEO_SPHERE], true);
				modelStack.PopMatrix();
			}

			meshList[GEO_SPHERE]->material.kShininess = 1.f;
			meshList[GEO_SPHERE]->material.kAmbient.Set(0.f, 0.079f, 0.1f);
			meshList[GEO_SPHERE]->material.kDiffuse.Set(0.f, 0.474f, 0.6f);
			meshList[GEO_SPHERE]->material.kSpecular.Set(0.f, 0.247f, 0.3f);
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();
		}

		meshList[GEO_SPHERE]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		RenderMesh(meshList[GEO_SPHERE], true);
		modelStack.PopMatrix();
	}

	// Hat
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0.5, -0.2);
		modelStack.Rotate(-10, 1, 0, 0);

		{
			modelStack.PushMatrix();
			modelStack.Scale(0.8, 0.8, 0.8);
			meshList[GEO_TORUS]->material.kAmbient.Set(0.1f, 0.f, 0.f);
			meshList[GEO_TORUS]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
			meshList[GEO_TORUS]->material.kSpecular.Set(0.3f, 0.f, 0.f);
			RenderMesh(meshList[GEO_TORUS], true);
			modelStack.PopMatrix();
		}

		{
			modelStack.PushMatrix();
			modelStack.Translate(0, -0.1, 0.2);
			modelStack.Scale(1, 0.02, 1.2);
			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.f, 0.f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.3f, 0.f, 0.f);
			RenderMesh(meshList[GEO_CYLINDER], true);
			modelStack.PopMatrix();
		}

		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0.2, 1.1);
			modelStack.Rotate(25, 1, 0, 0);
			modelStack.Scale(0.25, 0.25, 0.1);
			meshList[GEO_SPHERE]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
			meshList[GEO_SPHERE]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
			meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();
		}

		modelStack.Shear('y', 0, 0, 1);
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Scale(1, 0.7, 1);
		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.1f, 0.f, 0.f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.3f, 0.f, 0.f);
		RenderMesh(meshList[GEO_HEMISPHERE], true);
		modelStack.PopMatrix();
	}

	// Left Mustache
	{
		modelStack.PushMatrix();
		modelStack.Translate(0.1, -0.45, 0.9);
		modelStack.Rotate(5, 0, 0, 1);
		modelStack.Scale(0.12, 0.1 , 0.01);

		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0.5, 1);
			modelStack.Scale(2, 1.5, 2);
			meshList[GEO_QUAD]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
			meshList[GEO_QUAD]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
			meshList[GEO_QUAD]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
			RenderMesh(meshList[GEO_QUAD], true);
			modelStack.PopMatrix();
		}

		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_HEMISPHERE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0.3, -0.40, 0.9);
		modelStack.Rotate(15, 0, 0, 1);
		modelStack.Scale(0.12, 0.1, 0.01);

		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0.5, 1);
			modelStack.Scale(2, 1.5, 2);
			meshList[GEO_QUAD]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
			meshList[GEO_QUAD]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
			meshList[GEO_QUAD]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
			RenderMesh(meshList[GEO_QUAD], true);
			modelStack.PopMatrix();
		}

		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_HEMISPHERE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0.5, -0.325, 0.9);
		modelStack.Rotate(30, 0, 0, 1);
		modelStack.Scale(0.12, 0.1, 0.01);

		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0.5, 1);
			modelStack.Scale(2, 1.5, 2);
			meshList[GEO_QUAD]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
			meshList[GEO_QUAD]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
			meshList[GEO_QUAD]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
			RenderMesh(meshList[GEO_QUAD], true);
			modelStack.PopMatrix();
		}

		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_HEMISPHERE], true);
		modelStack.PopMatrix();
	}

	// Right Mustache
	{
		modelStack.PushMatrix();
		modelStack.Translate(-0.1, -0.45, 0.9);
		modelStack.Rotate(-5, 0, 0, 1);
		modelStack.Scale(0.12, 0.1, 0.01);

		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0.5, 1);
			modelStack.Scale(2, 1.5, 2);
			meshList[GEO_QUAD]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
			meshList[GEO_QUAD]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
			meshList[GEO_QUAD]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
			RenderMesh(meshList[GEO_QUAD], true);
			modelStack.PopMatrix();
		}

		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_HEMISPHERE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-0.3, -0.40, 0.9);
		modelStack.Rotate(-15, 0, 0, 1);
		modelStack.Scale(0.12, 0.1, 0.01);

		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0.5, 1);
			modelStack.Scale(2, 1.5, 2);
			meshList[GEO_QUAD]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
			meshList[GEO_QUAD]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
			meshList[GEO_QUAD]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
			RenderMesh(meshList[GEO_QUAD], true);
			modelStack.PopMatrix();
		}

		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_HEMISPHERE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-0.5, -0.325, 0.9);
		modelStack.Rotate(-30, 0, 0, 1);
		modelStack.Scale(0.12, 0.1, 0.01);

		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0.5, 1);
			modelStack.Scale(2, 1.5, 2);
			meshList[GEO_QUAD]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
			meshList[GEO_QUAD]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
			meshList[GEO_QUAD]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
			RenderMesh(meshList[GEO_QUAD], true);
			modelStack.PopMatrix();
		}

		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_HEMISPHERE], true);
		modelStack.PopMatrix();
	}
	// Right Ear
	{
		modelStack.PushMatrix();
		modelStack.Translate(-0.9, 0, 0);
		modelStack.Rotate(80, 1, 0, 0);
		modelStack.Rotate(60, 0, 0, 1);
		modelStack.Scale(0.175, 0.175, 0.225);

		{
			modelStack.PushMatrix();
			modelStack.Scale(1, 0.05, 1);
			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.0894f, 0.071f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.6f, 0.5364f, 0.426f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.3f, 0.268f, 0.213f);
			RenderMesh(meshList[GEO_CYLINDER], true);
			modelStack.PopMatrix();
		}

		meshList[GEO_TORUS]->material.kAmbient.Set(0.1f, 0.0894f, 0.071f);
		meshList[GEO_TORUS]->material.kDiffuse.Set(0.6f, 0.5364f, 0.426f);
		meshList[GEO_TORUS]->material.kSpecular.Set(0.3f, 0.268f, 0.213f);
		RenderMesh(meshList[GEO_TORUS], true);
		modelStack.PopMatrix();
	}

	// Left Ear
	{
		modelStack.PushMatrix();
		modelStack.Translate(0.9, 0, 0);
		modelStack.Rotate(-80, 1, 0, 0);
		modelStack.Rotate(60, 0, 0, 1);
		modelStack.Scale(0.175, 0.175, 0.225);

		{
			modelStack.PushMatrix();
			modelStack.Scale(1, 0.05, 1);
			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.0894f, 0.071f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.6f, 0.5364f, 0.426f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.3f, 0.268f, 0.213f);
			RenderMesh(meshList[GEO_CYLINDER], true);
			modelStack.PopMatrix();
		}

		meshList[GEO_TORUS]->material.kAmbient.Set(0.1f, 0.0894f, 0.071f);
		meshList[GEO_TORUS]->material.kDiffuse.Set(0.6f, 0.5364f, 0.426f);
		meshList[GEO_TORUS]->material.kSpecular.Set(0.3f, 0.268f, 0.213f);
		RenderMesh(meshList[GEO_TORUS], true);
		modelStack.PopMatrix();
	}

	modelStack.Scale(0.9, 0.9, 0.9);
	meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.0894f, 0.071f);
	meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.5364f, 0.426f);
	meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.268f, 0.213f);
	RenderMesh(meshList[GEO_SPHERE], true);
	modelStack.PopMatrix();
}

void Assignment::ModelRun(double dt)
{
	time += dt * 15;
	bodyAngle = 20;
	headAngle = -15;

	rightShoulderAngle = 90 * sin(time);
	rightElbowAngle = 30;

	leftShoulderAngle = 90 * sin(time);
	leftElbowAngle = -30;

	leftHipAngle = 75 * sin(time) - 15;
	leftKneeAngle = 17.5 * sin(time) + 22.5;

	rightHipAngle = -75 * sin(time) - 15;
	rightKneeAngle = -17.5 * sin(time) + 22.5;

	MarioPos.y = 0.2 * sin(time * 2) + 0.1;

	if (bodyDirectionAngle == 0) {
		MarioPos.z += dt * 10;
	}

	if (bodyDirectionAngle == 90) {
		MarioPos.x += dt * 10;
	}

	if (bodyDirectionAngle == 180) {
		MarioPos.z += -dt * 10;
	}

	if (bodyDirectionAngle == 270) {
		MarioPos.x += -dt * 10;
	}

}

void Assignment::ModelJump(double dt)
{
	time += dt * 12;

	MarioPos.y = 5 * sin(time) + 5;
	bodyAngle = -(7.5 * sin(time) + 7.5);

	rightHipAngle = -(50 * sin(time) + 50);
	rightKneeAngle = (35 * sin(time) + 35);

	leftHipAngle = (25 * sin(time) + 25);
	leftKneeAngle = (10 * sin(time) + 10);

	rightShoulderAngle = (35 * sin(time) + 35);
	leftShoulderAngle = (70 * sin(time) + 70);

	if (time > 4.5) {
		jump = false;
		MarioPos.y = 0;
		return;
	}

}

void Assignment::ModelScale(double dt)
{
	frames += 2.5;
	
	switch (frames) {
	case 10:
		bodySize = 1.33;
		break;
	case 20:
		bodySize = 1;
		break;
	case 30:
		bodySize = 1.66;
		break;
	case 40:
		bodySize = 1.33;
		break;
	case 50:
		bodySize = 2;

		break;
	}
		

}

void Assignment::ResetAnimation()
{
	bodyAngle = 0;
	headAngle = 0;

	// Right Arm
	rightShoulderAngle = 0;
	leftShoulderAngle = 0;

	// Left Arm
	rightElbowAngle = 0;
	leftElbowAngle = 0;

	// Right Leg
	rightHipAngle = 0;
	leftHipAngle = 0;

	// Left Leg
	rightKneeAngle = 0;
	leftKneeAngle = 0;
}

void Assignment::RenderFireFlower()
{

	// Stalk
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.75, 0);

	// Leaf
	{
		modelStack.PushMatrix();
		modelStack.Translate(0.15, -.1, 0);
		modelStack.Rotate(-25, 0, 0, 1);
		modelStack.Scale(0.1, 0.1, 0.1);

		{
			modelStack.PushMatrix();
			modelStack.Scale(1, 1, 0.1);
			meshList[GEO_CUBE]->material.kAmbient.Set(0.f, 0.1f, 0.f);
			meshList[GEO_CUBE]->material.kDiffuse.Set(0.f, 0.9f, 0.f);
			meshList[GEO_CUBE]->material.kSpecular.Set(0.f, 0.3f, 0.f);
			RenderMesh(meshList[GEO_CUBE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(2, 2, 0);
			modelStack.Scale(1, 1, 0.1);
			meshList[GEO_CUBE]->material.kAmbient.Set(0.f, 0.1f, 0.f);
			meshList[GEO_CUBE]->material.kDiffuse.Set(0.f, 0.9f, 0.f);
			meshList[GEO_CUBE]->material.kSpecular.Set(0.f, 0.3f, 0.f);
			RenderMesh(meshList[GEO_CUBE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(1, 1, 0);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(2, 0.1, 2);
			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.f, 0.1f, 0.f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.f, 0.9f, 0.f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.f, 0.3f, 0.f);
			RenderMesh(meshList[GEO_CYLINDER], true);
			modelStack.PopMatrix();
		}

		modelStack.PopMatrix();
	}

	// Leaf
	{
		modelStack.PushMatrix();
		modelStack.Translate(-0.15, -.1, 0);
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Rotate(-25, 0, 0, 1);
		modelStack.Scale(0.1, 0.1, 0.1);

		{
			modelStack.PushMatrix();
			modelStack.Scale(1, 1, 0.1);
			meshList[GEO_CUBE]->material.kAmbient.Set(0.f, 0.1f, 0.f);
			meshList[GEO_CUBE]->material.kDiffuse.Set(0.f, 0.9f, 0.f);
			meshList[GEO_CUBE]->material.kSpecular.Set(0.f, 0.3f, 0.f);
			RenderMesh(meshList[GEO_CUBE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(2, 2, 0);
			modelStack.Scale(1, 1, 0.1);
			meshList[GEO_CUBE]->material.kAmbient.Set(0.f, 0.1f, 0.f);
			meshList[GEO_CUBE]->material.kDiffuse.Set(0.f, 0.9f, 0.f);
			meshList[GEO_CUBE]->material.kSpecular.Set(0.f, 0.3f, 0.f);
			RenderMesh(meshList[GEO_CUBE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(1, 1, 0);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(2, 0.1, 2);
			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.f, 0.1f, 0.f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.f, 0.9f, 0.f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.f, 0.3f, 0.f);
			RenderMesh(meshList[GEO_CYLINDER], true);
			modelStack.PopMatrix();
		}

		modelStack.PopMatrix();
	}

	// Head
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0.75, 0);

		{
			// Eye
			{
				modelStack.PushMatrix();
				modelStack.Translate(0.1, 0, 0.2);
				modelStack.Scale(0.05, 0.15, 0.05);
				meshList[GEO_SPHERE]->material.kAmbient.Set(0.05f, 0.05f, 0.05f);
				meshList[GEO_SPHERE]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
				meshList[GEO_SPHERE]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);
				RenderMesh(meshList[GEO_SPHERE], true);
				modelStack.PopMatrix();
			}

			// Eye
			{
				modelStack.PushMatrix();
				modelStack.Translate(-0.1, 0, 0.2);
				modelStack.Scale(0.05, 0.15, 0.05);
				meshList[GEO_SPHERE]->material.kAmbient.Set(0.05f, 0.05f, 0.05f);
				meshList[GEO_SPHERE]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
				meshList[GEO_SPHERE]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);
				RenderMesh(meshList[GEO_SPHERE], true);
				modelStack.PopMatrix();
			}
		}

		modelStack.Scale(0.3, 0.24, 0.24);

		// Yellow Ring
		{
			modelStack.PushMatrix();
			modelStack.Rotate(90, 1, 0, 0);
			meshList[GEO_TORUS2]->material.kAmbient.Set(0.1f, 0.1f, 0.f);
			meshList[GEO_TORUS2]->material.kDiffuse.Set(0.6f, 0.6f, 0.f);
			meshList[GEO_TORUS2]->material.kSpecular.Set(0.3f, 0.3f, 0.f);
			RenderMesh(meshList[GEO_TORUS2], true);
			modelStack.PopMatrix();
		}

		// Red Ring
		{
			modelStack.PushMatrix();
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(1.6, 1.6, 1.6);
			meshList[GEO_TORUS]->material.kAmbient.Set(0.1f, 0.f, 0.f);
			meshList[GEO_TORUS]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
			meshList[GEO_TORUS]->material.kSpecular.Set(0.3f, 0.f, 0.f);
			RenderMesh(meshList[GEO_TORUS], true);
			modelStack.PopMatrix();
		}

		meshList[GEO_SPHERE]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		RenderMesh(meshList[GEO_SPHERE], true);
		modelStack.PopMatrix();
	}

	modelStack.Scale(0.05, 0.75, 0.05);
	meshList[GEO_CYLINDER]->material.kAmbient.Set(0.f, 0.1f, 0.f);
	meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.f, 0.9f, 0.f);
	meshList[GEO_CYLINDER]->material.kSpecular.Set(0.f, 0.3f, 0.f);
	RenderMesh(meshList[GEO_CYLINDER], true);
	modelStack.PopMatrix();
}

void Assignment::RenderQuestionMark() {
	{
		modelStack.PushMatrix();
		modelStack.Scale(1, 1, 0.4);

		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 3, 0);

			{
				modelStack.PushMatrix();
				modelStack.Translate(0.5, 1.5, 0);
				modelStack.Scale(0.5, 0.5, 1);

				{
					modelStack.PushMatrix();
					modelStack.Translate(3, 3, 0);
					modelStack.Scale(2, 4, 1);
					meshList[GEO_CUBE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
					meshList[GEO_CUBE]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
					meshList[GEO_CUBE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
					RenderMesh(meshList[GEO_CUBE], true);
					modelStack.PopMatrix();
				}

				{
					modelStack.PushMatrix();
					modelStack.Translate(-2, 8, 0);

					{
						modelStack.PushMatrix();
						modelStack.Translate(-5, -4, 0);
						modelStack.Scale(2, 3, 1);
						meshList[GEO_CUBE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
						meshList[GEO_CUBE]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
						meshList[GEO_CUBE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
						RenderMesh(meshList[GEO_CUBE], true);
						modelStack.PopMatrix();
					}

					modelStack.Scale(5, 1, 1);
					meshList[GEO_CUBE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
					meshList[GEO_CUBE]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
					meshList[GEO_CUBE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
					RenderMesh(meshList[GEO_CUBE], true);
					modelStack.PopMatrix();
				}

				meshList[GEO_CUBE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
				meshList[GEO_CUBE]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
				meshList[GEO_CUBE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
				RenderMesh(meshList[GEO_CUBE], true);
				modelStack.PopMatrix();
			}

			meshList[GEO_CUBE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
			meshList[GEO_CUBE]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
			meshList[GEO_CUBE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
			RenderMesh(meshList[GEO_CUBE], true);
			modelStack.PopMatrix();
		}

		meshList[GEO_CUBE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_CUBE]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_CUBE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
		RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();
	}
}

void Assignment::RenderBlocks() {
	modelStack.PushMatrix();
	modelStack.Scale(0.3, 0.3, 0.3);

	modelStack.PushMatrix();
	modelStack.Translate(0.5, -4, 6);
	RenderQuestionMark();
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(0.5, -4, 6);
	RenderQuestionMark();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Translate(0.5, -4, 6);
	RenderQuestionMark();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Translate(0.5, -4, 6);
	RenderQuestionMark();
	modelStack.PopMatrix();

	modelStack.Scale(6, 6, 6);
	meshList[GEO_CUBE]->material.kAmbient.Set(.3f, 0.2529f, 0.0f);
	meshList[GEO_CUBE]->material.kDiffuse.Set(0.9f, 0.7587f, 0.f);
	meshList[GEO_CUBE]->material.kSpecular.Set(0.7f, 0.590f, 0.f);
	RenderMesh(meshList[GEO_CUBE], true);
	modelStack.PopMatrix();
}