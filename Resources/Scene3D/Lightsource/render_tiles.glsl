
#version 430

#define MAX_LIGHTS 1023
#define TILE_SIZE 16
#define SHADOW_MAP_SIZE 1024
//#define DEBUG_LIGHT_COUNT
//#define DEBUG_BLACK_OUTPUT

// Direct3D's render targets are top-down, while OpenGL uses bottom-up
//#define TOP_DOWN_RENDER_TARGET

struct Light
{
	vec4 position; // vec3(pos), shadow_index
	vec4 color;
	vec4 range; // pow(attenuation_end, 2), pow(attenation_start, 2), 1/pow(attenuation_end-attenuation_start, 2), hotspot
	vec4 spot; // vec3(dir), falloff
	mat4 eye_to_shadow_projection;
};

layout (local_size_x = TILE_SIZE, local_size_y = TILE_SIZE) in;

layout(std140) uniform Uniforms
{
	float rcp_f;
	float rcp_f_div_aspect;
	vec2 two_rcp_viewport_size;
	uint num_lights;
	uint num_tiles_x;
	uint num_tiles_y;
	uint padding; // 16 byte boundary alignment
};

readonly buffer Lights
{
	Light lights[];
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
layout(rgba32f) writeonly uniform image2D out_final;

vec3 unproject_direction(vec2 pos)
{
//	float field_of_view_y_rad = field_of_view_y_degrees * M_PI / 180.0f;
//	float f = 1.0f / tan(field_of_view_y_rad * 0.5f);
//	float rcp_f = 1.0f / f;
//	float rcp_f_div_aspect = 1.0f / (f / aspect);
	vec2 normalized = pos * two_rcp_viewport_size;
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

float spot_attenuation(uint light_index, vec3 light_direction_in_eye)
{
	float falloff = lights[light_index].spot.w;
	float hotspot_range = lights[light_index].range.w;
	return clamp((dot(lights[light_index].spot.xyz, light_direction_in_eye) - falloff) * hotspot_range, 0, 1);
}

float ReduceLightBleeding(float p_max, float amount)
{
	// Remove the [0, Amount] tail and linearly rescale (Amount, 1].
	return smoothstep(amount, 1.0, p_max);
}

float ChebyshevUpperBound(vec2 moments, float t)
{
	float LightVsmEpsilon = 0.1f;

	// Standard shadow map comparison
	float p = (t <= moments.x) ? 1.0 : 0.0;

	// Variance shadow mapping
	float variance = moments.y - (moments.x * moments.x);
	variance = max(variance, LightVsmEpsilon); // apply min variance
	float d = t - moments.x; // probabilitic upper bound
	float p_max = variance / (variance + d * d);
	p_max = ReduceLightBleeding(p_max, 0.1);
	return max(p, p_max);
}

float shadow_attenuation(uint light_index, vec3 position_in_eye, float distance_to_light)
{
	mat4 eye_to_shadow_projection = lights[light_index].eye_to_shadow_projection;
	vec4 position_in_shadow_projection = eye_to_shadow_projection * vec4(position_in_eye, 1.0f);
#if defined(TOP_DOWN_RENDER_TARGET)
	position_in_shadow_projection.y = -position_in_shadow_projection.y;
#endif
	vec2 projected = position_in_shadow_projection.xy / position_in_shadow_projection.w * 0.5f + 0.5f;
	float shadow_index = lights[light_index].position.w;
	vec2 moments = texelFetch(shadow_maps, ivec3(clamp(projected * SHADOW_MAP_SIZE + 0.5f, 0.0f, SHADOW_MAP_SIZE - 1.0f), shadow_index), 0).xy;
	return ChebyshevUpperBound(moments, distance_to_light);
}

void render_lights(uint x, uint y, uint local_x, uint local_y, uint visible_light_indices_start)
{
	ivec2 pos = ivec2(x,y);

	vec4 normal_and_z = texelFetch(normal_z, pos, 0);
	vec2 glossiness_and_specular_level = texelFetch(specular_level, pos, 0).xy;

	vec3 material_diffuse_color = texelFetch(diffuse, pos, 0).xyz;
	vec3 material_specular_color = texelFetch(specular, pos, 0).xyz;
	float material_glossiness = glossiness_and_specular_level.x;
	float material_specular_level = glossiness_and_specular_level.y;
	vec3 normal_in_eye = normal_and_z.xyz;
	float z_in_eye = normal_and_z.w;

	vec3 position_in_eye = unproject(vec2(x, y) + 0.5f, z_in_eye);

	vec3 color = material_diffuse_color * 0.02f;
#if defined(DEBUG_LIGHT_COUNT)
	uint item_index = local_x + local_y * TILE_SIZE;
	if (item_index < in_visible_light_indices[visible_light_indices_start])
		color = vec3(1.0f, 1.0f, 1.0f);
#else
	uint num_visible_lights = in_visible_light_indices[visible_light_indices_start];
	for (uint i = 0; i < num_visible_lights; i++)
	{
		uint light_index = in_visible_light_indices[visible_light_indices_start + 1 + i];
		vec3 light_direction_in_eye = lights[light_index].position.xyz - position_in_eye;
		float sqr_distance = dot(light_direction_in_eye, light_direction_in_eye);
		float sqr_attenuation_start = lights[light_index].range.y;
		float rcp_sqr_delta_attenuation = lights[light_index].range.z;
		float attenuation = clamp(1.0f - (sqr_distance - sqr_attenuation_start) * rcp_sqr_delta_attenuation, 0, 1);
		light_direction_in_eye = normalize(light_direction_in_eye);

		if (lights[light_index].spot.w != 0.0f)
		{
			attenuation *= spot_attenuation(light_index, light_direction_in_eye);
			attenuation *= shadow_attenuation(light_index, position_in_eye, sqrt(sqr_distance));
		}

		float diffuse_contribution = lambertian_diffuse_contribution(light_direction_in_eye, normal_in_eye);
		float specular_contribution = blinn_specular_contribution(light_direction_in_eye, position_in_eye, normal_in_eye, material_glossiness, material_specular_level);
		vec3 diffuse_color = material_diffuse_color * lights[light_index].color.xyz;
		vec3 specular_color = material_specular_color * lights[light_index].color.xyz;
		color += attenuation * (diffuse_color * diffuse_contribution + specular_color * specular_contribution);
	}
#endif
	imageStore(out_final, pos, vec4(color.x, color.y, color.z, 1.0f));
}

void main()
{
	if (gl_GlobalInvocationID.x >= textureSize(out_final).x || gl_GlobalInvocationID.y >= textureSize(out_final).y)
		return;

#if defined(DEBUG_BLACK_OUTPUT)
	vec4 color = vec4((gl_GlobalInvocationID.x % 2560) / 2560.0f, (gl_GlobalInvocationID.y % 1600) / 1600.0f, 1.0f, 1.0f);
	imageStore(out_final, gl_GlobalInvocationID.xy, color);
#else
	uint tile_x = gl_GlobalInvocationID.x / TILE_SIZE;
	uint tile_y = gl_GlobalInvocationID.y / TILE_SIZE;
	uint tile_pos = tile_x + tile_y * num_tiles_x;

	uint visible_light_indices_start = tile_pos * (MAX_LIGHTS+1);

	render_lights(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y, gl_LocalInvocationID.x, gl_LocalInvocationID.y, visible_light_indices_start);
#endif
}
