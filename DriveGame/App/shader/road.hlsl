
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
};

cbuffer PSConstants2D : register(b0) {
	float4 g_colorAdd;
	float4 g_sdfParam;
	float4 g_sdfOutlineColor;
	float4 g_sdfShadowColor;
	float4 g_internal;
};

cbuffer TimeStruct : register(b1) {
	float time;
};

cbuffer RoadData : register(b2) {
	float start0;
	float start1;
	float curve0;
	float curve1;
};

static float CAMERA_H = 145.0f;
static float SCREEN_H = 480.0f;

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

// ����: ���{��̃R�����g�͍Ō�ɃZ�~�R�����������ƃG���[�f�����ۂ�;

// PixelMain_Texture
float4 ps_main(s3d::PSInput input) : SV_TARGET {
	float2 uv = input.uv;

	float start[2] = { start0, start1 };
	float curve[2] = { -curve0, -curve1 };
	
	for (int i = 0; i < 2; ++i) {
		if (start[i] < 0)
			start[i] = 0;
		curve[i] /= 1000;
	}
	
	// y�����]�i�������֐L�΂��j;
	float y = uv.y * -1.0 + 1.0;
	
	// �n�ʃG���A���ɐ��K���������;
	float yG = y * 2.0f;
	
	// z�l�����߂�;
	// Yc / Zc = Ys ���AZc = Yc / Ys;
	// Yc�̓J�����̍����ɓ������i�J�������猩���n�ʁj;
	// Ys�͒n������0�Ƃ��āA���Ɏ���L�΂����Ƃ��̃X�N���[�����W�ł���;
	float z = (CAMERA_H / ((uv.y - 0.5) * SCREEN_H));
	
	// �J�[�u�Ƃ̋��E����Y���W;
	// ������0�A���[�� SCREEN_H / 2 �Ƃ����Ƃ��̃s�N�Z����;
	const float borderY_GI_toD = CAMERA_H / start[1];
	
	// ��L�𐳋K����������;
	const float borderY_GF_toD = borderY_GI_toD / (SCREEN_H / 2);
	
	// ��L�̎����A�������֐L�т�悤�ɂ����o�[�W����;
	const float borderY_GF_toU = borderY_GF_toD * -1 + 1.0f;
	
	float borderZ = start[1];
	
	float aaa = 0;
	
	// ���X�^�X�N���[��;
	// �������̉E�ӂɂ̓R���p�C�����萔�����u���Ȃ��݂���;
	if (z - start[0] > 0 && z - start[1] < 0) {
		uv.x += curve[0] * z;
	}
	else if (z - start[1] > 0) {		
		float horizon = 1.0f;
		float rate = (yG - borderY_GF_toU) / (horizon - borderY_GF_toU);
		rate = pow(rate, 2);
		if (rate < 0)
			rate = 0;
		if (rate > 1)
			rate = 1;
		if (borderZ < 0.00001f)
			rate = 1;

		uv.x += (curve[1] * z * rate) + (curve[0] * z * (1.0f - rate));	
	}

	float4 texColor = g_texture0.Sample(g_sampler0, uv);
	
	if (length(texColor.rgb - float3(0.0, 0.0, 0.0)) < 0.01)
		discard;
	
	if (aaa > 20)
		texColor.rgb = float3(0.0, 1.0, 0.0);
	
	return texColor;
}
