#version 150
in vec2 TexCoord;

uniform sampler2D Texture;
out vec4 cl_FragColor;

layout (std140) uniform ProgramUniforms
{
	mat4 cl_ModelViewProjectionMatrix;
	float Amount;
	float Timer;
};


vec4 scanlines(in vec4 fragment)
{
    fragment.b = mix(fragment.b, ((fragment.b * sin(TexCoord.y * 3000.0)) * 3.0), Amount);
    return fragment;
}

vec4 noise()
{
    float x = TexCoord.x * TexCoord.y * 123456.0 * Timer;
    x = (mod(x, 13.0) * mod(x, 123.0));
    float dx = mod(x, 0.0015) * Amount;
    float dy = mod(x, 0.0005) * Amount;
    vec4 c = texture(Texture, (TexCoord + vec2(dx, dy)));
    return c;
}

vec4 grey(in vec4 fragment)
{
	vec3 grey = vec3(dot(fragment.rgb, vec3(0.3, 0.59, 0.11)));
	fragment.rgb = mix(fragment.rgb, grey, Amount);
	fragment.a = 1.0;
	return fragment;
}

void main() 
{
	cl_FragColor = grey(scanlines(noise()));
//	cl_FragColor = grey(scanlines(texture(Texture, TexCoord)));
}
