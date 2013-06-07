
#define DiffuseUV UVMap0
#define BumpMapUV UVMap1
#define SelfIlluminationUV UVMap2
#define SpecularUV UVMap3

layout(std140) uniform ModelMaterialUniforms
{
	vec4 MaterialAmbient;
	vec4 MaterialDiffuse;
	vec4 MaterialSpecular;
	float MaterialGlossiness;
	float MaterialSpecularLevel;
	uint ModelIndex;
	uint VectorsPerInstance;
	uint MaterialOffset;
};

in vec3 NormalInEye;
in vec3 BitangentInEye;
in vec3 TangentInEye;
in vec4 PositionInWorld;
in vec4 PositionInEye;
#if defined(DIFFUSE_UV)
in vec2 UVMap0;
#endif
#if defined(BUMPMAP_UV)
in vec2 UVMap1;
#endif
#if defined(SI_UV)
in vec2 UVMap2;
#endif
#if defined(SPECULAR_UV)
in vec2 UVMap3;
#endif
in float ArrayTextureIndex;
in vec4 SelfIllumination;
#if defined(USE_COLORS)
in vec4 VertexColor;
#endif
in vec4 LightProbeColor;

out vec4 FragDiffuseColor;
out vec4 FragSpecularColor;
out vec2 FragSpecularLevel;
out vec4 FragSelfIllumination;
out vec4 FragNormal;

#if defined(DIFFUSE_ARRAY)
uniform sampler2DArray DiffuseTexture;
#else
uniform sampler2D DiffuseTexture;
#endif
uniform sampler2D BumpMapTexture;
uniform sampler2D SelfIlluminationTexture;
uniform sampler2D SpecularTexture;

vec3 ApplyNormalMap(bool frontFacing);
vec3 DiffuseColor();
vec3 SpecularColor();
vec3 SelfIlluminationColor(vec4 diffuseColor);

void main()
{
	vec3 normalInEyeNormalized = ApplyNormalMap(gl_FrontFacing);
	FragDiffuseColor = vec4(DiffuseColor(), 1);
	FragSpecularColor = vec4(SpecularColor(), 1);
	FragSpecularLevel = vec2(MaterialGlossiness, MaterialSpecularLevel);
	FragSelfIllumination = vec4(FragDiffuseColor.rgb * LightProbeColor.rgb + SelfIlluminationColor(output.FragDiffuseColor), 1);
	FragNormal = vec4(normalInEyeNormalized, PositionInEye.z);
}

#if defined(BUMPMAP_UV)

vec3 ApplyNormalMap(bool frontFacing)
{
	// Get normal vector in tangent space from normal map:
	vec3 normalInTangent = texture(BumpMapTexture, BumpMapUV).rgb * 2 - 1;

	// Re-normalize normal vector in tangent space (gives smoother result and allows us to also only use a RG normal texture)
	normalInTangent.z = sqrt(1 - dot(normalInTangent.xy, normalInTangent.xy));

	// Find normal, tangent and bitang vectors for this fragment (taking face direction into account):
	vec3 frontNormal = normalize(frontFacing ? NormalInEye : -NormalInEye);
	vec3 frontTangent = normalize(frontFacing ? TangentInEye : -TangentInEye);
	vec3 frontBitangent = normalize(frontFacing ? BitangentInEye : -BitangentInEye);

	// Calculate normal adjusted by the normal map:
	vec3 bumpedNormal = normalInTangent.x * frontTangent - normalInTangent.y * frontBitangent + normalInTangent.z * frontNormal;

	// Mix between the original normal vector and the bumped normal vector according to the material properties
	float bumpAmount = 0.30; // Hardcoded to the 3ds max default for now
	return normalize(mix(frontNormal, bumpedNormal, bumpAmount));
}

#else

vec3 ApplyNormalMap(bool frontFacing)
{
	vec3 frontNormal = frontFacing ? NormalInEye : -NormalInEye;
	return normalize(frontNormal);
}

#endif

#if defined(DIFFUSE_UV)

vec3 DiffuseColor()
{
#if defined(DIFFUSE_ARRAY)
	vec4 color = texture(DiffuseTexture, DiffuseUV, ArrayTextureIndex);
#else
	vec4 color = texture(DiffuseTexture, DiffuseUV);
#endif
	if (color.a < 0.5)
		discard;
	return color.rgb;
}

#else

vec3 DiffuseColor()
{
	return MaterialDiffuse.xyz;
}

#endif

#if defined(SPECULAR_UV)

vec3 SpecularColor()
{
	return texture(SpecularTexture, SpecularUV).rgb;
}

#else

vec3 SpecularColor()
{
	return MaterialSpecular.xyz;
}

#endif

#if defined(SI_UV)

vec3 SelfIlluminationColor(vec4 diffuseColor)
{
#if defined(USE_COLORS)
	vec3 siColor = SelfIllumination.rgb + VertexColor.rgb * diffuseColor.rgb;
	return mix(siColor, texture(SelfIlluminationTexture, SelfIlluminationUV).rgb, SelfIllumination.w);
#else
	return mix(SelfIllumination.rgb, texture(SelfIlluminationTexture, SelfIlluminationUV).rgb, SelfIllumination.w);
#endif
}

#else

vec3 SelfIlluminationColor(vec4 diffuseColor)
{
#if defined(USE_COLORS)
	return SelfIllumination.rgb + VertexColor.rgb * diffuseColor.rgb;
#else
	return SelfIllumination.rgb;
#endif
}

#endif
