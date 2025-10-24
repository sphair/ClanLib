
attribute vec4 Position;
attribute float HueOffset0;
attribute vec2 TexCoord0;
varying float HueOffset;
varying vec2 TexCoord;
uniform mat4 cl_ModelViewProjectionMatrix;

void main(void)
{
	gl_Position = cl_ModelViewProjectionMatrix*Position;
	HueOffset = HueOffset0;
	TexCoord = TexCoord0;
}
