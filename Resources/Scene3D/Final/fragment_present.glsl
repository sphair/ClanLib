
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D FinalColors;
//uniform sampler2D LogAverageLight;
uniform sampler2D BloomColors;
uniform sampler2D AmbientOcclusion;

vec4 sRGB(vec4 c);
float Exposure(float light, float averageLight, float key);
float ExposureWithMaximumLuminance(float light, float averageLight, float key, float maxWhite);

void main()
{
	vec4 color = texture(FinalColors, TexCoord);
	vec4 bloom = texture(BloomColors, TexCoord);
	float ambientOcclusion = 1;//texture(AmbientOcclusion, TexCoord).x;
	color = vec4(color.rgb * ambientOcclusion, color.a);
	FragColor = sRGB(vec4(color.rgb, 1) + bloom);
}

vec4 sRGB(vec4 c)
{
	float gamma = 1.0 / 2.2;
	return vec4(pow(c.rgb, vec3(gamma)), c.a);
	// return vec4(sqrt(c.rgb), c.a); // cheaper, but assuming gamma of 2.0 instead of 2.2
}

// high-key in photography means that contrast and shadows are minimized and all is brightly (but not overly) lit
// low-key tends to maximize contrast between light and dark
// low key is around 0.045, high key is around 0.72, normal key around 0.18
float Exposure(float light, float averageLight, float key)
{
	float l = (key / averageLight) * light;
	return l / (1 + l);
}

float ExposureWithMaximumLuminance(float light, float averageLight, float key, float maxWhite)
{
	float l = (key / averageLight) * light;
	return l * (1 + l / (maxWhite * maxWhite)) / (1 + l);
}
