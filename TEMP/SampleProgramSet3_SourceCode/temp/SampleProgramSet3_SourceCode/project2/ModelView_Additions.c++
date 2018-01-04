// Extracted from: ModelView.c++ - an Abstract Base Class for a combined Model and View for OpenGL

#include "ModelView.h"

void ModelView::addToGlobalPan(double dxInLDS, double dyInLDS, double dzInLDS)
{
	// TODO: Delete or comment out the following std::cout statement when
	//       everything else is done here.
	std::cout << "In project 3, you will implement ModelView::addToPan in ModelView_Additions.c++\n";
	// TODO: 1. Use last_ecXmin, et al. with dxInLDS, et al. to translate the LDS
	//          pan vector to an EC pan vector.
	//       2. UPDATE dynamic_view
	// TODO: 3. The updated dynamic_view will be used in ModelView::getMatrices
}

void ModelView::addToGlobalRotationDegrees(double rx, double ry, double rz)
{
	// TODO: Delete or comment out the following std::cout statement when
	//       everything else is done here.
	std::cout << "In project 3, you will implement ModelView::addToGlobalRotationDegrees in ModelView_Additions.c++\n";
	// TODO: 1. UPDATE dynamic_view
	// TODO: 2. The updated dynamic_view will be used in ModelView::getMatrices
}

void ModelView::getMatrices(cryph::Matrix4x4& mc_ec, cryph::Matrix4x4& ec_lds)
{
	// TODO: Delete or comment out the following std::cout statement when
	//       everything else is done here.
	std::cout << "In project 2, you must implement ModelView::getMatrices in ModelView_Additions.c++\n";
	// 1. Create the mc_ec matrix:
	//    Create a local variable of type Matrix4x4 called M_ECu from the eye,
	//    center, and up. Recall that those values are initialized in the main
	//    program and may be modified at any time while processing interactive
	//    events. Their current values are stored in protected class variables
	//    that you directly access.
	//
	cryph::Matrix4x4 M_ECu = cryph::Matrix4x4::lookAt(ModelView::eye,
			ModelView::center, ModelView::up);

	//    For project 2:
	mc_ec = M_ECu;

	//    For project 3: Either:
	//        mc_ec = dynamic_view * M_ECu (if rotations are to be about the eye)
	//    or:
	//        mc_ec = postTrans * dynamic_view * preTrans * M_ECu (if
	//                         rotations are to be about the center of attention)
	//    NOTE: If using preTrans/postTrans, be sure you build them CORRECTLY here!!!

	// 2. Create the ec_lds matrix:
	//    Use the mcRegionOfInterest. (As with eye, center, up, the mcRegionOfInterest
	//    is initialized in main and is subject to modification at any time while
	//    processing events. Its current values are also stored in protected class
	//    variables that you can directly access.

	//    2.a. Set the ECx and ECy extents of the view frustum on the projection plane.
	//         One strategy is to use the radius of the sphere that circumscribes the
	//         MC Bounding Box.
	double circumscribingSphereRadius = 0.0; // Don't use 0.0; compute it!!!
	//    2.b. In project 3 & 4: Scale "circumscribingSphereRadius" by "dynamic_zoomScale"
	//    2.c. initialize the XY direction of the view volume as follows.
	//    NOTE: last_ecXmin, et al. are ModelView class variables. See ModelView.h.
	//    BE SURE YOU UNDERSTAND WHY THE FOLLOWING INITIALIZATIONS WORK. REVIEW YOUR NOTES.
	//    YOU CAN BE PRETTY CONFIDENT YOU WILL BE ASKED A QUESTION AT LEAST RELATED TO THIS
	//    ON AN EXAM!
	last_ecXmin = -circumscribingSphereRadius; last_ecXmax = circumscribingSphereRadius;
	last_ecYmin = -circumscribingSphereRadius; last_ecYmax = circumscribingSphereRadius;
	// TODO:
	//    2.d. Use ModelView::matchAspectRatio.

	if (ModelView::projType == ORTHOGONAL)
		ec_lds = cryph::Matrix4x4::orthogonal(last_ecXmin, last_ecXmax, last_ecYmin, last_ecYmax,
				ModelView::ecZmin, ModelView::ecZmax);
	else if (ModelView::projType == PERSPECTIVE)
		ec_lds = cryph::Matrix4x4::perspective(ModelView::ecZpp,
				last_ecXmin, last_ecXmax, last_ecYmin, last_ecYmax, ModelView::ecZmin, ModelView::ecZmax);
	else // Must be OBLIQUE
		ec_lds = cryph::Matrix4x4::oblique(ModelView::ecZpp,
				last_ecXmin, last_ecXmax, last_ecYmin, last_ecYmax, ModelView::ecZmin, ModelView::ecZmax,
				ModelView::obliqueProjectionDir);

	// THEN IN THE CALLER OF THIS METHOD:
	//
	// float mat[16];
	// glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(mat));
	// glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(mat));
	//
	// (The extractColMajor method copies the elements of the matrix into the given
	// array which is assumed to be of length 16. It then returns the array pointer
	// so it can be used as indicated in the two calls. Since the array is immediately
	// copied by glUniformMatrix to the GPU, "mat" can be reused as indicated.)
}

void ModelView::scaleGlobalZoom(double multiplier)
{
	dynamic_zoomScale *= multiplier;
	// TODO: Project 3: Use dynamic_zoomScale in ModelView::getMatrices
}
