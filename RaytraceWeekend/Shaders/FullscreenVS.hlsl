struct V2F
{
    float4                     Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

V2F
main(uint vertexID
     : SV_VertexID)
{
    V2F OUT = (V2F)0;

    OUT.TexCoord = float2((vertexID << 1) & 2, vertexID & 2);
    OUT.Position = float4(OUT.TexCoord * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);

    return OUT;
}