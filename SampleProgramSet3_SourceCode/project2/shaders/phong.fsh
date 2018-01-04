#version 410 core

// phong.fsh - a fragment shader that implements a Phong Lighting model.

in PVA
{
	vec3 ecPosition;
	vec3 ecUnitNormal;
	float obj;
} pvaIn;


out vec4 fragmentColor;

// Phong material properties (just kd for now; you will add the rest later):
//uniform vec3 kd = // "kd" - diffuse reflectivity; basic object color
//vec3(0.8, 0.0, 0.0); // default: darkish red
	//uniform vec3 kd;
	//uniform vec3 ka;
	//uniform vec3 ambientStrength;



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

	vec3 kd;
	vec3 ka;

	//value of pvaIn.obj is approximate. doesnt always equal an integer value exactly
	//may equal 0.998 instead of 1
	//so I use <= instead of == to account for this
	//poor progamming technique. Will use better solution in future projects
	vec3 ambientStrength = vec3(1.0, 1.0, 1.0);
	if (pvaIn.obj <= 0.2) {						//house ~==0
		kd = vec3(0.95, 0.08, 0.08);
		ka = vec3(0.95, 0.08, 0.08);
	} else if (pvaIn.obj <= 1.2) {		//roof ~==1
		kd = vec3(0.4, 0.2, 0.1);
		ka = vec3(0.4, 0.2, 0.1);
	} else if (pvaIn.obj <= 2.2) {		//ground ~==2
		kd = vec3(0.1, 0.7, 0.1);
		ka = vec3(0.1, 0.8, 0.1);
	} else if (pvaIn.obj <= 3.2) {		//tree trunk ~==3
		kd = vec3(0.35, 0.18, 0.08);
		ka = vec3(0.35, 0.18, 0.08);
	} else if (pvaIn.obj <= 4.2) {		//house trim ~==4
		kd = vec3(0.6, 0.6, 0.6);
		ka = vec3(0.6, 0.6, 0.6);
	} else if (pvaIn.obj <= 5.2) {		//tree leaves ~==5
		kd = vec3(0.0, 0.2, 0.0);
		ka = vec3(0.0, 0.2, 0.0);
	} else if (pvaIn.obj <= 6.2) {		//fence ~==6
		kd = vec3(0.3, 0.15, 0.08);
		ka = vec3(0.3, 0.15, 0.08);
	} else {
		kd = vec3(1.0, 1.0, 1.0);
		ka = vec3(1.0, 1.0, 1.0);
	}

	vec3 liHat = vec3(0.0, 0.0, 1.0);
	vec3 liStrength = vec3(1.0, 1.0, 1.0);
	float factor = dot(liHat, pvaIn.ecUnitNormal);

	vec3 result = ka * ambientStrength + factor * kd * liStrength;

	return vec4(result, 1.0);
}
//ka and kd establish color
void main ()
{
	fragmentColor = evaluateLightingModel();
}
