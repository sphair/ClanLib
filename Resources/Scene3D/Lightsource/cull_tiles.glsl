
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

struct Tile
{
	vec3 aabb_min;
	vec3 aabb_max;
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

buffer VisibleLightIndices
{
	uint out_visible_light_indices[];
};

uniform sampler2D zminmax;

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

bool is_light_visible(Tile tile, uint light_index)
{
	vec3 e = max(tile.aabb_min - lights[light_index].position.xyz, 0.0f) + max(lights[light_index].position.xyz - tile.aabb_max, 0.0f);
	float sqr_radius = lights[light_index].range.x;
	return dot(e,e) <= sqr_radius;
}

Tile find_tile_frustum(float eye_z_near, float eye_z_far, uint tile_x, uint tile_y)
{
	uint tile_width = TILE_SIZE;
	uint tile_height = TILE_SIZE;
	uint x = tile_x * tile_width;
	uint y = tile_y * tile_height;
	vec3 tl_direction = unproject_direction(vec2(x, y));
	vec3 br_direction = unproject_direction(vec2(x + tile_width, y + tile_height));
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
//		if (lights[light_index].spot.w == 0.0f) // debug code; disables omni lights
//			continue;

		if (is_light_visible(tile, light_index))
		{
			uint offset = ++out_visible_light_indices[visible_light_indices_start];
			out_visible_light_indices[visible_light_indices_start + offset] = light_index;
		}

//		if (lights[light_index].spot.w != 0.0f) // debug code; only displays first spot light
//			break;
	}
}

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

	uint visible_light_indices_start = tile_pos * (MAX_LIGHTS+1);

	Tile tile = find_tile_frustum(eye_z_near, eye_z_far, tile_x, tile_y);
	cull_lights(tile, visible_light_indices_start);
}
