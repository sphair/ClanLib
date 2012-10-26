// Shader idea and code from http://www.geeks3d.com/20091116/shader-library-2d-shockwave-post-processing-filter-glsl/
// Enhanced for clanlib

varying vec2 TexCoord;
uniform sampler2D sceneTex; // 0
uniform vec2 center; // Mouse position
uniform float time; // effect elapsed time
uniform vec3 shockParams; // 10.0, 0.8, 0.1
uniform float glow;

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

		gl_FragColor = texture2D(sceneTex, texture_coord);

		gl_FragColor.r += powDiff * glow;
		gl_FragColor.g += powDiff * glow;
		gl_FragColor.b += powDiff * glow;

	}
	else
		gl_FragColor = texture2D(sceneTex, texture_coord);

}
