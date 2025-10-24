attribute vec4 Position, Color0;
attribute vec2 TexCoord0;

varying vec4 Color;
varying vec2 TexCoord;

void main(void) 
{ 
	gl_Position = gl_ModelViewProjectionMatrix*Position; 
	Color = Color0; 
	TexCoord = TexCoord0; 
}
