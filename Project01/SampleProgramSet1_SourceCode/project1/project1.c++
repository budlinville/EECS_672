// project1.c++

#include "GLFWController.h"
#include "ModelView.h"
#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char* argv[]) {
	GLFWController c("PROJECT 1");
	c.reportVersions(std::cout);

	ShaderIF* sIF = new ShaderIF("shaders/project1.vsh", "shaders/project1.fsh");

	// TODO: one or more ModelView dynamic allocations, adding
	//       each to the Controller using "c.addModel(...);"
	//
	//       NOTE: You will likely want to modify the ModelView
	//             constructor to take additional parameters.

	//initialize fileStream
	std::string fileName = argv[1];
  std::ifstream inp(fileName);
	double nextVal = 0;
	int count = 0;

	vec4 coordsA = {0.0, 0.0, 0.0 ,0.0};
	vec4 coordsB = {0.0, 0.0, 0.0 ,0.0};
	vec2 minMax = {0.0, 0.0};
	int numPoints = 0;
	int colorCount = 0;			//variable used for iterating between 6 colors

	while (inp >> nextVal) {
		if (count < 4) {
			coordsA[count] = nextVal;
		} else if (count < 8) {
			coordsB[count - 4] = nextVal;
		} else if (count < 10) {
			minMax[count - 8] = nextVal;
		} else {			//count = 10
			numPoints = nextVal;
		}

		count++;

		//read in 11 numbers per model view instance
		if (count >= 11) {			//will never actually be greater than 11
			c.addModel( new ModelView(sIF, coordsA, coordsB, minMax, numPoints, colorCount) );
			count = 0;
			colorCount++;
			if (colorCount > 5) {
				colorCount = 0;
			}
		}
	}


/***************************************************
										  TESTING
****************************************************
vec4 coordsA = {7.69375, 1.89999, -1.99686, -0.20626};
vec4 coordsB = {-11.7812, 4.19375, 2.89376, -2.62501};
vec3 vecN = {0.0, 1.0, 30};

c.addModel( new ModelView(sIF, coordsA, coordsB, vecN, 1) );
****************************************************
                     PURPOSES
***************************************************/



	// initialize 2D viewing information:
	// Get the overall scene bounding box in Model Coordinates:
	double xyz[6]; // xyz limits, even though this is 2D
	c.getOverallMCBoundingBox(xyz);
	// Tell class ModelView we initially want to see the whole scene:
	ModelView::setMCRegionOfInterest(xyz);

	glClearColor(1, 1, 1, 1);
	c.run();

	delete sIF;

	return 0;
}
