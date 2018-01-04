// Tree.c++

#include "Tree.h"

Tree::Tree(ShaderIF* sIF, PhongMaterial& matl, float cx, float cy, float cz, float th, float tr, float lr, float tt) : SceneElement(sIF, matl) {
	xCenter = cx;
	zCenter = cz;

	trunkHeight = th;
	trunkRadius = tr;
	leavesRadius = lr;

	xmin = cx - leavesRadius;
	xmax = cx + leavesRadius;
	ymin = cy;
	ymax = cy + trunkHeight + leavesRadius;
	zmin = cz - leavesRadius;
	zmax = cz + leavesRadius;

	treeType = tt;


	defineTree();
}

Tree::~Tree() {
	delete trunk;
	delete treeLeaves1;
	delete treeLeaves2;
	delete treeLeaves3;
	delete treeLeaves4;
	delete treeLeaves5;
}

void Tree::defineTree() {
	cryph::AffPoint trunkBottom(xCenter, ymin, zCenter);
	cryph::AffPoint trunkTop(xCenter, ymin + trunkHeight, zCenter);
	trunk = BasicShape::makeBoundedCylinder(trunkBottom, trunkTop, trunkRadius, 30, 30);

	if (treeType == 0.0) {
		cryph::AffPoint center(xCenter, (ymin + trunkHeight + .3 * leavesRadius), zCenter);
		treeLeaves1 = BasicShape::makeSphere(center, leavesRadius, 30, 30);

		center.assign((xCenter - .6 * leavesRadius), (ymin + trunkHeight), zCenter);
		treeLeaves2 = BasicShape::makeSphere(center, leavesRadius, 30, 30);

		center.assign((xCenter + .6 * leavesRadius), (ymin + trunkHeight), zCenter);
		treeLeaves3 = BasicShape::makeSphere(center, leavesRadius, 30, 30);

		center.assign(xCenter, (ymin + trunkHeight - .2 * leavesRadius), (zCenter - .4 * leavesRadius));
		treeLeaves4 = BasicShape::makeSphere(center, leavesRadius, 30, 30);

		center.assign(xCenter, (ymin + trunkHeight - .2 * leavesRadius), (zCenter + .4 * leavesRadius));
		treeLeaves5 = BasicShape::makeSphere(center, leavesRadius, 30, 30);
	} else {
		float leaves1Bottom = ymin + trunkHeight;
		float leaves1Top = ymin + 4.0 * trunkHeight / 3.0;
		float leaves2Bottom = ymin + trunkHeight / 3.0;
		float leaves2Top = ymin + 2.0 * trunkHeight / 3.0;
		float leaves3Bottom = ymin + 2.0 * trunkHeight / 3.0;
		float leaves3Top = ymin + trunkHeight;

		cryph::AffPoint bottom(xCenter, leaves1Bottom, zCenter);
		cryph::AffPoint top(xCenter, leaves1Top, zCenter);
		treeLeaves1 = BasicShape::makeBoundedCone(bottom, top, leavesRadius, 1.0, 30, 30);

		bottom.assign(xCenter, leaves2Bottom, zCenter);
		top.assign(xCenter, leaves2Top, zCenter);
		treeLeaves2 = BasicShape::makeBoundedCone(bottom, top, (4.0 * leavesRadius / 3.0), trunkRadius, 30, 30);

		bottom.assign(xCenter, leaves3Bottom, zCenter);
		top.assign(xCenter, leaves3Top, zCenter);
		treeLeaves3 = BasicShape::makeBoundedCone(bottom, top, (6.0 * leavesRadius / 5.0), trunkRadius, 30, 30);

		treeLeaves4 = nullptr;
		treeLeaves5 = nullptr;
	}
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Tree::getMCBoundingBox(double* xyzLimits) const {
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Tree::renderTree() {
	glUniform3fv(shaderIF->ppuLoc("kd"), 1, matl.kd);
	glUniform3fv(shaderIF->ppuLoc("ka"), 1, matl.ka);
	glUniform3fv(shaderIF->ppuLoc("ks"), 1, matl.ks);

	myRenderer = new BasicShapeRenderer(shaderIF, trunk);
	myRenderer->drawShape();

	PhongMaterial trimPhong = PhongMaterial(0.0, 0.2, 0.0, 0.0, 0.2, 0.0, 0.0, 0.2, 0.0, 1.0, 1.0);
	glUniform3fv(shaderIF->ppuLoc("kd"), 1, trimPhong.kd);
	glUniform3fv(shaderIF->ppuLoc("ka"), 1, trimPhong.ka);
	glUniform3fv(shaderIF->ppuLoc("ks"), 1, trimPhong.ks);

	if (treeType == 0.0) {
		myRenderer =  new BasicShapeRenderer(shaderIF, treeLeaves1);
		myRenderer->drawShape();

		myRenderer =  new BasicShapeRenderer(shaderIF, treeLeaves2);
		myRenderer->drawShape();

		myRenderer =  new BasicShapeRenderer(shaderIF, treeLeaves3);
		myRenderer->drawShape();

		myRenderer =  new BasicShapeRenderer(shaderIF, treeLeaves4);
		myRenderer->drawShape();

		myRenderer =  new BasicShapeRenderer(shaderIF, treeLeaves5);
		myRenderer->drawShape();
	} else {
		myRenderer =  new BasicShapeRenderer(shaderIF, treeLeaves1);
		myRenderer->drawShape();

		myRenderer =  new BasicShapeRenderer(shaderIF, treeLeaves2);
		myRenderer->drawShape();

		myRenderer =  new BasicShapeRenderer(shaderIF, treeLeaves3);
		myRenderer->drawShape();
	}
}


void Tree::render() {
	// 1. Save current and establish new current shader program
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderIF->getShaderPgmID());

	// 2. Establish "mc_ec" and "ec_lds" matrices
	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(mat));

	renderTree();

	glUseProgram(pgm);
}
