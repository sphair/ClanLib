attribute vec4 Position, Color0;
attribute vec2 TexCoord0;
uniform mat4 cl_ModelViewProjectionMatrix;

varying vec4 Color;
varying vec2 TexCoord;

void main(void) 
{ 
	gl_Position = cl_ModelViewProjectionMatrix*Position; 
	Color = Color0; 
	TexCoord = TexCoord0; 
}
