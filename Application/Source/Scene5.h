#ifndef SCENE_5_H
#define SCENE_5_H

#include "Scene.h"
#include "Camera2.h"
#include "Mesh.h"
#include "MatrixStack.h"

class Scene5 : public Scene {

	enum UNIFORM_TYPE {
		U_MVP = 0,
		U_TOTAL,
	};

	enum GEOMETRY_TYPE {
		GEO_AXES,
		GEO_QUAD,
		GEO_CUBE,
		GEO_CIRCLE,
		GEO_RING,
		GEO_SPHERE1,
		GEO_SPHERE2,
		GEO_SPHERE3,
		GEO_SPHERE4,
		NUM_GEOMETRY,
	};

public:
	Scene5();
	~Scene5();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:

	float rotateAngle;
	float translateX;
	float scaleAll;

	MS modelStack, viewStack, projectionStack;

	Camera2 camera;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_vertexArrayID;
	unsigned m_parameters[U_TOTAL];
	unsigned m_programID;
};

#endif