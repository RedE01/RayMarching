#include "Input.h"
#include "Window.h"

Input::Input(Window* window) : window(window) {

}

bool Input::getKey(int keycode) {
	return glfwGetKey((GLFWwindow*)window->getWindow(), keycode);
}

void Input::getMouseMovement(float& x, float& y) {
	double xpos, ypos;
	glfwGetCursorPos((GLFWwindow*)window->getWindow(), &xpos, &ypos);
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