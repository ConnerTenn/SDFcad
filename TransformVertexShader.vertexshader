#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

// Output data ; will be interpolated for each fragment.
out vec3 fragmentColor;

void main(){	

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  vec4(vertexPosition_modelspace,1);

	// The color of each vertex will be interpolated
	// to produce the color of each fragment
	fragmentColor = vec3(1,1,1);
}

