#version 130

//Input data from the VBO. Each vertex is 2 floats for position and 4 for color.
in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

//Used to send color info to fragment shader
out vec4 fragmentColor;
out vec2 fragmentPosition;
out vec2 fragmentUV;

//Camera Matrix
uniform mat4 P;

void main() {
	//Set the x,y position on the screen
	gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0) ).xy;
	//We are in 2D, so Z = 0
	gl_Position.z = 0.0;
	
	//Indicate that coordinates are normalized
	gl_Position.w = 1.0;
	
	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
	
	fragmentUV = vertexUV;
}