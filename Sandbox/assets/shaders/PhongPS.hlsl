cbuffer LightCBuf
{
    float3 LightPos;
    float3 Ambient;
    float3 DiffuseColor;
    float DiffuseIntensity;
    float AttenuateConst;
    float AttenuateLatency;
    float AttenuateQuad;
};

cbuffer ObjectCBuf
{
    float3 MaterialColor;
};

float4 main(float3 WorldPos : Position, float3 N : Normal) : SV_Target
{
    const float3 vToL = LightPos - WorldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    
    const float att = 1.0f / (AttenuateConst + AttenuateLatency * distToL + AttenuateQuad * (distToL * distToL));
    const float3 diffuse = DiffuseColor * DiffuseIntensity * att * max(0.0f, dot(dirToL, N));
    
    return float4(saturate(diffuse + Ambient) * MaterialColor, 1.0f);
}