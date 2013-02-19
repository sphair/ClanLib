
#version 430

#define MAX_LIGHTS 1023
#define MAX_LOCAL_LIGHTS 128
#define TILE_SIZE 16
//#define DEBUG_LIGHT_COUNT
//#define DEBUG_BLACK_OUTPUT

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

struct Tile
{
	vec3 aabb_min;
	vec3 aabb_max;
};

layout(std140) uniform Uniforms
{
	float rcp_f;
	float rcp_f_div_aspect;
	float2 two_rcp_viewport_size;
	uint num_lights;
	uint num_tiles_x;
	uint num_tiles_y;
	uint padding; // 16 byte boundary alignment
}

readonly buffer lights
{
	Light in_lights[];
};

buffer VisibleLightIndices
{
	uint out_visible_light_indices[];
};

uniform sampler2D zminmax;

vec3 unproject_direction(float2 pos)
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
	return vec3(normalized.x * rcp_f_div_aspect, normalized.y * rcp_f, 1.0f);
}

vec3 unproject(float2 pos, float eye_z)
{
	return unproject_direction(pos) * eye_z;
}

bool is_plane_visible(vec4 plane, vec3 aabb_center, vec3 aabb_extents)
{
	float e = dot(aabb_extents, abs(plane.xyz));
	float s = dot(aabb_center, plane.xyz) + plane.w;
	return s - e >= 0;
}

bool is_light_visible(Tile tile, uint light_index)
{
	// Do simple sphere test for the light
	vec3 light_position = in_lights[light_index].position.xyz;
	vec3 e = max(tile.aabb_min - light_position, 0.0f) + max(light_position - tile.aabb_max, 0.0f);
	float sqr_radius = in_lights[light_index].range.x;
#if !defined(SPOT_CONE_CULL)
	return dot(e,e) <= sqr_radius;
#else
	if (dot(e,e) <= sqr_radius) // sphere is visible
	{
		float light_type = in_lights[light_index].spot_x.w;
		if (light_type == 0.0f) // omni light
		{
			return true;
		}
		else // spot light
		{
			// unit vectors in projected light space
			vec3 spot_x = in_lights[light_index].spot_x.xyz;
			vec3 spot_y = in_lights[light_index].spot_y.xyz;
			vec3 spot_z = in_lights[light_index].spot_z.xyz;

			// calculate falloff angles in x and y directions (they may differ for square lights)
			float falloff_x = sqrt(dot(spot_x, spot_x) / dot(spot_z, spot_z));
			float falloff_y = sqrt(dot(spot_y, spot_y) / dot(spot_z, spot_z));

			// normalize unit vectors for the next calculation
			spot_x = normalize(spot_x);
			spot_y = normalize(spot_y);
			spot_z = normalize(spot_z);

			// calculate cone plane normal vectors:
			vec3 normal_left = lerp(spot_z, spot_x, falloff_x);
			vec3 normal_right = lerp(spot_z, -spot_x, falloff_x);
			vec3 normal_up = lerp(spot_z, spot_y, falloff_y);
			vec3 normal_down = lerp(spot_z, -spot_y, falloff_y);

			vec4 plane_left = vec4(normal_left, -dot(normal_left, light_position));
			vec4 plane_right = vec4(normal_right, -dot(normal_right, light_position));
			vec4 plane_up = vec4(normal_up, -dot(normal_up, light_position));
			vec4 plane_down = vec4(normal_down, -dot(normal_down, light_position));

			vec3 aabb_center = (tile.aabb_max + tile.aabb_min) * 0.5;
			vec3 aabb_extents = (tile.aabb_max - tile.aabb_min) * 0.5;
			return 
				is_plane_visible(plane_left, aabb_center, aabb_extents) ||
				is_plane_visible(plane_right, aabb_center, aabb_extents) ||
				is_plane_visible(plane_up, aabb_center, aabb_extents) ||
				is_plane_visible(plane_down, aabb_center, aabb_extents);
		}
	}
	else
	{
		return false;
	}
#endif
}

Tile find_tile_frustum(float eye_z_near, float eye_z_far, uint tile_x, uint tile_y)
{
	uint tile_width = TILE_SIZE;
	uint tile_height = TILE_SIZE;
	uint x = tile_x * tile_width;
	uint y = tile_y * tile_height;
	vec3 tl_direction = unproject_direction(float2(x, y));
	vec3 br_direction = unproject_direction(float2(x + tile_width, y + tile_height));
	vec3 front_tl = tl_direction * eye_z_near;
	vec3 front_br = br_direction * eye_z_near;
	vec3 back_tl = tl_direction * eye_z_far;
	vec3 back_br = br_direction * eye_z_far;
	Tile tile;
	tile.aabb_min = min(min(min(front_tl, front_br), back_tl), back_br);
	tile.aabb_max = max(max(max(front_tl, front_br), back_tl), back_br);
	return tile;
}

void cull_lights(Tile tile, uint visible_light_indices_start)
{
	uint num_work_items = TILE_SIZE * TILE_SIZE;
	out_visible_light_indices[visible_light_indices_start] = 0;
	for (uint light_index = 0; light_index < num_lights; light_index++)
	{
//		if (in_lights[light_index].spot.w == 0.0f) // debug code; disables omni lights
//			continue;

		if (is_light_visible(tile, light_index))
		{
			uint offset = ++out_visible_light_indices[visible_light_indices_start];
			out_visible_light_indices[visible_light_indices_start + offset] = light_index;
		}

//		if (in_lights[light_index].spot.w != 0.0f) // debug code; only displays first spot light
//			break;
	}
}

layout (local_size_x = TILE_SIZE, local_size_y = TILE_SIZE) in;

void main()
{
	if (gl_GlobalInvocationID.x >= num_tiles_x || gl_GlobalInvocationID.y >= num_tiles_y)
		return;

	uint tile_x = gl_GlobalInvocationID.x;
	uint tile_y = gl_GlobalInvocationID.y;
	uint tile_pos = tile_x + tile_y * num_tiles_x;

	vec2 minmax = texelFetch(zminmax, ivec2(tile_x, tile_y), 0).xy;
	float eye_z_near = minmax.x;
	float eye_z_far = minmax.y;

	uint visible_light_indices_start = tile_pos * MAX_LOCAL_LIGHTS;

	Tile tile = find_tile_frustum(eye_z_near, eye_z_far, tile_x, tile_y);
	cull_lights(tile, visible_light_indices_start);
}
