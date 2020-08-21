#version 130
//The fragment shader operates on each pixel in a given poligon

//Receives color info from vertex shader
in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

//A 3 component float vector is outputted to the screen for each pixel.
out vec4 color;

uniform sampler2D mySampler;

void main() {
	
	vec4 textureColor = texture(mySampler, fragmentUV);
	
	color = textureColor * fragmentColor;
	
}