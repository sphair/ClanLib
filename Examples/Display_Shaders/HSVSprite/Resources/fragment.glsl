#version 150

uniform sampler2D Texture0;
varying float HueOffset;
varying vec2 TexCoord;

float min_channel(vec3 v);
float max_channel(vec3 v);
vec3 rgb_to_hsv(vec3 rgb);
vec3 hsv_to_rgb(vec3 hsv);

void main(void)
{
	vec4 rgba = texture2D(Texture0, TexCoord);
	vec3 hsv = rgb_to_hsv(rgba.rgb);
	hsv.x += HueOffset;
	if (hsv.x < 0.0)
		hsv.x += 1.0;
	if (hsv.x > 1.0)
		hsv.x -= 1.0;
	
	gl_FragColor.rgb = hsv_to_rgb(hsv);
	gl_FragColor.a = rgba.a;
}

float min_channel(vec3 v)
{
    float t = (v.x<v.y) ? v.x : v.y;
    t = (t < v.z) ? t : v.z;
    return t;
}

float max_channel(vec3 v)
{
    float t = (v.x>v.y) ? v.x : v.y;
    t = (t > v.z) ? t : v.z;
    return t;
}

vec3 rgb_to_hsv(vec3 rgb)
{
    vec3 hsv = vec3(0.0, 0.0, 0.0);
    float min_val = min_channel(rgb);
    float max_val = max_channel(rgb);
    float delta = max_val - min_val;
    hsv.z = max_val;
    if (delta != 0.0)
    {
        hsv.y = delta / max_val;
        vec3 del_rgb = ( ( ( vec3(max_val,max_val,max_val) - rgb ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
        if (rgb.x >= max_val)
            hsv.x = del_rgb.z - del_rgb.y;
        else if (rgb.y == max_val)
            hsv.x = (1.0/3.0) + del_rgb.x - del_rgb.z;
        else if (rgb.z == max_val)
            hsv.x = (2.0/3.0) + del_rgb.y - del_rgb.x;
        if (hsv.x < 0.0)
            hsv.x += 1.0;
        if (hsv.x > 1.0)
            hsv.x -= 1.0;
    }
    return hsv;
}

vec3 hsv_to_rgb(vec3 hsv)
{
    vec3 rgb = vec3(hsv.z,hsv.z,hsv.z);
    if (hsv.y != 0.0)
    {
        float h = hsv.x * 6.0;
        float i = floor(h);
        float v1 = hsv.z * (1.0 - hsv.y);
        float v2 = hsv.z * (1.0 - hsv.y * (h-i));
        float v3 = hsv.z * (1.0 - hsv.y * (1.0-(h-i)));
        if (i == 0.0)
            rgb = vec3(hsv.z, v3, v1);
        else if (i == 1.0)
            rgb = vec3(v2, hsv.z, v1);
        else if (i == 2.0)
            rgb = vec3(v1, hsv.z, v3);
        else if (i == 3.0)
            rgb = vec3(v1, v2, hsv.z);
        else if (i == 4.0)
            rgb = vec3(v3, v1, hsv.z);
        else
            rgb = vec3(hsv.z, v1, v2);
    }
    return rgb;
}
