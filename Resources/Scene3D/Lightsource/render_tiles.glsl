
#version 430

#define USE_FAKE_GI

#define MAX_LOCAL_LIGHTS 128
#define TILE_SIZE 16
//#define DEBUG_LIGHT_COUNT
//#define DEBUG_BLACK_OUTPUT
//#define DEBUG_DIFFUSE

// Direct3D's render targets are top-down, while OpenGL uses bottom-up
//#define TOP_DOWN_RENDER_TARGET

struct Light
{
	vec4 position; // vec3(pos), shadow_index (negative = no shadows)
	vec4 color; // rgb, unused
#if defined(USE_QUADRATIC_ATTENUATION)
	vec4 range; // pow(attenuation_end, 2), pow(attenation_start, 2), 1/pow(attenuation_end-attenuation_start, 2), falloff_begin
#else
	vec4 range; // pow(attenuation_end, 2), attenation_start, 1/(attenuation_end-attenuation_start), falloff_begin
#endif
	vec4 spot_x; // xyz, light type (0 = omni, 1 = circle spot, 2 = rect spot)
	vec4 spot_y; // xyz, unused
	vec4 spot_z; // xyz, unused
};

layout(std140) uniform Uniforms
{
	float rcp_f;
	float rcp_f_div_aspect;
	vec2 two_rcp_viewport_size;
	uint num_lights;
	uint num_tiles_x;
	uint num_tiles_y;
	uint padding; // 16 byte boundary alignment
}

readonly buffer lights
{
	Light in_lights[];
};

readonly buffer VisibleLightIndices
{
	uint in_visible_light_indices[];
};

uniform sampler2DArray shadow_maps;
uniform sampler2D normal_z;
uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D specular_level;
uniform sampler2D self_illumination;

layout(rgba32f) writeonly uniform image2D out_final;

shared Light local_lights[MAX_LOCAL_LIGHTS]; 



void render_lights(int x, int y, int local_x, int local_y, uint num_visible_lights);

float distance_attenuation(Light light, vec3 fragment_to_light);
float circle_falloff_attenuation(Light light, vec3 shadow_projection);
float rect_falloff_attenuation(Light light, vec3 shadow_projection);
float shadow_attenuation(Light light, vec3 fragment_to_light, vec3 shadow_projection);

vec3 project_on_shadow_map(Light light, vec3 fragment_to_light);

vec3 unproject_direction(vec2 pos);
vec3 unproject(vec2 pos, float eye_z);

float vsm_reduce_light_bleeding(float p_max, float amount);
float vsm_attenuation(vec2 moments, float t);

float lambertian_diffuse_contribution(vec3 light_direction_in_eye, vec3 normal_in_eye);
float phong_specular_contribution(vec3 light_direction_in_eye, vec3 position_in_eye, vec3 normal_in_eye, float glossiness, float specular_level);
float blinn_specular_contribution(vec3 light_direction_in_eye, vec3 position_in_eye, vec3 normal_in_eye, float glossiness, float specular_level);

layout (local_size_x = TILE_SIZE, local_size_y = TILE_SIZE) in;

void main()
{
	uint tile_x = gl_GlobalInvocationID.x / TILE_SIZE;
	uint tile_y = gl_GlobalInvocationID.y / TILE_SIZE;
	uint tile_pos = tile_x + tile_y * num_tiles_x;

	uint visible_light_indices_start = tile_pos * MAX_LOCAL_LIGHTS;

	// Copy all lights to local memory
	uint num_visible_lights = min(in_visible_light_indices[visible_light_indices_start], MAX_LOCAL_LIGHTS);
	if (gl_LocalInvocationIndex < num_visible_lights)
		local_lights[gl_LocalInvocationIndex] = in_lights[in_visible_light_indices[visible_light_indices_start + 1 + gl_LocalInvocationIndex]];

	groupMemoryBarrier();

	if (gl_GlobalInvocationID.x >= textureSize(out_final).x || gl_GlobalInvocationID.y >= textureSize(out_final).y)
		return;

#if defined(DEBUG_BLACK_OUTPUT)
	vec4 color = vec4((gl_GlobalInvocationID.x % 2560) / 2560.0f, (gl_GlobalInvocationID.y % 1600) / 1600.0f, 1.0f, 1.0f);
	imageStore(out_final, gl_GlobalInvocationID.xy, color);
#else
	render_lights(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y, gl_LocalInvocationID.x, gl_LocalInvocationID.y, num_visible_lights);
#endif
}

void render_lights(int x, int y, int local_x, int local_y, uint num_visible_lights)
{
	ivec2 pos = ivec2(x,y);

	vec4 normal_and_z = texelFetch(normal_z, pos, 0);
	vec2 glossiness_and_specular_level = texelFetch(specular_level, pos, 0).xy;

	vec4 material_diffuse_color = texelFetch(diffuse, pos, 0);
	vec3 material_specular_color = texelFetch(specular, pos, 0).xyz;
	float material_glossiness = glossiness_and_specular_level.x;
	float material_specular_level = glossiness_and_specular_level.y;
	vec3 material_self_illumination = texelFetch(self_illumination, pos, 0).xyz;
	vec3 normal_in_eye = normal_and_z.xyz;
	float z_in_eye = normal_and_z.w;

#if defined(DEBUG_DIFFUSE)
	vec3 color = material_diffuse_color.xyz;
	imageStore(out_final, pos, vec4(color.x, color.y, color.z, 1.0f));
#else
	vec3 position_in_eye = unproject(vec2(x, y) + 0.5f, z_in_eye);

#if defined(USE_FAKE_GI)
	vec3 color = material_self_illumination * 4;
#else
	float ambience = 0.01f;
	vec3 color = material_diffuse_color.xyz * ambience + material_self_illumination * 4;
#endif

#if defined(DEBUG_LIGHT_COUNT)
	uint item_index = local_x + local_y * TILE_SIZE;
	if (item_index < in_visible_light_indices[visible_light_indices_start])
		color = vec3(1.0f, 1.0f, 1.0f);
#else

	for (uint i = 0; i < num_visible_lights; i++)
	{
		Light light = local_lights[i];

		vec3 fragment_to_light = light.position.xyz - position_in_eye;

		vec3 light_direction_in_eye = normalize(fragment_to_light);
		float diffuse_contribution = lambertian_diffuse_contribution(light_direction_in_eye, normal_in_eye);
		float specular_contribution = blinn_specular_contribution(light_direction_in_eye, position_in_eye, normal_in_eye, material_glossiness, material_specular_level);
		vec3 diffuse_color = material_diffuse_color.xyz * light.color.xyz;
		vec3 specular_color = material_specular_color * light.color.xyz;
		vec3 lit_color = (diffuse_color * diffuse_contribution + specular_color * specular_contribution);

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
			vec3 shadow_projection = project_on_shadow_map(light, fragment_to_light);
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
			vec3 shadow_projection = project_on_shadow_map(light, fragment_to_light);
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
		vec3 shadow_projection = project_on_shadow_map(light, fragment_to_light);
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
		color += attenuation * extra_attenuation * lit_color + attenuation * 0.025f * diffuse_color;
#else
		color += attenuation * extra_attenuation * lit_color;
#endif

#endif
	}
#endif
#if defined(DEBUG_BLOOM)
	color = clamp(color - 1.0, 0, 1);
#endif
	imageStore(out_final, pos, vec4(color.x, color.y, color.z, 1.0f));
#endif
}

float distance_attenuation(Light light, vec3 fragment_to_light)
{
#if defined(USE_QUADRATIC_ATTENUATION)
	float sqr_distance = dot(fragment_to_light, fragment_to_light);
	float sqr_attenuation_start = light.range.y;
	float rcp_sqr_delta_attenuation = light.range.z;
	return clamp(1.0f - (sqr_distance - sqr_attenuation_start) * rcp_sqr_delta_attenuation, 0, 1);
#else
	float distance = length(fragment_to_light);
	float attenuation_start = light.range.y;
	float rcp_attenuation_delta = light.range.z;
	return clamp(1.0f - (distance - attenuation_start) * rcp_attenuation_delta, 0, 1);
#endif
}

float circle_falloff_attenuation(Light light, vec3 shadow_projection)
{
	float falloff_begin = light.range.w; // hotspot
	const float falloff_end = 1.0f;
	return 1.0f - smoothstep(falloff_begin, falloff_end, dot(shadow_projection.xy, shadow_projection.xy));
}

float rect_falloff_attenuation(Light light, vec3 shadow_projection)
{
	float falloff_begin = light.range.w; // hotspot
	const float falloff_end = 1.414213562373f;
	return 1.0f - smoothstep(falloff_begin, falloff_end, max(shadow_projection.x * shadow_projection.x, shadow_projection.y * shadow_projection.y) * 2.0f);
}

float shadow_attenuation(Light light, vec3 fragment_to_light, vec3 shadow_projection)
{
#if defined(TOP_DOWN_RENDER_TARGET)
	shadow_projection.y = -shadow_projection.y;
#endif
	shadow_projection = shadow_projection * 0.5f + 0.5f;

	float shadow_index = light.position.w;
	vec2 moments = shadow_maps.SampleLevel(shadow_maps_sampler, vec3(shadow_projection.xy, shadow_index), 0).xy;
	return vsm_attenuation(moments, length(fragment_to_light));
}

vec3 project_on_shadow_map(Light light, vec3 fragment_to_light)
{
	// the following calculations assume the vector points from the light towards the fragment position:
	vec3 light_to_fragment = -fragment_to_light;

	// project fragment position on shadow map:
	vec3 position_in_shadow_projection = vec3(
		dot(light.spot_x.xyz, light_to_fragment),
		dot(light.spot_y.xyz, light_to_fragment),
		dot(light.spot_z.xyz, light_to_fragment));
	return vec3(position_in_shadow_projection.xy / position_in_shadow_projection.z, position_in_shadow_projection.z);
}

float vsm_reduce_light_bleeding(float p_max, float amount)
{
	// Remove the [0, Amount] tail and linearly rescale (Amount, 1].
	return smoothstep(amount, 1.0, p_max);
}

float vsm_attenuation(vec2 moments, float t)
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

vec3 unproject_direction(vec2 pos)
{
//	float field_of_view_y_rad = field_of_view_y_degrees * M_PI / 180.0f;
//	float f = 1.0f / tan(field_of_view_y_rad * 0.5f);
//	float rcp_f = 1.0f / f;
//	float rcp_f_div_aspect = 1.0f / (f / aspect);
	vec2 normalized = vec2(pos * two_rcp_viewport_size);
#if defined(TOP_DOWN_RENDER_TARGET)
	normalized.x = normalized.x - 1.0f;
	normalized.y = 1.0f - normalized.y;
#else
	normalized -= 1.0f;
#endif
	return vec3(normalized.x * rcp_f_div_aspect, normalized.y * rcp_f, 1.0f);
}

vec3 unproject(vec2 pos, float eye_z)
{
	return unproject_direction(pos) * eye_z;
}

float lambertian_diffuse_contribution(vec3 light_direction_in_eye, vec3 normal_in_eye)
{
	return max(dot(normal_in_eye, light_direction_in_eye), 0.0f);
}

float phong_specular_contribution(vec3 light_direction_in_eye, vec3 position_in_eye, vec3 normal_in_eye, float glossiness, float specular_level)
{
	vec3 E = normalize(-position_in_eye);
	vec3 R = normalize(-reflect(light_direction_in_eye, normal_in_eye));
	float x = max(dot(R, E), 0.0f);
	float ph_exp = pow(2.0f, glossiness * 10.0f);
	return specular_level * pow(x, ph_exp);
}

float blinn_specular_contribution(vec3 light_direction_in_eye, vec3 position_in_eye, vec3 normal_in_eye, float glossiness, float specular_level)
{
	vec3 half_vector = normalize(light_direction_in_eye - position_in_eye);
	float x = max(dot(normal_in_eye, half_vector), 0.0f);
	float ph_exp = pow(2.0f, glossiness * 10.0f) * 4.0f;
	return specular_level * pow(x, ph_exp);
}
