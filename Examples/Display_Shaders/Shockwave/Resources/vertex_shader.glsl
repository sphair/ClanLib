#version 430

in vec4 Position;
in vec2 TexCoord0;

out vec2 TexCoord;

layout (std140) uniform ProgramUniforms
{
    mat4 cl_ModelViewProjectionMatrix;
    vec3 shockParams; // 10.0, 0.8, 0.1
    float time;       // effect elapsed time
    vec2 center;      // Mouse position
    float glow;
};

void main(void) 
{ 
    gl_Position = cl_ModelViewProjectionMatrix * Position; 
    TexCoord = TexCoord0; 
}

