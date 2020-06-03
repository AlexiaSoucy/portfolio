// Author: Alexia Soucy, 2020

#include "hypersolid.h"

#include <iostream>
using namespace std;

// Linearly interpolates between two points
float interpolate(float f1, float f2, float t) {
	return (1 - t) * f1 + t * f2;
}

// Creates a regular 4D hypertetrahedron
Hypersolid::Hypersolid()
{
	// Set up tetrahedral mesh vertices and double them up along w
	vertices4D =
	{
		// Base tetra 1
		-1.0f,  0.0f, -1 / sqrtf(2), -1.0f,
		 1.0f,  0.0f, -1 / sqrtf(2), -1.0f,
		 0.0f, -1.0f,  1 / sqrtf(2), -1.0f,
		 0.0f,  1.0f,  1 / sqrtf(2), -1.0f,

		 // Base tetra 2
		-1.0f,  0.0f, -1 / sqrtf(2),  1.0f,
		 1.0f,  0.0f, -1 / sqrtf(2),  1.0f,
		 0.0f, -1.0f,  1 / sqrtf(2),  1.0f,
		 0.0f,  1.0f,  1 / sqrtf(2),  1.0f
	};

	/* Triangle mapping for reference

		// Base tetra 1
		0,1,3,
		0,2,1,
		0,3,2,
		1,2,3,

		// Base tetra 2
		4,5,7,
		4,6,5,
		4,7,6,
		5,6,7

	*/

	// Map tetrahedral meshes' tetrahedrons connecting analogous triangles
	tetras4D =
	{
		// Base tetras
		0,1,2,3,
		4,5,6,7,

		// Prism tetras
		0,1,3,4,
		1,4,5,7,
		1,3,4,7,

		0,2,1,4,
		2,4,6,5,
		2,4,1,5,

		0,3,2,4,
		3,4,7,6,
		3,4,2,6,

		1,2,3,5,
		2,5,6,7,
		2,5,3,7
	};

	edgesProj =
	{
		// Base tetra 1
		0,1,
		0,2,
		0,3,
		1,2,
		1,3,
		2,3,

		// Base tetra 2
		4,5,
		4,6,
		4,7,
		5,6,
		5,7,
		6,7,

		// Linkings
		0,4,
		1,5,
		2,6,
		3,7
	};

	// Determine initial 3D properties
	update();

	// Establish render system
	renderSetUp();
}

// Set up required VAOs, VBOs, and shaders
void Hypersolid::renderSetUp()
{
	// CROSS DATA
	// Create and bind VAO
	glGenVertexArrays(1, &crossVAO);

	// Create and bind VBO to GL_ARRAY_BUFFER
	glGenBuffers(1, &crossVBO);

	// Assign vertices to VBO
	/* GL draw type reminder
	GL_STREAM_DRAW: data set once, only used a few times.
	GL_STATIC_DRAW: data set once, used many times. (fixed objects)
	GL_DYNAMIC_DRAW: data changes a lot, used many times. (moving objects)
	*/
	glBindBuffer(GL_ARRAY_BUFFER, crossVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices3D.size() * sizeof(float), &vertices3D[0], GL_DYNAMIC_DRAW);

	// Bind VAO
	glBindVertexArray(crossVAO);

	// Vertex attribute setup
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute setup
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind VAO
	glBindVertexArray(0);

	// PROJ DATA
	// Create and bind VAO
	glGenVertexArrays(1, &projVAO);

	// Create and bind VBO to GL_ARRAY_BUFFER
	glGenBuffers(1, &projVBO);

	// Assign vertices to VBO
	/* GL draw type reminder
	GL_STREAM_DRAW: data set once, only used a few times.
	GL_STATIC_DRAW: data set once, used many times. (fixed objects)
	GL_DYNAMIC_DRAW: data changes a lot, used many times. (moving objects)
	*/
	glBindBuffer(GL_ARRAY_BUFFER, projVBO);
	glBufferData(GL_ARRAY_BUFFER, verticesProj.size() * sizeof(float), &verticesProj[0], GL_DYNAMIC_DRAW);

	// Bind VAO
	glBindVertexArray(crossVAO);

	// Vertex attribute setup
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind VAO
	glBindVertexArray(0);

	// Set up shader
	phongShader = Shader("src/phongVertex.vs", "src/phongFragment.fs");
	lineShader = Shader("src/simpleVertex.vs", "src/simpleFragment.fs");
}

// Find normal of three vertices via cross-product
vec3 Hypersolid::pointNorm(vec3* v1, vec3* v2, vec3* v3)
{
	return normalize(cross(*v2 - *v1, *v3 - *v1));
}

// Find the 3D vertex that describes where the edge between two 4D vertices intersect the w = 0 hyperplane
vec3 Hypersolid::crossPoint(int v1, int v2)
{
	// Find the point between v1 and v2 where the hyperplane w = 0 is crossed
	float t = -vertices4D[v1 * VERT_4D_LENGTH + VERT_W_POS] / (vertices4D[v2 * VERT_4D_LENGTH + VERT_W_POS] - vertices4D[v1 * VERT_4D_LENGTH + VERT_W_POS]);

	// Interpolate between the two vertices to find the xyz coordinates of the cross-point
	return vec3(interpolate(vertices4D[v1 * VERT_4D_LENGTH + VERT_X_POS], vertices4D[v2 * VERT_4D_LENGTH + VERT_X_POS], t),
				interpolate(vertices4D[v1 * VERT_4D_LENGTH + VERT_Y_POS], vertices4D[v2 * VERT_4D_LENGTH + VERT_Y_POS], t),
				interpolate(vertices4D[v1 * VERT_4D_LENGTH + VERT_Z_POS], vertices4D[v2 * VERT_4D_LENGTH + VERT_Z_POS], t));
}

// Update all 3D properties
void Hypersolid::update()
{
	updateCross();
	updateProj();
}

// Construct the 3D cross-section of the hypersolid
void Hypersolid::updateCross()
{
	// Clear 3D properties
	vertices3D.clear();

	// Iterate through tetrahedrons to find those intersecting the w = 0 hyperplane
	for (int tetra = 0; tetra < tetras4D.size(); tetra += TETRA_LENGTH)
	{
		// Vectors recording the indices of relevant vertices
		vector<int> over;
		vector<int> under;

		// Check w position of each vertex on the tetrahedron
		for (int i = 0; i < TETRA_LENGTH; i++)
		{
			// Check the ith vertex of this tetra to see on which side of the w = 0 hyperplane it lies, record vertex's index
			if (vertices4D[tetras4D[tetra + i] * VERT_4D_LENGTH + VERT_W_POS] >= 0.0f)
				over.push_back(tetras4D[tetra + i]);
			else if (vertices4D[tetras4D[tetra + i] * VERT_4D_LENGTH + VERT_W_POS] < 0.0f)
				under.push_back(tetras4D[tetra + i]);
		}

		// Two meaningful scenarios: 2:2 or 1:3 split
		if (over.size() == 2)
		{
			// Four vertices create two triangles
			vec3 v1 = crossPoint(over[0], under[0]);
			vec3 v2 = crossPoint(over[0], under[1]);
			vec3 v3 = crossPoint(over[1], under[0]);
			vec3 v4 = crossPoint(over[1], under[1]);
			vec3 norm = pointNorm(&v1, &v2, &v3);

			vertices3D.insert(vertices3D.end(), { v1.x, v1.y, v1.z });
			vertices3D.insert(vertices3D.end(), { norm.x, norm.y, norm.z });
			vertices3D.insert(vertices3D.end(), { v2.x, v2.y, v2.z });
			vertices3D.insert(vertices3D.end(), { norm.x, norm.y, norm.z });
			vertices3D.insert(vertices3D.end(), { v3.x, v3.y, v3.z });
			vertices3D.insert(vertices3D.end(), { norm.x, norm.y, norm.z });

			vertices3D.insert(vertices3D.end(), { v2.x, v2.y, v2.z });
			vertices3D.insert(vertices3D.end(), { norm.x, norm.y, norm.z });
			vertices3D.insert(vertices3D.end(), { v3.x, v3.y, v3.z });
			vertices3D.insert(vertices3D.end(), { norm.x, norm.y, norm.z });
			vertices3D.insert(vertices3D.end(), { v4.x, v4.y, v4.z });
			vertices3D.insert(vertices3D.end(), { norm.x, norm.y, norm.z });
		}
		else if (over.size() == 1)
		{
			// Three vertices create one triangle
			vec3 v1 = crossPoint(over[0], under[0]);
			vec3 v2 = crossPoint(over[0], under[1]);
			vec3 v3 = crossPoint(over[0], under[2]);
			vec3 norm = pointNorm(&v1, &v2, &v3);

			vertices3D.insert(vertices3D.end(), { v1.x, v1.y, v1.z });
			vertices3D.insert(vertices3D.end(), { norm.x, norm.y, norm.z });
			vertices3D.insert(vertices3D.end(), { v2.x, v2.y, v2.z });
			vertices3D.insert(vertices3D.end(), { norm.x, norm.y, norm.z });
			vertices3D.insert(vertices3D.end(), { v3.x, v3.y, v3.z });
			vertices3D.insert(vertices3D.end(), { norm.x, norm.y, norm.z });
		}
		else if (over.size() == 3)
		{
			// Three vertices create one triangle
			vec3 v1 = crossPoint(over[0], under[0]);
			vec3 v2 = crossPoint(over[1], under[0]);
			vec3 v3 = crossPoint(over[2], under[0]);
			vec3 norm = pointNorm(&v1, &v2, &v3);

			vertices3D.insert(vertices3D.end(), { v1.x, v1.y, v1.z });
			vertices3D.insert(vertices3D.end(), { norm.x, norm.y, norm.z });
			vertices3D.insert(vertices3D.end(), { v2.x, v2.y, v2.z });
			vertices3D.insert(vertices3D.end(), { norm.x, norm.y, norm.z });
			vertices3D.insert(vertices3D.end(), { v3.x, v3.y, v3.z });
			vertices3D.insert(vertices3D.end(), { norm.x, norm.y, norm.z });
		}
	}
}

// Construct the 3D projection of the hypersolid
void Hypersolid::updateProj()
{
	// Clear projection data
	verticesProj.clear();

	// For each edge, remove the W coordinate of each vertex
	for (int vertex = 0; vertex < edgesProj.size(); vertex += EDGE_LENGTH)
	{
		verticesProj.insert(verticesProj.end(), { 
			vertices4D[edgesProj[vertex] * VERT_4D_LENGTH + VERT_X_POS], 
			vertices4D[edgesProj[vertex] * VERT_4D_LENGTH + VERT_Y_POS], 
			vertices4D[edgesProj[vertex] * VERT_4D_LENGTH + VERT_Z_POS] });
		verticesProj.insert(verticesProj.end(), { 
			vertices4D[edgesProj[vertex + 1] * VERT_4D_LENGTH + VERT_X_POS], 
			vertices4D[edgesProj[vertex + 1] * VERT_4D_LENGTH + VERT_Y_POS], 
			vertices4D[edgesProj[vertex + 1] * VERT_4D_LENGTH + VERT_Z_POS] });
	}
}

// Rotate the hypersolid in 4D about XW, YW, or ZW
void Hypersolid::rotate4D(float degrees, RotationAxis axis)
{
	float angle = radians(degrees);

	// Find rotation matrix based on rotation axis and update it
	switch (axis) {
	case RotationAxis::XW:
		rotationXW = mat4(
			cosf(angle), 0, 0, -sinf(angle),
			0,			 1, 0,	0,
			0,			 0, 1,	0,
			sinf(angle), 0, 0,	cosf(angle)
		);
		break;
	case RotationAxis::YW:
		rotationYW = mat4(
			1, 0,			0,	0,
			0, cosf(angle), 0, -sinf(angle),
			0, 0,			1,	0,
			0, sinf(angle), 0,	cosf(angle)
		);
		break;
	case RotationAxis::ZW:
		rotationZW = mat4(
			1, 0, 0,			0,
			0, 1, 0,			0,
			0, 0, cosf(angle), -sinf(angle),
			0, 0, sinf(angle),	cosf(angle)
		);
		break;
	}

	// Apply rotation to each vertex
	for (int vertex = 0; vertex < vertices4D.size(); vertex += VERT_4D_LENGTH)
	{
		vec4 vert = vec4(vertices4D[vertex + VERT_X_POS], vertices4D[vertex + VERT_Y_POS], vertices4D[vertex + VERT_Z_POS], vertices4D[vertex + VERT_W_POS]);
		vert = rotationXW * rotationYW * rotationZW * vert;
		vertices4D[vertex + VERT_X_POS] = vert.x;
		vertices4D[vertex + VERT_Y_POS] = vert.y;
		vertices4D[vertex + VERT_Z_POS] = vert.z;
		vertices4D[vertex + VERT_W_POS] = vert.w;
	}

	// Update 3D properties
	update();
}

// Draws the 3D cross-section of a 4D hypersolid
void Hypersolid::drawCross(vec3* colorFill, vec3* colorLines, vec3* colorLight, vec3* lightPos, mat4* model, mat4* view, mat4* proj, bool* fillToggle, bool* wireframeToggle)
{
	// Assign vertices to VBO
	/* GL draw type reminder
	GL_STREAM_DRAW: data set once, only used a few times.
	GL_STATIC_DRAW: data set once, used many times. (fixed objects)
	GL_DYNAMIC_DRAW: data changes a lot, used many times. (moving objects)
	*/
	glBindBuffer(GL_ARRAY_BUFFER, crossVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices3D.size() * sizeof(float), &vertices3D[0], GL_DYNAMIC_DRAW);

	// Bind VAO
	glBindVertexArray(crossVAO);

	// Vertex attribute setup
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute setup
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Draw fill
	if (*fillToggle)
	{
		// Set up shader
		phongShader.use();
		phongShader.setVec3("objectColor", *colorFill);
		phongShader.setVec3("lightColor", *colorLight);
		phongShader.setVec3("lightPos", *lightPos);
		phongShader.setMat4("model", *model);
		phongShader.setMat4("view", *view);
		phongShader.setMat4("projection", *proj);

		// Draw
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, vertices3D.size() / VERT_3D_LENGTH);
	}

	// Draw wireframe
	if (*wireframeToggle)
	{
		// Set up shader
		lineShader.use();
		lineShader.setVec3("objectColor", *colorFill);
		lineShader.setMat4("model", *model);
		lineShader.setMat4("view", *view);
		lineShader.setMat4("projection", *proj);

		// Draw
		glLineWidth(1.5f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, vertices3D.size() / VERT_3D_LENGTH);
	}

	// Unbind VAO
	glBindVertexArray(0);
}

// Draws the 3D projection of a 4D hypersolid
void Hypersolid::drawProj(vec3* colorLines, mat4* model, mat4* view, mat4* proj)
{
	// Set shader and bind VAO
	glBindVertexArray(projVAO);

	// Assign vertices to VBO
	/* GL draw type reminder
	GL_STREAM_DRAW: data set once, only used a few times.
	GL_STATIC_DRAW: data set once, used many times. (fixed objects)
	GL_DYNAMIC_DRAW: data changes a lot, used many times. (moving objects)
	*/
	glBindBuffer(GL_ARRAY_BUFFER, projVBO);
	glBufferData(GL_ARRAY_BUFFER, verticesProj.size() * sizeof(float), &verticesProj[0], GL_DYNAMIC_DRAW);

	// Bind VAO
	glBindVertexArray(projVAO);

	// Vertex attribute setup
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Draw wireframe
	lineShader.use();
	lineShader.setVec3("objectColor", *colorLines);
	lineShader.setMat4("model", *model);
	lineShader.setMat4("view", *view);
	lineShader.setMat4("projection", *proj);

	// Draw
	glLineWidth(1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_LINES, 0, verticesProj.size() / VERT_PROJ_LENGTH);

	// Unbind VAO
	glBindVertexArray(0);
}