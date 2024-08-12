#version 430

in vec4 Position;
in vec2 TexCoord0;

layout (std140) uniform ProgramUniforms
{
	mat4 cl_ModelViewProjectionMatrix;
	float elapsedTime; // seconds
	float luminanceThreshold;
	float colorAmplification;
	float effectCoverage;
};

out vec2 TexCoord;

void main(void) 
{ 
	gl_Position = cl_ModelViewProjectionMatrix * Position; 
	TexCoord = TexCoord0; 
}

