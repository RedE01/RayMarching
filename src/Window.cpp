#include "Window.h"
#include <iostream>

Window::Window() {
	if (!glfwInit())
        std::cout << "Could not initialize glfw" << std::endl;

	width = 1280, height = 720;
    window = glfwCreateWindow(width, height, "RayMarching", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "Could not create window" << std::endl;
    }

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    glfwMakeContextCurrent(window);	
    glfwSwapInterval(1);
}

Window::~Window() {
    glfwTerminate();
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(window);
}

void Window::setShouldClose(bool value) {
	glfwSetWindowShouldClose(window, value);
}

void Window::swapBuffer() {
	glfwSwapBuffers(window);
}

void Window::pollEvents() {
	glfwPollEvents();
}