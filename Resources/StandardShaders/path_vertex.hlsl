
struct VertexIn
{
	uint4 Vertex : Vertex;
};

struct VertexOut
{
	float4 gl_Position : SV_Position;
	/*flat*/ float4 brush_data1 : brush_data1;
	/*flat*/ float4 brush_data2 : brush_data2;
	float4 vary_data : vary_data;
	float2 mask_position : mask_position;
	uint2 instance_offset : instance_offset;
};

Texture2D instance_data;

VertexOut main(VertexIn input)
{
	VertexOut output;

	const uint mask_block_size = 16;
	const uint mask_width = 1024;
	const uint instance_width = 512;

	float4 canvas_data = instance_data.Load(uint3(0, 0, 0));
	uint2 size = uint2((input.Vertex.z % 2), (input.Vertex.z / 2));
	output.gl_Position = float4(((input.Vertex.x + size.x * mask_block_size)*2.0 / canvas_data.x) - 1.0, ((input.Vertex.y + size.y * mask_block_size)*-2.0 / canvas_data.y) + 1.0, 0.0, 1.0);
	uint mask_offset = input.Vertex.w % 65536;

	uint y_offset = (mask_offset*mask_block_size) / mask_width;
	output.mask_position = float2(mask_offset * mask_block_size - y_offset * mask_width, y_offset * mask_block_size);
	output.mask_position += size * mask_block_size;
	output.mask_position /= mask_width;
	uint instance_block = input.Vertex.w / 65536;

	y_offset = instance_block / instance_width;
	output.instance_offset = uint2(instance_block - y_offset *instance_width, y_offset);

	output.brush_data1 = instance_data.Load(uint3(output.instance_offset, 0));
	output.brush_data2 = instance_data.Load(uint3(output.instance_offset.x + 1, output.instance_offset.y, 0));
	float4 brush_data3 = instance_data.Load(uint3(output.instance_offset.x + 2, output.instance_offset.y, 0));

	// Calculate for linear and radial gradient
	output.vary_data.x = input.Vertex.x + size.x * mask_block_size - brush_data3.x;
	output.vary_data.y = input.Vertex.y + size.y * mask_block_size - brush_data3.y;

	// Calculate for texture coords
	output.vary_data.z = (input.Vertex.x + size.x * mask_block_size);
	output.vary_data.w = (input.Vertex.y + size.y * mask_block_size);

	float4x4 inv_transform = float4x4(
		instance_data.Load(uint3(output.instance_offset.x + 2, output.instance_offset.y, 0)),
		instance_data.Load(uint3(output.instance_offset.x + 3, output.instance_offset.y, 0)),
		instance_data.Load(uint3(output.instance_offset.x + 4, output.instance_offset.y, 0)),
		instance_data.Load(uint3(output.instance_offset.x + 5, output.instance_offset.y, 0)));
	output.vary_data.zw = mul(float4(output.vary_data.zw, 0, 1), inv_transform).xy;

	output.vary_data.z = (output.vary_data.z + output.brush_data1.x) / output.brush_data2.x;
	output.vary_data.w = (output.vary_data.w + output.brush_data1.y) / output.brush_data2.y;

	return output;
}
