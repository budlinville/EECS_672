// Cylinder.c++

#include <iostream>
#include <math.h>

#include "Cylinder.h"

Cylinder::Cylinder(ShaderIF* sIF, float cx, float cy, float cz, float h, float r) : shaderIF(sIF) {
	kd[0] = 0.25; kd[1] = 1.0; kd[2] = 0.5;

	xCenter = cx;
	zCenter = cz;

	xmin = cx - r; xmax = cx + r;
	ymin = cy; ymax = cy + h;
	zmin = cz - r; zmax = cz + r;

	defineCylinder();
}

Cylinder::~Cylinder()
{
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, vao);
}

const int N_POINTS_AROUND_SLICE = 18; // number points around a cross-section

void Cylinder::defineCylinder() {
	typedef float vec3[3];
	int nPoints = 2 * (N_POINTS_AROUND_SLICE + 1);			//Because we need to define two circles
	vec3* coords = new vec3[nPoints];
	vec3* normals = new vec3[nPoints];
	double theta = 0.0;
	double dTheta = 2.0*M_PI/N_POINTS_AROUND_SLICE;

	ymin = -1; ymax = 1;
	xCenter = 0.0; zCenter = 0.0;
	radius = .5;
	for (int i=0 ; i<=N_POINTS_AROUND_SLICE ; i++) {
		int j = 2 * i;
		double dz = sin(theta);
		double dx = cos(theta);

		normals[j][0] = dx; normals[j][1] = 0.0; normals[j][2] = dz;
		coords[j][0] = xCenter + radius * dx; coords[j][1] = ymin; coords[j][2] = zCenter + radius * dz;
		normals[j+1][0] = dx; normals[j+1][1] = 0.0; normals[j+1][2] = dz;
		coords[j+1][0] = xCenter + radius * dx; coords[j+1][1] = ymax; coords[j+1][2] = zCenter + radius * dz;

		theta += dTheta;
	}

	// ************************************************************************
	// EXERCISE: Create/fill VAOs and VBOs here.
	//           Also use glVertexAttribPointer and glEnableVertexAttribArray
	// ************************************************************************
	//std::cout << "Cylinder::defineCylinder: create/fill VBOs. You will see errors until you do so.\n";
	// Create and bind the VAO
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	//create the VBO's
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, nPoints * sizeof(vec3), coords, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, nPoints * sizeof(vec3), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

	delete [] coords;
	delete [] normals;
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Cylinder::getMCBoundingBox(double* xyzLimits) const {
	/*xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;*/

	xyzLimits[0] = -1.0;
	xyzLimits[1] = 1.0;
	xyzLimits[2] = -1.0;
	xyzLimits[3] = 1.0;
	xyzLimits[4] = -1.0;
	xyzLimits[5] = 1.0;
}

void Cylinder::render()
{
	typedef float vec3[3];
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderIF->getShaderPgmID());

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(mat));

	//glUniform3fv(shaderIF->ppuLoc("kd"), 1, kd);
	glVertexAttrib1f(shaderIF->pvaLoc("Object"), 3);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(N_POINTS_AROUND_SLICE+1));

	glUseProgram(pgm);
}
