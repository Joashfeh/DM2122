#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "Light.h"
#include "Scene.h"
#include "Camera2.h"
#include "Mesh.h"
#include "MatrixStack.h"

class Assignment : public Scene {

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
		U_TOTAL,
	};

	enum GEOMETRY_TYPE {
		GEO_AXES,
		GEO_FLOOR,
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
		NUM_GEOMETRY,
	};

public:
	Assignment();
	~Assignment();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:

	Position MarioPos;
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

	Light light[2];
	MS modelStack, viewStack, projectionStack;

	Camera2 camera;

	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_vertexArrayID;
	unsigned m_parameters[U_TOTAL];
	unsigned m_programID;

	void RenderMesh(Mesh* mesh, bool enableLight);
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