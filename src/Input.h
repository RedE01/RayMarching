#pragma once

class GLFWwindow;

class Input {
private:
	GLFWwindow* window;
	bool firstFrame = false;
	float lastXpos, lastYpos;

public:
	Input(GLFWwindow* window);

	bool getKey(int keycode);
	void getMouseMovement(float& x, float& y);
};