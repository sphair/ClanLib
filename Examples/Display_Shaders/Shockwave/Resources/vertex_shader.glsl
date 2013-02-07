#version 150

attribute vec4 Position;
attribute vec2 TexCoord0;

layout (std140) uniform ProgramUniforms
{
	mat4 cl_ModelViewProjectionMatrix;
	vec3 shockParams; // 10.0, 0.8, 0.1
	float time; // effect elapsed time
	vec2 center; // Mouse position
	float glow;
};

out vec2 TexCoord;

void main(void) 
{ 
	gl_Position = cl_ModelViewProjectionMatrix*Position; 
	TexCoord = TexCoord0; 
}
