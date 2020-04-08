#version 150

in vec4 Position;
in vec2 TexCoord0;
uniform mat4 cl_ModelViewProjectionMatrix;

out vec2 TexCoord;

void main(void) 
{ 
	gl_Position = cl_ModelViewProjectionMatrix*Position; 
	TexCoord = TexCoord0; 
}
