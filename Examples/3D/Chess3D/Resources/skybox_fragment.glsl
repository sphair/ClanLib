
varying vec3 v;
uniform samplerCube texture1;

void main (void)  
{  
	vec3 cube_color = vec3(textureCube(texture1, v.xyz));
	gl_FragColor = vec4(cube_color, 1.0);
}
