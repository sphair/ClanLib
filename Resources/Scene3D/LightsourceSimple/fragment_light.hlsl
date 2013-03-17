
// Direct3D's render targets are top-down, while OpenGL uses bottom-up
#define TOP_DOWN_RENDER_TARGET

struct PixelIn
{
	float4 ScreenPos : SV_Position;
	nointerpolation float4 PositionInEye : PixelPositionInEye;
	nointerpolation float4 Color : PixelColor;
	nointerpolation float4 Range : PixelRange; // pow(attenuation_end, 2), pow(attenation_start, 2), 1/pow(attenuation_end-attenuation_start, 2), hotspot
	nointerpolation float4 SpotX : PixelSpotX;
	nointerpolation float4 SpotY : PixelSpotY;
	nointerpolation float4 SpotZ : PixelSpotZ;
};

struct PixelOut
{
	float4 PixelColor : SV_Target0;
};

cbuffer Uniforms
{
	float4x4 ObjectToEye;
	float4x4 EyeToProjection;
	float rcp_f;
	float rcp_f_div_aspect;
	float2 two_rcp_viewport_size;
}

Texture2DArray<float4> ShadowMapsTexture;
SamplerState ShadowMapsTextureSampler;
Texture2D<float4> NormalZTexture;
Texture2D<float4> DiffuseColorTexture;
Texture2D<float4> SpecularTexture;
Texture2D<float4> SpecularLevelTexture;
Texture2D<float4> SelfIlluminationTexture;

float distance_attenuation(PixelIn light, float3 fragment_to_light);
float circle_falloff_attenuation(PixelIn light, float3 shadow_projection);
float rect_falloff_attenuation(PixelIn light, float3 shadow_projection);
float shadow_attenuation(PixelIn light, float3 fragment_to_light, float3 shadow_projection);

float3 project_on_shadow_map(PixelIn light, float3 fragment_to_light);

float3 unproject_direction(float2 pos);
float3 unproject(float2 pos, float eye_z);

float vsm_reduce_light_bleeding(float p_max, float amount);
float vsm_attenuation(float2 moments, float t);

float lambertian_diffuse_contribution(float3 light_direction_in_eye, float3 normal_in_eye);
float phong_specular_contribution(float3 light_direction_in_eye, float3 position_in_eye, float3 normal_in_eye, float glossiness, float specular_level);
float blinn_specular_contribution(float3 light_direction_in_eye, float3 position_in_eye, float3 normal_in_eye, float glossiness, float specular_level);

PixelOut main(PixelIn input)
{
	PixelOut output;

	int x = int(input.ScreenPos.x);
	int y = int(input.ScreenPos.y);
	int2 pos = int2(x,y);
	uint3 texelpos = uint3(pos,0);

	float4 normal_and_z = NormalZTexture.Load(texelpos);
	float2 glossiness_and_specular_level = SpecularLevelTexture.Load(texelpos).xy;

	float4 material_diffuse_color = DiffuseColorTexture.Load(texelpos);
	float3 material_specular_color = SpecularTexture.Load(texelpos).xyz;
	float material_glossiness = glossiness_and_specular_level.x;
	float material_specular_level = glossiness_and_specular_level.y;
	float3 material_self_illumination = SelfIlluminationTexture.Load(texelpos).xyz;
	float3 normal_in_eye = normal_and_z.xyz;
	float z_in_eye = normal_and_z.w;

	float3 position_in_eye = unproject(float2(x, y) + 0.5f, z_in_eye);

	// To do: we need to apply material SI in its own pass
	float3 color = float3(0,0,0); // material_self_illumination * 4;

	float3 fragment_to_light = input.PositionInEye.xyz - position_in_eye;

	float3 light_direction_in_eye = normalize(fragment_to_light);
	float diffuse_contribution = lambertian_diffuse_contribution(light_direction_in_eye, normal_in_eye);
	float specular_contribution = blinn_specular_contribution(light_direction_in_eye, position_in_eye, normal_in_eye, material_glossiness, material_specular_level);
	float3 diffuse_color = material_diffuse_color.xyz * input.Color.xyz;
	float3 specular_color = material_specular_color * input.Color.xyz;
	float3 lit_color = diffuse_color * diffuse_contribution + specular_color * specular_contribution;

	float light_type = input.SpotX.w;
#if defined(ONLY_OMNI_LIGHTS)
	if (light_type == 0)
	{
		float attenuation = distance_attenuation(input, fragment_to_light);
		color += attenuation * lit_color;
	}
#elif defined(ONLY_SPOT_LIGHTS)
	if (light_type != 0 && input.PositionInEye.w < 0.0f)
	{
		float attenuation = distance_attenuation(input, fragment_to_light);
		float3 shadow_projection = project_on_shadow_map(input, fragment_to_light);
		attenuation *= step(0, shadow_projection.z);
		if (light_type == 1)
			attenuation *= circle_falloff_attenuation(input, shadow_projection);
		else if (light_type == 2)
			attenuation *= rect_falloff_attenuation(input, shadow_projection);
		color += attenuation * lit_color;
	}
#elif defined(ONLY_SHADOW_SPOT_LIGHTS)
	if (light_type != 0 && input.PositionInEye.w >= 0.0f)
	{
		float attenuation = distance_attenuation(input, fragment_to_light);
		float3 shadow_projection = project_on_shadow_map(input, fragment_to_light);
		attenuation *= step(0, shadow_projection.z);
		if (light_type == 1)
			attenuation *= circle_falloff_attenuation(input, shadow_projection);
		else if (light_type == 2)
			attenuation *= rect_falloff_attenuation(input, shadow_projection);
		attenuation *= shadow_attenuation(input, fragment_to_light, shadow_projection);
		color += attenuation * lit_color;
	}
#else
	float attenuation = distance_attenuation(input, fragment_to_light);
	float3 shadow_projection = project_on_shadow_map(input, fragment_to_light);
	if (light_type != 0)
		attenuation *= step(0, shadow_projection.z);
	if (light_type == 1)
		attenuation *= circle_falloff_attenuation(input, shadow_projection);
	else if (light_type == 2)
		attenuation *= rect_falloff_attenuation(input, shadow_projection);

	float shadow_att = 1.0f;
	if (input.PositionInEye.w >= 0.0f)
		shadow_att = shadow_attenuation(input, fragment_to_light, shadow_projection);

	color += attenuation * (shadow_att * lit_color + input.Color.a * diffuse_color);
#endif

	output.PixelColor = float4(color, 0);
	return output;
}

float distance_attenuation(PixelIn light, float3 fragment_to_light)
{
#if defined(USE_QUADRATIC_ATTENUATION)
	float sqr_distance = dot(fragment_to_light, fragment_to_light);
	float sqr_attenuation_start = light.Range.y;
	float rcp_sqr_delta_attenuation = light.Range.z;
	return saturate(1.0f - (sqr_distance - sqr_attenuation_start) * rcp_sqr_delta_attenuation);
#else
	float distance = length(fragment_to_light);
	float attenuation_start = light.Range.y;
	float rcp_attenuation_delta = light.Range.z;
	return saturate(1.0f - (distance - attenuation_start) * rcp_attenuation_delta);
#endif
}

float circle_falloff_attenuation(PixelIn light, float3 shadow_projection)
{
	float falloff_begin = light.Range.w; // hotspot
	const float falloff_end = 1.0f;
	return 1.0f - smoothstep(falloff_begin, falloff_end, dot(shadow_projection.xy, shadow_projection.xy));
}

float rect_falloff_attenuation(PixelIn light, float3 shadow_projection)
{
	float falloff_begin = light.Range.w; // hotspot
	const float falloff_end = 1.414213562373f;
	return 1.0f - smoothstep(falloff_begin, falloff_end, max(shadow_projection.x * shadow_projection.x, shadow_projection.y * shadow_projection.y) * 2.0f);
}

float shadow_attenuation(PixelIn light, float3 fragment_to_light, float3 shadow_projection)
{
#if defined(TOP_DOWN_RENDER_TARGET)
	shadow_projection.y = -shadow_projection.y;
#endif
	shadow_projection = shadow_projection * 0.5f + 0.5f;

	float shadow_index = light.PositionInEye.w;
	float2 moments = ShadowMapsTexture.SampleLevel(ShadowMapsTextureSampler, float3(shadow_projection.xy, shadow_index), 0).xy;
	return vsm_attenuation(moments, length(fragment_to_light));
}

float3 project_on_shadow_map(PixelIn light, float3 fragment_to_light)
{
	// the following calculations assume the vector points from the light towards the fragment position:
	float3 light_to_fragment = -fragment_to_light;

	// project fragment position on shadow map:
	float3 position_in_shadow_projection = float3(
		dot(light.SpotX.xyz, light_to_fragment),
		dot(light.SpotY.xyz, light_to_fragment),
		dot(light.SpotZ.xyz, light_to_fragment));
	return float3(position_in_shadow_projection.xy / position_in_shadow_projection.z, position_in_shadow_projection.z);
}

float vsm_reduce_light_bleeding(float p_max, float amount)
{
	// Remove the [0, Amount] tail and linearly rescale (Amount, 1].
	return smoothstep(amount, 1.0, p_max);
}

float vsm_attenuation(float2 moments, float t)
{
	float PixelInVsmEpsilon = 0.1f;

	// Standard shadow map comparison
	float p = (t <= moments.x) ? 1.0 : 0.0;

	// Variance shadow mapping
	float variance = moments.y - (moments.x * moments.x);
	variance = max(variance, PixelInVsmEpsilon); // apply min variance
	float d = t - moments.x; // probabilitic upper bound
	float p_max = variance / (variance + d * d);
	p_max = vsm_reduce_light_bleeding(p_max, 0.4);
	return max(p, p_max);
}

float3 unproject_direction(float2 pos)
{
//	float field_of_view_y_rad = field_of_view_y_degrees * M_PI / 180.0f;
//	float f = 1.0f / tan(field_of_view_y_rad * 0.5f);
//	float rcp_f = 1.0f / f;
//	float rcp_f_div_aspect = 1.0f / (f / aspect);
	float2 normalized = float2(pos * two_rcp_viewport_size);
#if defined(TOP_DOWN_RENDER_TARGET)
	normalized.x = normalized.x - 1.0f;
	normalized.y = 1.0f - normalized.y;
#else
	normalized -= 1.0f;
#endif
	return float3(normalized.x * rcp_f_div_aspect, normalized.y * rcp_f, 1.0f);
}

float3 unproject(float2 pos, float eye_z)
{
	return unproject_direction(pos) * eye_z;
}

float lambertian_diffuse_contribution(float3 light_direction_in_eye, float3 normal_in_eye)
{
	return max(dot(normal_in_eye, light_direction_in_eye), 0.0f);
}

float phong_specular_contribution(float3 light_direction_in_eye, float3 position_in_eye, float3 normal_in_eye, float glossiness, float specular_level)
{
	float3 E = normalize(-position_in_eye);
	float3 R = normalize(-reflect(light_direction_in_eye, normal_in_eye));
	float x = max(dot(R, E), 0.0f);
	float ph_exp = pow(2.0f, glossiness * 10.0f);
	return specular_level * pow(x, ph_exp);
}

float blinn_specular_contribution(float3 light_direction_in_eye, float3 position_in_eye, float3 normal_in_eye, float glossiness, float specular_level)
{
	float3 half_vector = normalize(light_direction_in_eye - position_in_eye);
	float x = max(dot(normal_in_eye, half_vector), 0.0f);
	float ph_exp = pow(2.0f, glossiness * 10.0f) * 4.0f;
	return specular_level * pow(x, ph_exp);
}
