
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

struct VertexIn
{
	float4 AttrPositionInObject : AttrPositionInObject;
#if defined(USE_BONES)
	float4 AttrBoneWeights : AttrBoneWeights;
	uint4 AttrBoneSelectors : AttrBoneSelectors;
#endif
};

struct VertexOut
{
	float4 PositionInProjection : SV_Position;
	float4 PositionInEye : PositionInEye;
};

Texture2D InstanceOffsets;
Texture2D InstanceVectors;


float4 ApplyBones(VertexIn input, uint instanceBonesOffset);
float4x4 GetBoneTransform(uint boneIndex, uint instanceBonesOffset);
float2x4 GetBoneDualQuaternion(float2x4 currentDual, uint boneIndex, uint instanceBonesOffset);
int3 GetTexelPosition(uint index);
uint GetVectorsOffset(uint index);

float3x3 loadMat3(uint offset)
{
	return float3x3(
		InstanceVectors.Load(GetTexelPosition(offset + 0)).xyz,
		InstanceVectors.Load(GetTexelPosition(offset + 1)).xyz,
		InstanceVectors.Load(GetTexelPosition(offset + 2)).xyz);
}

float4x4 loadMat4(uint offset)
{
	return float4x4(
		InstanceVectors.Load(GetTexelPosition(offset + 0)),
		InstanceVectors.Load(GetTexelPosition(offset + 1)),
		InstanceVectors.Load(GetTexelPosition(offset + 2)),
		InstanceVectors.Load(GetTexelPosition(offset + 3)));
}

VertexOut main(VertexIn input, uint instanceId : SV_InstanceId)
{
	uint vectorsOffset = GetVectorsOffset(ModelIndex);

	float4x4 ObjectToWorld = loadMat4(vectorsOffset + instanceId * VectorsPerInstance + 3);
	float4x4 WorldToEye = loadMat4(vectorsOffset + instanceId * VectorsPerInstance + 7);
	float4x4 EyeToProjection = loadMat4(vectorsOffset + instanceId * VectorsPerInstance + 11);

	VertexOut output;
	float4 positionInObject = ApplyBones(input, vectorsOffset + instanceId * VectorsPerInstance + 16);
	output.PositionInEye = mul(WorldToEye, mul(ObjectToWorld, positionInObject));
	output.PositionInProjection = mul(EyeToProjection, output.PositionInEye);
	return output;
}

#if defined(USE_BONES) && defined(USE_DUALQUATERNION)
float4 ApplyBones(VertexIn input, uint instanceBonesOffset)
{
	if (input.AttrBoneWeights.x != 0.0 || input.AttrBoneWeights.y != 0.0 || input.AttrBoneWeights.z != 0.0 || input.AttrBoneWeights.w != 0.0)
	{
		// We use low precision input for our bone weights. Rescale so the sum still is 1.0
		float totalWeight = input.AttrBoneWeights.x + input.AttrBoneWeights.y + input.AttrBoneWeights.z + input.AttrBoneWeights.w;
		float weightMultiplier = 1 / totalWeight;
		input.AttrBoneWeights *= weightMultiplier;

		// For details, read the paper "Geometric Skinning with Dual Skinning" by L. Kavan et al.
		float2x4 dual = (float2x4)0;
		dual  = input.AttrBoneWeights.x * GetBoneDualQuaternion(dual, input.AttrBoneSelectors.x, instanceBonesOffset);
		dual += input.AttrBoneWeights.y * GetBoneDualQuaternion(dual, input.AttrBoneSelectors.y, instanceBonesOffset);
		dual += input.AttrBoneWeights.z * GetBoneDualQuaternion(dual, input.AttrBoneSelectors.z, instanceBonesOffset);
		dual += input.AttrBoneWeights.w * GetBoneDualQuaternion(dual, input.AttrBoneSelectors.w, instanceBonesOffset);
		dual /= length(dual[0]);

		float4 positionInObject;
		positionInObject.xyz = input.AttrPositionInObject.xyz + 2.0 * cross(dual[0].xyz, cross(dual[0].xyz, input.AttrPositionInObject.xyz) + dual[0].w * input.AttrPositionInObject.xyz);
		positionInObject.xyz += 2.0 * (dual[0].w * dual[1].xyz - dual[1].w * dual[0].xyz + cross(dual[0].xyz, dual[1].xyz));
		positionInObject.w = 1;
		return positionInObject;
	}
	else
	{
		return input.AttrPositionInObject;
	}
}
#elif defined(USE_BONES)
float4 ApplyBones(VertexIn input, uint instanceBonesOffset)
{
	if (input.AttrBoneWeights.x != 0.0 || input.AttrBoneWeights.y != 0.0 || input.AttrBoneWeights.z != 0.0 || input.AttrBoneWeights.w != 0.0)
	{
		// We use low precision input for our bone weights. Rescale so the sum still is 1.0
		float totalWeight = input.AttrBoneWeights.x + input.AttrBoneWeights.y + input.AttrBoneWeights.z + input.AttrBoneWeights.w;
		float weightMultiplier = 1 / totalWeight;
		input.AttrBoneWeights *= weightMultiplier;

		float4 positionInObject = float4(0,0,0,0);

		if (input.AttrBoneWeights.x != 0.0)
		{
			float4x4 transform = GetBoneTransform(input.AttrBoneSelectors.x, instanceBonesOffset);
			positionInObject += mul(transform, input.AttrPositionInObject) * input.AttrBoneWeights.x;
		}

		if (input.AttrBoneWeights.y != 0.0)
		{
			float4x4 transform = GetBoneTransform(input.AttrBoneSelectors.y, instanceBonesOffset);
			positionInObject += mul(transform, input.AttrPositionInObject) * input.AttrBoneWeights.y;
		}

		if (input.AttrBoneWeights.z != 0.0)
		{
			float4x4 transform = GetBoneTransform(input.AttrBoneSelectors.z, instanceBonesOffset);
			positionInObject += mul(transform, input.AttrPositionInObject) * input.AttrBoneWeights.z;
		}

		if (input.AttrBoneWeights.w != 0.0)
		{
			float4x4 transform = GetBoneTransform(input.AttrBoneSelectors.w, instanceBonesOffset);
			positionInObject += mul(transform, input.AttrPositionInObject) * input.AttrBoneWeights.w;
		}

		positionInObject.w = 1.0; // For numerical stability
		return positionInObject;
	}
	else
	{
		return input.AttrPositionInObject;
	}
}
#else
float4 ApplyBones(VertexIn input, uint instanceBonesOffset)
{
	return input.AttrPositionInObject;
}
#endif

#if !defined(USE_DUALQUATERNION)
float4x4 GetBoneTransform(uint boneIndex, uint instanceBonesOffset)
{
	float4 row0 = InstanceVectors.Load(GetTexelPosition(instanceBonesOffset + boneIndex * 3 + 0));
	float4 row1 = InstanceVectors.Load(GetTexelPosition(instanceBonesOffset + boneIndex * 3 + 1));
	float4 row2 = InstanceVectors.Load(GetTexelPosition(instanceBonesOffset + boneIndex * 3 + 2));
	float4 row3 = float4(0,0,0,1);
	return float4x4(row0, row1, row2, row3);
}
#else
float2x4 GetBoneDualQuaternion(float2x4 currentDual, uint boneIndex, uint instanceBonesOffset)
{
	float4 row0 = InstanceVectors.Load(GetTexelPosition(instanceBonesOffset + boneIndex * 2 + 0));
	float4 row1 = InstanceVectors.Load(GetTexelPosition(instanceBonesOffset + boneIndex * 2 + 1));
	float2x4 newDual = float2x4(row0, row1);
	if (dot(currentDual[0], newDual[0]) < 0)
		return -newDual;
	else
		return newDual;
}
#endif

int3 GetTexelPosition(uint index)
{
	int width, height, num_levels;
	InstanceVectors.GetDimensions(0, width, height, num_levels);
	return int3(index % width, index / width, 0);
}

uint GetVectorsOffset(uint index)
{
	int width, height, num_levels;
	InstanceOffsets.GetDimensions(0, width, height, num_levels);
	return (uint)InstanceOffsets.Load(int3(index % width, index / width, 0)).x;
}
