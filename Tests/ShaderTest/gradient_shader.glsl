void main()
{
	gl_FragColor = vec4(gl_FragCoord.x/640.0, gl_FragCoord.y/480.0, 0.0, 1.0);
}
