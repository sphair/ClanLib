#version 330

in vec2 UV;
out vec4 FragColor;

layout(std140) uniform Uniforms
{
	uniform vec3 resolution;
	uniform float time;
};

uniform sampler2D Tux;

layout(origin_upper_left) in vec4 gl_FragCoord;

void main( void ) {
	float radius = length(gl_FragCoord.xy - resolution.xy/2.0);
	vec4 color = vec4(48.0/radius * (1.0 + sin(time)), 24.0/radius * (1.0 + cos(time)), 6.0/radius * (1.0 + tan(time)), 1.0);
	FragColor = mix(color, texture2D(Tux, UV), 0.5);
}
