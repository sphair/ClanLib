#version 150
// Shader based on: http://www.geeks3d.com/20091009/shader-library-night-vision-post-processing-filter-glsl/

in vec2 TexCoord;
out vec4 cl_FragColor;
uniform sampler2D sceneBuffer;
uniform sampler2D noiseTex;
uniform sampler2D maskTex;
layout (std140) uniform ProgramUniforms
{
	mat4 cl_ModelViewProjectionMatrix;
	float elapsedTime; // seconds
	float luminanceThreshold;
	float colorAmplification;
	float effectCoverage;
};

void main ()
{
	vec4 finalColor;
	// Set effectCoverage to 1.0 for normal use.
	if (TexCoord.x < effectCoverage)
	{
		vec2 uv;
		uv.x = 0.4*sin(elapsedTime*50.0);
		uv.y = 0.4*cos(elapsedTime*50.0);
		float m = texture(maskTex, TexCoord).r;
		vec3 n = texture(noiseTex, (TexCoord*3.5) + uv).rgb;
		vec3 c = texture(sceneBuffer, TexCoord + (n.xy*0.005)).rgb;

		float lum = dot(vec3(0.30, 0.59, 0.11), c);

		if (lum < luminanceThreshold)
			c *= colorAmplification; 

		vec3 visionColor = vec3(0.1, 0.95, 0.2);
		finalColor.rgb = (c + (n*0.2)) * visionColor * m;
	}
	else
	{
		finalColor = texture(sceneBuffer,
		TexCoord);
	}

	cl_FragColor.rgb = finalColor.rgb;
	cl_FragColor.a = 1.0;
	}

