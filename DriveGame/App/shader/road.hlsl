
Texture2D		g_texture0 : register(t0);
SamplerState	g_sampler0 : register(s0);

namespace s3d {

	struct VSInput {
		float2 position	: POSITION;
		float2 uv		: TEXCOORD0;
		float4 color	: COLOR0;
	};

	struct PSInput {
		float4 position	: SV_POSITION;
		float4 color	: COLOR0;
		float2 uv		: TEXCOORD0;
	};

	float4 Transform2D(float2 pos, float2x4 t) {
		return float4((t._13_14 + (pos.x * t._11_12) + (pos.y * t._21_22)), t._23_24);
	}
}

cbuffer VSConstants2D : register(b0) {
	row_major float2x4 g_transform;
	float4 g_colorMul;
}

cbuffer PSConstants2D : register(b0) {
	float4 g_colorAdd;
	float4 g_sdfParam;
	float4 g_sdfOutlineColor;
	float4 g_sdfShadowColor;
	float4 g_internal;
}

cbuffer TimeStruct : register(b1) {
	float time;
}

cbuffer RoadData : register(b2) {
    float start_;
    float end_;
    float curve_;
}

//cbuffer DistortionSize : register(b2) {
//    float dim;
//}

//��//��//��//��//��// �֐� //��//��//��//��//��//

// VertexMain
s3d::PSInput vs_main(s3d::VSInput input) {
	s3d::PSInput psIn;
	psIn.position = s3d::Transform2D(input.position, g_transform);
	psIn.color = input.color * g_colorMul;
	psIn.uv = input.uv;
	return psIn;
}

float4 gray(const float4 texColor) {
	float value = 0.213f * texColor.r + 0.715 * texColor.g + 0.0722f * texColor.b;
	float4 color = float4(value, value, value, texColor.a);
	return color;
}

// PixelMain_Shape
float4 ps_main_shape(s3d::PSInput input) : SV_TARGET {
	return input.color;
}

//// PixelMain_Texture
//float4 ps_main_texture(s3d::PSInput input) : SV_TARGET{
//	const float4 texColor = g_texture0.Sample(g_sampler0, input.uv);
//	const float4 grayColor = gray(texColor);
//
//	float rate = time / 3;
//	if (rate > 1.0)
//		rate = 1.0;
//	return lerp(texColor, grayColor, rate);
//}

// ����: ���{��̃R�����g�͍Ō�ɃZ�~�R�����������ƃG���[�f�����ۂ�;

static float d = 4;

float f(const float y, const float curve) {
	return pow(y, d) * curve;
}

float f_dash(const float y, const float curve) {
	return d * pow(y, d - 1) * curve;
}


// PixelMain_Texture
float4 ps_main(s3d::PSInput input) : SV_TARGET {
	float2 uv = input.uv;

	//// ����𒴂���ƃJ�[�u���͂��܂�;
	//float start = 0.0;

	//// �J�[�u�͂����܂�;
	//float end = 0.7;

	//// �J�[�u�̊����I��;
	//// �ł����قǋ}�J�[�u;
	//float curve = -6.0;

	////curve = fmod(time/3, 4) - 2;

	//start = 1.0 - fmod(time / 2, 4);
	//end = start + 2;
    float start = start_;	// ����𒴂���ƃJ�[�u���͂��܂�;
    float end = end_;		// �J�[�u�͂����܂�;
    float curve = -curve_;	// �ł����قǋ}�J�[�u;
    if (start > 1)
        start = 1;
	if (start < 0)
		start = 0;
	if (end > 1)
		end = 1;
	if (end < 0)
		end = 0;
	
	// y�����]�i�������֐L�΂��j;
	float y = uv.y * -1.0 + 1.0;

	// ���X�^�X�N���[��;
	if (y > start)
		if (y < end) {
			uv.x += f(y - start, curve);
		}
		else {
			// �J�[�u�I���n�_�ɂ����� X�̃Y��;
			float endX = f(end - start, curve);

			// �����ł̌X��;
			float slope = f_dash(end - start, curve);

			// ���Ƃ͈ꎟ�֐��̎��ŏo��;
			uv.x += endX + (y - end) * slope;
		}

	const float4 texColor = g_texture0.Sample(g_sampler0, uv);
	if (length(texColor.rgb - float3(0.0, 0.0, 0.0)) < 0.01)
		discard;
	
	return texColor;
}
