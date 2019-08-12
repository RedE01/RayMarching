#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define HALF_PI 1.57079632679

std::string loadFileToString(std::string filePath) {
	std::ifstream fileStream(filePath);

	if(!fileStream.is_open()) {
		std::cout << "ERROR: File: " << filePath << " could not be opened" << std::endl;
		return "";
	}

	std::stringstream fileBuffer;
	fileBuffer << fileStream.rdbuf();
	fileStream.close();

	return fileBuffer.str();
}

unsigned int createShader(std::string& shaderSource, GLenum shaderType) {
	unsigned int shaderId = glCreateShader(shaderType);

	const char* src = shaderSource.c_str();
	int srcLen = shaderSource.size();
	glShaderSource(shaderId, 1, &src, &srcLen);

	glCompileShader(shaderId);

	int params;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &params);
	if(params != GL_TRUE) {
		int logLen = 0;
		char message[1024];
		glGetShaderInfoLog(shaderId, 1024, &logLen, message);

		std::cout << message << std::endl;
	}

	return shaderId;
}

void getProgramErrorMessage(unsigned int programId, GLenum pname) {
	int params;
	glGetProgramiv(programId, pname, &params);
	if(params != GL_TRUE) {
		int logLen = 0;
		char message[1024];
		glGetProgramInfoLog(programId, 1024, &logLen, message);

		std::cout << message << std::endl;
	}
}

unsigned int createShaderProgram(std::string& vertexShaderSource, std::string& fragmentShaderSource) {
	unsigned int vertexShaderId = createShader(vertexShaderSource, GL_VERTEX_SHADER);
	unsigned int fragmentShaderId = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	unsigned int programId = glCreateProgram();

	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	glLinkProgram(programId);
	getProgramErrorMessage(programId, GL_LINK_STATUS);

	glValidateProgram(programId);
	getProgramErrorMessage(programId, GL_VALIDATE_STATUS);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}

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
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
	int windowSize[2] = {1280, 720};
    window = glfwCreateWindow(windowSize[0], windowSize[1], "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	if(glewInit() != GLEW_OK) {
		std::cout << "glewInit error" << std::endl;
		return -1;
	}

	std::string vertexShaderSrc = loadFileToString("src/vertexShader.glsl");
	std::string fragmentShaderSrc = loadFileToString("src/fragmentShader.glsl");
	unsigned int shaderProgram = createShaderProgram(vertexShaderSrc, fragmentShaderSrc); 
	glUseProgram(shaderProgram);

	int windowSizeUniformLocation = glGetUniformLocation(shaderProgram, "u_windowSize");
	glUniform2f(windowSizeUniformLocation, windowSize[0], windowSize[1]);

	int eyePosUniformLocation = glGetUniformLocation(shaderProgram, "u_eyePos");
	int rotYUniformLocation = glGetUniformLocation(shaderProgram, "u_rotY"); 

	int spherePositionsUniformLocation = glGetUniformLocation(shaderProgram, "u_spherePositions");
	std::vector<point> spherePositions = { point(-1.25, 0, -5), point(1.25, 0, -5), point(0, 1.25, -5) };
	glUniform3fv(spherePositionsUniformLocation, spherePositions.size(), &spherePositions[0].x);

	int sphereColorsUniformLocation = glGetUniformLocation(shaderProgram, "u_sphereColors");
	std::vector<point> sphereColors = { point(1, 0, 0), point(0, 1, 0), point(0, 0, 1) };
	glUniform3fv(sphereColorsUniformLocation, sphereColors.size(), &sphereColors[0].x);

	int sphereCountUniformLocation = glGetUniformLocation(shaderProgram, "u_sphereCount");
	glUniform1i(sphereCountUniformLocation, spherePositions.size());

	float verticies[] = {
		-1, -1,
		 1, -1,
		 1,  1,
		 1,  1,
		-1,  1,
		-1, -1
	};

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	point eyePos(0, 0, 0);
	float rotY = 0.0, walkingSpeed = 1.4, sneakingSpeed = walkingSpeed * 0.5, runningSpeed = walkingSpeed * 2.0, speed = walkingSpeed;

	double lastXpos = 0.0, lastYpos = 0.0;
	bool firstFrame = true;

	double lastTime = glfwGetTime(), deltaTime = 0.0;;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		if(glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, true);

		speed = walkingSpeed;
		if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) speed = sneakingSpeed;
		if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) speed = runningSpeed;

		if(glfwGetKey(window, GLFW_KEY_W)) eyePos += calculateMovement(-speed * deltaTime, rotY);
		if(glfwGetKey(window, GLFW_KEY_S)) eyePos += calculateMovement( speed * deltaTime, rotY);
		if(glfwGetKey(window, GLFW_KEY_A)) eyePos += calculateMovement(-speed * deltaTime, rotY - HALF_PI);
		if(glfwGetKey(window, GLFW_KEY_D)) eyePos += calculateMovement( speed * deltaTime, rotY - HALF_PI);

		if(glfwGetKey(window, GLFW_KEY_Q)) eyePos.y -= speed * deltaTime;
		if(glfwGetKey(window, GLFW_KEY_E)) eyePos.y += speed * deltaTime;

		glUniform3fv(eyePosUniformLocation, 1, &eyePos.x);

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		if(firstFrame) {
			lastXpos = xpos;
			lastYpos = ypos;
			firstFrame = false;
		}

		double deltaXpos = xpos - lastXpos, deltaYpos = ypos - lastYpos;

		lastXpos = xpos;
		lastYpos = ypos;

		rotY += deltaXpos * 0.001;
		glUniform1f(rotYUniformLocation, rotY);

		// spherePositions[0].x += 0.5 * deltaTime;
		glUniform3fv(spherePositionsUniformLocation, 2, &spherePositions[0].x);

		glDrawArrays(GL_TRIANGLES,0, 6);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}