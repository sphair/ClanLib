
#define USE_FAKE_GI

#define MAX_LOCAL_LIGHTS 128
#define TILE_SIZE 16
//#define DEBUG_LIGHT_COUNT
//#define DEBUG_BLACK_OUTPUT
//#define DEBUG_DIFFUSE

// Direct3D's render targets are top-down, while OpenGL uses bottom-up
#define TOP_DOWN_RENDER_TARGET

struct Light
{
	float4 position; // vec3(pos), shadow_index (negative = no shadows)
	float4 color; // rgb, unused
#if defined(USE_QUADRATIC_ATTENUATION)
	float4 range; // pow(attenuation_end, 2), pow(attenation_start, 2), 1/pow(attenuation_end-attenuation_start, 2), falloff_begin
#else
	float4 range; // pow(attenuation_end, 2), attenation_start, 1/(attenuation_end-attenuation_start), falloff_begin
#endif
	float4 spot_x; // xyz, light type (0 = omni, 1 = circle spot, 2 = rect spot)
	float4 spot_y; // xyz, unused
	float4 spot_z; // xyz, unused
};

cbuffer Uniforms
{
	float rcp_f;
	float rcp_f_div_aspect;
	float2 two_rcp_viewport_size;
	uint num_lights;
	uint num_tiles_x;
	uint num_tiles_y;
	uint padding; // 16 byte boundary alignment
}
StructuredBuffer<Light> lights;


StructuredBuffer<uint> VisibleLightIndices;
Texture2DArray<float4> shadow_maps;
SamplerState shadow_maps_sampler;
Texture2D<float4> normal_z;
Texture2D<float4> diffuse;
Texture2D<float4> specular;
Texture2D<float4> specular_level;
Texture2D<float4> self_illumination;
RWTexture2D<float4> out_final;

groupshared Light local_lights[MAX_LOCAL_LIGHTS]; 



void render_lights(int x, int y, int local_x, int local_y, uint num_visible_lights);

float distance_attenuation(Light light, float3 fragment_to_light);
float circle_falloff_attenuation(Light light, float3 shadow_projection);
float rect_falloff_attenuation(Light light, float3 shadow_projection);
float shadow_attenuation(Light light, float3 fragment_to_light, float3 shadow_projection);

float3 project_on_shadow_map(Light light, float3 fragment_to_light);

float3 unproject_direction(float2 pos);
float3 unproject(float2 pos, float eye_z);

float vsm_reduce_light_bleeding(float p_max, float amount);
float vsm_attenuation(float2 moments, float t);

float lambertian_diffuse_contribution(float3 light_direction_in_eye, float3 normal_in_eye);
float phong_specular_contribution(float3 light_direction_in_eye, float3 position_in_eye, float3 normal_in_eye, float glossiness, float specular_level);
float blinn_specular_contribution(float3 light_direction_in_eye, float3 position_in_eye, float3 normal_in_eye, float glossiness, float specular_level);

[numthreads(TILE_SIZE,TILE_SIZE,1)]
void main(uint3 globalThreadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID, uint localThreadIndex : SV_GroupIndex)
{
	uint tile_x = globalThreadID.x / TILE_SIZE;
	uint tile_y = globalThreadID.y / TILE_SIZE;
	uint tile_pos = tile_x + tile_y * num_tiles_x;

	uint visible_light_indices_start = tile_pos * MAX_LOCAL_LIGHTS;

	// Copy all lights to local memory
	uint num_visible_lights = min(VisibleLightIndices[visible_light_indices_start], MAX_LOCAL_LIGHTS);
	if (localThreadIndex < num_visible_lights)
		local_lights[localThreadIndex] = lights[VisibleLightIndices[visible_light_indices_start + 1 + localThreadIndex]];
	GroupMemoryBarrierWithGroupSync();

	uint width, height;
	out_final.GetDimensions(width, height);
	if (globalThreadID.x >= width || globalThreadID.y >= height)
		return;

#if defined(DEBUG_BLACK_OUTPUT)
	float4 color = float4((globalThreadID.x % 2560) / 2560.0f, (globalThreadID.y % 1600) / 1600.0f, 1.0f, 1.0f);
	out_final[globalThreadID.xy] = color;
#else
	render_lights(globalThreadID.x, globalThreadID.y, groupThreadID.x, groupThreadID.y, num_visible_lights);
#endif
}

void render_lights(int x, int y, int local_x, int local_y, uint num_visible_lights)
{
	int2 pos = int2(x,y);
	uint3 texelpos = uint3(pos,0);

	float4 normal_and_z = normal_z.Load(texelpos);
	float2 glossiness_and_specular_level = specular_level.Load(texelpos).xy;

	float4 material_diffuse_color = diffuse.Load(texelpos);
	float3 material_specular_color = specular.Load(texelpos).xyz;
	float material_glossiness = glossiness_and_specular_level.x;
	float material_specular_level = glossiness_and_specular_level.y;
	float3 material_self_illumination = self_illumination.Load(texelpos).xyz;
	float3 normal_in_eye = normal_and_z.xyz;
	float z_in_eye = normal_and_z.w;

#if defined(DEBUG_DIFFUSE)
	float3 color = material_diffuse_color.xyz;
	out_final[pos] = float4(color.x, color.y, color.z, 1.0f);
#else
	float3 position_in_eye = unproject(float2(x, y) + 0.5f, z_in_eye);

#if defined(USE_FAKE_GI)
	float ambience = 0.001f;
#else
	float ambience = 0.01f;
#endif

	float3 color = material_diffuse_color.xyz * ambience + material_self_illumination * 4;
#if defined(DEBUG_LIGHT_COUNT)
	uint item_index = local_x + local_y * TILE_SIZE;
	if (item_index < VisibleLightIndices[visible_light_indices_start])
		color = float3(1.0f, 1.0f, 1.0f);
#else

	for (uint i = 0; i < num_visible_lights; i++)
	{
		Light light = local_lights[i];

		float3 fragment_to_light = light.position.xyz - position_in_eye;

		float3 light_direction_in_eye = normalize(fragment_to_light);
		float diffuse_contribution = lambertian_diffuse_contribution(light_direction_in_eye, normal_in_eye);
		float specular_contribution = blinn_specular_contribution(light_direction_in_eye, position_in_eye, normal_in_eye, material_glossiness, material_specular_level);
		float3 diffuse_color = material_diffuse_color.xyz * light.color.xyz;
		float3 specular_color = material_specular_color * light.color.xyz;
		float3 lit_color = (diffuse_color * diffuse_contribution + specular_color * specular_contribution);

		float light_type = light.spot_x.w;
#if defined(ONLY_OMNI_LIGHTS)
		if (light_type == 0)
		{
			float attenuation = distance_attenuation(light, fragment_to_light);
			color += attenuation * lit_color;
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
			color += attenuation * lit_color;
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
			color += attenuation * lit_color;
		}
#else
		float attenuation = distance_attenuation(light, fragment_to_light);
		float3 shadow_projection = project_on_shadow_map(light, fragment_to_light);
		float extra_attenuation = 1.0f;
		if (light_type != 0)
			extra_attenuation *= step(0, shadow_projection.z);
		if (light_type == 1)
			extra_attenuation *= circle_falloff_attenuation(light, shadow_projection);
		else if (light_type == 2)
			extra_attenuation *= rect_falloff_attenuation(light, shadow_projection);

		if (light.position.w >= 0.0f)
			extra_attenuation *= shadow_attenuation(light, fragment_to_light, shadow_projection);

#if defined(USE_FAKE_GI)
		attenuation *= max(extra_attenuation, 0.05f);
#else
		attenuation *= extra_attenuation;
#endif
		
		color += attenuation * lit_color;
#endif
	}
#endif
#if defined(DEBUG_BLOOM)
	color = saturate(color - 1.0);
#endif
	out_final[pos] = float4(color.x, color.y, color.z, 1.0f);
#endif
}

float distance_attenuation(Light light, float3 fragment_to_light)
{
#if defined(USE_QUADRATIC_ATTENUATION)
	float sqr_distance = dot(fragment_to_light, fragment_to_light);
	float sqr_attenuation_start = light.range.y;
	float rcp_sqr_delta_attenuation = light.range.z;
	return saturate(1.0f - (sqr_distance - sqr_attenuation_start) * rcp_sqr_delta_attenuation);
#else
	// To do: fix shader input to be linear */
	float distance = length(fragment_to_light);
	float attenuation_start = light.range.y;
	float rcp_attenuation_delta = light.range.z;
	return saturate(1.0f - (distance - attenuation_start) * rcp_attenuation_delta);
#endif
}

float circle_falloff_attenuation(Light light, float3 shadow_projection)
{
	float falloff_begin = light.range.w; // hotspot
	const float falloff_end = 1.0f;
	return 1.0f - smoothstep(falloff_begin, falloff_end, dot(shadow_projection.xy, shadow_projection.xy));
}

float rect_falloff_attenuation(Light light, float3 shadow_projection)
{
	float falloff_begin = light.range.w; // hotspot
	const float falloff_end = 1.414213562373f;
	return 1.0f - smoothstep(falloff_begin, falloff_end, max(shadow_projection.x * shadow_projection.x, shadow_projection.y * shadow_projection.y) * 2.0f);
}

float shadow_attenuation(Light light, float3 fragment_to_light, float3 shadow_projection)
{
#if defined(TOP_DOWN_RENDER_TARGET)
	shadow_projection.y = -shadow_projection.y;
#endif
	shadow_projection = shadow_projection * 0.5f + 0.5f;

	float shadow_index = light.position.w;
	float2 moments = shadow_maps.SampleLevel(shadow_maps_sampler, float3(shadow_projection.xy, shadow_index), 0).xy;
	return vsm_attenuation(moments, length(fragment_to_light));
}

float3 project_on_shadow_map(Light light, float3 fragment_to_light)
{
	// the following calculations assume the vector points from the light towards the fragment position:
	float3 light_to_fragment = -fragment_to_light;

	// project fragment position on shadow map:
	float3 position_in_shadow_projection = float3(
		dot(light.spot_x.xyz, light_to_fragment),
		dot(light.spot_y.xyz, light_to_fragment),
		dot(light.spot_z.xyz, light_to_fragment));
	return float3(position_in_shadow_projection.xy / position_in_shadow_projection.z, position_in_shadow_projection.z);
}

float vsm_reduce_light_bleeding(float p_max, float amount)
{
	// Remove the [0, Amount] tail and linearly rescale (Amount, 1].
	return smoothstep(amount, 1.0, p_max);
}

float vsm_attenuation(float2 moments, float t)
{
	float LightVsmEpsilon = 0.1f;

	// Standard shadow map comparison
	float p = (t <= moments.x) ? 1.0 : 0.0;

	// Variance shadow mapping
	float variance = moments.y - (moments.x * moments.x);
	variance = max(variance, LightVsmEpsilon); // apply min variance
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
