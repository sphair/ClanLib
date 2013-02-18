
#define DiffuseUV UVMap0
#define BumpMapUV UVMap1
#define SelfIlluminationUV UVMap2
#define SpecularUV UVMap3

cbuffer ModelMaterialUniforms
{
	float4 MaterialAmbient;
	float4 MaterialDiffuse;
	float4 MaterialSpecular;
	float MaterialGlossiness;
	float MaterialSpecularLevel;
	uint ModelIndex;
	uint VectorsPerInstance;
	uint MaterialOffset;
};

struct PixelIn
{
	float4 ScreenPos : SV_Position;
	float3 NormalInEye : NormalInEye;
	float3 BitangentInEye : BitangentInEye;
	float3 TangentInEye : TangentInEye;
	float4 PositionInWorld : PositionInWorld;
	float4 PositionInEye : PositionInEye;
#if defined(DIFFUSE_UV)
	float2 UVMap0 : UVMap0;
#endif
#if defined(BUMPMAP_UV)
	float2 UVMap1 : UVMap1;
#endif
#if defined(SI_UV)
	float2 UVMap2 : UVMap2;
#endif
#if defined(SPECULAR_UV)
	float2 UVMap3 : UVMap3;
#endif
	float ArrayTextureIndex : ArrayTextureIndex;
	float4 SelfIllumination : SelfIllumination;
};

struct PixelOut
{
	float4 FragDiffuseColor : SV_Target0;
	float4 FragSpecularColor : SV_Target1;
	float2 FragSpecularLevel : SV_Target2;
	float4 FragSelfIllumination : SV_Target3;
	float4 FragNormal : SV_Target4;
};

#if defined(DIFFUSE_ARRAY)
Texture2DArray DiffuseTexture;
#else
Texture2D DiffuseTexture;
#endif
Texture2D BumpMapTexture;
Texture2D SelfIlluminationTexture;
Texture2D SpecularTexture;

SamplerState DiffuseSampler;
SamplerState BumpMapSampler;
SamplerState SelfIlluminationSampler;
SamplerState SpecularSampler;

float3 ApplyNormalMap(PixelIn input, bool frontFacing);
float3 DiffuseColor(PixelIn input);
float3 SpecularColor(PixelIn input);
float3 SelfIlluminationColor(PixelIn input);

PixelOut main(PixelIn input, bool frontFacing : SV_IsFrontFace)
{
	PixelOut output;
	float3 normalInEyeNormalized = ApplyNormalMap(input, frontFacing);
	output.FragDiffuseColor = float4(DiffuseColor(input), 1);
	output.FragSpecularColor = float4(SpecularColor(input), 1);
	output.FragSpecularLevel = float2(MaterialGlossiness, MaterialSpecularLevel);
	output.FragSelfIllumination = float4(SelfIlluminationColor(input), 1);
	output.FragNormal = float4(normalInEyeNormalized, input.PositionInEye.z);
	return output;
}

#if defined(BUMPMAP_UV)

float3 ApplyNormalMap(PixelIn input, bool frontFacing)
{
	// Get normal vector in tangent space from normal map:
	float3 normalInTangent = BumpMapTexture.Sample(BumpMapSampler, input.BumpMapUV).rgb * 2 - 1;

	// Re-normalize normal vector in tangent space (gives smoother result and allows us to also only use a RG normal texture)
	normalInTangent.z = sqrt(1 - dot(normalInTangent.xy, normalInTangent.xy));

	// Find normal, tangent and bitang vectors for this fragment (taking face direction into account):
	float3 frontNormal = normalize(frontFacing ? input.NormalInEye : -input.NormalInEye);
	float3 frontTangent = normalize(frontFacing ? input.TangentInEye : -input.TangentInEye);
	float3 frontBitangent = normalize(frontFacing ? input.BitangentInEye : -input.BitangentInEye);

	// Calculate normal adjusted by the normal map:
	float3 bumpedNormal = normalInTangent.x * frontTangent - normalInTangent.y * frontBitangent + normalInTangent.z * frontNormal;

	// Mix between the original normal vector and the bumped normal vector according to the material properties
	float bumpAmount = 0.30; // Hardcoded to the 3ds max default for now
	return normalize(lerp(frontNormal, bumpedNormal, bumpAmount));
}

#else

float3 ApplyNormalMap(PixelIn input, bool frontFacing)
{
	float3 frontNormal = frontFacing ? input.NormalInEye : -input.NormalInEye;
	return normalize(frontNormal);
}

#endif

#if defined(DIFFUSE_UV)

float3 DiffuseColor(PixelIn input)
{
#if defined(DIFFUSE_ARRAY)
	float4 color = DiffuseTexture.Sample(DiffuseSampler, float3(input.DiffuseUV, input.ArrayTextureIndex));
#else
	float4 color = DiffuseTexture.Sample(DiffuseSampler, input.DiffuseUV);
#endif
	if (color.a < 0.5)
		discard;
	return color.rgb;
}

#else

float3 DiffuseColor(PixelIn input)
{
	return MaterialDiffuse.xyz;
}

#endif

#if defined(SPECULAR_UV)

float3 SpecularColor(PixelIn input)
{
	return SpecularTexture.Sample(SpecularSampler, input.SpecularUV).rgb;
}

#else

float3 SpecularColor(PixelIn input)
{
	return MaterialSpecular.xyz;
}

#endif

#if defined(SI_UV)

float3 SelfIlluminationColor(PixelIn input)
{
	return lerp(input.SelfIllumination.rgb, SelfIlluminationTexture.Sample(SelfIlluminationSampler, input.SelfIlluminationUV).rgb, input.SelfIllumination.w);
}

#else

float3 SelfIlluminationColor(PixelIn input)
{
	return input.SelfIllumination.rgb;
}

#endif
