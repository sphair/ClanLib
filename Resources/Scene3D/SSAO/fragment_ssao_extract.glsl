
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D NormalZ;
uniform vec3 SampleVector[160];
uniform float SampleWeight[160];
uniform float f;
uniform float f_div_aspect;

float SampleAttenuation(int sampleIndex, vec3 positionInEye);

void main()
{
	ivec2 screenPos = ivec2(gl_FragCoord.xy);
	int patchStart = (screenPos.x % 4 + (screenPos.y % 4) * 4) * 10;

	vec2 positionProjected = TexCoord * 2 - 1;
	vec3 positionInEye = vec3(positionProjected.x / f_div_aspect, positionProjected.y / f, 1) * texture(NormalZ, TexCoord).w;

	float attenuation = clamp(
		SampleAttenuation(patchStart + 0, positionInEye) +
		SampleAttenuation(patchStart + 1, positionInEye) +
		SampleAttenuation(patchStart + 2, positionInEye) +
		SampleAttenuation(patchStart + 3, positionInEye) +
		SampleAttenuation(patchStart + 4, positionInEye) +
		SampleAttenuation(patchStart + 5, positionInEye) +
		SampleAttenuation(patchStart + 6, positionInEye) +
		SampleAttenuation(patchStart + 7, positionInEye) +
		SampleAttenuation(patchStart + 8, positionInEye) +
		SampleAttenuation(patchStart + 9, positionInEye),
		0.0, 1.0);
	FragColor = vec4(attenuation);
}

float SampleAttenuation(int sampleIndex, vec3 positionInEye)
{
	vec3 samplePositionInEye = positionInEye + SampleVector[sampleIndex];
	vec2 sampleNormalized = vec2(samplePositionInEye.x * f_div_aspect, samplePositionInEye.y * f) / samplePositionInEye.z;
	float depthAtSamplePosition = texture(NormalZ, sampleNormalized * 0.5 + 0.5).w;
	bool depthTestPass = depthAtSamplePosition > samplePositionInEye.z;
	return depthTestPass ? SampleWeight[sampleIndex] : 0.0f;
}
