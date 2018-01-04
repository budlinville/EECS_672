// Fence.h

#ifndef Fence_H
#define Fence_H

#include "ModelView.h"
#include "ShaderIF.h"
#include "Block.h"
#include "../mvcutil/BasicShape.h"
#include "../mvcutil/BasicShapeRenderer.h"

#define NUM_POSTS_PER_SIDE 10

class Fence : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	Fence(ShaderIF* sIF, float cx, float cy, float cz, float l, float w, float h);
	virtual ~Fence();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	ShaderIF* shaderIF;
	float kd[3];

	float xmin, xmax, ymin, ymax, zmin, zmax;
	float length;
	float postWidth;
	float postHeight;

	BasicShapeRenderer *myRenderer;
	BasicShape *horizontalPost1;
	BasicShape *horizontalPost2;
	BasicShape *horizontalPost3;
	BasicShape *horizontalPost4;
	BasicShape *horizontalPost5;
	BasicShape *horizontalPost6;
	BasicShape *horizontalPost7;
	BasicShape *horizontalPost8;

	Block *post[4 * NUM_POSTS_PER_SIDE];

	void renderFence();
	void defineFence();
};

#endif
