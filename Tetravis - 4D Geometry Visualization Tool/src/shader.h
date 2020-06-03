// Based on work by Joey de Vries© (https://twitter.com/JoeyDeVriez)
// Website: https://learnopengl.com/
// Source: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader.h
// License: https://creativecommons.org/licenses/by-nc/4.0/
// Modifications by Alexia Soucy, 2020

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // Contains OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

// A simple shader class to keep shader functionality out of the way.
class Shader
{
public:
	// Program ID
	unsigned int ID;

	// Read shaders from source and compile into a program
	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);

	// Use shader program
	void use();

	// Uniform input
	void setVec3(const string& name, vec3& vec) const;
	void setMat4(const string& name, mat4& mat) const;

};

#endif