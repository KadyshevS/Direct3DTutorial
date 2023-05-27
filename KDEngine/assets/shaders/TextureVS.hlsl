cbuffer CBuf
{
	matrix transform;
};

struct VSOut
{
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float2 tex : TexCoord)
{
	VSOut vout;
	vout.pos = mul(float4(pos, 1.0), transform);
	vout.tex = tex;
	return vout;
}