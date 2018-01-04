// Ground.h

#ifndef Ground_H
#define Ground_H

#include "ModelView.h"
#include "ShaderIF.h"
#include "GLFWController.h"
#include "SceneElement.h"
#include "PhongMaterial.h"

#define NUM_TETS_1D (int)5
#define  NUM_TETS_2D (int)(NUM_TETS_1D * NUM_TETS_1D)

class Ground : public SceneElement
{
public:
	Ground(ShaderIF* sIF, PhongMaterial& matl, float cx, float cz, float l);
	virtual ~Ground();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	float xmin, xmax, ymin, ymax, zmin, zmax;
	GLuint vao[1];
	GLuint vbo[2];
	float kd[3];
	float ka[3];
	float ambientStrength[3];
	//Tetrahedron *t[];

	void defineGround();
	void renderGround();
};

#endif
