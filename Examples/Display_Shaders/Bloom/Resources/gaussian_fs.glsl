varying vec2 TexCoord;
uniform vec2 SampleOffsets[15];
uniform float SampleWeights[15];
uniform sampler2D SourceTexture;

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
	gl_FragColor = GaussianBlur();
}
