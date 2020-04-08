#version 150

uniform sampler2D SourceTexture;
uniform float Threshold;
out vec2 TexCoord;
out vec4 fragColor;

vec4 ExtractHighlights()
{
	vec4 c = texture(SourceTexture, TexCoord);
    return clamp((c - Threshold) / (1.0 - Threshold), 0.0, 1.0);
}

void main() 
{
    fragColor = ExtractHighlights();
}
