// House.h

#ifndef House_H
#define House_H

#include <GL/gl.h>

#include "ModelView.h"
#include "ShaderIF.h"

class House : public ModelView
{
public:
	House(ShaderIF* sIF, float cx, float cy, float cz, // lower left corner
	      float lx, float ly, float lz);// lengths in 3 directions
	virtual ~House();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	bool handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY);
	void render();
private:
	ShaderIF* shaderIF;
	GLuint vao[1];
	GLuint vbo[1];
	GLuint ebo[4];

	float xmin, xmax, ymin, ymax, zmin, zmax;
	float kd[3];
	//float ka[3];
	//float ambientStrength[3];

	static GLuint indexList[4][4];

	void defineHouse();
	void renderHouse();
};

#endif
