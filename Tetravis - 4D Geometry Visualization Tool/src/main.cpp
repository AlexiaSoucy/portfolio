// Author: Alexia Soucy, 2020

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
using namespace std;

#include "hypersolid.h"

// Set starting window dimensions
int width = 800;
int height = 600;

// Toggles
bool wireframeToggle = true;
bool fillToggle = true;

// Set colors & lighting
vec3 colorCrossBG = vec3(0.25f, 0.25f, 0.25f); // Dark grey
vec3 colorProjBG = vec3(0.0f, 0.0f, 0.0f); // Black
vec3 colorFill = vec3(0.75f, 0.75f, 0.75f); // Light grey
vec3 colorLines = vec3(1.0f, 1.0f, 1.0f); // White
vec3 colorLight = vec3(1.0f, 1.0f, 1.0f); // White
vec3 lightPos = vec3(0.0f, 3.0f, 1.0f);

// Declare functions
void resizeWindow(GLFWwindow* window, int w, int h);
void keystrokes(GLFWwindow*, int key, int scancode, int action, int mods);

// Set up projection
mat4 model = mat4(1.0f);
mat4 view = mat4(1.0f);
mat4 proj = mat4(1.0f);

int main()
{
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(width, height, "Tetravis", NULL, NULL);

	// Check GLFW init
	if (window == NULL)
	{
		cout << "ERROR::GLFW::INITIALIZATION_FAILED" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Check GLAD init
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "ERROR::GLAD::INITIALIZATION_FAILED" << endl;
		glfwTerminate();
		return -1;
	}

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Declare callbacks
	glfwSetFramebufferSizeCallback(window, resizeWindow);
	glfwSetKeyCallback(window, keystrokes);

	// Set camera angle and perspective
	view = lookAt(
		vec3(0.0f, 0.0f, 5.0f),
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f)
	);
	proj = perspective(radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	// Set default rotation for model
	model = rotate(model, radians(45.0f), vec3(1.0f, 0.0f, 0.0f));

	// Hypersolid used for demo
	Hypersolid tetra;
	glfwSetWindowUserPointer(window, &tetra);

	// Print instructions
	cout << "Tetravis - 4D geometry visualizer" << endl;
	cout << "by Alexia Soucy" << endl << endl;
	cout << "INSTRUCTIONS" << endl;
	cout << "W/S - Rotate about X" << endl;
	cout << "A/D - Rotate about Y" << endl;
	cout << "Q/E - Rotate about Z" << endl;
	cout << "CTRL + [WASDQE] - Add rotation about W" << endl;
	cout << "Z - Toggle fill" << endl;
	cout << "X - Toggle wireframe" << endl;
	cout << "ESC - Exit" << endl;

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Prep for cross-section rendering
		glClearColor(colorCrossBG[0], colorCrossBG[1], colorCrossBG[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Create full viewport for cross-section
		glViewport(0, 0, width, height);

		// Draw cross-section
		tetra.drawCross(&colorFill, &colorLines, &colorLight, &lightPos, &model, &view, &proj, &fillToggle, &wireframeToggle);

		// Prep for projection rendering
		glScissor(width / 20, height / 20, width / 5, height / 5);
		glEnable(GL_SCISSOR_TEST);
		glClearColor(colorProjBG[0], colorProjBG[1], colorProjBG[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Create smaller viewport for projection
		glViewport(width / 20, height / 20, width / 5, height / 5);

		// Draw projection
		tetra.drawProj(&colorLines, &model, &view, &proj);

		glDisable(GL_SCISSOR_TEST);

		// Check for events, swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	// Clean up and close
	glfwTerminate();
	return 0;
}

// Changes viewport size when window is resized
void resizeWindow(GLFWwindow* window, int w, int h)
{
	width = w;
	height = h;

	// Adjust projection dimensions
	proj = perspective(radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	glViewport(0, 0, w, h);
}

// Process single keystroke notifications
void keystrokes(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// ESC closes the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	// F toggles fill
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
		fillToggle = !fillToggle;
	// W toggles wireframe
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		wireframeToggle = !wireframeToggle;
	if (mods == GLFW_MOD_CONTROL)
	{
		// Access the hypersolid
		Hypersolid* tetra = reinterpret_cast<Hypersolid*>(glfwGetWindowUserPointer(window));

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			tetra->rotate4D(1.0f, RotationAxis::XW);
		}
		else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			tetra->rotate4D(-1.0f, RotationAxis::XW);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			tetra->rotate4D(1.0f, RotationAxis::YW);
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			tetra->rotate4D(-1.0f, RotationAxis::YW);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			tetra->rotate4D(1.0f, RotationAxis::ZW);
		}
		else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			tetra->rotate4D(-1.0f, RotationAxis::ZW);
		}
	}
	else
	{
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			model = rotate(model, radians(1.0f), vec3(1.0f, 0.0f, 0.0f));
		}
		else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			model = rotate(model, -radians(1.0f), vec3(1.0f, 0.0f, 0.0f));
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			model = rotate(model, -radians(1.0f), vec3(0.0f, 1.0f, 0.0f));
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			model = rotate(model, radians(1.0f), vec3(0.0f, 1.0f, 0.0f));
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			model = rotate(model, -radians(1.0f), vec3(0.0f, 0.0f, 1.0f));
		}
		else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			model = rotate(model, radians(1.0f), vec3(0.0f, 0.0f, 1.0f));
		}
	}
}