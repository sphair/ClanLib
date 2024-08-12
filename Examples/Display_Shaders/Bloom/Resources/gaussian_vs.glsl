#version 430

in vec4 Position;
in vec2 TexCoord0;

out vec2 TexCoord;

layout (std140) uniform ProgramUniforms
{
	mat4 cl_ModelViewProjectionMatrix;
	vec4 Sample[15]; // x,y = Offset, z = Weight
};

void main(void) 
{ 
	gl_Position = cl_ModelViewProjectionMatrix * Position; 
	TexCoord = TexCoord0; 
}

