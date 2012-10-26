
attribute vec4 Position;
attribute float HueOffset0;
attribute vec2 TexCoord0;
uniform mat4 cl_ModelViewProjectionMatrix;
varying float HueOffset;
varying vec2 TexCoord;

void main(void)
{
	gl_Position = cl_ModelViewProjectionMatrix*Position;
	HueOffset = HueOffset0;
	TexCoord = TexCoord0;
}
