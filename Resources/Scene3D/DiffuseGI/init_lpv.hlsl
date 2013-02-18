
#define TILE_SIZE 16

Texture2D<float4> rsm_diffuse;
Texture2D<float4> rsm_normal;
Texture2D<float> rsm_depth;

RWTexture2D<float4> lpvRed;
RWTexture2D<float4> lpvGreen;
RWTexture2D<float4> lpvBlue;

cbuffer Uniforms
{
	float4x4 shadowProjectionToEye;
	float4x4 eyeToLPV;
	float3 displacement; // displacement / float3(LPV_WIDTH, LPV_HEIGHT, LPV_DEPTH);
	int3 lpvSize; // Dimensions of the LPV 3D texture
	float fluxWeightMultiplier; // ((2 * tan_Fov_X_half)/RSMWidth) * ((2 * tan_Fov_Y_half)/RSMHeight)
	float lightScale;
};

float4 clampledCosineCoeff(float3 xyz);

[numthreads(TILE_SIZE,TILE_SIZE,1)]
void main(uint3 globalThreadID : SV_DispatchThreadID)
{
	// Find diffuse color, face normal, and position of shadow map texel:
	float3 color = rsm_diffuse.Load(x,y).xyz;
	float3 normalInLPV = normalize(rsm_normal.Load(x,y).xyz);
	float4 positionInEye = unproject(x, y, rsm_depth.Load(x,y), shadowProjectionToEye);
	float4 positionInLPV = eyeToLPV * positionInEye;

	float fluxWeight = positionInEye.z * positionInEye.z * fluxWeightMultiplier;

	bool outside =
		positionInLPV.x < 0.0f || positionInLPV.x >= 1.0 ||
		positionInLPV.y < 0.0f || positionInLPV.y >= 1.0 ||
		positionInLPV.z < 0.0f || positionInLPV.z >= 1.0;
	if (outside)
		return;

	// displace position half a cell size
	positionInLPV += normalInLPV * displacement;

	// find LPV texel for our position
	int4 lpvPos = int4(
		floor(positionInLPV.x * lpvSize.x + 0.5f),
		floor(positionInLPV.y * lpvSize.y + 0.5f),
		floor(positionInLPV.z * lpvSize.z + 0.5f),
		0);

	float4 lpvCoefficientsRed = lpvRed[lpvPos];
	float4 lpvCoefficientsGreen = lpvGreen[lpvPos];
	float4 lpvCoefficientsBlue = lpvBlue[lpvPos];

	// Calculate the flux for our virtual point light (VPL)
	float3 flux = color * fluxWeight * lightScale;

	// Add VPL represented as SH coefficients of clamped cosine lobes oriented in the direction of the normal
	float4 normalCoefficients = clampledCosineCoeff(normalInLPV);
	lpvCoefficientsRed += normalCoefficients * flux.r;
	lpvCoefficientsGreen += normalCoefficients * flux.g;
	lpvCoefficientsBlue += normalCoefficients * flux.b;

	// Save LPV texel
	lpvRed[lpvPos] = lpvCoefficientsRed;
	lpvGreen[lpvPos] = lpvCoefficientsGreen;
	lpvBlue[lpvPos] = lpvCoefficientsBlue;
}

/////////////////////////////////////////////////////////////////////////////
// Spherical harmonics (SH):

#define PI 3.14159265

//clamped cosine oriented in z direction expressed in zonal harmonics and rotated to direction 'd'
//   zonal harmonics of clamped cosine in z:
//     l=0 : A0 = 0.5*sqrt(PI)
//     l=1 : A1 = sqrt(PI/3)
//     l=2 : A2 = (PI/4)*sqrt(5/(4*PI))
//     l=3 : A3 = 0
//   to rotate zonal harmonics in direction 'd' : sqrt( (4*PI)/(2*l+1)) * zl * SH coefficients in direction 'd'
//     l=0 : PI * SH coefficients in direction 'd' 
//     l=1 : 2*PI/3 * SH coefficients in direction 'd' 
//     l=2 : PI/4 * SH coefficients in direction 'd' 
//     l=3 : 0
float4 clampledCosineCoeff(float3 xyz)
{
	return float4(
		PI * 0.282094792,
		((2.0*PI)/3.0f) * -0.4886025119 * xyz.y,
		((2.0*PI)/3.0f) *  0.4886025119 * xyz.z,
		((2.0*PI)/3.0f) * -0.4886025119 * xyz.x;
}
