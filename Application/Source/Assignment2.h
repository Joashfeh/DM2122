#ifndef ASSIGNMENT_2_H
#define ASSIGNMENT_2_H

#include "Light.h"
#include "Scene.h"
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
		GEO_MODEL1,
		GEO_MODEL2,
		GEO_MODEL3,
		GEO_MODEL4,
		GEO_MODEL5,
		GEO_MODEL6,
		GEO_BRICK,
		GEO_FLOOR,
		NUM_GEOMETRY,
	};

public:

	std::vector<Entities*> World;
	static Mario player;

	Assignment2();
	~Assignment2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:

	int frames;

	float translateX, translateY, translateZ;
	float bodySize;
	float bodyDirectionAngle;

	float bodyAngle;
	float headAngle;

	// Right Arm
	float rightShoulderAngle;
	float leftShoulderAngle;

	// Left Arm
	float rightElbowAngle;
	float leftElbowAngle;

	// Right Leg
	float rightHipAngle;
	float leftHipAngle;

	// Left Leg
	float rightKneeAngle;
	float leftKneeAngle;

	bool running;
	bool jump;
	bool scaling;

	double time;

	bool toggleLight;
	Light light[2];
	MS modelStack, viewStack, projectionStack;

	Camera5 camera;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_vertexArrayID;
	unsigned m_parameters[U_TOTAL];
	unsigned m_programID;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();
	void RenderFloor();

	void RenderRightArm();
	void RenderLeftArm();
	void RenderRightLeg();
	void RenderLeftLeg();
	void RenderHead();
	void RenderMario();

	void RenderFireFlower();
	void RenderQuestionMark();
	void RenderBlocks();

	void ModelRun(double dt);
	void ModelJump(double dt);
	void ModelScale(double dt);

	void ResetAnimation();

};


#endif