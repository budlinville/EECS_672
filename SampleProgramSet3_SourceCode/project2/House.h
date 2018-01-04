// House.h

#ifndef House_H
#define House_H

#include <GL/gl.h>

#include "ModelView.h"
#include "ShaderIF.h"
#include "Block.h"

class House : public ModelView
{
public:
	House(ShaderIF* sIF, float cx, float cy, float cz, /* lower left corner */
				float l); // length in 3 directions
	virtual ~House();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	void render();
private:
	ShaderIF* shaderIF;
	GLuint vao[1];
	GLuint vbo[1];
	GLuint ebo[9];

	float xmin, xmax, ymin, ymax, zmin, zmax;
	float length;
	float yHeight;

	float kd[3];
	float ka[3];
	float ambientStrength[3];

	static GLuint sideWalls[2][4];
	static GLuint backWall[5];

	static GLuint roofRight[4];
	static GLuint roofLeft[4];
	static GLuint roofRightTop[4];
	static GLuint roofRightBottom[4];
	static GLuint roofLeftTop[4];
	static GLuint roofLeftBottom[4];

	Block *trim[13];

	void defineHouse();
	void renderHouse();
	float* getNormal(float u[3], float v[3]);
};

#endif
