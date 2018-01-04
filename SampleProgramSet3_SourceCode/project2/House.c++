// House.c++

#include <iostream>
#include <math.h>

#include "House.h"

#define DOOR_WIDTH (float)15.0
#define DOOR_HEIGHT (float)(2.0 * DOOR_WIDTH)
#define ROOF_DELTA (float)4
#define TRIM_DELTA (float)2

typedef float vec3[3];

// index lists for the three faces that can't be drawn with glDrawArrays
GLuint House::sideWalls[2][4] = {
	{ 0, 2, 18, 17 }, 	// xmin face
	{ 6, 8, 15, 14 },		// xmax face
};

GLuint House::backWall[5] = { 14, 18, 15, 17, 16 };		// zmin face

GLuint House::roofRight[4] = { 19, 20, 25, 26 };
GLuint House::roofLeft[4] = { 23, 24, 29, 30 };
GLuint House::roofRightTop[4] = { 20, 22, 26, 28 };
GLuint House::roofRightBottom[4] = { 19, 21, 25, 27 };
GLuint House::roofLeftTop[4] = { 24, 22, 30, 28 };
GLuint House::roofLeftBottom[4] = { 21, 23, 27, 29 };

House::House(ShaderIF* sIF, float cx, float cy, float cz, float l) : shaderIF(sIF) {
	xmin = cx; xmax = cx + l;
	ymin = cy; ymax = cy + (1.5 * l);
	zmin = cz; zmax = cz + l;
	length = l;
	yHeight = cy + l;

	//kd[0] = 1.0; kd[1] = 0.1; kd[2] = 0.1;
	//ka[0] = 1.0; ka[1] = 0.1; ka[2] = 0.1;
	//ambientStrength[0] = 0.8; ambientStrength[1] = 0.8; ambientStrength[2] = 0.8;



	defineHouse();
}

House::~House() {
	glDeleteBuffers(3,ebo);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void House::defineHouse() {
	float midway = xmin + ((xmax - xmin) / 2);

	float windowCenterHeight = ymin + DOOR_HEIGHT + (length - DOOR_HEIGHT) / 2;

	float doorTop = ymin + DOOR_HEIGHT;
	float doorWindowLeft = midway - (DOOR_WIDTH / 2);
	float doorWindowRight = midway + (DOOR_WIDTH / 2);
	float windowLength = doorWindowRight - doorWindowLeft;

	float windowBottom = windowCenterHeight - DOOR_WIDTH / 2;
	float windowTop = windowCenterHeight + DOOR_WIDTH / 2;

	float roofDelta = ROOF_DELTA;

	vec3 vtx[] = {

		/*********************************************
											Front Wall Points
		*********************************************/

		{xmin, ymin, zmax},											//0 needed for left side wall		(B)
		{doorWindowLeft, ymin, zmax},
		{xmin, yHeight, zmax},									//2 needed for left side wall		(T)
		{doorWindowLeft, windowTop, zmax},

		{midway, ymax, zmax},										//4 front wall roof
		{doorWindowRight, windowTop, zmax},
		{xmax, yHeight, zmax},									//6 needed for right side wall 	(T)
		{doorWindowRight, windowBottom, zmax},

		{xmax, ymin, zmax},											//8 needed for right side wall	(B)
		{doorWindowRight, ymin, zmax},
		{doorWindowRight, doorTop, zmax},
		{doorWindowRight, windowBottom, zmax},		//REPEAT --- issue?

		{doorWindowLeft, doorTop, zmax},
		{doorWindowLeft, windowBottom, zmax},
		//{doorWindowRight, doorTop, zmax},
		//{doorWindowLeft, windowBottom, zmax}

		/*********************************************
											Back Wall Points
		*********************************************/

		{xmax, ymin, zmin},			// 14	bottom right
		{xmax, yHeight, zmin},	// 15 tom right
		{midway, ymax, zmin},		// 16 roof point
		{xmin, yHeight, zmin},	// 17 top left
		{xmin, ymin, zmin},			// 18 bottom left


		/********************************************
								FRONT OF ROOF
		********************************************/
		{xmax + roofDelta, yHeight - roofDelta, zmax + roofDelta},		//bottom right
		{xmax + roofDelta, yHeight, zmax + roofDelta},								//top right
		{midway, ymax, zmax + roofDelta},															//bottom middle
		{midway, ymax + roofDelta, zmax + roofDelta},									//top middle
		{xmin - roofDelta, yHeight - roofDelta, zmax + roofDelta},		//bottom left
		{xmin - roofDelta, yHeight, zmax + roofDelta},								//top left

		/********************************************
								Back OF ROOF
		********************************************/
		{xmax + roofDelta, yHeight - roofDelta, zmin - roofDelta},		//bottom right
		{xmax + roofDelta, yHeight, zmin - roofDelta},								//top right
		{midway, ymax, zmin - roofDelta},															//bottom middle
		{midway, ymax + roofDelta, zmin - roofDelta},									//top middle
		{xmin - roofDelta, yHeight - roofDelta, zmin - roofDelta},		//bottom left
		{xmin - roofDelta, yHeight, zmin - roofDelta},								//top left

	};

	/**********************************
				ADD SOME TRIM USING BLOCK
	***********************************/
	//trim[0] = new Block(shaderIF, (xmax - .5 * TRIM_DELTA), ymin, (zmax - .5 * TRIM_DELTA), TRIM_DELTA, length, TRIM_DELTA);

	//HOUSE TRIM
	trim[0] = new Block(shaderIF, xmax, ymin, zmax, TRIM_DELTA, length, TRIM_DELTA, 0.0);									//front right vertical trim
	trim[1] = new Block(shaderIF, (xmin - TRIM_DELTA), ymin, zmax, TRIM_DELTA, length, TRIM_DELTA, 0.0);		//front left vertical trim
	trim[2] = new Block(shaderIF, (xmin - TRIM_DELTA), ymin, zmin - TRIM_DELTA, TRIM_DELTA, length, TRIM_DELTA, 0.0);		//back left vertical trim
	trim[3] = new Block(shaderIF, xmax, ymin, zmin - TRIM_DELTA, TRIM_DELTA, length, TRIM_DELTA, 0.0);									//back right vertical trim

	//WINDOW TRIM
	trim[4] = new Block(shaderIF, doorWindowLeft - TRIM_DELTA, windowBottom - TRIM_DELTA, zmax, windowLength + 2 * TRIM_DELTA, TRIM_DELTA, TRIM_DELTA, 0.0);		//window bottom trim
	trim[5] = new Block(shaderIF, doorWindowLeft - TRIM_DELTA, windowTop, zmax, windowLength + 2 * TRIM_DELTA, TRIM_DELTA, TRIM_DELTA, 0.0);		//window bottom trim
	trim[6] = new Block(shaderIF, doorWindowLeft - TRIM_DELTA, windowBottom, zmax, TRIM_DELTA, windowLength, TRIM_DELTA, 0.0);		//window left trim
	trim[7] = new Block(shaderIF, doorWindowRight, windowBottom, zmax, TRIM_DELTA, windowLength, TRIM_DELTA, 0.0);		//window right trim

	//DOOR TRIM
	trim[8] = new Block(shaderIF, doorWindowLeft - TRIM_DELTA, ymin, zmax, TRIM_DELTA, DOOR_HEIGHT + TRIM_DELTA, TRIM_DELTA, 0.0);		//left left trim
	trim[9] = new Block(shaderIF, doorWindowRight, ymin, zmax, TRIM_DELTA, DOOR_HEIGHT + TRIM_DELTA, TRIM_DELTA, 0.0);		//left left trim
	trim[10] = new Block(shaderIF, doorWindowLeft, doorTop, zmax, windowLength, TRIM_DELTA, TRIM_DELTA, 0.0);		//window bottom trim

	trim[11] = new Block(shaderIF, xmin, yHeight, zmax, length, TRIM_DELTA, TRIM_DELTA, 0.0);		//top house trim
	trim[12] = new Block(shaderIF, xmin, yHeight, zmin - TRIM_DELTA, length, TRIM_DELTA, TRIM_DELTA, 0.0);		//top house trim



	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 31*sizeof(vec3), vtx, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));


	glGenBuffers(9, ebo);
	for (int i=0 ; i<2 ; i++) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), sideWalls[i], GL_STATIC_DRAW);
	}

	//back wall
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 5*sizeof(GLuint), backWall, GL_STATIC_DRAW);


	//roof right
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), roofRight, GL_STATIC_DRAW);
	//roof left
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), roofLeft, GL_STATIC_DRAW);


	//roof right top
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[5]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), roofRightTop, GL_STATIC_DRAW);
	//roof right bottom
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[6]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), roofRightBottom, GL_STATIC_DRAW);


	//roof left top
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[7]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), roofLeftTop, GL_STATIC_DRAW);
	//roof left bottom
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[8]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), roofLeftBottom, GL_STATIC_DRAW);

	glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void House::getMCBoundingBox(double* xyzLimits) const {
	xyzLimits[0] = xmin - ROOF_DELTA;
	xyzLimits[1] = xmax - ROOF_DELTA;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax + ROOF_DELTA;
	xyzLimits[4] = zmin - ROOF_DELTA;
	xyzLimits[5] = zmax + ROOF_DELTA;
}

void House::renderHouse() {
	glBindVertexArray(vao[0]);

/*
	glUniform3fv(shaderIF->ppuLoc("kd"), 1, kd);
	glUniform3fv(shaderIF->ppuLoc("ka"), 1, ka);
	glUniform3fv(shaderIF->ppuLoc("ambientStrength"), 1, ambientStrength);
*/
	glVertexAttrib1f(shaderIF->pvaLoc("Object"), 0);

	// The front face that can be drawn with glDrawArrays
	//glUniform1i(shaderIF->ppuLoc("color"), 0); 		//
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);

	// The three faces that are drawn with glDrawElements
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), -1.0, 0.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 1.0, 0.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, -1.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
	glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_INT, nullptr);


	/*************************************
							ROOF - change color
	*************************************/
	/*kd[0] = 0.4; kd[1] = 0.2; kd[2] = 0.0;
	ka[0] = 0.4; ka[1] = 0.2; ka[2] = 0.0;
	ambientStrength[0] = 0.8; ambientStrength[1] = 0.8; ambientStrength[2] = 0.8;
	glUniform3fv(shaderIF->ppuLoc("kd"), 1, kd);
	glUniform3fv(shaderIF->ppuLoc("ka"), 1, ka);
	glUniform3fv(shaderIF->ppuLoc("ambientStrength"), 1, ambientStrength);
	*/
	glVertexAttrib1f(shaderIF->pvaLoc("Object"), 1);

	//FRONT OF ROOF
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 19, 6);

	//BACK OF ROOF
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), -1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 25, 6);

	//ROOF SIDES
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 1.0, 0.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[3]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), -1.0, 0.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[4]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);


	/******************************************
	 now need to do normal vector calculations
	******************************************
	float* norm = nullptr;
	float u[3] = {1, 1, 0};
	float v[3] = {2, 2, 2};
	norm = getNormal(u,v);
	*/

	//RIGHT SIDE OF ROOF
	//sqrt(2) / 2 == 0.7071
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.7071, 0.7071, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[5]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), -0.7071, -0.7071, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[6]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);

	//LEFT SIDE OF ROOF
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), -0.7071, 0.7071, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[7]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.7071, -0.7071, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[8]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
}

void House::render() {
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderIF->getShaderPgmID());

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(mat));

	renderHouse();
	for (int i = 0; i < 13; i++) {
		//std::cout << i << std::endl;
		trim[i]->render();
	}

	glUseProgram(pgm);
}

float* House::getNormal(float u[3], float v[3]) {
	float normalX = u[1] * v[2] - u[2] * v[1];
	float normalY = u[2] * v[0] - u[0] * v[2];
	float normalZ = u[0] * v[1] - u[1] * v[0];

	float *norm = new float[3];
	norm[0] = normalX;
	norm[1] = normalY;
	norm[2] = normalZ;

	return norm;
}
