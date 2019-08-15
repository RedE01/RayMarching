#include <math.h>
#include <vector>

#include "Renderer.h"
#include "Window.h"
#include "Input.h"

#define HALF_PI 1.57079632679

struct point {
	float x, y, z;

	point(float x, float y, float z) : x(x), y(y), z(z) {}

	point& operator+=(const point& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		return *this;
	}
};

point calculateMovement(float speed, float yAngle) {
	return point(std::sin(yAngle) * speed * -1.0, 0, std::cos(yAngle) * speed);
} 

int main(void) {
	Window window;
	Input input((GLFWwindow*)window.getWindow());
	Renderer renderer;

	renderer.setUniform2f("u_windowSize", window.getWidth(), window.getHeight());

	std::vector<point> spherePositions = { point(-1.25, 0, -5), point(1.25, 0, -5), point(0, 1.25, -5) };
	renderer.setUniform3fv("u_spherePositions", spherePositions.size(), &spherePositions[0].x);

	std::vector<point> sphereColors = { point(1, 0, 0), point(0, 1, 0), point(0, 0, 1) };
	renderer.setUniform3fv("u_sphereColors", sphereColors.size(), &sphereColors[0].x);

	renderer.setUniform1i("u_sphereCount", spherePositions.size());

	point eyePos(0, 0, 0);
	float rotY = 0.0, walkingSpeed = 1.4, sneakingSpeed = walkingSpeed * 0.5, runningSpeed = walkingSpeed * 2.0, speed = walkingSpeed;

	double lastTime = glfwGetTime(), deltaTime = 0.0;;

    while (!window.shouldClose()) {     
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		if(input.getKey(GLFW_KEY_ESCAPE)) window.setShouldClose(true);

		speed = walkingSpeed;
		if(input.getKey(GLFW_KEY_LEFT_CONTROL)) speed = sneakingSpeed;
		if(input.getKey(GLFW_KEY_LEFT_SHIFT)) speed = runningSpeed;

		if(input.getKey(GLFW_KEY_W)) eyePos += calculateMovement(-speed * deltaTime, rotY);
		if(input.getKey(GLFW_KEY_S)) eyePos += calculateMovement( speed * deltaTime, rotY);
		if(input.getKey(GLFW_KEY_A)) eyePos += calculateMovement(-speed * deltaTime, rotY - HALF_PI);
		if(input.getKey(GLFW_KEY_D)) eyePos += calculateMovement( speed * deltaTime, rotY - HALF_PI);

		if(input.getKey(GLFW_KEY_Q)) eyePos.y -= speed * deltaTime;
		if(input.getKey(GLFW_KEY_E)) eyePos.y += speed * deltaTime;

		renderer.setUniform3fv("u_eyePos", 1, &eyePos.x);

		float deltaXpos, deltaYpos;
		input.getMouseMovement(deltaXpos, deltaYpos);

		rotY += deltaXpos * 0.001;
		renderer.setUniform1f("u_rotY", rotY);

		renderer.renderFrame();

		window.swapBuffer();
		window.pollEvents();
    }

    return 0;
}