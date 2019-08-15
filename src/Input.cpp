#include "Input.h"
#include <GLFW/glfw3.h>

Input::Input(GLFWwindow* window) : window(window) {

}

bool Input::getKey(int keycode) {
	return glfwGetKey(window, keycode);
}

void Input::getMouseMovement(float& x, float& y) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	if(firstFrame) {
		lastXpos = xpos;
		lastYpos = ypos;
		firstFrame = false;
	}

	x = xpos - lastXpos;
	y = ypos - lastYpos;

	lastXpos = xpos;
	lastYpos = ypos;
}