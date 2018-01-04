#version 410 core

// phong.fsh - a fragment shader that implements a Phong Lighting model.

in PVA {
	vec3 ecPosition;
	vec3 ecUnitNormal;
} pvaIn;

//what we are outputting
out vec4 fragmentColor;

//KD: diffuse reflectivity - basic color - white
uniform vec3 kd = vec3(1.0, 1.0, 1.0);

//KA: ambient reflectivity	- set to low by default
uniform vec3 ka = vec3(0.2, 0.2, 0.2);

//KS: specular reflectivity - white
uniform vec3 ks = vec3(0.5, 0.5, 0.5);

//IA: ambient light intensity	- white
uniform vec3 ia = vec3(1.0, 1.0, 1.0);

//Light sources		TODO: SET THESE APPROPRIATELY
uniform vec4 p_ecLightSource[] = vec4[3]( vec4(100.0, 50.0, 100.0, 1.0),
																					vec4(-100.0, 50.0, 100.0, 1.0),
																					vec4(0.0, -1.0, -1.0, 0.0));

//LI: strength of light sources - white
uniform vec3 lightStrength[] = vec3[3]( vec3(1.0, 0.0, 0.0),
																				vec3(0.0, 1.0, 0.0),
																				vec3(0.0, 0.0, 1.0));

vec4 evaluateLightingModel()
{
	// THIS IS JUST A PLACEHOLDER FOR A LIGHTING MODEL.
	// It only currently implements simple Lambert shading.

	// NOTES:
	// 1. We assume for now a single directional light source defined in EC (liHat).
	// 2. We assume it will be "full strength" (see liStrength).
	//
	// In project 3, both #1 and #2 will be generalized by introducing uniform
	// arrays ("vec4 p_ecLightSourcePos" and "vec3 ecLightSourceStrength") and
	// using them INSTEAD OF the liHat and liStrength you see here.
	//
	// 3. The use of "abs" here is a temporary hack. As we study the Phong
	//    lighting model more carefully, you will REMOVE "abs" since it will
	//    no longer be appropriate.


	//vec3 normalLocal = pvaIn.ecUnitNormal;

	vec3 liHat = vec3(0.0, 0.0, 1.0);
	vec3 liStrength = vec3(1.0, 1.0, 1.0);
	float factor = dot(liHat, pvaIn.ecUnitNormal);

	//need to set appropriately
	vec3 result = ka * ambientStrength + factor * kd * liStrength;

	return vec4(result, 1.0);
}
//ka and kd establish color
void main ()
{
	fragmentColor = evaluateLightingModel();
}
