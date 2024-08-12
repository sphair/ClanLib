#version 430

in vec2 TexCoord;
out vec4 cl_FragColor;

uniform sampler2D SourceTexture;

layout (std140) uniform ProgramUniforms
{
	mat4 cl_ModelViewProjectionMatrix;
	vec4 Sample[15]; // x,y = Offset, z = Weight
};

vec4 GaussianBlur()
{
	vec4 c = vec4(0.0);
	for (int i = 0; i < 15; i++)
	{
		c += texture(SourceTexture, TexCoord + Sample[i].xy) * Sample[i].z;
	}
	return c;
}

void main()
{
	cl_FragColor = GaussianBlur();
}

