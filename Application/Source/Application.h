	
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

enum SCENE {
	UI,
	GAME,
	ASSIGNMENT1,
};

class Application
{
public:

	static SCENE sceneType;

	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static bool IsKeyReleased(unsigned short key);

	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double* xpos, double* ypos);
	static unsigned m_width;
	static unsigned m_height;

	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	static double camera_yaw, camera_pitch;

	const static int m_window_deadzone = 100;
	const static int m_window_width = 800;
	const static int m_window_height = 600;

	static int GetWindowWidth();
	static int GetWindowHeight();

private:

	//Declare a window object
	bool GetMouseUpdate();
	StopWatch m_timer;
	
};

#endif