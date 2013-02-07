#version 150
in vec2 TexCoord;
out vec4 cl_FragColor;

uniform sampler2D SourceTexture;

layout (std140) uniform ProgramUniforms
{
	mat4 cl_ModelViewProjectionMatrix;
	vec2 SampleOffsets[15];
	float SampleWeights[15];
};

vec4 GaussianBlur()
{
	vec4 c = vec4(0.0);
	for (int i = 0 ; i < 15; i++)
	{
		c += texture2D(SourceTexture, (TexCoord + SampleOffsets[i])) * SampleWeights[i];
	}

	return c;
}

void main()
{
	cl_FragColor = GaussianBlur();
}
