#version 430

uniform sampler2D BaseTexture;
uniform float BaseIntensity;
uniform float BaseSaturation;

uniform sampler2D BloomTexture;
uniform float BloomIntensity;
uniform float BloomSaturation;

in vec2 TexCoord;
out vec4 cl_fragColor;

vec4 AdjustSaturation(in vec4 color, in float saturation)
{
	float grey = dot(color.rgb, vec3(0.3, 0.59, 0.11));
	vec4 grey_color = vec4(vec3(grey), color.a);
	
	return mix(grey_color, color, saturation);
}

vec4 BloomCombine()
{
    vec4 bloom = texture(BloomTexture, TexCoord);
    bloom = AdjustSaturation(bloom, BloomSaturation) * BloomIntensity;

    vec4 base = texture(BaseTexture, TexCoord);
    base = AdjustSaturation(base, BaseSaturation) * BaseIntensity;
    
    base *= (1.0 - clamp(bloom, 0.0, 1.0));
    
    return base + bloom;
}

void main() 
{
    cl_fragColor = BloomCombine();
}

