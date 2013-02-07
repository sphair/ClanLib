#version 150
attribute vec4 Position;
attribute vec2 TexCoord0;

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
	gl_Position = cl_ModelViewProjectionMatrix*Position; 
	TexCoord = TexCoord0; 
}
