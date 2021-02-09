#ifndef ASSIGNMENT_2_H
#define ASSIGNMENT_2_H

#include "Light.h"
#include "Scene.h"
#include "Camera3.h"
#include "Camera5.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Mario.h"
#include "Entities.h"
#include <vector>

class Assignment2 : public Scene {

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	enum GEOMETRY_TYPE {
		GEO_AXES,
		GEO_QUAD,
		GEO_CUBE,
		GEO_CIRCLE,
		GEO_RING,
		GEO_CONE,
		GEO_TORUS,
		GEO_TORUS2,
		GEO_CYLINDER,
		GEO_SPHERE,
		GEO_HEMISPHERE,
		GEO_LIGHTBALL,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_BLOB,
		GEO_BRICK,
		GEO_PIPE,
		GEO_GOOMBA,
		GEO_FIREBALL,
		GEO_FLOWER,
		GEO_STAR,
		GEO_MUSHROOM,
		GEO_COIN,
		GEO_FLAGPOLE,
		GEO_FLAG,
		GEO_FLOOR,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

public:

	static std::vector<Entities*> World;
	static Mario player;
	static int frames;

	Assignment2();
	~Assignment2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:

	int textWidthData[256];

	float translateX, translateY, translateZ;
	float bodySize;
	float bodyDirectionAngle;
	float bodyAngle;
	float headAngle;
	float rightShoulderAngle;
	float rightElbowAngle;
	float rightHipAngle;
	float rightKneeAngle;
	float leftShoulderAngle;
	float leftElbowAngle;
	float leftHipAngle;
	float leftKneeAngle;

	float flagHeight;

	bool running;
	bool jump;

	double time;
	double hitTimer;

	bool toggleLight;
	Light light[2];
	MS modelStack, viewStack, projectionStack;

	Camera5 camera;

	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_vertexArrayID;
	unsigned m_parameters[U_TOTAL];
	unsigned m_programID;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderSkybox();
	void RenderFloor();

	void RenderRightArm();
	void RenderLeftArm();
	void RenderRightLeg();
	void RenderLeftLeg();
	void RenderHead();
	void RenderMario();
	void RenderQuestionMark();
	void RenderQuestionBlock();
	void RenderFireFlower();
	void RenderBlocks();

	void ModelRun(double dt);
	void ModelJump(double dt);
	void ModelScale(double dt);

	void Generate1_1();
	void UpdateStarAnimation(double dt);
	void ResetAnimation();

	void ResetGame();

};


#endif