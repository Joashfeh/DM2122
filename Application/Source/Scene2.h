#ifndef SCENE_2_H
#define SCENE_2_H

#include "Scene.h"

class Scene2 : public Scene {

	enum UNIFORM_TYPE {
		U_MVP = 0,
		U_TOTAL,
	};

	enum GEOMETRY_TYPE {
		GEO_TRIANGLE_1,
		GEO_TRIANGLE_2,
		GEO_TRIANGLE_3,
		NUM_GEOMETRY,
	};

public:
	Scene2();
	~Scene2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	float rotateAngle;
	float translateX;
	float scaleAll;

	unsigned m_vertexArrayID;
	unsigned m_vertexBuffer[NUM_GEOMETRY];
	unsigned m_colorBuffer[NUM_GEOMETRY];
	unsigned m_parameters[U_TOTAL];
	unsigned m_programID;
};

#endif