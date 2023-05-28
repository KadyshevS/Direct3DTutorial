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
    float SpecularIntensity;
    float SpecularPower;
};

Texture2D tex;
SamplerState splr;

float4 main(float3 cameraPos : Position, float2 tc : TexCoord, float3 n : Normal) : SV_Target
{
//  Diffuse
    const float3 vToL = LightPos - cameraPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    
    const float att = 1.0f / (AttenuateConst + AttenuateLatency * distToL + AttenuateQuad * (distToL * distToL));
    const float3 Diffuse = DiffuseColor * DiffuseIntensity * att * max(0.0f, dot(dirToL, n));
    
//  Specular
    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.0f - vToL;
	
    const float3 Specular = att * ((DiffuseColor * DiffuseIntensity) * SpecularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(cameraPos))), SpecularPower));
    
    return float4(saturate(Diffuse + Ambient + Specular) * MaterialColor, 1.0f) * tex.Sample(splr, tc);
}