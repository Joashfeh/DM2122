	
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static bool IsKeyReleased(unsigned short key);

	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	static double camera_yaw, camera_pitch;

	const static int m_window_deadzone = 100;
	const static int m_window_width = 800;
	const static int m_window_height = 600;

private:

	//Declare a window object
	bool GetMouseUpdate();
	StopWatch m_timer;
};

#endif