
/////////////////////////////////////////////////////////////////////////////
// cmyk operations

vec3 rgb2cmy(vec3 rgbColor)
{
    return (vec3(1,1,1) - rgbColor);	// simplest conversion
}

vec3 cmy2rgb(vec3 cmyColor)
{
    return (vec3(1,1,1) - cmyColor);	// simplest conversion
}

//
// There are many device-specific methods for determining 'K' (black ink).
//		This is a popular and simple one.
//
vec4 cmy2cmyk(vec3 cmyColor)
{
    float k = 1.0;
    k = min(k,cmyColor.x);
    k = min(k,cmyColor.y);
    k = min(k,cmyColor.z);
    vec4 cmykColor;
    cmykColor.xyz = (cmyColor - k)/(1.0-k);
    cmykColor.w = k;
    return cmykColor;
}

vec3 cmyk2cmy(vec4 cmykColor)
{
    float k = cmykColor.w;
    return cmykColor.xyz * (vec3(1,1,1)-k) + k;
}

vec4 rgb2cmyk(vec3 rgbColor)
{
    return cmy2cmyk(rgb2cmy(rgbColor));
}

vec3 cmyk2rgb(vec4 cmykColor)
{
    return cmy2rgb(cmyk2cmy(cmykColor));
}

/////////////////////////////////////////////////////////////////////////////
// hsv operations

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
    vec3 hsv(0.0, 0.0, 0.0);
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
    vec3 rgb = hsv.z;
    if (hsv.y != 0.0)
    {
        float h = hsv.x * 6;
        float i = floor(h);
        float v1 = hsv.z * (1.0 - hsv.y);
        float v2 = hsv.z * (1.0 - hsv.y * (h-i));
        float v3 = hsv.z * (1.0 - hsv.y * (1-(h-i)));
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

vec3 hsv_safe(vec3 InColor)
{
    vec3 safeC = InColor;
    safeC.x = frac(safeC.x);
    return safeC;
}

vec3 hsv_complement(vec3 InColor)
{
    vec3 complement = InColor;
    complement.x -= 0.5;
    if (complement.x<0.0) { complement.x += 1.0; } // faster than hsv_safe()
    return complement;
}

#define COLOR_PI (3.141592652589793238)
#define COLOR_TWO_PI (2.0 * COLOR_PI)

vec3 color_cylinder(vec3 hsv)
{
    float a = hsv.x * COLOR_TWO_PI;
    vec3 p;
    p.x = hsv.y * cos(a);
    p.y = hsv.y * sin(a);
    p.z = hsv.z;
    return p;
}

vec3 from_cylinder(vec3 p)
{
    vec3 hsv;
    hsv.z = p.z;
    float q = p.x*p.x+p.y*p.y;
    q = sqrt(q);
    hsv.y = (q);
    float a = atan2(p.y,p.x);
    hsv.x = a / COLOR_TWO_PI;
    return hsv;
}

// lerp the shortest distance through the color solid
vec3 hsv_lerp(vec3 C0, vec3 C1, float T)
{
    vec3 p0 = color_cylinder(C0);
    vec3 p1 = color_cylinder(C1);
    vec3 pg = lerp(p0,p1,T);
    return from_cylinder(pg);
}

// lerp the shorterst distance around the color wheel - ONLY color
vec3 hsv_tint(vec3 SrcColor, vec3 TintColor, float T)
{
    vec3 tt = hsv_lerp(SrcColor,TintColor,T);
    tt.yz = SrcColor.yz;
    return tt;
}

/////////////////////////////////////////////////////////////////////////////
// yuv operations

vec3 rgb_to_yuv(vec3 RGB)
{
    float y = dot(RGB,vec3(0.299,0.587,0.114));
    float u = (RGB.z - y) * 0.565;
    float v = (RGB.x - y) * 0.713;
    return vec3(y,u,v);
}

vec3 yuv_to_rgb(vec3 YUV)
{
    float u = YUV.y;
    float v = YUV.z;
    float r = YUV.x + 1.403*v;
    float g = YUV.x - 0.344*u - 1.403*v;
    float b = YUV.x + 1.770*u;
    return vec3(r,g,b);
}
