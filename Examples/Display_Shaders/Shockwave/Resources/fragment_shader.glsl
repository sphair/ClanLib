#version 150

// Shader idea and code from http://www.geeks3d.com/20091116/shader-library-2d-shockwave-post-processing-filter-glsl/
// Enhanced for clanlib

in vec2 TexCoord;
out vec4 cl_FragColor;
uniform sampler2D Texture0;

layout (std140) uniform ProgramUniforms
{
	mat4 cl_ModelViewProjectionMatrix;
	vec3 shockParams; // 10.0, 0.8, 0.1
	float time; // effect elapsed time
	vec2 center; // Mouse position
	float glow;
};

void main()
{
	vec2 uv = TexCoord;
	vec2 texture_coord = uv;
	float distance = distance(uv, center);
	if ( (distance <= (time + shockParams.z)) && (distance >= (time - shockParams.z)) )
	{
		float diff = (distance - time);
		float powDiff = 1.0 - pow(abs(diff*shockParams.x), shockParams.y);
		float diffTime = diff  * powDiff;
		vec2 diffUV = normalize(uv - center);
		texture_coord = uv + (diffUV * diffTime);

		cl_FragColor = texture(Texture0, texture_coord);

		cl_FragColor.r += powDiff * glow;
		cl_FragColor.g += powDiff * glow;
		cl_FragColor.b += powDiff * glow;

	}
	else
		cl_FragColor = texture(Texture0, texture_coord);

}
