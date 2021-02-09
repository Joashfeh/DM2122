#include "Assignment2.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "UpdateHandler.h"
#include "LoadTextData.h"
#include <sstream>
#include <iomanip>
#include "Blocks.h"
#include "Goomba.h"
#include "Fireball.h"
#include "QuestionBlock.h"

double pipeTime = 0;
int Assignment2::frames;
Mario Assignment2::player;
std::vector<Entities*> Assignment2::World;

Assignment2::Assignment2()
{
}

Assignment2::~Assignment2()
{
}

void Assignment2::Init()
{
	// Variable Initialisation
	{
		player.position.Set(-15, 10, 0);

		bodyDirectionAngle = 90;
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

		time = 0;
		hitTimer = 0;
		translateX = 0;
		translateY = 0;
		translateZ = 0;

		flagHeight = 0;

		toggleLight = true;
	}

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
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
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	Mesh::SetMaterialLoc(
		m_parameters[U_MATERIAL_AMBIENT],
		m_parameters[U_MATERIAL_DIFFUSE],
		m_parameters[U_MATERIAL_SPECULAR],
		m_parameters[U_MATERIAL_SHININESS]
	);

	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(0, 1000, 1000);
	light[0].color.Set(0.8, 0.8, 0.8);
	light[0].power = 1.0f;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 1.f;
	light[0].spotDirection.Set(0.f, 0.f, 0.f);

	light[1].type = Light::LIGHT_POINT;
	light[1].position.Set(0, 50, 50);
	light[1].color.Set(1, 1, 1);
	light[1].power = 1.0f;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 1.f;
	light[1].spotDirection.Set(2.f, 1.f, 0.f);

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 1000, 1000, 1000);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("Light ball", Color(1, 1, 1), 16, 16);

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

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);

	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");

	meshList[GEO_BRICK] = MeshBuilder::GenerateOBJMTL("brick", "OBJ//Brick.obj", "OBJ//Brick.mtl");

	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("Circle", Color(1, 1, 1), 48);
	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("Floor", Color(1, 1, 1), 1.f);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//Arial.tga");

	meshList[GEO_GOOMBA] = MeshBuilder::GenerateOBJMTL("goomba", "OBJ//goomba.obj", "OBJ//goomba.mtl");
	meshList[GEO_GOOMBA]->textureID = LoadTGA("Image//goomba.tga");

	meshList[GEO_FIREBALL] = MeshBuilder::GenerateOBJMTL("fireball", "OBJ//fireball_mario.obj", "OBJ//fireball_mario.mtl");
	meshList[GEO_FIREBALL]->textureID = LoadTGA("Image//fireball_mario.tga");

	meshList[GEO_STAR] = MeshBuilder::GenerateOBJMTL("MarioStar", "OBJ//star.obj", "OBJ//star.mtl");
	meshList[GEO_PIPE] = MeshBuilder::GenerateOBJMTL("Pipe", "OBJ//pipe.obj", "OBJ//pipe.mtl");

	meshList[GEO_MUSHROOM] = MeshBuilder::GenerateOBJMTL("Mushroom", "OBJ//powerup.obj", "OBJ//powerup.mtl");
	meshList[GEO_MUSHROOM]->textureID = LoadTGA("Image//skin.tga");

	meshList[GEO_COIN] = MeshBuilder::GenerateOBJMTL("coin", "OBJ//coin.obj", "OBJ//coin.mtl");

	meshList[GEO_FLAGPOLE] = MeshBuilder::GenerateOBJMTL("flagpole", "OBJ//flagpole.obj", "OBJ//flagpole.mtl");
	meshList[GEO_FLAG] = MeshBuilder::GenerateOBJMTL("flag", "OBJ//flag.obj", "OBJ//flag.mtl");

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	camera.Init(Vector3(0, player.position.y + 2.4, -15), Vector3(0, player.position.y + 2.4, 0), Vector3(0, 1, 0));

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
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);

	Generate1_1();

}
           
void Assignment2::Update(double dt) {

	light[1].position.Set(camera.position.x, 50, 30);

	if (player.dead && camera.type == SIDESCROLL) {
		ResetGame();
		player.dead = false;
	}

	if (player.hitTimerActive) {
		hitTimer += dt;
		if (hitTimer > 1) {
			hitTimer = 0;
			player.hitTimerActive = false;
		}

	}

	if (player.position.x < camera.position.x - 18 && camera.type == SIDESCROLL)
		player.position.x = camera.position.x - 18;

	// THIS IS SO BAD
	// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	if (player.canMove && camera.type == SIDESCROLL) {
		UpdateHandler(bodyDirectionAngle, jump, dt);
		for (int i = 0; i < World.size(); ++i) {
			if (World[i] != nullptr) {
				// success checks if the world pointer should be set to null after object deletion to save memory space
				bool success = player.Collision(*World[i]);
				if (success) {
					World[i] = nullptr;
					continue;
				}

				if (World[i]->type == GOOMBA) {
					if (((Goomba*)World[i])->defaultPosition.x - World[i]->position.x < 10 && ((Goomba*)World[i])->bodyAngle == -90)
						World[i]->position.x -= dt * 2;

					else
						((Goomba*)World[i])->bodyAngle = 90;

					if (World[i]->position.x - ((Goomba*)World[i])->defaultPosition.x < 0 && ((Goomba*)World[i])->bodyAngle == 90)
						World[i]->position.x += dt * 2;

					else
						((Goomba*)World[i])->bodyAngle = -90;
				}
			}
		}
	}

	// IOSADNGOIPDGNPOANHGIPFUADEJHBIOPJAJH
	else {
		ResetAnimation();
		if (player.Win) {
		
			rightShoulderAngle = -70;
			leftShoulderAngle = 70;

			rightHipAngle = -70;
			leftHipAngle = -70;

			if (player.position.y > 3) {
				if (flagHeight + 18 - player.position.y < 2 && flagHeight + 18 > 2)
					flagHeight -= dt * 9;

				player.position.y -= dt * 9;

			}
		}

		// OH MY GOD WHY
		if (player.pipeAnimationActive) {
			if (pipeTime < 1.25) {
				player.position.y -= dt * 9;
			}
			else if (pipeTime > 1.25 && pipeTime < 1.3) {
				player.position.x = 317;
				player.position.y = -1;
			}
			else {
				if (player.position.y < 5.5)
					player.position.y += dt * 9;
				else {
					player.pipeAnimationActive = false;
					player.canMove = true;
				}
			}

			pipeTime += dt;
		}
		// i need a proper engine
	}

	for (int i = 0; i < World.size(); ++i) {
		if (World[i] != nullptr) {
			if (World[i]->type == FIREBALL) {
				((Fireball*)World[i])->Update(bodyDirectionAngle);
				if ((World[i]->position.x > 500) || (World[i]->position.x < -16)) {
					delete World[i];
					World[i] = nullptr;
					continue;
				}

				for (int k = 0; k < World.size(); ++k) {
					if (World[k] != nullptr) {
						if (((Fireball*)World[i])->AABB(*World[i], *World[k]) && World[k]->type != FIREBALL) {
							if (World[k]->type == GOOMBA) {
								delete World[k];
								World[k] = nullptr;
							}
							delete World[i];
							World[i] = nullptr;
							break;
						}
					}
				}
			}
		}
	}

	UpdateStarAnimation(dt);

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

	if (Application::IsKeyPressed('`')) {
		if (toggleLight == true)
			toggleLight = false;

		else if(toggleLight == false)
			toggleLight = true;
	}

	// Movement Control
	{
		if (Application::IsKeyPressed('A'))
		{
			running = true;
		}
		if (Application::IsKeyPressed('D'))
		{
			running = true;
		}
		else if (Application::IsKeyReleased('A') && Application::IsKeyReleased('D'))
			running = false;
	}

	// Function Updates
	if (player.canMove)
	{
		if (running && jump)
			ModelJump(dt);

		if (running && !jump)
			ModelRun(dt);

		if (player.scaled)
			ModelScale(dt);

		if (jump)
			ModelJump(dt);

		else if (!running && !jump) {
			ResetAnimation();
			time = 0;
		}
	}

	// Reset
	if (Application::IsKeyPressed('R')) {
		ResetGame();
	}

	camera.Update(dt);
		
}

void Assignment2::Render() {

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

	RenderSkybox();
	RenderMario();
	RenderBlocks();

	//RenderFloor();
	//RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	modelStack.Scale(0.1, 0.1, 0.1);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	std::stringstream ss;
	ss.precision(6);
	ss << "SCORE: " << std::setw(6) << std::setfill('0') << player.Score;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 4, 2, 55);


	Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);

}

void Assignment2::Exit()
{
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

}

void Assignment2::RenderMesh(Mesh* mesh, bool enableLight)
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

	// Texture
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

void Assignment2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	LoadTextData(textWidthData, "TextData//ArialFontData.csv");
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	float accumulator = 0;
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.5f + accumulator, 0.5f, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);

		accumulator += textWidthData[text[i]] / 64.0f;
	}

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void Assignment2::RenderSkybox()
{

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);

	modelStack.PushMatrix();
	modelStack.Translate(-500, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -500);
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Rotate(0, 0, 1, 0);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 500);
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Rotate(-180, 0, 1, 0);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 500, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -500, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void Assignment2::RenderFloor() {
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_FLOOR], toggleLight);
	modelStack.PopMatrix();
}

void Assignment2::RenderMario() {


	// Body
	modelStack.PushMatrix();
	modelStack.Translate(player.position.x, player.position.y - 1.5, player.position.z);
	modelStack.Rotate(bodyDirectionAngle, 0, 1, 0);
	modelStack.Scale(player.bodySize, player.bodySize, player.bodySize);
	modelStack.Translate(0, 2.4, 0);
	modelStack.Rotate(bodyAngle, 1, 0, 0);
	{
		// Bottom Hemisphere
		modelStack.PushMatrix();
		modelStack.Scale(1, 1, 0.8);
		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.f, 0.f, 0.1f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.f, 0.f, 0.3f);
		RenderMesh(meshList[GEO_HEMISPHERE], toggleLight);

		// Top Hemisphere
		modelStack.PushMatrix();
		modelStack.Translate(0, 0.4, 0);
		modelStack.Rotate(180, 1, 0, 0);
		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.1f, 0.f, 0.f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.3f, 0.f, 0.f);
		RenderMesh(meshList[GEO_HEMISPHERE], toggleLight);
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
				RenderMesh(meshList[GEO_TORUS], toggleLight);
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
				RenderMesh(meshList[GEO_TORUS], toggleLight);
				modelStack.PopMatrix();
			}

			modelStack.Rotate(180, 1, 0, 0);
			modelStack.Scale(1, 0.3, 1);
			meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.f, 0.f, 0.1f);
			meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
			meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.f, 0.f, 0.3f);
			RenderMesh(meshList[GEO_HEMISPHERE], toggleLight);
			modelStack.PopMatrix();
		}

		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Scale(1, 0.3, 1);
		meshList[GEO_CYLINDER]->material.kAmbient.Set(0.f, 0.f, 0.1f);
		meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
		meshList[GEO_CYLINDER]->material.kSpecular.Set(0.f, 0.f, 0.3f);
		RenderMesh(meshList[GEO_CYLINDER], toggleLight);
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
		RenderMesh(meshList[GEO_SPHERE], toggleLight);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Rotate(25, 0, 1, 0);
		modelStack.Translate(0, 0.5, 1);
		modelStack.Rotate(-14, 1, 0, 0);
		modelStack.Scale(0.15, 0.15, 0.05);
		meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.6f, 0.f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.6f, 0.6f, 0.f);
		RenderMesh(meshList[GEO_SPHERE], toggleLight);
		modelStack.PopMatrix();

	}

	RenderRightLeg();
	RenderLeftLeg();
	RenderRightArm();
	RenderLeftArm();
	RenderHead();
	modelStack.PopMatrix();
}

void Assignment2::RenderLeftArm()
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
							RenderMesh(meshList[GEO_SPHERE], toggleLight);
							modelStack.PopMatrix();

						}

						meshList[GEO_TORUS]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
						meshList[GEO_TORUS]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
						meshList[GEO_TORUS]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
						RenderMesh(meshList[GEO_TORUS], toggleLight);
						modelStack.PopMatrix();
					}

					meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.f, 0.f);
					meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
					meshList[GEO_CYLINDER]->material.kSpecular.Set(0.3f, 0.f, 0.f);
					RenderMesh(meshList[GEO_CYLINDER], toggleLight);
					modelStack.PopMatrix();
				}

				meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.f, 0.f);
				meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
				meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.f, 0.f);
				RenderMesh(meshList[GEO_SPHERE], toggleLight);
				modelStack.PopMatrix();
			}

			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.f, 0.f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.3f, 0.f, 0.f);
			RenderMesh(meshList[GEO_CYLINDER], toggleLight);
			modelStack.PopMatrix();
		}

		meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.f, 0.f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.f, 0.f);
		RenderMesh(meshList[GEO_SPHERE], toggleLight);

		modelStack.PopMatrix();
		modelStack.PopMatrix();

	}
	modelStack.PopMatrix();
}				

void Assignment2::RenderRightLeg()
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
									RenderMesh(meshList[GEO_HEMISPHERE], toggleLight);
									modelStack.PopMatrix();
								}

								modelStack.Scale(1, 0.4, 1);
								meshList[GEO_CYLINDER]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
								meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
								meshList[GEO_CYLINDER]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
								RenderMesh(meshList[GEO_CYLINDER], toggleLight);
								modelStack.PopMatrix();

								modelStack.PushMatrix();
								modelStack.Translate(0, 0.4, 0.2);
								modelStack.Rotate(90, 1, 0, 0);
								modelStack.Scale(1, 0.5, 0.5);
								meshList[GEO_CYLINDER]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
								meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
								meshList[GEO_CYLINDER]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
								RenderMesh(meshList[GEO_CYLINDER], toggleLight);
								modelStack.PopMatrix();

							}

							modelStack.Scale(1, 0.4, 0.625);
							meshList[GEO_CUBE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
							meshList[GEO_CUBE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
							meshList[GEO_CUBE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
							RenderMesh(meshList[GEO_CUBE], toggleLight);
							modelStack.PopMatrix();


						}

						modelStack.Scale(1, 0.4, 1);
						meshList[GEO_CYLINDER]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
						meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
						meshList[GEO_CYLINDER]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
						RenderMesh(meshList[GEO_CYLINDER], toggleLight);
						modelStack.PopMatrix();

					}

					meshList[GEO_CYLINDER]->material.kAmbient.Set(0.f, 0.f, 0.1f);
					meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
					meshList[GEO_CYLINDER]->material.kSpecular.Set(0.f, 0.f, 0.3f);
					RenderMesh(meshList[GEO_CYLINDER], toggleLight);
					modelStack.PopMatrix();
				}

				meshList[GEO_SPHERE]->material.kAmbient.Set(0.f, 0.f, 0.1f);
				meshList[GEO_SPHERE]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
				meshList[GEO_SPHERE]->material.kSpecular.Set(0.f, 0.f, 0.3f);
				RenderMesh(meshList[GEO_SPHERE], toggleLight);
				modelStack.PopMatrix();
			}

			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.f, 0.f, 0.1f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.f, 0.f, 0.3f);
			RenderMesh(meshList[GEO_CYLINDER], toggleLight);
			modelStack.PopMatrix();

		}

		meshList[GEO_SPHERE]->material.kAmbient.Set(0.f, 0.f, 0.1f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.f, 0.f, 0.3f);
		RenderMesh(meshList[GEO_SPHERE], toggleLight);
		modelStack.PopMatrix();

	}
}

void Assignment2::RenderLeftLeg()
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
									RenderMesh(meshList[GEO_HEMISPHERE], toggleLight);
									modelStack.PopMatrix();
								}

								modelStack.Scale(1, 0.4, 1);
								meshList[GEO_CYLINDER]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
								meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
								meshList[GEO_CYLINDER]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
								RenderMesh(meshList[GEO_CYLINDER], toggleLight);
								modelStack.PopMatrix();

								modelStack.PushMatrix();
								modelStack.Translate(0, 0.4, 0.2);
								modelStack.Rotate(90, 1, 0, 0);
								modelStack.Scale(1, 0.5, 0.5);
								meshList[GEO_CYLINDER]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
								meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
								meshList[GEO_CYLINDER]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
								RenderMesh(meshList[GEO_CYLINDER], toggleLight);
								modelStack.PopMatrix();

							}

							modelStack.Scale(1, 0.4, 0.625);
							meshList[GEO_CUBE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
							meshList[GEO_CUBE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
							meshList[GEO_CUBE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
							RenderMesh(meshList[GEO_CUBE], toggleLight);
							modelStack.PopMatrix();


						}

						modelStack.Scale(1, 0.4, 1);
						meshList[GEO_CYLINDER]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
						meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
						meshList[GEO_CYLINDER]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
						RenderMesh(meshList[GEO_CYLINDER], toggleLight);
						modelStack.PopMatrix();

					}

					meshList[GEO_CYLINDER]->material.kAmbient.Set(0.f, 0.f, 0.1f);
					meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
					meshList[GEO_CYLINDER]->material.kSpecular.Set(0.f, 0.f, 0.3f);
					RenderMesh(meshList[GEO_CYLINDER], toggleLight);
					modelStack.PopMatrix();
				}

				meshList[GEO_SPHERE]->material.kAmbient.Set(0.f, 0.f, 0.1f);
				meshList[GEO_SPHERE]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
				meshList[GEO_SPHERE]->material.kSpecular.Set(0.f, 0.f, 0.3f);
				RenderMesh(meshList[GEO_SPHERE], toggleLight);
				modelStack.PopMatrix();
			}

			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.f, 0.f, 0.1f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.f, 0.f, 0.3f);
			RenderMesh(meshList[GEO_CYLINDER], toggleLight);
			modelStack.PopMatrix();

		}

		meshList[GEO_SPHERE]->material.kAmbient.Set(0.f, 0.f, 0.1f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.f, 0.f, 0.6f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.f, 0.f, 0.3f);
		RenderMesh(meshList[GEO_SPHERE], toggleLight);
		modelStack.PopMatrix();

	}
}		

void Assignment2::RenderRightArm()
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
							RenderMesh(meshList[GEO_SPHERE], toggleLight);
							modelStack.PopMatrix();

						}

						meshList[GEO_TORUS]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
						meshList[GEO_TORUS]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
						meshList[GEO_TORUS]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
						RenderMesh(meshList[GEO_TORUS], toggleLight);
						modelStack.PopMatrix();
					}

					meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.f, 0.f);
					meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
					meshList[GEO_CYLINDER]->material.kSpecular.Set(0.3f, 0.f, 0.f);
					RenderMesh(meshList[GEO_CYLINDER], toggleLight);
					modelStack.PopMatrix();
				}

				meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.f, 0.f);
				meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
				meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.f, 0.f);
				RenderMesh(meshList[GEO_SPHERE], toggleLight);
				modelStack.PopMatrix();
			}

			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.f, 0.f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.3f, 0.f, 0.f);
			RenderMesh(meshList[GEO_CYLINDER], toggleLight);
			modelStack.PopMatrix();
		}

		meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.f, 0.f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.f, 0.f);
		RenderMesh(meshList[GEO_SPHERE], toggleLight);

		modelStack.PopMatrix();
		modelStack.PopMatrix();

	}
}
		
void Assignment2::RenderHead() {
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
		RenderMesh(meshList[GEO_SPHERE], toggleLight);
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
		RenderMesh(meshList[GEO_TORUS2], toggleLight);
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
		RenderMesh(meshList[GEO_SPHERE], toggleLight);
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
			RenderMesh(meshList[GEO_SPHERE], toggleLight);
			modelStack.PopMatrix();
		}

		modelStack.Scale(0.15, 0.4, 0.1);
		meshList[GEO_SPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_SPHERE], toggleLight);
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
			RenderMesh(meshList[GEO_SPHERE], toggleLight);
			modelStack.PopMatrix();
		}

		modelStack.Scale(0.15, 0.4, 0.1);
		meshList[GEO_SPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_SPHERE], toggleLight);
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
		RenderMesh(meshList[GEO_SPHERE], toggleLight);
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
			RenderMesh(meshList[GEO_CIRCLE], toggleLight);
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
				RenderMesh(meshList[GEO_SPHERE], toggleLight);
				modelStack.PopMatrix();
			}

			meshList[GEO_SPHERE]->material.kShininess = 1.f;
			meshList[GEO_SPHERE]->material.kAmbient.Set(0.f, 0.079f, 0.1f);
			meshList[GEO_SPHERE]->material.kDiffuse.Set(0.f, 0.474f, 0.6f);
			meshList[GEO_SPHERE]->material.kSpecular.Set(0.f, 0.247f, 0.3f);
			RenderMesh(meshList[GEO_SPHERE], toggleLight);
			modelStack.PopMatrix();
		}

		meshList[GEO_SPHERE]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		RenderMesh(meshList[GEO_SPHERE], toggleLight);
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
			RenderMesh(meshList[GEO_CIRCLE], toggleLight);
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
				RenderMesh(meshList[GEO_SPHERE], toggleLight);
				modelStack.PopMatrix();
			}

			meshList[GEO_SPHERE]->material.kShininess = 1.f;
			meshList[GEO_SPHERE]->material.kAmbient.Set(0.f, 0.079f, 0.1f);
			meshList[GEO_SPHERE]->material.kDiffuse.Set(0.f, 0.474f, 0.6f);
			meshList[GEO_SPHERE]->material.kSpecular.Set(0.f, 0.247f, 0.3f);
			RenderMesh(meshList[GEO_SPHERE], toggleLight);
			modelStack.PopMatrix();
		}

		meshList[GEO_SPHERE]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		RenderMesh(meshList[GEO_SPHERE], toggleLight);
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
			RenderMesh(meshList[GEO_TORUS], toggleLight);
			modelStack.PopMatrix();
		}

		{
			modelStack.PushMatrix();
			modelStack.Translate(0, -0.1, 0.2);
			modelStack.Scale(1, 0.02, 1.2);
			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.f, 0.f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.3f, 0.f, 0.f);
			RenderMesh(meshList[GEO_CYLINDER], toggleLight);
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
			RenderMesh(meshList[GEO_SPHERE], toggleLight);
			modelStack.PopMatrix();
		}

		modelStack.Shear('y', 0, 0, 1);
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Scale(1, 0.7, 1);
		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.1f, 0.f, 0.f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.6f, 0.f, 0.f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.3f, 0.f, 0.f);
		RenderMesh(meshList[GEO_HEMISPHERE], toggleLight);
		modelStack.PopMatrix();
	}

	// Left Mustache
	{
		modelStack.PushMatrix();
		modelStack.Translate(0.1, -0.45, 0.9);
		modelStack.Rotate(5, 0, 0, 1);
		modelStack.Scale(0.12, 0.1, 0.01);

		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0.5, 1);
			modelStack.Scale(2, 1.5, 2);
			meshList[GEO_QUAD]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
			meshList[GEO_QUAD]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
			meshList[GEO_QUAD]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
			RenderMesh(meshList[GEO_QUAD], toggleLight);
			modelStack.PopMatrix();
		}

		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_HEMISPHERE], toggleLight);
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
			RenderMesh(meshList[GEO_QUAD], toggleLight);
			modelStack.PopMatrix();
		}

		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_HEMISPHERE], toggleLight);
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
			RenderMesh(meshList[GEO_QUAD], toggleLight);
			modelStack.PopMatrix();
		}

		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_HEMISPHERE], toggleLight);
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
			RenderMesh(meshList[GEO_QUAD], toggleLight);
			modelStack.PopMatrix();
		}

		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_HEMISPHERE], toggleLight);
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
			RenderMesh(meshList[GEO_QUAD], toggleLight);
			modelStack.PopMatrix();
		}

		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_HEMISPHERE], toggleLight);
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
			RenderMesh(meshList[GEO_QUAD], toggleLight);
			modelStack.PopMatrix();
		}

		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.055f, 0.027f, 0.0075f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.33f, 0.16f, 0.045f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.16f, 0.081f, 0.0225f);
		RenderMesh(meshList[GEO_HEMISPHERE], toggleLight);
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
			RenderMesh(meshList[GEO_CYLINDER], toggleLight);
			modelStack.PopMatrix();
		}

		meshList[GEO_TORUS]->material.kAmbient.Set(0.1f, 0.0894f, 0.071f);
		meshList[GEO_TORUS]->material.kDiffuse.Set(0.6f, 0.5364f, 0.426f);
		meshList[GEO_TORUS]->material.kSpecular.Set(0.3f, 0.268f, 0.213f);
		RenderMesh(meshList[GEO_TORUS], toggleLight);
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
			RenderMesh(meshList[GEO_CYLINDER], toggleLight);
			modelStack.PopMatrix();
		}

		meshList[GEO_TORUS]->material.kAmbient.Set(0.1f, 0.0894f, 0.071f);
		meshList[GEO_TORUS]->material.kDiffuse.Set(0.6f, 0.5364f, 0.426f);
		meshList[GEO_TORUS]->material.kSpecular.Set(0.3f, 0.268f, 0.213f);
		RenderMesh(meshList[GEO_TORUS], toggleLight);
		modelStack.PopMatrix();
	}

	modelStack.Scale(0.9, 0.9, 0.9);
	meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.0894f, 0.071f);
	meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.5364f, 0.426f);
	meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.268f, 0.213f);
	RenderMesh(meshList[GEO_SPHERE], toggleLight);
	modelStack.PopMatrix();
}

void Assignment2::RenderQuestionMark() {
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

void Assignment2::RenderQuestionBlock() {
	modelStack.PushMatrix();
	modelStack.Scale(0.18, 0.18, 0.18);

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

void Assignment2::RenderFireFlower()
{

	// Stalk
	modelStack.PushMatrix();
	modelStack.Scale(1.3, 1.3, 1.3);

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
			RenderMesh(meshList[GEO_CUBE], toggleLight);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(2, 2, 0);
			modelStack.Scale(1, 1, 0.1);
			meshList[GEO_CUBE]->material.kAmbient.Set(0.f, 0.1f, 0.f);
			meshList[GEO_CUBE]->material.kDiffuse.Set(0.f, 0.9f, 0.f);
			meshList[GEO_CUBE]->material.kSpecular.Set(0.f, 0.3f, 0.f);
			RenderMesh(meshList[GEO_CUBE], toggleLight);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(1, 1, 0);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(2, 0.1, 2);
			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.f, 0.1f, 0.f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.f, 0.9f, 0.f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.f, 0.3f, 0.f);
			RenderMesh(meshList[GEO_CYLINDER], toggleLight);
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
			RenderMesh(meshList[GEO_CUBE], toggleLight);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(2, 2, 0);
			modelStack.Scale(1, 1, 0.1);
			meshList[GEO_CUBE]->material.kAmbient.Set(0.f, 0.1f, 0.f);
			meshList[GEO_CUBE]->material.kDiffuse.Set(0.f, 0.9f, 0.f);
			meshList[GEO_CUBE]->material.kSpecular.Set(0.f, 0.3f, 0.f);
			RenderMesh(meshList[GEO_CUBE], toggleLight);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(1, 1, 0);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(2, 0.1, 2);
			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.f, 0.1f, 0.f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.f, 0.9f, 0.f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.f, 0.3f, 0.f);
			RenderMesh(meshList[GEO_CYLINDER], toggleLight);
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
				RenderMesh(meshList[GEO_SPHERE], toggleLight);
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
				RenderMesh(meshList[GEO_SPHERE], toggleLight);
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
			RenderMesh(meshList[GEO_TORUS2], toggleLight);
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
			RenderMesh(meshList[GEO_TORUS], toggleLight);
			modelStack.PopMatrix();
		}

		meshList[GEO_SPHERE]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		RenderMesh(meshList[GEO_SPHERE], toggleLight);
		modelStack.PopMatrix();
	}

	modelStack.Scale(0.05, 0.75, 0.05);
	meshList[GEO_CYLINDER]->material.kAmbient.Set(0.f, 0.1f, 0.f);
	meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.f, 0.9f, 0.f);
	meshList[GEO_CYLINDER]->material.kSpecular.Set(0.f, 0.3f, 0.f);
	RenderMesh(meshList[GEO_CYLINDER], toggleLight);
	modelStack.PopMatrix();
}

// this is like
// decent
void Assignment2::RenderBlocks() {
	for (int i = 0; i < World.size(); ++i) {
		if (World[i] != nullptr) {
			switch (World[i]->type) {
			case BLOCK:

				modelStack.PushMatrix();
				modelStack.Translate(World[i]->position.x, World[i]->position.y, World[i]->position.z);
				// modelStack.Scale(1.1, 1.1, 1.1);

				switch (((Blocks*)World[i])->blockType) {
				case BARRIER:
					break;
				case BRICK:
					RenderMesh(meshList[GEO_BRICK], toggleLight);
					break;
				case UNBREAKABLE:
					RenderMesh(meshList[GEO_BRICK], toggleLight);
					break;
				case QUESTION_BLOCK:
					RenderQuestionBlock();
					break;
				case PIPE:
					RenderMesh(meshList[GEO_PIPE], toggleLight);
					break;
				case STAR:
					modelStack.Rotate(((Blocks*)World[i])->starRotateAmount, 0, 1, 0);
					modelStack.Scale(0.8, 0.8, 0.8);
					RenderMesh(meshList[GEO_STAR], toggleLight);
					break;
				case FLOWER:
					modelStack.Rotate(((Blocks*)World[i])->starRotateAmount, 0, 1, 0);
					modelStack.Scale(0.8, 0.8, 0.8);
					RenderFireFlower();
					break;
				case SHROOM:
					modelStack.Rotate(((Blocks*)World[i])->starRotateAmount, 0, 1, 0);
					modelStack.Scale(0.75, 0.75, 0.75);
					modelStack.Translate(0, -1, 0);
					RenderMesh(meshList[GEO_MUSHROOM], toggleLight);
					break;
				case COIN:
					modelStack.Rotate(((Blocks*)World[i])->starRotateAmount, 0, 1, 0);
					RenderMesh(meshList[GEO_COIN], toggleLight);
					break;
				case POLE:
					RenderMesh(meshList[GEO_FLAGPOLE], toggleLight);
					modelStack.PushMatrix();
					modelStack.Translate(0, flagHeight, 0);
					RenderMesh(meshList[GEO_FLAG], toggleLight);
					modelStack.PopMatrix();
				}
				modelStack.PopMatrix();
				break;

			case GOOMBA:
				modelStack.PushMatrix();
				modelStack.Translate(World[i]->position.x, World[i]->position.y + 0.3, World[i]->position.z);
				modelStack.Rotate(((Goomba*)World[i])->bodyAngle, 0, 1, 0);
				modelStack.Scale(0.5, 0.5, 0.5);
				RenderMesh(meshList[GEO_GOOMBA], toggleLight);
				modelStack.PopMatrix();
				break;
			case FIREBALL:
				modelStack.PushMatrix();
				modelStack.Translate(World[i]->position.x, World[i]->position.y - 0.3, World[i]->position.z);
				modelStack.Rotate(-90, 0, 1, 0);
				modelStack.Scale(World[i]->xSize / 4, World[i]->ySize / 4, World[i]->zSize / 4);
				RenderMesh(meshList[GEO_FIREBALL], toggleLight);
				modelStack.PopMatrix();
			}

		}
	}

}

void Assignment2::ModelRun(double dt)
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

	//player.position.y = 0.2 * sin(time * 2) + 0.1;

}

void Assignment2::ModelJump(double dt)
{
	time += dt * 12;

	// MarioPos.y = 5 * sin(time) + 5;
	bodyAngle = -15;

	rightHipAngle = -100;
	rightKneeAngle = 70;

	leftHipAngle = 50;
	leftKneeAngle = 20;

	rightShoulderAngle = 70;
	leftShoulderAngle = 140;

	//if (time > 4.5) {
	//	jump = false;
	//	MarioPos.y = 0;
	//	return;
	//}

	if (player.position.y == 0) {
		jump = false;
	}

}

void Assignment2::ModelScale(double dt)
{
	frames += 2.5;

	switch (frames) {
	case 10:
		player.bodySize = 0.575;
		break;
	case 20:
		player.bodySize = 0.5;
		break;
	case 30:
		player.bodySize = 0.625;
		break;
	case 40:
		player.bodySize = 0.575;
		break;
	case 50:
		player.bodySize = 0.675;
		break;
	}

}

void Assignment2::ResetAnimation()
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

void Assignment2::UpdateStarAnimation(double dt) {
	for (int i = 0; i < World.size(); ++i) {
		if (World[i] != nullptr) {
			if (((Blocks*)World[i])->blockType == STAR || ((Blocks*)World[i])->blockType == FLOWER || ((Blocks*)World[i])->blockType == SHROOM || ((Blocks*)World[i])->blockType == COIN) {
				((Blocks*)World[i])->starRotateAmount += dt * 60;
				((Blocks*)World[i])->timeCounter += dt;

				if (((Blocks*)World[i])->timeCounter < 0.3)
					World[i]->position.y += dt * 8;

				else {
					World[i]->position.y = 0.3 * sin(((Blocks*)World[i])->timeCounter) + ((Blocks*)World[i])->defaultPosition.y;
				}

				if (((Blocks*)World[i])->starRotateAmount > 360)
					((Blocks*)World[i])->starRotateAmount = 0;

			}
		}
	}
}

void Assignment2::ResetGame()
{
	player.Init();
	camera.Reset();
	Generate1_1();
	flagHeight = 0;
	frames = 0;
	pipeTime = 0;
}

// this code sucks
// dont take away marks for this
// .
// please
void Assignment2::Generate1_1() {

	World.clear();

	int generateX = 0;

	// first 69 blocks
	for (int x = generateX; generateX < 122; generateX += 2) {
		for (int y = 1; y < 11; y += 2) {
			World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX, -y, 1), 2, 2, 2));
			World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX, -y, -1), 2, 2, 2));
		}
	}
	World.push_back(new QuestionBlock(GOLDCOIN, Vector3(16, 7, 0), 2, 2, 2));

	World.push_back(new Blocks(BRICK, Vector3(24, 7, 0), 2, 2, 2));
	World.push_back(new QuestionBlock(MUSHROOM, Vector3(26, 7, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(28, 7, 0), 2, 2, 2));
	World.push_back(new QuestionBlock(GOLDCOIN, Vector3(30, 7, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(32, 7, 0), 2, 2, 2));

	World.push_back(new Goomba(Vector3(28, 1, 0)));

	World.push_back(new Blocks(PIPE, Vector3(41, -1, 0), 3.5, 9.5, 3.5));
	World.push_back(new Blocks(PIPE, Vector3(61, 1, 0), 3.5, 9.5, 3.5));
	World.push_back(new Blocks(PIPE, Vector3(77, 3, 0), 3.5, 9.5, 3.5));
	World.push_back(new Blocks(PIPE, Vector3(99, 3, 0), 3.5, 9.5, 3.5, ENTRANCE));

	World.push_back(new Goomba(Vector3(90, 1, 0)));
	World.push_back(new Goomba(Vector3(94, 1, 0)));

	// after 2 empty spaces
	generateX += 8;

	World.push_back(new Blocks(BRICK, Vector3(generateX + 14, 7, 0), 2, 2, 2));
	World.push_back(new QuestionBlock(MUSHROOM, Vector3(generateX + 16, 7, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 18, 7, 0), 2, 2, 2));

	World.push_back(new Blocks(BRICK, Vector3(generateX + 20, 15, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 22, 15, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 24, 15, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 26, 15, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 28, 15, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 30, 15, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 32, 15, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 34, 15, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 36, 15, 0), 2, 2, 2));

	World.push_back(new Goomba(Vector3(generateX + 32, 17, 0)));
	World.push_back(new Goomba(Vector3(generateX + 36, 17, 0)));

	// 15 blocks
	for (int x = generateX; generateX - x < 30; generateX += 2) {
		for (int y = 1; y < 11; y += 2) {
			World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX, -y, 1), 2, 2, 2));
			World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX, -y, -1), 2, 2, 2));
		}
	}

	// 6 empty spaces
	generateX += 12;

	World.push_back(new Blocks(BRICK, Vector3(generateX + 4, 15, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 6, 15, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 8, 15, 0), 2, 2, 2));
	World.push_back(new QuestionBlock(GOLDCOIN, Vector3(generateX + 10, 15, 0), 2, 2, 2));

	World.push_back(new Goomba(Vector3(generateX + 16, 1, 0)));
	World.push_back(new Goomba(Vector3(generateX + 20, 1, 0)));

	World.push_back(new QuestionBlock(GOLDCOIN, Vector3(generateX + 10, 7, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 22, 7, 0), 2, 2, 2));
	World.push_back(new QuestionBlock(SUPERSTAR, Vector3(generateX + 24, 7, 0), 2, 2, 2));

	World.push_back(new QuestionBlock(GOLDCOIN, Vector3(generateX + 34, 7, 0), 2, 2, 2));
	World.push_back(new QuestionBlock(FIREFLOWER, Vector3(generateX + 40, 15, 0), 2, 2, 2));
	World.push_back(new QuestionBlock(GOLDCOIN, Vector3(generateX + 40, 7, 0), 2, 2, 2));
	World.push_back(new QuestionBlock(GOLDCOIN, Vector3(generateX + 46, 7, 0), 2, 2, 2));

	World.push_back(new Goomba(Vector3(generateX + 50, 1, 0)));
	World.push_back(new Goomba(Vector3(generateX + 54, 1, 0)));

	// help

	World.push_back(new Blocks(BRICK, Vector3(generateX + 52, 7, 0), 2, 2, 2));

	World.push_back(new Blocks(BRICK, Vector3(generateX + 58, 15, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 60, 15, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 62, 15, 0), 2, 2, 2));

	World.push_back(new Goomba(Vector3(generateX + 63, 1, 0)));
	World.push_back(new Goomba(Vector3(generateX + 67, 1, 0)));

	World.push_back(new Goomba(Vector3(generateX + 73, 1, 0)));
	World.push_back(new Goomba(Vector3(generateX + 77, 1, 0)));

	World.push_back(new Blocks(BRICK, Vector3(generateX + 70, 15, 0), 2, 2, 2));
	World.push_back(new QuestionBlock(GOLDCOIN, Vector3(generateX + 72, 15, 0), 2, 2, 2));
	World.push_back(new QuestionBlock(GOLDCOIN, Vector3(generateX + 74, 15, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 72, 7, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 74, 7, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 76, 15, 0), 2, 2, 2));

	World.push_back(new Blocks(BRICK, Vector3(generateX + 72, 7, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 74, 7, 0), 2, 2, 2));

	for (int z = -1; z < 2; z += 2) {
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 82, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 84, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 86, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 88, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 84, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 86, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 88, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 86, 5, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 88, 5, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 88, 7, z), 2, 2, 2));
	}

	for (int z = -1; z < 2; z += 2) {
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 94, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 96, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 98, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 100, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 94, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 96, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 98, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 94, 5, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 96, 5, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 94, 7, z), 2, 2, 2));
	}

	for (int z = -1; z < 2; z += 2) {
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 82 + 28, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 84 + 28, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 86 + 28, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 88 + 28, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 84 + 28, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 86 + 28, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 88 + 28, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 86 + 28, 5, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 88 + 28, 5, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 88 + 28, 7, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 90 + 28, 7, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 90 + 28, 5, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 90 + 28, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 90 + 28, 1, z), 2, 2, 2));
	}

	for (int z = -1; z < 2; z += 2) {
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 94 + 34, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 96 + 34, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 98 + 34, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 100 + 34, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 94 + 34, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 96 + 34, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 98 + 34, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 94 + 34, 5, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 96 + 34, 5, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX + 94 + 34, 7, z), 2, 2, 2));
	}

	// next 64 blocks
	for (int x = generateX; generateX - x < 120; generateX += 2) {
		for (int y = 1; y < 11; y += 2) {
			World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX, -y, 1), 2, 2, 2));
			World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX, -y, -1), 2, 2, 2));
		}
	}

	generateX += 8;

	World.push_back(new Blocks(PIPE, Vector3(generateX + 17, -1, 0), 3.5, 9.5, 3.5, EXIT));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 26, 7, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 28, 7, 0), 2, 2, 2));
	World.push_back(new QuestionBlock(GOLDCOIN, Vector3(generateX + 30, 7, 0), 2, 2, 2));
	World.push_back(new Blocks(BRICK, Vector3(generateX + 32, 7, 0), 2, 2, 2));

	World.push_back(new Goomba(Vector3(generateX + 38, 1, 0)));
	World.push_back(new Goomba(Vector3(generateX + 42, 1, 0)));

	World.push_back(new Blocks(PIPE, Vector3(generateX + 49, -1, 0), 3.5, 9.5, 3.5));

	// last staircase
	int tempX = generateX;
	int width = 16;
	int height = 16;

	for (int z = -1; z < 2; z += 2) {
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 52, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 54, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 56, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 58, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 60, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 62, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 64, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 66, 1, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 68, 1, z), 2, 2, 2));

		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 54, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 56, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 58, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 60, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 62, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 64, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 66, 3, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 68, 3, z), 2, 2, 2));

		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 56, 5, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 58, 5, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 60, 5, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 62, 5, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 64, 5, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 66, 5, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 68, 5, z), 2, 2, 2));

		// help
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 58, 7, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 60, 7, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 62, 7, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 64, 7, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 66, 7, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 68, 7, z), 2, 2, 2));
		// me
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 60, 9, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 62, 9, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 64, 9, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 66, 9, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 68, 9, z), 2, 2, 2));
		// please
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 62, 11, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 64, 11, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 66, 11, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 68, 11, z), 2, 2, 2));
		// no
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 64, 13, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 66, 13, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 68, 13, z), 2, 2, 2));

		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 66, 15, z), 2, 2, 2));
		World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 68, 15, z), 2, 2, 2));
	}

	World.push_back(new Blocks(UNBREAKABLE, Vector3(tempX + 86, 1, 0), 2, 2, 2));
	World.push_back(new Blocks(POLE, Vector3(tempX + 86, 11, 0), 0.5, 18, 0.5));

	for (int x = generateX; generateX - x < 106; generateX += 2) {
		for (int y = 1; y < 11; y += 2) {
			World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX, -y, 1), 2, 2, 2));
			World.push_back(new Blocks(UNBREAKABLE, Vector3(generateX, -y, -1), 2, 2, 2));
		}
	}

	for (int i = -24; i < 0; i += 2) {
		for (int y = 1; y < 11; y += 2) {
			World.push_back(new Blocks(UNBREAKABLE, Vector3(i, -y, 1), 2, 2, 2));
			World.push_back(new Blocks(UNBREAKABLE, Vector3(i, -y, -1), 2, 2, 2));
		}
	}

}
