
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

static float HEIGHT = 0.5f;

static float CAMERA_H = 145.0f;
static float SCREEN_H = 480.0f;

static float DIM = 1.0f;

static float Z_SPAN = 5.0f;

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

static float d = 12.0f;
static float d2 = d - 6.0f;

// pow�̑�2������7�ȏ�ɂ�����G���[�o���̂ŁA2��pow���g���Ă���;
float f(const float y, const float curve) {
	return pow(y, 6.0f) * pow(y, d2) * curve;
}

float f_dash(const float y, const float curve) {
	return d * pow(y, 6.0f - 1) * pow(y, d2) * curve;
}


// PixelMain_Texture
float4 ps_main(s3d::PSInput input) : SV_TARGET {
	float2 uv = input.uv;

	float start[2] = { start0, start1 };
	float curve[2] = { -curve0, -curve1 };
	
	//start[0] = 0.0f;
	//start[1] = 0.2f;
	//curve[0] = 0.0f;
	//curve[1] = -15500.0f;
	
	for (int i = 0; i < 2; ++i) {
		// �Ƃ肠���� 0�`1 �ɂ��Ă���;
		//if (start[i] > 1)
		//	start[i] = 1;
		if (start[i] < 0)
			start[i] = 0;
		
		// �n�����ʒu��0.5�Ȃ̂ŁA�����ɐ��K�����ۂ����Ƃ���;
		// ���ł�0�`1�ɂ͂��Ă���̂ŁA�����ɂ���΂���;
		//start[i] *= HEIGHT;
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
	
	//z = pow(z, DIM);
	
	// �n�ʃG���A�i�X�N���[���������j�ɂ����āA2�J�[�u�̋��ڂ��ǂ��ɂ��邩��border�ŕ\��;
	// ���[��0�A�n�ʃG���A��[��1�Ƃ���;
	//float borderY = CAMERA_H / start[1] + SCREEN_H / 2;
	//borderY /= SCREEN_H;
	//borderY = (borderY - 0.5f) * 2.0f;
	
	float borderY = (CAMERA_H / start[1]) / (SCREEN_H / 2);
	//borderY = (borderY - 0.5f) * 2.0f;
	if (borderY < 0.0f)
		borderY = 0.0f;
	if (borderY > 1.0f)
		borderY = 1.0f;
	float border = borderY;
	
	// �J�[�u�Ƃ̋��E����Y���W;
	// ������0�A���[�� SCREEN_H / 2 �Ƃ����Ƃ��̃s�N�Z����;
	const float borderY_GI_toD = CAMERA_H / start[1];
	
	// ��L�𐳋K����������;
	const float borderY_GF_toD = borderY_GI_toD / (SCREEN_H / 2);
	
	// ��L�̎����A�������֐L�т�悤�ɂ����o�[�W����;
	const float borderY_GF_toU = borderY_GF_toD * -1 + 1.0f;
	
	float borderZ = start[1];
	
	//// border��菭����iZ_SPAN�������炵����j�̈ʒu�����߂�;
	//float border2 = CAMERA_H / (start[1] + Z_SPAN) + SCREEN_H / 2;
	//border2 /= SCREEN_H;
	//border2 = (border2 - 0.5f) * 2.0f;
	//if (border2 < 0.0f)
	//	border2 = 0.0f;
	//if (border2 > 1.0f)
	//	border2 = 1.0f;
	
	float aaa = 0;
	float rateA = 0;
	
	// ���X�^�X�N���[��;
	// �������̉E�ӂɂ̓R���p�C�����萔�����u���Ȃ��݂���;
	if (z - start[0] > 0 && z - start[1] < 0) {
		//uv.x += f(y - start[0], curve[0]);
		uv.x += curve[0] * z;
	}
	//else if (z - start[1] > 0 && z - (start[1] + Z_SPAN) < 0) {
	//	float a = (z - start[1]) / Z_SPAN;
	//	uv.x += (curve[1] * z * a) + (curve[0] * z * (1.0f - a));
	//}
	//else if (z - (start[1] + Z_SPAN) > 0) {	// (z - start[1] > 0)
	else if (z - start[1] > 0) {
		//uv.x += f(y - start[1], curve[1]);// + f(start[1] - start[0], curve[0]);
		//float endX = f(start[1] - start[0], curve[0]);
		float slope = f_dash(start[1] - start[0], curve[0]);
		//uv.x += f(y, curve[1]) + endX + (y - start[1]) * slope;
		//uv.x += f(y - start[1], curve[1]) + endX;// + f(start[1] - start[0], curve[0]);
		
		float endX = curve[0] * start[1];
		//uv.x += curve[1] * z * (1 - start[1]) + curve[0] * (start[1]);
		//uv.x += curve[1] * (z - start[1]); // + curve[0] * start[1];
		//uv.x += curve[1] * pow(z, DIM) * (1.0f - v) + curve[0] * pow(z, DIM) * (v);
		//uv.x += curve[1] * z * (1.0f - border) + curve[0] * z * border;
		
		//float rate = (z - borderZ) / 50;
		//float rate = (y * (SCREEN_H / 2) - borderY) / (SCREEN_H / 2 - borderY);
		
		//float rate = (y - borderY) / (1.0f - borderY);
		
		//float borderY2 = borderY * 2.0f;
		//float y2 = y * 2.0f;
		//float horizon = 1.0f;
		//float rate = (y2 - borderY2) / (horizon - borderY2);
		
		float horizon = 1.0f;
		float rate = (yG - borderY_GF_toU) / (horizon - borderY_GF_toU);
		rate = pow(rate, 2);
		if (rate < 0)
			rate = 0;
		if (rate > 1)
			rate = 1;
		if (borderZ < 0.00001f)
			rate = 1;
		
		if (borderZ < 0.0f) {
			uv.x += curve[1] * z;
			uv.x -= curve[1] * z;
			uv.x = 0.5;
		}
		else
			uv.x += (curve[1] * z * rate) + (curve[0] * z * (1.0f - rate));
		
		rateA = rate;
		//if (y > 0.5)
		//	rate = 1;
		//if (rate < 0)
		//	rate = 0;
		//if (rate > 1)
		//	rate = 1;
		
		//if (rate > 0.001 && rate < 0.999)
		//	aaa = 100;
		
		//if (y > borderY)
		//	aaa = 100;
		
		//if (z - start[1])
		//	aaa = 100;
		
		//uv.x += curve[1] * z;
		//uv.x += (curve[1] * z * border) + (curve[0] * z * (1.0f - border));
		//uv.x += (curve[1] * z * rate) + (curve[0] * z * (1.0f - rate));
		//uv.x += (curve[1] * z * (1.0f - rate)) + (curve[0] * z * rate);
		
		//uv.x += f(y - start[1], curve[1]) * (1 - start[1]) + f(y - start[0], curve[0]) * (start[1] - 0);
		//uv.x += endX + f(y - start[1], curve[1]) * (1 - start[1]) + f(y - start[0], curve[0]) * (start[1] - 0);
		//uv.x += endX + f(y - start[1], curve[1]) * (1 - start[1]) + f(y - start[0], curve[0]) * (start[1] - start[0]);
		
	}
	
	//borderY = (CAMERA_H / start[1]) / (SCREEN_H / 2);
	// borderY = borderY * -1 + 1.0;
	
	//if (yG > borderY_GF_toU && yG < borderY_GF_toU + 0.05)
	//	aaa = 100;
	//if (z > borderZ - 0.03f && z < borderZ + 0.03f)
	//	aaa = 100;
	
	//if (yG < rateA)
	//	aaa = 100;
	
	//if (borderZ < 0.001)
	//	aaa = 100;

	//// ���X�^�X�N���[��;
	//if (y > start)
	//	if (y < end) {
	//		uv.x += f(y - start, curve);
	//	}
	//	else {
	//		// �J�[�u�I���n�_�ɂ����� X�̃Y��;
	//		float endX = f(end - start, curve);

	//		// �����ł̌X��;
	//		float slope = f_dash(end - start, curve);

	//		// ���Ƃ͈ꎟ�֐��̎��ŏo��;
	//		uv.x += endX + (y - end) * slope;
	//	}

	float4 texColor = g_texture0.Sample(g_sampler0, uv);
	
	if (length(texColor.rgb - float3(0.0, 0.0, 0.0)) < 0.01)
		discard;
	
	if (aaa > 20)
		texColor.rgb = float3(0.0, 1.0, 0.0);
	
	return texColor;
}
