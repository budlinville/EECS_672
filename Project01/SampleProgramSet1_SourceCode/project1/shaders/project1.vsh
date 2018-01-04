#version 410 core

// replace the placeholder implementation here
in vec2 coords;

uniform vec4 scaleTrans; // for mapping coordinates into Logical Device Space

void main()
{
	//	    ldsX =      sx      *  mcX        +      tx
		float ldsX = scaleTrans[0]*coords.x + scaleTrans[1];

	//	    ldsY =      sy      *  mcY        +      ty
		float ldsY = scaleTrans[2]*coords.y + scaleTrans[3];

		gl_Position = vec4(ldsX, ldsY, 0, 1);
}
