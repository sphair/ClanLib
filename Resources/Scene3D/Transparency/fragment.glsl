
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

out vec4 FragColor;

#if defined(DIFFUSE_ARRAY)
sampler2DArray DiffuseTexture;
#else
sampler2D DiffuseTexture;
#endif
sampler2D BumpMapTexture;
sampler2D SelfIlluminationTexture;
sampler2D SpecularTexture;

vec3 ApplyNormalMap(bool frontFacing);
vec3 DiffuseColor();
vec3 SpecularColor();
vec4 SelfIlluminationColor();

void main()
{
	vec3 normalInEyeNormalized = ApplyNormalMap(gl_FrontFacing);

	vec3 diffuse = DiffuseColor();
	vec3 specular = SpecularColor();

	vec4 si = SelfIlluminationColor();
	float opacity = si.a;

	float ambience = 0.01f;
	vec3 color = diffuse.rgb * ambience + si.rgb;
/*
	for (int i = 0; i < 4; i++)
	{
		color += apply_light(lights[i], position_in_eye, diffuse.rgb, specular, MaterialSpecularLevel, MaterialGlossiness);
	}
*/
	output.FragColor = vec4(color * opacity, opacity);
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
	return normalize(lerp(frontNormal, bumpedNormal, bumpAmount));
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
	return texture(DiffuseTexture, vec3(DiffuseUV, ArrayTextureIndex)).rgb;
#else
	return texture(DiffuseTexture, DiffuseUV).rgb;
#endif
}

#else

vec3 DiffuseColor()
{
	return MaterialDiffuse.rgb;
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

vec4 SelfIlluminationColor()
{
	vec4 texel = texture(SelfIlluminationTexture, SelfIlluminationUV);
	return vec4(lerp(SelfIllumination.rgb, texel.rgb, SelfIllumination.w), texel.a);
}

#else

vec4 SelfIlluminationColor()
{
	return vec4(SelfIllumination.rgb, 1);
}

#endif
