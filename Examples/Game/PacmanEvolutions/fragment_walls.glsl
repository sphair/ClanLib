
#version 330

in vec4 PositionInWorld;
in vec4 PositionInEye;
in vec3 NormalInEye;
in vec2 TexCoord;
in vec4 LightInEye;

out vec4 FragColor;

uniform sampler2D Texture;

vec3 sRGB(vec3 c);

void main()
{
	vec3 lightVector = normalize(LightInEye.xyz - PositionInEye.xyz);
	float attenuation = max(0.15, clamp(dot(lightVector, NormalInEye), 0, 1));
	vec4 color = texture2D(Texture, TexCoord);
	FragColor = vec4(sRGB(color.rgb * attenuation), color.a);
}

vec3 sRGB(vec3 c)
{
	float gamma = 1.0 / 2.2;
	return pow(c, vec3(gamma));
}
