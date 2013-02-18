
#define DiffuseUV UVMap0
#define BumpMapUV UVMap1
#define LightMapUV UVMap2
#define SpecularUV UVMap3

layout(std140) uniform ModelUniformBlock
{
	mat4 UVTextureMatrix0;
	mat4 UVTextureMatrix1;
	mat4 UVTextureMatrix2;
	mat4 UVTextureMatrix3;
	vec4 MaterialAmbient;
	vec4 MaterialDiffuse;
	vec4 MaterialSpecular;
	float MaterialGlossiness;
	float MaterialSpecularLevel;
};

in vec3 NormalInEye;
in vec3 BitangentInEye;
in vec3 TangentInEye;
in vec4 PositionInWorld;
in vec4 PositionInEye;

in vec2 UVMap0;
in vec2 UVMap1;
in vec2 UVMap2;
in vec2 UVMap3;

uniform sampler2D DiffuseTexture;
uniform sampler2D BumpMapTexture;
uniform sampler2D LightMapTexture;
uniform sampler2D SpecularTexture;

out vec3 FragDiffuseColor;
out vec4 FragSpecularColor;
out vec2 FragSpecularLevel;
out vec4 FragNormal;

vec3 NormalInEyeNormalized;

void ApplyNormalMap();
vec3 AmbientColor();
vec3 DiffuseColor();
vec3 SpecularColor();

void main()
{
	ApplyNormalMap();
	FragDiffuseColor = DiffuseColor();
	FragSpecularColor = vec4(SpecularColor(), 1);
	FragSpecularLevel = vec2(MaterialGlossiness, MaterialSpecularLevel);
	FragNormal = vec4(NormalInEyeNormalized, PositionInEye.z);
}

#if defined(BUMPMAP_UV)

void ApplyNormalMap()
{
	vec3 frontNormal = gl_FrontFacing ? NormalInEye : -NormalInEye;
	vec3 frontTangent = gl_FrontFacing ? TangentInEye : -TangentInEye;
	vec3 frontBitangent = gl_FrontFacing ? BitangentInEye : -BitangentInEye;

#if defined(USE_DF_FUNCTIONS)
	vec3 q0 = dFdx(PositionInEye.xyz);
	vec3 q1 = dFdy(PositionInEye.xyz);
	vec2 st0 = dFdx(BumpMapUV.st);
	vec2 st1 = dFdy(BumpMapUV.st);
	vec3 S = normalize( q0 * st1.t - q1 * st0.t);
	vec3 T = normalize(-q0 * st1.s + q1 * st0.s);
	vec3 normal = texture2D(BumpMapTexture, BumpMapUV).rgb * 2 - 1;
	NormalInEyeNormalized = normalize(normal.x * S + normal.y * T + normal.z * frontNormal);

#else

#if defined(PARALLAX_EFFECT)
	float holeDepth = 0.2;
	mat3 ToTangentSpace = mat3(frontBitangent, frontTangent, frontNormal); // left hand system
	//mat3 ToTangentSpace = mat3(frontTangent, frontBitangent, frontNormal); // right hand system

	//float depth = (texture2D(BumpMapTexture, BumpMapUV).a) * holeDepth;
	//vec2 offset = depth * EyeToTangentSpace * BumpMapUV;

	vec2 offset = holeDepth * ToTangentSpace * BumpMapUV;
	float depth_at_1 = texture2D(BumpMapTexture, BumpMapUV + offset).w;
	if (depth_at_1 < 0.96f)
		offset *= (depth_at_1 + (texture2D(BumpMapTexture, BumpMapUV).w)) * 0.5;

	BumpMapUV += offset;
	DiffuseUV += offset;
#endif
/*
	vec3 normal = normalize(2.0 * texture2D(BumpMapTexture, BumpMapUV).rgb - 1.0);
	normal = vec3(dot(normal, frontTangent), dot(normal, frontBitangent), dot(normal, frontNormal));
	NormalInEyeNormalized = normalize(normal);
*/

	float bumpAmount = 0.30;
	vec3 normal = texture2D(BumpMapTexture, BumpMapUV).rgb * 2 - 1;
	//vec3 normal = texture2D(BumpMapTexture, BumpMapUV).rgb;
	//normal.xy = normal.xy * 2.0 - 1.0; // transform to [-1,1] range
	//normal.z = sqrt(1.0 - dot(normal.xy, normal.xy)); // compute z component
	normal = normal.x * frontTangent - normal.y * frontBitangent + normal.z * frontNormal;
	NormalInEyeNormalized = normalize(mix(frontNormal, normal, bumpAmount));
#endif
}

#else

void ApplyNormalMap()
{
	vec3 frontNormal = gl_FrontFacing ? NormalInEye : -NormalInEye;
	NormalInEyeNormalized = normalize(frontNormal);
}

#endif

#if defined(DIFFUSE_UV)

vec3 AmbientColor()
{
	return texture2D(DiffuseTexture, DiffuseUV).rgb;
}

vec3 DiffuseColor()
{
	return texture2D(DiffuseTexture, DiffuseUV).rgb;
}

#else

vec3 AmbientColor()
{
	return MaterialAmbient.xyz;
}

vec3 DiffuseColor()
{
	return MaterialDiffuse.xyz;
}

#endif

#if defined(SPECULAR_UV)

vec3 SpecularColor()
{
	return LightSpecular * texture2D(SpecularTexture, SpecularUV).rgb;
}

#else

vec3 SpecularColor()
{
	return MaterialSpecular.xyz;
}

#endif
