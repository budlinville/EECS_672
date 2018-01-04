// Tree.h
#include "Block.h"

#ifndef Tree_H
#define Tree_H

#include "ModelView.h"
#include "ShaderIF.h"
#include "../mvcutil/BasicShape.h"
#include "../mvcutil/BasicShapeRenderer.h"
#include "SceneElement.h"
#include "PhongMaterial.h"

class Tree : public SceneElement {
public:
	// As before: you will likely want to add parameters to the constructor
	Tree(ShaderIF* sIF, PhongMaterial& matl, float cx, float cy, float cz, float th, float tr, float lr, float tt);
	virtual ~Tree();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	float kd[3];

	GLuint vao[1];
	GLuint vbo[1];

	float xmin, xmax, ymin, ymax, zmin, zmax;
	float treeType;
	float trunkRadius, trunkHeight, leavesRadius;
	float xCenter, zCenter;

	BasicShapeRenderer *myRenderer;
	BasicShape *trunk;
	BasicShape *treeLeaves1;
	BasicShape *treeLeaves2;
	BasicShape *treeLeaves3;
	BasicShape *treeLeaves4;
	BasicShape *treeLeaves5;


	void defineTree();
	void renderTree();
};

#endif
