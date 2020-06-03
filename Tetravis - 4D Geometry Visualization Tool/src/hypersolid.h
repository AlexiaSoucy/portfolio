#ifndef HYPERSOLID_H
#define HYPERSOLID_H

#include <vector>
using namespace std;

#include <glad/glad.h>
// Author: Alexia Soucy, 2020

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "shader.h"

// Data length constants to iterate through vectors
const int VERT_X_POS = 0;
const int VERT_Y_POS = 1;
const int VERT_Z_POS = 2;
const int EDGE_LENGTH = 2;
const int VERT_W_POS = 3;
const int VERT_PROJ_LENGTH = 3;
const int PRISM_LENGTH = 3;
const int VERT_4D_LENGTH = 4;
const int TETRA_LENGTH = 4;
const int VERT_3D_LENGTH = 6;

// Transform input axes
enum class RotationAxis { XW, YW, ZW };

// 4D hypersolid object
class Hypersolid
{
private:
	// Mesh containers
	vector<float> vertices4D; // 4D hypersolid vertex collection
	vector<int> tetras4D; // 4D hypersolid tetrahedron mapping
	vector<float> vertices3D; // 3D cross-section vertex collection
	vector<float> verticesProj; // 3D projection vertices
	vector<int> edgesProj; // 3D projection edge mapping

	// 4D rotation management matrices
	mat4 rotationXW = mat4(1.0f); // Matrix describing rotation about XW axes
	mat4 rotationYW = mat4(1.0f); // Matrix describing rotation about YW axes
	mat4 rotationZW = mat4(1.0f); // Matrix describing rotation about ZW axes

	// Rendering management variables
	unsigned int crossVAO;
	unsigned int crossVBO;
	unsigned int projVAO;
	unsigned int projVBO;
	Shader phongShader; // Used for solid shading
	Shader lineShader; // Used for lines

	void renderSetUp();
	vec3 pointNorm(vec3* v1, vec3* v2, vec3* v3);
	vec3 crossPoint(int v1, int v2);
	void update();
	void updateCross();
	void updateProj();

public:
	Hypersolid();
	void rotate4D(float degrees, RotationAxis axis);
	void drawCross(vec3* colorFill, vec3* colorLines, vec3* colorLight, vec3* lightPos, mat4* model, mat4* view, mat4* proj, bool* fillToggle, bool* wireframeToggle);
	void drawProj(vec3* colorLines, mat4* model, mat4* view, mat4* proj);
};

#endif