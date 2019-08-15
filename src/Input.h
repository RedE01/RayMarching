#pragma once

class Window;

class Input {
private:
	Window* window;
	bool firstFrame = false;
	float lastXpos, lastYpos;

public:
	Input(Window* window);

	bool getKey(int keycode);
	void getMouseMovement(float& x, float& y);
};