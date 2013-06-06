#version 150

in vec4 Position;
in vec2 TexCoord0;
out float HueOffset;
out vec2 TexCoord;

layout (std140) uniform ProgramUniforms
{
	mat4 cl_ModelViewProjectionMatrix;
	float HueOffset0;
};

void main(void)
{
	gl_Position = cl_ModelViewProjectionMatrix*Position;
	HueOffset = HueOffset0;
	TexCoord = TexCoord0;
}
