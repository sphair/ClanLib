
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D FinalColors;

void main()
{
	vec4 color = texture(FinalColors, TexCoord);
	const float blurBegin = 0.5;
	const float blurEnd = 1.0;
	FragColor = vec4(clamp((color.rgb - blurBegin) / (blurEnd - blurBegin), 0, 1), 0);
}
