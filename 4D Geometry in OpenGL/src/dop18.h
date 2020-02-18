// Author: Alexia Soucy, Concordia University, Fall 2019, COMP 477 with Serguei Mokhov, Team 8
// Implementation adapted from Christer Ericson's 8-DOP implementation (Real-Time Collision Detection, pp. 117-120)

#pragma once
#include "ParticleSystemPCH.h"

// 18-DOP for bounding
struct DOP18 {
	float min[9];
	float max[9];
};

// Computes the minimums and maximums of an 18-DOP bounding volume.
inline void setDOP18(std::vector<glm::vec3> verts, DOP18& dop) {
	// 18-DOP initialization
	for (int i = 0; i < 9; i++) {
		dop.min[i] = FLT_MAX;
		dop.max[i] = -FLT_MAX;
	}

	// 18-DOP computation
	float val;
	for (int v = 0; v < verts.size(); v++) {
		// Axis (1,0,0)
		val = verts[v].x;
		if (val < dop.min[0]) dop.min[0] = val;
		if (val > dop.max[0]) dop.max[0] = val;
		// Axis (0,1,0)
		val = verts[v].y;
		if (val < dop.min[1]) dop.min[1] = val;
		if (val > dop.max[1]) dop.max[1] = val;
		// Axis (0,0,1)
		val = verts[v].z;
		if (val < dop.min[2]) dop.min[2] = val;
		if (val > dop.max[2]) dop.max[2] = val;
		// Axis (1,1,0)
		val = verts[v].x + verts[v].y;
		if (val < dop.min[3]) dop.min[3] = val;
		if (val > dop.max[3]) dop.max[3] = val;
		// Axis (-1,1,0)
		val = -verts[v].x + verts[v].y;
		if (val < dop.min[4]) dop.min[4] = val;
		if (val > dop.max[4]) dop.max[4] = val;
		// Axis (0,1,1)
		val = verts[v].y + verts[v].z;
		if (val < dop.min[5]) dop.min[5] = val;
		if (val > dop.max[5]) dop.max[5] = val;
		// Axis (0,1,-1)
		val = verts[v].y - verts[v].z;
		if (val < dop.min[6]) dop.min[6] = val;
		if (val > dop.max[6]) dop.max[6] = val;
		// Axis (1,0,1)
		val = verts[v].x + verts[v].z;
		if (val < dop.min[7]) dop.min[7] = val;
		if (val > dop.max[7]) dop.max[7] = val;
		// Axis (1,0,-1)
		val = verts[v].x - verts[v].z;
		if (val < dop.min[8]) dop.min[8] = val;
		if (val > dop.max[8]) dop.max[8] = val;
	}
}

// Computes the 18-DOP axis minimums and maximums for a single point.
inline void setDOP18(vec3 vert, DOP18& dop) {
	// 18-DOP initialization
	for (int i = 0; i < 9; i++) {
		dop.min[i] = FLT_MAX;
		dop.max[i] = -FLT_MAX;
	}

	// 18-DOP computation
	float val;
		// Axis (1,0,0)
		val = vert.x;
		if (val < dop.min[0]) dop.min[0] = val;
		if (val > dop.max[0]) dop.max[0] = val;
		// Axis (0,1,0)
		val = vert.y;
		if (val < dop.min[1]) dop.min[1] = val;
		if (val > dop.max[1]) dop.max[1] = val;
		// Axis (0,0,1)
		val = vert.z;
		if (val < dop.min[2]) dop.min[2] = val;
		if (val > dop.max[2]) dop.max[2] = val;
		// Axis (1,1,0)
		val = vert.x + vert.y;
		if (val < dop.min[3]) dop.min[3] = val;
		if (val > dop.max[3]) dop.max[3] = val;
		// Axis (-1,1,0)
		val = -vert.x + vert.y;
		if (val < dop.min[4]) dop.min[4] = val;
		if (val > dop.max[4]) dop.max[4] = val;
		// Axis (0,1,1)
		val = vert.y + vert.z;
		if (val < dop.min[5]) dop.min[5] = val;
		if (val > dop.max[5]) dop.max[5] = val;
		// Axis (0,1,-1)
		val = vert.y - vert.z;
		if (val < dop.min[6]) dop.min[6] = val;
		if (val > dop.max[6]) dop.max[6] = val;
		// Axis (1,0,1)
		val = vert.x + vert.z;
		if (val < dop.min[7]) dop.min[7] = val;
		if (val > dop.max[7]) dop.max[7] = val;
		// Axis (1,0,-1)
		val = vert.x - vert.z;
		if (val < dop.min[8]) dop.min[8] = val;
		if (val > dop.max[8]) dop.max[8] = val;
}

// Checks for a collision between two 18-DOPs. Returns true if intersecting.
inline bool testDOP18(DOP18 dop1, DOP18 dop2) {
	// For all 18-DOP axes, if one axis is not intersecting, there is no collision.
	for (int a = 0; a < 9; a++) {
		if (dop1.min[a] > dop2.max[a] || dop1.max[a] < dop2.min[a]) return false;
	}

	// Intersection exists on all axes; collision found.
	return true;
}

// Checks for an intersection of a point and an 18-DOP. Returns true if intersecting.
inline bool testDOP18(DOP18 dop, vec3 p) {
	DOP18 pdop;
	setDOP18(p, pdop);

	// For all 18-DOP axes, if one axis is not intersecting, there is no collision.
	for (int a = 0; a < 9; a++) {
		if (pdop.min[a] > dop.max[a] || pdop.max[a] < dop.min[a]) return false;
	}

	// Intersection exists on all axes; collision found.
	return true;
}