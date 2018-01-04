// project2.c++: Starter for EECS 672 Project 2

#include "GLFWController.h"
#include "Block.h"
#include "House.h"
#include "Ground.h"
#include "Tree.h"
#include "Fence.h"

typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];

void set3DViewingInformation(double xyz[6])
{
	ModelView::setMCRegionOfInterest(xyz);
	// Set values for eye-center-up to produce a reasonable off-axis
	// mathod for determining reasonable view of scene, as discussed in lecture
	double maxDelta = xyz[1] - xyz[0];
	double delta = xyz[3] - xyz[2];
	if (delta > maxDelta)
		maxDelta = delta;
	delta = xyz[5] - xyz[4];
	if (delta > maxDelta)
		maxDelta = delta;
	//  Determine the center of the created scene:
	double xmid = 0.5 * (xyz[0] + xyz[1]);
	double ymid = 0.5 * (xyz[2] + xyz[3]);
	double zmid = 0.5 * (xyz[4] + xyz[5]);
	cryph::AffPoint center(xmid, ymid, zmid);

	// Set reasonable eye point
	double distEyeCenter = 2.0 * maxDelta;
	cryph::AffPoint eye(xmid, ymid, zmid + distEyeCenter);

	// up vector
	cryph::AffVector up = cryph::AffVector::yu;

	// Notify the ModelView of our MC->EC viewing requests:
	ModelView::setEyeCenterUp(eye, center, up);


	//place projection plane roughly at front of scene and place
	//eye coordinate clipping planes relative to it
	double ecZpp = -(distEyeCenter - 0.5*maxDelta);
	double ecZmin = ecZpp - maxDelta;
	double ecZmax = ecZpp + 0.5*maxDelta;

	// Set values for ecZpp, ecZmin, ecZmax that make sense in the context of
	// the EC system established above, then:

	ModelView::setProjection(PERSPECTIVE);
	ModelView::setProjectionPlaneZ(ecZpp);
	ModelView::setECZminZmax(ecZmin, ecZmax);
}

int main(int argc, char* argv[])
{
	GLFWController c("Simple Farm Model", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);

	ShaderIF* sIF = new ShaderIF("shaders/basic.vsh", "shaders/phong.fsh");

	// create your scene, adding things to the Controller....
	vec3 corner = {0, 0, 0};
	float length = 100;
	//c.addModel(new House(sIF, 80, 120, 50, 40));
	c.addModel(new Ground(sIF, 0, 0, 420));

	c.addModel(new House(sIF, 320, 0, 60, 80));
	c.addModel(new House(sIF, 40, 0, 240, 80));

	c.addModel(new Tree(sIF, 290, 0, 290, 50, 6, 20, 0));
	c.addModel(new Tree(sIF, 400, 0, 175, 50, 6, 20, 1));
	c.addModel(new Tree(sIF, 270, 0, 90, 50, 6, 20, 0));
	c.addModel(new Tree(sIF, 120, 0, 40, 50, 6, 20, 0));
	c.addModel(new Tree(sIF, 20, 0, 290, 50, 6, 20, 0));
	c.addModel(new Tree(sIF, 180, 0, 350, 50, 6, 20, 1));

	c.addModel(new Fence(sIF, 0, 0, 0, 420, 4, 15));

	glClearColor(1.0, 1.0, 1.0, 1.0);

	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	set3DViewingInformation(xyz);

	glClearColor(0.6, 0.8, 1.0, 1.0);
	c.run();

	delete sIF;

	return 0;
}
