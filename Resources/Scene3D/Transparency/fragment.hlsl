
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
	float4 FragColor : SV_Target0;
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
float4 SelfIlluminationColor(PixelIn input);

PixelOut main(PixelIn input, bool frontFacing : SV_IsFrontFace)
{
	PixelOut output;
	float3 normalInEyeNormalized = ApplyNormalMap(input, frontFacing);

	float3 diffuse = DiffuseColor(input);
	float3 specular = SpecularColor(input);

	float4 si = SelfIlluminationColor(input);
	float opacity = si.a;

	float ambience = 0.01f;
	float3 color = diffuse.rgb * ambience + si.rgb;
/*
	for (int i = 0; i < 4; i++)
	{
		color += apply_light(lights[i], position_in_eye, diffuse.rgb, specular, MaterialSpecularLevel, MaterialGlossiness);
	}
*/
	output.FragColor = float4(color * opacity, opacity);
	return output;
}
/*
float3 apply_light(Light light, float3 position_in_eye, float3 material_diffuse_color, float3 material_specular_color, float material_specular_level, float material_glossiness)
{
	float3 fragment_to_light = light.position.xyz - position_in_eye;

	float3 light_direction_in_eye = normalize(fragment_to_light);
	float diffuse_contribution = lambertian_diffuse_contribution(light_direction_in_eye, normal_in_eye);
	float specular_contribution = blinn_specular_contribution(light_direction_in_eye, position_in_eye, normal_in_eye, material_glossiness, material_specular_level);
	float3 diffuse_color = material_diffuse_color * light.color.xyz;
	float3 specular_color = material_specular_color * light.color.xyz;
	float3 lit_color = (diffuse_color * diffuse_contribution + specular_color * specular_contribution);

	float light_type = light.spot_x.w;
#if defined(ONLY_OMNI_LIGHTS)
	if (light_type == 0)
	{
		float attenuation = distance_attenuation(light, fragment_to_light);
		return attenuation * lit_color;
	}
#elif defined(ONLY_SPOT_LIGHTS)
	if (light_type != 0 && light.position.w < 0.0f)
	{
		float attenuation = distance_attenuation(light, fragment_to_light);
		float3 shadow_projection = project_on_shadow_map(light, fragment_to_light);
		attenuation *= step(0, shadow_projection.z);
		if (light_type == 1)
			attenuation *= circle_falloff_attenuation(light, shadow_projection);
		else if (light_type == 2)
			attenuation *= rect_falloff_attenuation(light, shadow_projection);
		return attenuation * lit_color;
	}
#elif defined(ONLY_SHADOW_SPOT_LIGHTS)
	if (light_type != 0 && light.position.w >= 0.0f)
	{
		float attenuation = distance_attenuation(light, fragment_to_light);
		float3 shadow_projection = project_on_shadow_map(light, fragment_to_light);
		attenuation *= step(0, shadow_projection.z);
		if (light_type == 1)
			attenuation *= circle_falloff_attenuation(light, shadow_projection);
		else if (light_type == 2)
			attenuation *= rect_falloff_attenuation(light, shadow_projection);
		attenuation *= shadow_attenuation(light, fragment_to_light, shadow_projection);
		return attenuation * lit_color;
	}
#else
	float attenuation = distance_attenuation(light, fragment_to_light);
	float3 shadow_projection = project_on_shadow_map(light, fragment_to_light);
	if (light_type != 0)
		attenuation *= step(0, shadow_projection.z);
	if (light_type == 1)
		attenuation *= circle_falloff_attenuation(light, shadow_projection);
	else if (light_type == 2)
		attenuation *= rect_falloff_attenuation(light, shadow_projection);
	if (light.position.w >= 0.0f)
		attenuation *= shadow_attenuation(light, fragment_to_light, shadow_projection);
		
	return attenuation * lit_color;
#endif
}

float lambertian_diffuse_contribution(float3 light_direction_in_eye, float3 normal_in_eye)
{
	return max(dot(normal_in_eye, light_direction_in_eye), 0.0f);
}

float blinn_specular_contribution(float3 light_direction_in_eye, float3 position_in_eye, float3 normal_in_eye, float glossiness, float specular_level)
{
	float3 half_vector = normalize(light_direction_in_eye - position_in_eye);
	float x = max(dot(normal_in_eye, half_vector), 0.0f);
	float ph_exp = pow(2.0f, glossiness * 10.0f) * 4.0f;
	return specular_level * pow(x, ph_exp);
}
*/
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
	return DiffuseTexture.Sample(DiffuseSampler, float3(input.DiffuseUV, input.ArrayTextureIndex)).rgb;
#else
	return DiffuseTexture.Sample(DiffuseSampler, input.DiffuseUV).rgb;
#endif
}

#else

float3 DiffuseColor(PixelIn input)
{
	return MaterialDiffuse.rgb;
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

float4 SelfIlluminationColor(PixelIn input)
{
	float4 texel = SelfIlluminationTexture.Sample(SelfIlluminationSampler, input.SelfIlluminationUV);
	return float4(lerp(input.SelfIllumination.rgb, texel.rgb, input.SelfIllumination.w), texel.a);
}

#else

float4 SelfIlluminationColor(PixelIn input)
{
	return float4(input.SelfIllumination.rgb, 1);
}

#endif
