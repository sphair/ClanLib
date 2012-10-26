uniform sampler2D SourceTexture;
uniform float Threshold;
varying vec2 TexCoord;

vec4 ExtractHighlights()
{
	vec4 c = texture2D(SourceTexture, TexCoord);
    return clamp((c - Threshold) / (1.0 - Threshold), 0.0, 1.0);
}

void main() 
{
    gl_FragColor = ExtractHighlights();
}
