// Ground.c++

#include "Ground.h"

typedef float vec3[3];

Ground::Ground(ShaderIF* sIF, PhongMaterial& matl, float cx, float cz, float l) : SceneElement(sIF, matl) {
	xmin = cx; xmax = cx + l;
	ymin = 0.0; ymax = 0.0;
	zmin = cz; zmax = cz + l;
	//t = nullptr;

	kd[0] = 0.0; kd[1] = 1.0; kd[2] = 0.1;
	ka[0] = 0.0; ka[1] = 1.0; ka[2] = 0.1;
	ambientStrength[0] = 0.1; ambientStrength[1] = 0.1; ambientStrength[2] = 0.1;

	defineGround();
}

Ground::~Ground() {
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void Ground::defineGround() {
	vec3 normals[] = {
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0}
	};

	vec3 vtx[] = {
		{xmin, ymin, zmin},
		{xmax, ymin, zmin},
		{xmin, ymin, zmax},
		{xmax, ymin, zmax}
	};
/*
	float numTets = (float)NUM_TETS_1D;
	float x_incr = (xmax - xmin) / numTets;
	float z_incr = (zmax - zmin) / numTets;
	float xPoint = xmin;
	float zPoint = zmin;

	int tmp = 0;
	for (int i = 0; i < numTets; i++) {
		for (int j = 0; j < numTets; j++) {
			cryph::AffPoint p0(xPoint, 0, zPoint);
			cryph::AffPoint p1((xPoint - .5), 3, (zPoint - .5));
			cryph::AffPoint p2((xPoint), 3, (zPoint + .5));
			cryph::AffPoint p3((xPoint + .5), 3, (zPoint - .5));

			t[tmp + j] = new Tetrahedron(shaderIF, p0, p1, p2, p3, 0.1, 1.0, 0.2);

			xPoint += x_incr;
		}
		tmp += NUM_TETS_1D;
		zPoint += z_incr;
	}
*/
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 4*sizeof(vec3), vtx, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 4*sizeof(vec3), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Ground::getMCBoundingBox(double* xyzLimits) const {
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void  Ground::renderGround() {
	glUniform3fv(shaderIF->ppuLoc("kd"), 1, matl.kd);
	glUniform3fv(shaderIF->ppuLoc("ka"), 1, matl.ka);
	glUniform3fv(shaderIF->ppuLoc("ks"), 1, matl.ks);
	
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Ground::render() {
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderIF->getShaderPgmID());

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(mat));

	renderGround();
/*
	for (int i = 1; i < NUM_TETS_2D; i++) {
		std::cout << i << std::endl;
		t[i]->render();
	}
*/
	glUseProgram(pgm);
}
