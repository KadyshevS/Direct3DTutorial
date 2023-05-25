cbuffer LightCBuf
{
    float3 LightPos;
};

static const float3 MaterialColor = { 0.7f, 0.7f, 0.9f };
static const float3 Ambient = { 0.15f, 0.15f, 0.15f };
static const float3 DiffuseColor = { 1.0f, 1.0f, 1.0f, };
static const float DiffuseIntensity = 1.0f;
static const float AttenuateConst = 1.0f;
static const float AttenuateLatency = 1.0f;
static const float AttenuateQuad = 1.0f;

float4 main(float3 WorldPos : Position, float3 N : Normal) : SV_Target
{
    const float3 vToL = LightPos - WorldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    
    const float att = AttenuateConst + AttenuateLatency * distToL + AttenuateQuad * (distToL * distToL);
    const float3 diffuse = DiffuseColor * DiffuseIntensity * att * max(0.0f, dot(dirToL, N));
    
    return float4(saturate(diffuse + Ambient), 1.0f);
}