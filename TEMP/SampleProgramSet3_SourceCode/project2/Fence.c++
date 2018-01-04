// Fence.c++

#include "Fence.h"

Fence::Fence(ShaderIF* sIF, float cx, float cy, float cz, float l, float w, float h) : shaderIF(sIF) {
	xmin = cx;
	xmax = cx + l;
	ymin = cy;
	ymax = cy + l;
	zmin = cz;
	zmax = cz + l;

	length = l;
	postWidth = w;
	postHeight = h;

	for (int i = 0; i < (4 * NUM_POSTS_PER_SIDE); i++) {
		post[i] = nullptr;
	}

	defineFence();
}

Fence::~Fence() {
	delete[] post;

	delete horizontalPost1;
	delete horizontalPost2;
	delete horizontalPost3;
	delete horizontalPost4;
	delete horizontalPost5;
	delete horizontalPost6;
	delete horizontalPost7;
	delete horizontalPost8;
}

void Fence::defineFence() {
	float delta = length / NUM_POSTS_PER_SIDE;
	float newPost = xmin;
	int index = 0;

	for (int i = 0; i < NUM_POSTS_PER_SIDE; i++) {
		post[index] = new Block(shaderIF, newPost, ymin, zmin, postWidth, postHeight, postWidth);
		newPost += delta;
		index++;
	}


	index = NUM_POSTS_PER_SIDE;
	newPost = zmin;
	for (int i = 0; i < NUM_POSTS_PER_SIDE; i++) {
		post[index] = new Block(shaderIF, (xmax - postWidth), ymin, newPost, postWidth, postHeight, postWidth);
		newPost += delta;
		index++;
	}


	index = 2 * NUM_POSTS_PER_SIDE;
	newPost = xmax - postWidth;
	for (int i = 0; i < NUM_POSTS_PER_SIDE; i++) {
		post[index] = new Block(shaderIF, newPost, ymin, (zmax - postWidth), postWidth, postHeight, postWidth);
		newPost -= delta;
		index++;
	}


	index = 3 * NUM_POSTS_PER_SIDE;
	newPost = zmax - postWidth;
	for (int i = 0; i < NUM_POSTS_PER_SIDE; i++) {
		post[index] = new Block(shaderIF, xmin, ymin, newPost, postWidth, postHeight, postWidth);
		newPost -= delta;
		index++;
	}

	double postRadius = (postWidth / 4.0);
	float ph1 = ymin + (2.0 * postHeight / 3.0);
	float ph2 = ymin + (postHeight / 3.0);

	//BACK TWO
	cryph::AffPoint circle1(xmin, ph1, (zmin + .5 * postWidth));
	cryph::AffPoint circle2(xmax, ph1, (zmin + .5 * postWidth));
	horizontalPost1 = BasicShape::makeBoundedCylinder(circle1, circle2, postRadius, 30, 30);
	circle1.assign(xmin, ph2, (zmin + .5 * postWidth));
	circle2.assign(xmax, ph2, (zmin + .5 * postWidth));
	horizontalPost2= BasicShape::makeBoundedCylinder(circle1, circle2, postRadius, 30, 30);

	//FRONT TWO
	circle1.assign(xmin, ph1, (zmax - .5 * postWidth));
	circle2.assign(xmax, ph1, (zmax - .5 * postWidth));
	horizontalPost3 = BasicShape::makeBoundedCylinder(circle1, circle2, postRadius, 30, 30);
	circle1.assign(xmin, ph2, (zmax - .5 * postWidth));
	circle2.assign(xmax, ph2, (zmax - .5 * postWidth));
	horizontalPost4 = BasicShape::makeBoundedCylinder(circle1, circle2, postRadius, 30, 30);

	//LEFT TWO
	circle1.assign((xmin + .5 * postWidth), ph1, zmin);
	circle2.assign((xmin + .5 * postWidth), ph1, zmax);
	horizontalPost5 = BasicShape::makeBoundedCylinder(circle1, circle2, postRadius, 30, 30);
	circle1.assign((xmin + .5 * postWidth), ph2, zmin);
	circle2.assign((xmin + .5 * postWidth), ph2, zmax);
	horizontalPost6 = BasicShape::makeBoundedCylinder(circle1, circle2, postRadius, 30, 30);

	//RIGHT TWO
	circle1.assign((xmax - .5 * postWidth), ph1, zmin);
	circle2.assign((xmax - .5 * postWidth), ph1, zmax);
	horizontalPost7 = BasicShape::makeBoundedCylinder(circle1, circle2, postRadius, 30, 30);
	circle1.assign((xmax - .5 * postWidth), ph2, zmin);
	circle2.assign((xmax - .5 * postWidth), ph2, zmax);
	horizontalPost8 = BasicShape::makeBoundedCylinder(circle1, circle2, postRadius, 30, 30);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Fence::getMCBoundingBox(double* xyzLimits) const {
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Fence::renderFence() {
	for (int i = 0; i < (NUM_POSTS_PER_SIDE * 4); i++) {
		//std::cout << i << std::endl;
		post[i]->render();
	}

	glVertexAttrib1f(shaderIF->pvaLoc("Object"), 6);
	myRenderer = new BasicShapeRenderer(shaderIF, horizontalPost1);
	myRenderer->drawShape();
	myRenderer = new BasicShapeRenderer(shaderIF, horizontalPost2);
	myRenderer->drawShape();
	myRenderer = new BasicShapeRenderer(shaderIF, horizontalPost3);
	myRenderer->drawShape();
	myRenderer = new BasicShapeRenderer(shaderIF, horizontalPost4);
	myRenderer->drawShape();
	myRenderer = new BasicShapeRenderer(shaderIF, horizontalPost5);
	myRenderer->drawShape();
	myRenderer = new BasicShapeRenderer(shaderIF, horizontalPost6);
	myRenderer->drawShape();
	myRenderer = new BasicShapeRenderer(shaderIF, horizontalPost7);
	myRenderer->drawShape();
	myRenderer = new BasicShapeRenderer(shaderIF, horizontalPost8);
	myRenderer->drawShape();
}

void Fence::render() {
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

	renderFence();

	glUseProgram(pgm);
}
