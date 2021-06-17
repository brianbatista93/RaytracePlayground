struct V2F
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

Texture2D<float4> inputTex : register(t0);

float4
main(in V2F IN)
  : SV_Target
{
    uint3 coord = uint3(IN.Position.xy, 0);

    return inputTex.Load(coord);
}