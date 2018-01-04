// House.c++

#include <iostream>
#include <math.h>

#include "House.h"

#define DOOR_WIDTH (float)10.0
#define DOOR_HEIGHT (float)(2.0 * DOOR_WIDTH)

typedef float vec3[3];

// index lists for the three faces that can't be drawn with glDrawArrays
GLuint House::indexList[4][4] = {
	{ 0, 2, 10, 11 }, // xmin face
	{ 6, 4, 8, 9 },		// xmax face
	{ 8, 9, 10, 11 }, // zmin face
	{ 2, 4, 11, 9 } 	// ymax face
};

House::House(ShaderIF* sIF, float cx, float cy, float cz, float lx, float ly, float lz) :
	shaderIF(sIF)
{
	xmin = cx; xmax = cx + lx;
	ymin = cy; ymax = cy + ly;
	zmin = cz; zmax = cz + lz;

	kd[0] = 0.7; kd[1] = 0.7; kd[2] = 0.0;
	//ka[0] = 0.7; ka[1] = 0.7; ka[2] = 0.0;
	//ambientStrength[0] = 0.7; ambientStrength[1] = 0.7; ambientStrength[2] = 0.0;
	defineHouse();
}

House::~House()
{
	glDeleteBuffers(3,ebo);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void House::defineHouse()
{
	float midway = xmin + ((xmax - xmin) / 2);

	vec3 vtx[] = { // The 8 unique vertices (Note the order)
		{xmin ,ymin, zmax},
		{(midway - (DOOR_WIDTH / 2)), ymin, zmax},
		{xmin, ymax, zmax},
		{(midway - (DOOR_WIDTH / 2)), (ymin + DOOR_HEIGHT), zmax},
		{xmax, ymax, zmax},
		{(midway + (DOOR_WIDTH / 2)), (ymin + DOOR_HEIGHT), zmax},
		{xmax, ymin, zmax},
		{(midway + (DOOR_WIDTH / 2)), ymin, zmax},

		{xmax, ymin, zmin},
		{xmax, ymax, zmin},
		{xmin, ymin, zmin},
		{xmin, ymax, zmin}

	};
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(vec3), vtx, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

	glGenBuffers(4, ebo);
	for (int i=0 ; i<4 ; i++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), indexList[i], GL_STATIC_DRAW);
	}

	glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void House::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

bool House::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{
	// House does not look for events; just hand off to inherited handleCommand.
	return this->ModelView::handleCommand(anASCIIChar, ldsX, ldsY);
}

void House::renderHouse()
{
	glBindVertexArray(vao[0]);
	glUniform3fv(shaderIF->ppuLoc("kd"), 1, kd);
	//glUniform3fv(shaderIF->ppuLoc("ka"), 1, ka);
	//glUniform3fv(shaderIF->ppuLoc("ambientStrength"), 1, ambientStrength);

	// The three faces that can be drawn with glDrawArrays
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);

	// The three faces that are drawn with glDrawElements
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), -1.0, 0.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 1.0, 0.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, -1.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 1.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[3]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
}

void House::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderIF->getShaderPgmID());

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(mat));

	renderHouse();

	glUseProgram(pgm);
}
