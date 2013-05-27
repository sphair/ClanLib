
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
	float3 AttrNormal : AttrNormal;
	float3 AttrBitangent : AttrBitangent;
	float3 AttrTangent : AttrTangent;
#if defined(USE_BONES)
	float4 AttrBoneWeights : AttrBoneWeights;
	uint4 AttrBoneSelectors : AttrBoneSelectors;
#endif
#if defined(USE_COLORS)
	float4 AttrColor : AttrColor;
#endif
#if defined(DIFFUSE_UV)
	float2 AttrUVMap0 : AttrUVMapA;
#endif
#if defined(BUMPMAP_UV)
	float2 AttrUVMap1 : AttrUVMapB;
#endif
#if defined(SI_UV)
	float2 AttrUVMap2 : AttrUVMapC;
#endif
#if defined(SPECULAR_UV)
	float2 AttrUVMap3 : AttrUVMapD;
#endif
};

struct VertexOut
{
	float4 PositionInProjection : SV_Position;
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
#if defined(USE_COLORS)
	float4 VertexColor : VertexColor;
#endif
	float4 LightProbeColor : LightProbeColor;
};

Texture2D InstanceOffsets;
Texture2D InstanceVectors;

struct BonesResult
{
	float3 Normal;
	float3 Tangent;
	float3 Bitangent;
	float4 PositionInObject;
};

BonesResult ApplyBones(VertexIn input, uint instanceBonesOffset);
float4x4 GetBoneTransform(uint boneIndex, uint instanceBonesOffset);
float2x4 GetBoneDualQuaternion(float2x4 currentDual, uint boneIndex, uint instanceBonesOffset);
int3 GetTexelPosition(uint index);
uint GetVectorsOffset(uint instanceId);

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

float3x4 loadMat3x4(uint offset)
{
	return 
		float3x4(
			InstanceVectors.Load(GetTexelPosition(offset + 0)),
			InstanceVectors.Load(GetTexelPosition(offset + 1)),
			InstanceVectors.Load(GetTexelPosition(offset + 2)));
}

VertexOut main(VertexIn input, uint instanceId : SV_InstanceId)
{
	uint vectorsOffset = GetVectorsOffset(instanceId);

	float3x3 ObjectNormalToEye = loadMat3(vectorsOffset);
	float4x4 ObjectToWorld = loadMat4(vectorsOffset + 3);
	float4x4 WorldToEye = loadMat4(vectorsOffset + 7);
	float4x4 EyeToProjection = loadMat4(vectorsOffset + 11);
	float4 SelfIlluminationData = InstanceVectors.Load(GetTexelPosition(vectorsOffset + MaterialOffset));
	float4 MaterialInstanceData = InstanceVectors.Load(GetTexelPosition(vectorsOffset + MaterialOffset + 1));
	float4 LightProbeColor = InstanceVectors.Load(GetTexelPosition(vectorsOffset + 15));

	VertexOut output;
	BonesResult bonesResult = ApplyBones(input, vectorsOffset + 16);
	float3x3 TangentObjectToEye = (float3x3)mul(WorldToEye, ObjectToWorld);
	output.NormalInEye = normalize(mul(ObjectNormalToEye, bonesResult.Normal));
	output.TangentInEye = normalize(mul(TangentObjectToEye, bonesResult.Tangent));
	output.BitangentInEye = normalize(mul(TangentObjectToEye, bonesResult.Bitangent));
	//output.BitangentInEye = normalize(cross(output.NormalInEye, output.TangentInEye));
#if defined(DIFFUSE_UV)
	float3x4 UVTextureMatrix0 = loadMat3x4(vectorsOffset + MaterialOffset + 2);
	output.UVMap0 = mul(UVTextureMatrix0, float4(input.AttrUVMap0, 0, 1)).xy;
#endif
#if defined(BUMPMAP_UV)
	float3x4 UVTextureMatrix1 = loadMat3x4(vectorsOffset + MaterialOffset + 5);
	output.UVMap1 = mul(UVTextureMatrix1, float4(input.AttrUVMap1, 0, 1)).xy;
#endif
#if defined(SI_UV)
	float3x4 UVTextureMatrix2 = loadMat3x4(vectorsOffset + MaterialOffset + 8);
	output.UVMap2 = mul(UVTextureMatrix2, float4(input.AttrUVMap2, 0, 1)).xy;
#endif
#if defined(SPECULAR_UV)
	float3x4 UVTextureMatrix3 = loadMat3x4(vectorsOffset + MaterialOffset + 11);
	output.UVMap3 = mul(UVTextureMatrix3, float4(input.AttrUVMap3, 0, 1)).xy;
#endif
	output.PositionInWorld = mul(ObjectToWorld, bonesResult.PositionInObject);
	output.PositionInEye = mul(WorldToEye, output.PositionInWorld);
	output.PositionInProjection = mul(EyeToProjection, output.PositionInEye);
	output.ArrayTextureIndex = MaterialInstanceData.x;
	output.SelfIllumination = SelfIlluminationData;
#if defined(USE_COLORS)
	output.VertexColor = input.AttrColor;
#endif
	output.LightProbeColor = LightProbeColor;
	return output;
}

#if defined(USE_BONES) && defined(USE_DUALQUATERNION)
BonesResult ApplyBones(VertexIn input, uint instanceBonesOffset)
{
	BonesResult result;
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

		result.PositionInObject.xyz = input.AttrPositionInObject.xyz + 2.0 * cross(dual[0].xyz, cross(dual[0].xyz, input.AttrPositionInObject.xyz) + dual[0].w * input.AttrPositionInObject.xyz);
		result.PositionInObject.xyz += 2.0 * (dual[0].w * dual[1].xyz - dual[1].w * dual[0].xyz + cross(dual[0].xyz, dual[1].xyz));
		result.PositionInObject.w = 1;

		result.Normal = input.AttrNormal + 2.0 * cross(dual[0].xyz, cross(dual[0].xyz, input.AttrNormal) + dual[0].w * input.AttrNormal);
		result.Tangent = input.AttrTangent + 2.0 * cross(dual[0].xyz, cross(dual[0].xyz, input.AttrTangent) + dual[0].w * input.AttrTangent);
		result.Bitangent = input.AttrBitangent + 2.0 * cross(dual[0].xyz, cross(dual[0].xyz, input.AttrBitangent) + dual[0].w * input.AttrBitangent);
	}
	else
	{
		result.PositionInObject = input.AttrPositionInObject;
		result.Normal = input.AttrNormal;
		result.Tangent = input.AttrTangent;
		result.Bitangent = input.AttrBitangent;
	}
	return result;
}
#elif defined(USE_BONES)
BonesResult ApplyBones(VertexIn input, uint instanceBonesOffset)
{
	BonesResult result;
	if (input.AttrBoneWeights.x != 0.0 || input.AttrBoneWeights.y != 0.0 || input.AttrBoneWeights.z != 0.0 || input.AttrBoneWeights.w != 0.0)
	{
		result.PositionInObject = float4(0,0,0,0);
		result.Normal = float3(0,0,0);
		result.Tangent = float3(0,0,0);
		result.Bitangent = float3(0,0,0);

		// We use low precision input for our bone weights. Rescale so the sum still is 1.0
		float totalWeight = input.AttrBoneWeights.x + input.AttrBoneWeights.y + input.AttrBoneWeights.z + input.AttrBoneWeights.w;
		float weightMultiplier = 1 / totalWeight;
		input.AttrBoneWeights *= weightMultiplier;

		if (input.AttrBoneWeights.x != 0.0)
		{
			float4x4 transform = GetBoneTransform(input.AttrBoneSelectors.x, instanceBonesOffset);
			float3x3 rotation = (float3x3)transform;
			result.PositionInObject += mul(transform, input.AttrPositionInObject) * input.AttrBoneWeights.x;
			result.Normal += mul(rotation, input.AttrNormal) * input.AttrBoneWeights.x;
			result.Tangent += mul(rotation, input.AttrTangent) * input.AttrBoneWeights.x;
			result.Bitangent += mul(rotation, input.AttrBitangent) * input.AttrBoneWeights.x;
		}

		if (input.AttrBoneWeights.y != 0.0)
		{
			float4x4 transform = GetBoneTransform(input.AttrBoneSelectors.y, instanceBonesOffset);
			float3x3 rotation = (float3x3)transform;
			result.PositionInObject += mul(transform, input.AttrPositionInObject) * input.AttrBoneWeights.y;
			result.Normal += mul(rotation, input.AttrNormal) * input.AttrBoneWeights.y;
			result.Tangent += mul(rotation, input.AttrTangent) * input.AttrBoneWeights.y;
			result.Bitangent += mul(rotation, input.AttrBitangent) * input.AttrBoneWeights.y;
		}

		if (input.AttrBoneWeights.z != 0.0)
		{
			float4x4 transform = GetBoneTransform(input.AttrBoneSelectors.z, instanceBonesOffset);
			float3x3 rotation = (float3x3)transform;
			result.PositionInObject += mul(transform, input.AttrPositionInObject) * input.AttrBoneWeights.z;
			result.Normal += mul(rotation, input.AttrNormal) * input.AttrBoneWeights.z;
			result.Tangent += mul(rotation, input.AttrTangent) * input.AttrBoneWeights.z;
			result.Bitangent += mul(rotation, input.AttrBitangent) * input.AttrBoneWeights.z;
		}

		if (input.AttrBoneWeights.w != 0.0)
		{
			float4x4 transform = GetBoneTransform(input.AttrBoneSelectors.w, instanceBonesOffset);
			float3x3 rotation = (float3x3)transform;
			result.PositionInObject += mul(transform, input.AttrPositionInObject) * input.AttrBoneWeights.w;
			result.Normal += mul(rotation, input.AttrNormal) * input.AttrBoneWeights.w;
			result.Tangent += mul(rotation, input.AttrTangent) * input.AttrBoneWeights.w;
			result.Bitangent += mul(rotation, input.AttrBitangent) * input.AttrBoneWeights.w;
		}

		result.PositionInObject.w = 1.0; // For numerical stability
	}
	else
	{
		result.PositionInObject = input.AttrPositionInObject;
		result.Normal = input.AttrNormal;
		result.Tangent = input.AttrTangent;
		result.Bitangent = input.AttrBitangent;
	}
	return result;
}
#else
BonesResult ApplyBones(VertexIn input, uint instanceBonesOffset)
{
	BonesResult result;
	result.PositionInObject = input.AttrPositionInObject;
	result.Normal = input.AttrNormal;
	result.Tangent = input.AttrTangent;
	result.Bitangent = input.AttrBitangent;
	return result;
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

uint GetVectorsOffset(uint instanceId)
{
	int width, height, num_levels;
	InstanceOffsets.GetDimensions(0, width, height, num_levels);
	uint modelOffset = (uint)InstanceOffsets.Load(int3(ModelIndex % width, ModelIndex / width, 0)).x;
	return modelOffset + instanceId * VectorsPerInstance;
}
