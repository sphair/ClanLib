
attribute vec4 Position;
attribute float HueOffset0;
attribute vec2 TexCoord0;
varying float HueOffset;
varying vec2 TexCoord;

void main(void)
{
	gl_Position = Position;
	HueOffset = HueOffset0;
	TexCoord = TexCoord0;
}
