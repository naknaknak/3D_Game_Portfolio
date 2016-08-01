
// : 뒤에 붙는건 시멘틱(Sementic이라는것 어떤 타입인지 명확하게 하기위해 사용)

float4x4 world333 : World;
float4x4 view : View;
float4x4 projection : Projection;
float4x4 rotation;

struct VS_INPUT
{
	float4 Position : POSITION0;
};

struct VS_OUTPUT
{
	float4 Position : POSITION0;
};

VS_OUTPUT Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.Position = mul(Input.Position, world333);
	Output.Position = mul(Output.Position, view);
	Output.Position = mul(Output.Position, projection);
	return(Output);

}

VS_OUTPUT Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main2(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.Position = mul(Input.Position, rotation);
	Output.Position = mul(Output.Position, world333);
	Output.Position = mul(Output.Position, view);
	Output.Position = mul(Output.Position, projection);
	return(Output);
}

//색깔을 리턴
float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main() : COLOR0
{
	return(float4(0.0f, 1.0f, 0.0f, 1.0f));
}

//받은 색깔 중 파란색만 남기기
float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main2() : COLOR0
{
	return(float4(1.0f, 0.0f, 0.0f, 1.0f));
}

//모자이크 용
struct PS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
};

texture m_TextureDiffuse;
sampler sampleDiffuse = sampler_state
{
	texture = <m_TextureDiffuse>;
	AddressU = Clamp;
	AddressV = Clamp;

	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

float m_MosaikDelta = 20;              // Image Mosaic Delta
float4 Mosaik_Pass_0_Pixel_Shader(PS_INPUT In) : COLOR0
{
	float4 Out = 0;
	float u = In.Tex.x;
	float v = In.Tex.y;
	int iX = (int)(u * m_MosaikDelta);
	int iY = (int)(v * m_MosaikDelta);
	float x = iX / (m_MosaikDelta);
	float y = iY / (m_MosaikDelta);

	Out = tex2D(sampleDiffuse, float2(x, y));

	return Out;
}

//--------------------------------------------------------------//
// Technique Section for Default_DirectX_Effect
//--------------------------------------------------------------//
technique Default_DirectX_Effect
{
	pass Pass_0
	{
		VertexShader = compile vs_2_0 Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main();
	}
}

technique Default_DirectX_Effect2
{
	pass Pass_0
	{
		VertexShader = compile vs_2_0 Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main2();
		PixelShader = compile ps_2_0 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main2();
	}

}

technique Mosaik
{
	pass Pass_0
	{
		PixelShader = compile ps_2_0 Mosaik_Pass_0_Pixel_Shader();
	}
};
