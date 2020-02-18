// Author: Alexia Soucy, Concordia University, Fall 2019, COMP 477 with Serguei Mokhov, Team 8

#pragma once

// OpenGL functionality
/*#include <GL\glew.h>
#include <GL\freeglut.h>*/

// Linear algebra
#include "ParticleSystemPCH.h"
using namespace glm;

// Used for dynamic vertex storage
#include <iostream>
#include <vector>
using namespace std;

// Collision detection
#include "dop18.h"

// Transform input axes
enum RotationAxis { XW, YW, ZW };
enum Axis3D { X, Y, Z };

// Linearly interpolates between two points
float interpolate(float f1, float f2, float t) {
	return (1 - t) * f1 + t * f2;
}

// A 4D hyperobject
class Hyperobject {

	protected:
		vector<vec4> vertices; // Vertices making up the 4D geometry
		vector<vec4*> tetrahedra; // Tetrahedron vertex pointers

		// 18-DOP for bounding
		DOP18 dop18;

		// Used to keep track of the hyperobject's 4D rotation, starting off as identity matrices, and position along the W axis
		mat4 rotationXW = mat4(1.0f);
		mat4 rotationYW = mat4(1.0f);
		mat4 rotationZW = mat4(1.0f);
		vec3 rotation4D = vec3(0.0f, 0.0f, 0.0f);
		float pos4D = 0.0f;

		// Used to keep track of the hyperobject's 3D position, scale, and rotation, applied after cross-section
		mat4 rotationX = mat4(1.0f);
		mat4 rotationY = mat4(1.0f);
		mat4 rotationZ = mat4(1.0f);
		vec3 rotation = vec3(0.0f, 0.0f, 0.0f); // Keeps track of degrees per angle
		mat4 position = mat4(1, 0, 0, 0,
							 0, 1, 0, 0,
							 0, 0, 1, 0,
							 0, 0, 0, 1);
		mat4 scale = mat4(1, 0, 0, 0,
						  0, 1, 0, 0,
						  0, 0, 1, 0,
						  0, 0, 0, 1);

	public:
		// The dimensional threshold for cross-section projection.
		static float threshold;

		// Setter for dimensional threshold.
		static void setDimensionalThreshold(float t) { threshold = t; }

		// Get the point that sits on the dimensional threshold between two other points via linear interpolation
		static vec3 crossPoint(vec4 v1, vec4 v2) {
			// Find the relative position of the threshold
			float t = (threshold - v1.w) / (v2.w - v1.w);

			// Interpolate between the two points to find the position where the threshold is crossed
			return vec3(interpolate(v1.x, v2.x, t), interpolate(v1.y, v2.y, t), interpolate(v1.z, v2.z, t));
		}

		// Get the 4D rotation in degrees (XW, YW, ZW)
		vec3 getRotation4D() { return rotation4D; }

		// Rotate the hyperobject in 4D
		void setRotation4D(float degrees, RotationAxis axis) {
			float angle = radians(degrees);

			// Find rotation matrix based on rotation axis and update it
			switch (axis) {
			case XW:
				rotationXW = mat4(cosf(angle), 0, 0, -sinf(angle),
								  0,		   1, 0, 0,
								  0,		   0, 1, 0,
								  sinf(angle), 0, 0, cosf(angle));
				rotation4D = vec3(degrees, rotation4D.y, rotation4D.z);
				break;
			case YW:
				rotationYW = mat4(1, 0,			  0, 0,
								  0, cosf(angle), 0, -sinf(angle),
								  0, 0,			  1, 0,
								  0, sinf(angle), 0, cosf(angle));
				rotation4D = vec3(rotation4D.x, degrees, rotation4D.z);
				break;
			case ZW:
				rotationZW = mat4(1, 0, 0,			 0,
								  0, 1, 0,			 0,
								  0, 0, cosf(angle), -sinf(angle),
								  0, 0, sinf(angle), cosf(angle));
				rotation4D = vec3(rotation4D.x, rotation4D.y, degrees);
				break;
			}
		}

		// Get the hyperobject's position along the W axis
		float getPos4D() { return pos4D; }

		// Set the hyperobject's position along the W axis
		void setPos4D(float pos) { pos4D = pos; }

		// Get 3D rotation in degrees
		vec3 getRotation() { return rotation; }

		// Set 3D rotation about an axis
		void setRotation(Axis3D axis, float degrees) {
			float angle = radians(degrees);

			switch (axis) {
			case X:
				rotationX = mat4(1, 0,			0,			 0,
								 0, cos(angle), -sin(angle), 0,
								 0, sin(angle), cos(angle),  0,
								 0, 0,			0,			 1);
				rotation = vec3(degrees, rotation.y, rotation.z);
				break;
			case Y:
				rotationY = mat4(cos(angle),  0, sin(angle), 0,
								 0,			  1, 0,			 0,
								 -sin(angle), 0, cos(angle), 0,
								 0,			  0, 0,			 1);
				rotation = vec3(rotation.x, degrees, rotation.z);
				break;
			case Z:
				rotationZ = mat4(cos(angle), -sin(angle), 0, 0,
								 sin(angle), cos(angle),  0, 0,
								 0,			 0,			  1, 0,
								 0,			 0,			  0, 1);
				rotation = vec3(rotation.x, rotation.y, degrees);
				break;
			}
		}

		// Get 3D position
		vec3 getPosition() { return vec3(position[3][0], position[3][1], position[3][2]); }

		// Set 3D position on all axes
		void setPosition(vec3 coords) {
			position = mat4(1,		  0,		0,		  0,
							0,		  1,		0,		  0,
							0,		  0,		1,		  0,
							coords.x, coords.y, coords.z, 1);
		}

		// Get 3D scale
		vec3 getScale() { return vec3(scale[0][0], scale[1][1], scale[2][2]); }

		// Set 3D scale on all axes
		void setScale(vec3 mult) {
			scale = mat4(mult.x, 0,		 0,		 0,
						 0,		 mult.y, 0,		 0,
						 0,		 0,		 mult.z, 0,
						 0,		 0,		 0,		 1);
		}

		// Returns a pointer to the member 18-DOP
		DOP18 getDOP18() {
			return dop18;
		}

		// Updates the 18-DOP bounds
		void updateDOP18(vector<vec3> verts) {
			setDOP18(verts, dop18);
		}

		// Applies tranforms to a 4D vertex
		vec4 transform4D(vec4 vertex) {
			// Apply 4D rotations
			vec4 rotated = rotationXW * rotationYW * rotationZW * vertex;

			// Apply movement and return
			return vec4(rotated.x, rotated.y, rotated.z, rotated.w + pos4D);
		}

		// Applies transforms to a 3D vertex
		vec3 transform3D(vec3 vertex) {
			mat4 R = rotationX * rotationY * rotationZ;
			vec4 transformed = position * scale * R * vec4(vertex, 1);

			return vec3(transformed.x, transformed.y, transformed.z);
		}

		// Compute the 3D triangles of the object by finding the triangle cross-section of each tetrahedron as it intersects with the dimensional threshold
		vector<vec3> getCrossTris() {
			vector<vec3> crossTris;

			// For each tetrahedron, 
			for (int v1 = 0; v1 < tetrahedra.size(); v1 += 4) {
				// Get transformed vertices for this tetrahedron
				vec4 vert1 = transform4D(*tetrahedra[v1]);
				vec4 vert2 = transform4D(*tetrahedra[v1+1]);
				vec4 vert3 = transform4D(*tetrahedra[v1+2]);
				vec4 vert4 = transform4D(*tetrahedra[v1+3]);

				vector<vec4*> underThreshold;
				vector<vec4*> overThreshold;

				// Find which points lie on either side of the dimensional threshold (if any)
				if (vert1.w >= threshold) overThreshold.push_back(&vert1);
				else underThreshold.push_back(&vert1);

				if (vert2.w >= threshold) overThreshold.push_back(&vert2);
				else underThreshold.push_back(&vert2);

				if (vert3.w >= threshold) overThreshold.push_back(&vert3);
				else underThreshold.push_back(&vert3);

				if (vert4.w >= threshold) overThreshold.push_back(&vert4);
				else underThreshold.push_back(&vert4);

				/*
				Three possible scenarios:
					All on one side: do nothing
					Two on either side: make two triangles out of the resulting quad
					Uneven distribution: make one triangle
				*/
				if (underThreshold.size() == 2) {
					// Compute all four vertices defining this tetrahedron's intersection with the threshold
					vec3 p1 = crossPoint(*underThreshold[0], *overThreshold[0]);
					vec3 p2 = crossPoint(*underThreshold[0], *overThreshold[1]);
					vec3 p3 = crossPoint(*underThreshold[1], *overThreshold[0]);
					vec3 p4 = crossPoint(*underThreshold[1], *overThreshold[1]);

					// Define the two resulting triangles
					crossTris.push_back(p1);
					crossTris.push_back(p2);
					crossTris.push_back(p3);

					crossTris.push_back(p2);
					crossTris.push_back(p3);
					crossTris.push_back(p4);
				}
				else if (underThreshold.size() == 1) {
					// Compute all three vertices defining this tetrahedron's intersection with the threshold
					vec3 p1 = crossPoint(*underThreshold[0], *overThreshold[0]);
					vec3 p2 = crossPoint(*underThreshold[0], *overThreshold[1]);
					vec3 p3 = crossPoint(*underThreshold[0], *overThreshold[2]);

					// Define the resulting triangle
					crossTris.push_back(p1);
					crossTris.push_back(p2);
					crossTris.push_back(p3);
				}
				else if (overThreshold.size() == 1) {
					// Compute all three vertices defining this tetrahedron's intersection with the threshold
					vec3 p1 = crossPoint(*overThreshold[0], *underThreshold[0]);
					vec3 p2 = crossPoint(*overThreshold[0], *underThreshold[1]);
					vec3 p3 = crossPoint(*overThreshold[0], *underThreshold[2]);

					// Define the resulting triangle
					crossTris.push_back(p1);
					crossTris.push_back(p2);
					crossTris.push_back(p3);
				}
			}

			return crossTris;
		}

		// Returns transformed 3D cross-section vertices arranged to draw as triangles and updates the bounding volume
		vector<vec3> getVerts() {
			vector<vec3> transformed;
			vector<vec3> cross = getCrossTris();

			for (int v = 0; v < cross.size(); v++) {
				transformed.push_back(transform3D(cross[v]));
			}

			// Update 18-DOP
			setDOP18(transformed, dop18);

			return transformed;
		}
};

// Threshold initialization
float Hyperobject::threshold = 0;

class Hypercube : public Hyperobject {

	public:
		Hypercube() {

			// Set up hypercube vertices
			vertices.push_back(vec4(-1.0f, -1.0f, 1.0f, 1.0f));
			vertices.push_back(vec4(1.0f, -1.0f, 1.0f, 1.0f));
			vertices.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
			vertices.push_back(vec4(-1.0f, 1.0f, 1.0f, 1.0f));
			vertices.push_back(vec4(-1.0f, -1.0f, -1.0f, 1.0f));
			vertices.push_back(vec4(1.0f, -1.0f, -1.0f, 1.0f));
			vertices.push_back(vec4(1.0f, 1.0f, -1.0f, 1.0f));
			vertices.push_back(vec4(-1.0f, 1.0f, -1.0f, 1.0f));
			vertices.push_back(vec4(-1.0f, -1.0f, 1.0f, -1.0f));
			vertices.push_back(vec4(1.0f, -1.0f, 1.0f, -1.0f));
			vertices.push_back(vec4(1.0f, 1.0f, 1.0f, -1.0f));
			vertices.push_back(vec4(-1.0f, 1.0f, 1.0f, -1.0f));
			vertices.push_back(vec4(-1.0f, -1.0f, -1.0f, -1.0f));
			vertices.push_back(vec4(1.0f, -1.0f, -1.0f, -1.0f));
			vertices.push_back(vec4(1.0f, 1.0f, -1.0f, -1.0f));
			vertices.push_back(vec4(-1.0f, 1.0f, -1.0f, -1.0f));

			// Set up hypercube tetrahedra
			tetrahedra.push_back(&vertices[0]);
			tetrahedra.push_back(&vertices[1]);
			tetrahedra.push_back(&vertices[2]);
			tetrahedra.push_back(&vertices[5]);

			tetrahedra.push_back(&vertices[0]);
			tetrahedra.push_back(&vertices[2]);
			tetrahedra.push_back(&vertices[3]);
			tetrahedra.push_back(&vertices[7]);

			tetrahedra.push_back(&vertices[0]);
			tetrahedra.push_back(&vertices[2]);
			tetrahedra.push_back(&vertices[5]);
			tetrahedra.push_back(&vertices[7]);

			tetrahedra.push_back(&vertices[0]);
			tetrahedra.push_back(&vertices[4]);
			tetrahedra.push_back(&vertices[5]);
			tetrahedra.push_back(&vertices[7]);

			tetrahedra.push_back(&vertices[2]);
			tetrahedra.push_back(&vertices[5]);
			tetrahedra.push_back(&vertices[6]);
			tetrahedra.push_back(&vertices[7]);


			tetrahedra.push_back(&vertices[0]);
			tetrahedra.push_back(&vertices[1]);
			tetrahedra.push_back(&vertices[5]);
			tetrahedra.push_back(&vertices[9]);

			tetrahedra.push_back(&vertices[0]);
			tetrahedra.push_back(&vertices[5]);
			tetrahedra.push_back(&vertices[9]);
			tetrahedra.push_back(&vertices[12]);

			tetrahedra.push_back(&vertices[0]);
			tetrahedra.push_back(&vertices[8]);
			tetrahedra.push_back(&vertices[9]);
			tetrahedra.push_back(&vertices[12]);

			tetrahedra.push_back(&vertices[0]);
			tetrahedra.push_back(&vertices[4]);
			tetrahedra.push_back(&vertices[5]);
			tetrahedra.push_back(&vertices[12]);

			tetrahedra.push_back(&vertices[5]);
			tetrahedra.push_back(&vertices[9]);
			tetrahedra.push_back(&vertices[12]);
			tetrahedra.push_back(&vertices[13]);


			tetrahedra.push_back(&vertices[0]);
			tetrahedra.push_back(&vertices[1]);
			tetrahedra.push_back(&vertices[2]);
			tetrahedra.push_back(&vertices[9]);

			tetrahedra.push_back(&vertices[0]);
			tetrahedra.push_back(&vertices[2]);
			tetrahedra.push_back(&vertices[3]);
			tetrahedra.push_back(&vertices[11]);

			tetrahedra.push_back(&vertices[0]);
			tetrahedra.push_back(&vertices[2]);
			tetrahedra.push_back(&vertices[9]);
			tetrahedra.push_back(&vertices[11]);

			tetrahedra.push_back(&vertices[0]);
			tetrahedra.push_back(&vertices[8]);
			tetrahedra.push_back(&vertices[9]);
			tetrahedra.push_back(&vertices[11]);

			tetrahedra.push_back(&vertices[2]);
			tetrahedra.push_back(&vertices[9]);
			tetrahedra.push_back(&vertices[10]);
			tetrahedra.push_back(&vertices[11]);


			tetrahedra.push_back(&vertices[0]);
			tetrahedra.push_back(&vertices[3]);
			tetrahedra.push_back(&vertices[4]);
			tetrahedra.push_back(&vertices[8]);

			tetrahedra.push_back(&vertices[3]);
			tetrahedra.push_back(&vertices[4]);
			tetrahedra.push_back(&vertices[7]);
			tetrahedra.push_back(&vertices[15]);

			tetrahedra.push_back(&vertices[3]);
			tetrahedra.push_back(&vertices[4]);
			tetrahedra.push_back(&vertices[8]);
			tetrahedra.push_back(&vertices[15]);

			tetrahedra.push_back(&vertices[3]);
			tetrahedra.push_back(&vertices[8]);
			tetrahedra.push_back(&vertices[11]);
			tetrahedra.push_back(&vertices[15]);

			tetrahedra.push_back(&vertices[4]);
			tetrahedra.push_back(&vertices[8]);
			tetrahedra.push_back(&vertices[12]);
			tetrahedra.push_back(&vertices[15]);


			tetrahedra.push_back(&vertices[1]);
			tetrahedra.push_back(&vertices[2]);
			tetrahedra.push_back(&vertices[5]);
			tetrahedra.push_back(&vertices[9]);

			tetrahedra.push_back(&vertices[2]);
			tetrahedra.push_back(&vertices[5]);
			tetrahedra.push_back(&vertices[6]);
			tetrahedra.push_back(&vertices[14]);

			tetrahedra.push_back(&vertices[2]);
			tetrahedra.push_back(&vertices[5]);
			tetrahedra.push_back(&vertices[9]);
			tetrahedra.push_back(&vertices[14]);

			tetrahedra.push_back(&vertices[2]);
			tetrahedra.push_back(&vertices[9]);
			tetrahedra.push_back(&vertices[10]);
			tetrahedra.push_back(&vertices[14]);

			tetrahedra.push_back(&vertices[5]);
			tetrahedra.push_back(&vertices[9]);
			tetrahedra.push_back(&vertices[13]);
			tetrahedra.push_back(&vertices[14]);


			tetrahedra.push_back(&vertices[2]);
			tetrahedra.push_back(&vertices[3]);
			tetrahedra.push_back(&vertices[6]);
			tetrahedra.push_back(&vertices[10]);

			tetrahedra.push_back(&vertices[3]);
			tetrahedra.push_back(&vertices[6]);
			tetrahedra.push_back(&vertices[10]);
			tetrahedra.push_back(&vertices[15]);

			tetrahedra.push_back(&vertices[3]);
			tetrahedra.push_back(&vertices[10]);
			tetrahedra.push_back(&vertices[11]);
			tetrahedra.push_back(&vertices[15]);

			tetrahedra.push_back(&vertices[3]);
			tetrahedra.push_back(&vertices[6]);
			tetrahedra.push_back(&vertices[7]);
			tetrahedra.push_back(&vertices[15]);

			tetrahedra.push_back(&vertices[6]);
			tetrahedra.push_back(&vertices[10]);
			tetrahedra.push_back(&vertices[14]);
			tetrahedra.push_back(&vertices[15]);


			tetrahedra.push_back(&vertices[4]);
			tetrahedra.push_back(&vertices[5]);
			tetrahedra.push_back(&vertices[6]);
			tetrahedra.push_back(&vertices[13]);

			tetrahedra.push_back(&vertices[4]);
			tetrahedra.push_back(&vertices[6]);
			tetrahedra.push_back(&vertices[7]);
			tetrahedra.push_back(&vertices[15]);

			tetrahedra.push_back(&vertices[4]);
			tetrahedra.push_back(&vertices[6]);
			tetrahedra.push_back(&vertices[13]);
			tetrahedra.push_back(&vertices[15]);

			tetrahedra.push_back(&vertices[4]);
			tetrahedra.push_back(&vertices[12]);
			tetrahedra.push_back(&vertices[13]);
			tetrahedra.push_back(&vertices[15]);

			tetrahedra.push_back(&vertices[6]);
			tetrahedra.push_back(&vertices[13]);
			tetrahedra.push_back(&vertices[14]);
			tetrahedra.push_back(&vertices[15]);


			tetrahedra.push_back(&vertices[8]);
			tetrahedra.push_back(&vertices[9]);
			tetrahedra.push_back(&vertices[10]);
			tetrahedra.push_back(&vertices[13]);

			tetrahedra.push_back(&vertices[8]);
			tetrahedra.push_back(&vertices[10]);
			tetrahedra.push_back(&vertices[11]);
			tetrahedra.push_back(&vertices[15]);

			tetrahedra.push_back(&vertices[8]);
			tetrahedra.push_back(&vertices[10]);
			tetrahedra.push_back(&vertices[13]);
			tetrahedra.push_back(&vertices[15]);

			tetrahedra.push_back(&vertices[8]);
			tetrahedra.push_back(&vertices[12]);
			tetrahedra.push_back(&vertices[13]);
			tetrahedra.push_back(&vertices[15]);

			tetrahedra.push_back(&vertices[10]);
			tetrahedra.push_back(&vertices[13]);
			tetrahedra.push_back(&vertices[14]);
			tetrahedra.push_back(&vertices[15]);
		}
};