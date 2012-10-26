attribute vec4 Position;
attribute vec2 TexCoord0;
uniform mat4 cl_ModelViewProjectionMatrix;

varying vec2 TexCoord;

void main(void) 
{ 
	gl_Position = cl_ModelViewProjectionMatrix*Position; 
	TexCoord = TexCoord0; 
}
