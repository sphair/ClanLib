
struct PixelIn
{
	float4 screenpos : SV_Position;
	float4 brush_data1 : brush_data1;
	float4 brush_data2 : brush_data2;
	float4 vary_data : vary_data;
	float2 mask_position : mask_position;
	uint2 instance_offset : instance_offset;
};

struct PixelOut
{
	float4 cl_FragColor : SV_Target0;
};

Texture2D instance_data;
Texture2D image_texture;
SamplerState image_sampler;
Texture2D mask_texture;
SamplerState mask_sampler;

float4 mask(float2 mask_position, float4 color)
{
	return color * mask_texture.Sample(mask_sampler, mask_position).r;
}

PixelOut solid_fill(PixelIn input)
{
	PixelOut output;

	float4 fill_color = input.brush_data2;
	output.cl_FragColor = mask(input.mask_position, fill_color);

	return output;
}

float4 gradient_color(uint2 instance_offset, uint stop_start, uint stop_end, float t)
{
	float4 color = instance_data.Load(uint3(instance_offset.x + stop_start, instance_offset.y, 0));
	float last_stop_pos = instance_data.Load(uint3(instance_offset.x + stop_start + 1, instance_offset.y, 0)).x;
	for (uint i = stop_start; i < stop_end; i += 2)
	{
		float4 stop_color = instance_data.Load(uint3(instance_offset.x + i, instance_offset.y, 0));
		float stop_pos = instance_data.Load(uint3(instance_offset.x + i + 1, instance_offset.y, 0)).x;
		float tt = clamp((t - last_stop_pos) / (stop_pos - last_stop_pos), 0.0, 1.0);
		color = lerp(color, stop_color, tt);
		last_stop_pos = stop_pos;
	}
	return color;
}

PixelOut linear_gradient_fill(PixelIn input)
{
	PixelOut output;

	float2 grad_start = input.vary_data.xy;
	float2 grad_dir = input.brush_data1.zw;
	float rcp_grad_length = input.brush_data2.x;
	uint stop_start = uint(input.brush_data2.y);
	uint stop_end = uint(input.brush_data2.z);

	float t = dot(grad_start, grad_dir) * rcp_grad_length;
	output.cl_FragColor = mask(input.mask_position, gradient_color(input.instance_offset, stop_start, stop_end, t));

	return output;
}

PixelOut radial_gradient_fill(PixelIn input)
{
	PixelOut output;

	float2 grad_center = input.vary_data.xy;
	float rcp_grad_length = input.brush_data2.x;
	uint stop_start = uint(input.brush_data2.y);
	uint stop_end = uint(input.brush_data2.z);

	float t = length(grad_center) * rcp_grad_length;
	output.cl_FragColor = mask(input.mask_position, gradient_color(input.instance_offset, stop_start, stop_end, t));

	return output;
}

PixelOut image_fill(PixelIn input)
{
	PixelOut output;

	float2 uv = input.vary_data.zw;
	output.cl_FragColor = mask(input.mask_position, image_texture.Sample(image_sampler, uv));

	return output;
}

PixelOut main(PixelIn input)
{
	switch (uint(input.brush_data1.x))
	{
	default:
	case 0: return solid_fill(input);
	case 1: return linear_gradient_fill(input);
	case 2: return radial_gradient_fill(input);
	case 3: return image_fill(input);
	}
}
