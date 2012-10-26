varying vec2 TexCoord;
uniform sampler2D SourceTexture;

uniform vec4 color_offset;

void main()
{
	gl_FragColor = texture2D(SourceTexture, TexCoord) + color_offset;
}
