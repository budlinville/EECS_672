// ModelView.c++ - a basic combined Model and View for OpenGL

#include <iostream>

#include "ModelView.h"
#include "Controller.h"
#include "ShaderIF.h"
#include <math.h>       /* pow */

double ModelView::mcRegionOfInterest[6] = { -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 };
bool ModelView::aspectRatioPreservationEnabled = true;

// NOTE: You will likely want to modify the ModelView constructor to
//       take additional parameters.
ModelView::ModelView(ShaderIF* sIF, vec4 cA, vec4 cB, vec2 tMinMax, int n, int colCount) : shaderIF(sIF) {
	tmin = tMinMax[0];
	tmax = tMinMax[1];
	nPoints = n;
	dt = (tmax - tmin)/(nPoints - 1);

	float t[nPoints];

	for (int i = 0; i < nPoints; i++) {
		t[i] = tmin + (i * dt);
	}

	vec2 coords[nPoints];
	//working here

	for (int i = 0; i < nPoints; i++) {
		coords[i][0] = cA[0] + (cA[1] * t[i]) + (cA[2] * pow(t[i], 2)) + (cA[3] * pow(t[i], 3));
		coords[i][1] = cB[0] + (cB[1] * t[i]) + (cB[2] * pow(t[i], 2)) + (cB[3] * pow(t[i], 3));
	}


	// Create and bind the VAO
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	//create the VBO's
	glGenBuffers(1, vbo);

	/*
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	int xSize = nPoints * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, xSize, xCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("xCoords"), 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("xCoords"));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	int ySize = nPoints * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, ySize, yCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("yCoords"), 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("yCoords"));
	*/

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	int xySize = nPoints * sizeof(vec2);
	glBufferData(GL_ARRAY_BUFFER, xySize, coords, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("coords"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("coords"));

	if (colCount == 0) {
 		color[0] = 0.5; color[1] = 0.0; color[2] = 0.0;
	} else if (colCount == 1) {
 		color[0] = 0.0; color[1] = 0.5; color[2] = 0.0;
 	} else if (colCount == 2) {
 		color[0] = 0.0; color[1] = 0.0; color[2] = 0.5;
 	} else if (colCount == 3) {
 		color[0] = 0.5; color[1] = 0.5; color[2] = 0.0;
 	} else if (colCount == 4) {
 		color[0] = 0.5; color[1] = 0.0; color[2] = 0.5;
 	} else if (colCount == 5) {
 		color[0] = 0.0; color[1] = 0.5; color[2] = 0.5;
 	}

	// Determine and remember the min/max coordinates
	xmin = xmax = coords[0][0];
	ymin = ymax = coords[0][1];
	for (int i = 0; i < nPoints ; i++) {
		if (coords[i][0] < xmin) {
			xmin = coords[i][0];
		} else if (coords[i][0] > xmax) {
			xmax = coords[i][0];
		}

		if (coords[i][1] < ymin) {
			ymin = coords[i][1];
		} else if (coords[i][1] > ymax) {
			ymax = coords[i][1];
		}
	}
}

ModelView::~ModelView()
{
	// TODO: delete the vertex array objects and buffers here
	if (vao[0] > 0) { // hasn't already been deleted
		glDeleteBuffers(2, vbo);
		glDeleteVertexArrays(1, vao);
		vao[0] = vbo[0] = 0;
	}
}

void ModelView::compute2DScaleTrans(float* scaleTransF) // CLASS METHOD
{
	// TODO: This code can be used as is, BUT be absolutely certain you
	//       understand everything about how it works.

	double xmin = mcRegionOfInterest[0];
	double xmax = mcRegionOfInterest[1];
	double ymin = mcRegionOfInterest[2];
	double ymax = mcRegionOfInterest[3];

	if (aspectRatioPreservationEnabled)
	{
		// preserve aspect ratio. Make "region of interest" wider or taller to
		// match the Controller's viewport aspect ratio.
		double vAR = Controller::getCurrentController()->getViewportAspectRatio();
		matchAspectRatio(xmin, xmax, ymin, ymax, vAR);
	}
int nPoints;
    // We are only concerned with the xy extents for now, hence we will
    // ignore mcRegionOfInterest[4] and mcRegionOfInterest[5].
    // Map the overall limits to the -1..+1 range expected by the OpenGL engine:
	double scaleTrans[4];
	linearMap(xmin, xmax, -1.0, 1.0, scaleTrans[0], scaleTrans[1]);
	linearMap(ymin, ymax, -1.0, 1.0, scaleTrans[2], scaleTrans[3]);
	for (int i=0 ; i<4 ; i++)
		scaleTransF[i] = static_cast<float>(scaleTrans[i]);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void ModelView::getMCBoundingBox(double* xyzLimits) const
{
	// TODO:
	// Put this ModelView instance's min and max x, y, and z extents
	// into xyzLimits[0..5]. (-1 .. +1 is OK for z direction for 2D models)
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = -1;
	xyzLimits[5] = 1;
}

bool ModelView::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{
	return true;
}

// linearMap determines the scale and translate parameters needed in
// order to map a value, f (fromMin <= f <= fromMax) to its corresponding
// value, t (toMin <= t <= toMax). Specifically: t = scale*f + trans.
void ModelView::linearMap(double fromMin, double fromMax, double toMin, double toMax,
					  double& scale, double& trans) // CLASS METHOD
{
	scale = (toMax - toMin) / (fromMax - fromMin);
	trans = toMin - scale*fromMin;
}

void ModelView::matchAspectRatio(double& xmin, double& xmax,
        double& ymin, double& ymax, double vAR)
{
	// TODO: This code can be used as is, BUT be absolutely certain you
	//       understand everything about how it works.

	double wHeight = ymax - ymin;
	double wWidth = xmax - xmin;
	double wAR = wHeight / wWidth;
	if (wAR > vAR)
	{
		// make window wider
		wWidth = wHeight / vAR;
		double xmid = 0.5 * (xmin + xmax);
		xmin = xmid - 0.5*wWidth;
		xmax = xmid + 0.5*wWidth;
	}
	else
	{
		// make window taller
		wHeight = wWidth * vAR;
		double ymid = 0.5 * (ymin + ymax);
		ymin = ymid - 0.5*wHeight;
		ymax = ymid + 0.5*wHeight;
	}
}

void ModelView::render() const
{
	// save the current GLSL program in use
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);

	// draw the triangles using our vertex and fragment shaders
	glUseProgram(shaderIF->getShaderPgmID());

	// TODO: set scaleTrans (and all other needed) uniform(s)
	float scaleTrans[4];
	compute2DScaleTrans(scaleTrans);
	glUniform4fv(shaderIF->ppuLoc("scaleTrans"), 1, scaleTrans);


	// TODO: make require primitive call(s)
	// Establish the function's color
	glUniform3fv(shaderIF->ppuLoc("color"), 1, color);

	//Bind VAO along with all VBOs associated with it
	glBindVertexArray(vao[0]);

	// Draw the line
	glDrawArrays(GL_LINE_STRIP, 0, nPoints);

	// restore the previous program
	glUseProgram(pgm);
}

void ModelView::setMCRegionOfInterest(double xyz[6])
{
	for (int i=0 ; i<6 ; i++)
		mcRegionOfInterest[i] = xyz[i];
}
