in vec2 UV;
out vec4 FragColor;

layout(std140) uniform Uniforms
{
	uniform vec3 resolution;
	uniform float time;
	uniform vec4 mouse;
};

uniform sampler2D Tux;

layout(origin_upper_left) in vec4 gl_FragCoord;

void main( void ) {
	float radius = length(gl_FragCoord.xy - resolution.xy/2.0);
	vec4 color = vec4(48.0/radius * (1.0 + sin(time)), 24.0/radius * (1.0 + cos(time)), 6.0/radius * (1.0 + tan(time)), 1.0);

	vec2 position = ( gl_FragCoord.xy / resolution.xy ) + mouse.xy / 4.0;

	float color2 = 0.0;
	color2 += sin( position.x * cos( time / 15.0 ) * 80.0 ) + cos( position.y * cos( time / 15.0 ) * 10.0 );
	color2 += sin( position.y * sin( time / 10.0 ) * 40.0 ) + cos( position.x * sin( time / 25.0 ) * 40.0 );
	color2 += sin( position.x * sin( time / 5.0 ) * 10.0 ) + sin( position.y * sin( time / 35.0 ) * 80.0 );
	color2 *= sin( time / 10.0 ) * 0.5;

	FragColor = mix(texture2D(Tux, UV), mix(color, vec4( vec3( color2, color2 * 0.5, sin( color2 + time / 3.0 ) * 0.75 ), 1.0), 0.5), 0.8);
}
