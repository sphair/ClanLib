
#define USE_BONES
#define USE_DUAL_QUATERNIONS

layout(std140) uniform ModelMaterialUniforms
{
	mat4 UVTextureMatrix0;
	mat4 UVTextureMatrix1;
	mat4 UVTextureMatrix2;
	mat4 UVTextureMatrix3;
	vec4 MaterialAmbient;
	vec4 MaterialDiffuse;
	vec4 MaterialSpecular;
	float MaterialGlossiness;
	float MaterialSpecularLevel;
};

layout(std140) uniform ModelInstanceUniforms
{
	mat3 ObjectNormalToEye;
	mat4 ObjectToWorld;
	mat4 WorldToEye;
	mat4 EyeToProjection;
};

uniform sampler2D BonesTexture;

in vec4 AttrPositionInObject;
in vec3 AttrNormal;
in vec3 AttrBitangent;
in vec3 AttrTangent;
in vec2 AttrUVMap0;
in vec2 AttrUVMap1;
in vec2 AttrUVMap2;
in vec2 AttrUVMap3;
in vec4 AttrBoneSelectors;
in vec4 AttrBoneWeights;

out vec3 NormalInEye;
out vec3 BitangentInEye;
out vec3 TangentInEye;
out vec4 PositionInWorld;
out vec4 PositionInEye;
out vec2 UVMap0;
out vec2 UVMap1;
out vec2 UVMap2;
out vec2 UVMap3;

vec3 Normal;
vec3 Tangent;
vec3 Bitangent;
vec4 PositionInObject;

void ApplyBones();
void AddBone(int boneSelector, float weight);
mat3x4 GetBoneMatrix(int boneIndex);
mat2x4 GetBoneDualQuaternion(mat2x4 currentDual, int boneIndex);
ivec2 GetTexelPosition(int index);

void main()
{
	ApplyBones();
	mat3 TangentObjectToEye = mat3(WorldToEye * ObjectToWorld);
	NormalInEye = normalize(ObjectNormalToEye * Normal);
	TangentInEye = normalize(TangentObjectToEye * Tangent);
	BitangentInEye = normalize(TangentObjectToEye * Bitangent);//cross(NormalInEye, TangentInEye);
	UVMap0 = (UVTextureMatrix0 * vec4(AttrUVMap0, 0, 1)).xy;
	UVMap1 = (UVTextureMatrix1 * vec4(AttrUVMap1, 0, 1)).xy;
	UVMap2 = (UVTextureMatrix2 * vec4(AttrUVMap2, 0, 1)).xy;
	UVMap3 = (UVTextureMatrix3 * vec4(AttrUVMap3, 0, 1)).xy;
	PositionInWorld = ObjectToWorld * PositionInObject;
	PositionInEye = WorldToEye * PositionInWorld;
	gl_Position = EyeToProjection * PositionInEye;
}

void ApplyBones()
{
#if defined(USE_BONES) && defined(USE_DUAL_QUATERNIONS)

	if (AttrBoneWeights != vec4(0))
	{
		// For details, read the paper "Geometric Skinning with Dual Skinning" by L. Kavan et al.
		mat2x4 dual = mat2x4(0);
		dual  = AttrBoneWeights.x * GetBoneDualQuaternion(dual, int(AttrBoneSelectors.x));
		dual += AttrBoneWeights.y * GetBoneDualQuaternion(dual, int(AttrBoneSelectors.y));
		dual += AttrBoneWeights.z * GetBoneDualQuaternion(dual, int(AttrBoneSelectors.z));
		dual += AttrBoneWeights.w * GetBoneDualQuaternion(dual, int(AttrBoneSelectors.w));
		dual /= length(dual[0]);

		PositionInObject.xyz = AttrPositionInObject.xyz + 2.0 * cross(dual[0].xyz, cross(dual[0].xyz, AttrPositionInObject.xyz) + dual[0].w * AttrPositionInObject.xyz);
		PositionInObject.xyz += 2.0 * (dual[0].w * dual[1].xyz - dual[1].w * dual[0].xyz + cross(dual[0].xyz, dual[1].xyz));
		PositionInObject.w = 1;

		Normal = AttrNormal + 2.0 * cross(dual[0].xyz, cross(dual[0].xyz, AttrNormal) + dual[0].w * AttrNormal);
		Tangent = AttrTangent + 2.0 * cross(dual[0].xyz, cross(dual[0].xyz, AttrTangent) + dual[0].w * AttrTangent);
		Bitangent = AttrBitangent + 2.0 * cross(dual[0].xyz, cross(dual[0].xyz, AttrBitangent) + dual[0].w * AttrBitangent);
	}
	else
	{
		PositionInObject = AttrPositionInObject;
		Normal = AttrNormal;
		Tangent = AttrTangent;
		Bitangent = AttrBitangent;
	}

#elif defined(USE_BONES)

	if (AttrBoneWeights != vec4(0))
	{
		PositionInObject = vec4(0);
		Normal = vec3(0);
		Tangent = vec3(0);
		Bitangent = vec3(0);
		AddBone(int(AttrBoneSelectors.x), AttrBoneWeights.x);
		AddBone(int(AttrBoneSelectors.y), AttrBoneWeights.y);
		AddBone(int(AttrBoneSelectors.z), AttrBoneWeights.z);
		AddBone(int(AttrBoneSelectors.w), AttrBoneWeights.w);
		PositionInObject.w = 1;
	}
	else
	{
		PositionInObject = AttrPositionInObject;
		Normal = AttrNormal;
		Tangent = AttrTangent;
		Bitangent = AttrBitangent;
	}

#else

	PositionInObject = AttrPositionInObject;
	Normal = AttrNormal;
	Tangent = AttrTangent;
	Bitangent = AttrBitangent;

#endif
}

#if defined(USE_DUAL_QUATERNIONS)

mat2x4 GetBoneDualQuaternion(mat2x4 currentDual, int boneIndex)
{
	vec4 row0 = texelFetch(BonesTexture, GetTexelPosition(boneIndex * 2 + 0), 0);
	vec4 row1 = texelFetch(BonesTexture, GetTexelPosition(boneIndex * 2 + 1), 0);
	mat2x4 newDual = mat2x4(row0, row1);
	if (dot(currentDual[0], newDual[0]) < 0)
		return -newDual;
	else
		return newDual;
}

#else

void AddBone(int boneSelector, float weight)
{
	mat3x4 boneTransform = GetBoneMatrix(boneSelector);
	mat3 boneRotation = mat3(boneTransform); // only safe if all bones are uniformly scaled.  otherwise mat3(transpose(inverse(bone))) is needed for the normal vector (but not tangents!).
	PositionInObject.xyz += (AttrPositionInObject * boneTransform) * weight;
	Normal += (AttrNormal * boneRotation) * weight;
	Tangent += (AttrTangent * boneRotation) * weight;
	Bitangent += (AttrBitangent * boneRotation) * weight;
}

mat3x4 GetBoneMatrix(int boneIndex)
{
	vec4 row0 = texelFetch(BonesTexture, GetTexelPosition(boneIndex * 3 + 0), 0);
	vec4 row1 = texelFetch(BonesTexture, GetTexelPosition(boneIndex * 3 + 1), 0);
	vec4 row2 = texelFetch(BonesTexture, GetTexelPosition(boneIndex * 3 + 2), 0);
	return mat3x4(row0, row1, row2); // creates a row-major matrix, which means we must swap the matrix multiplication in AddBone
}

#endif

ivec2 GetTexelPosition(int index)
{
	return ivec2(index % textureSize(BonesTexture, 0).x, index / textureSize(BonesTexture, 0).x);
}
