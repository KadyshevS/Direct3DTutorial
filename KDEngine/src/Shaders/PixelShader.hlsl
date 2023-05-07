cbuffer CBuf2
{
	float3 face_colors[6];
};

float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	return float4(face_colors[tid / 2], 1.0);
}