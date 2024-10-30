
//***********************************************************************************//
// 道路のテクスチャをラスタスクロールさせ、カーブさせるプログラムです。              //
// テクスチャは、オフスクリーンレンダリングでC++側から描画したものを使います。       //
// 今回Siv3Dを使ったのは、オフスクリーンレンダリングが簡単にできそうだったからです。 //
//***********************************************************************************//

# version 410

// Textures
uniform sampler2D Texture0;

// PSInput
layout(location = 0) in vec4 Color;
layout(location = 1) in vec2 UV;

// PSOutput
layout(location = 0) out vec4 FragColor;

// Constant Buffer
layout(std140) uniform PSConstants2D {
	vec4 g_colorAdd;
	vec4 g_sdfParam;
	vec4 g_sdfOutlineColor;
	vec4 g_sdfShadowColor;
	vec4 g_internal;
};

//**********************************************************//
// ここから上は、Siv3Dのサンプルからコピーしてきた部分です  //
// ここから下は自分で書きました                             //
//**********************************************************//

// ゲームがスタートしてからの経過時間
layout(std140) uniform TimeStruct {
	float time;
};

// 遠くは描画しても見えないため、直近のカーブ3つ関する情報を受け取る
// このデータを元に道路をまげて、カーブを表現する
// start0～2: カメラから各カーブまでの距離
// curve0～2: 各カーブの曲がり具合
// 直線は、曲がり具合 0 のカーブとして扱う
// 
// 例えば、
// 40m後ろから10m先までが直線が続き、
// そこから曲がり具合5の右カーブが20m続く
// さらにその先は直線だとすると、
// 
// start0 = -40
// start1 = 10
// start2 = 30 (10 + 20)
//
// curve0 = 0
// curve1 = 5
// curve2 = 0
//
// のようなデータになる
// これがC++側から送信される
layout(std140) uniform RoadData {
	float start0;
	float start1;
	float start2;
	float curve0;
	float curve1;
	float curve2;
};

// カメラの高さ
layout(std140) uniform CameraData {
	float cameraH;
};

// スクリーンの縦幅
const float SCREEN_H = 480.0f;

void main() {
	vec2 uv = UV;

	// 道路のカーブデータを配列にまとめます
	float[3] start = float[](start0, start1, start2);
	float[3] curve = float[](curve0, curve1, curve2);
	for (int i = 0; i < 3; ++i) {
		if (start[i] < 0)
			start[i] = 0;
		curve[i] *= -1;
	}

	// v軸を反転したもの（下から上へ伸ばしたもの）
	float y = uv.y * -1.0 + 1.0;
	
	// yを地面エリア内に正規化したもの
	// 画面下半分が地面で、上半分は空になっている
	float yG = y * 2.0f;
	
	// ピクセルのz値を求める
	// カメラ座標系の y を z で割れば、スクリーン座標系の y が求まるはずである
	// これを式変形してカメラ座標系における z を算出
	float z = (cameraH/ ((uv.y - 0.5) * SCREEN_H));
	
	// 3つのカーブ情報を取得しているため、画面内にはカーブの境界が2つ存在する
	// その境界線のY座標を計算
	// 中央を0、下端を SCREEN_H / 2 としたときのピクセル数
	float borderY_GI_toD = cameraH / start[1];
	float border2Y_GI_toD = cameraH / start[2];
	
	// 上記を正規化したもの;
	float borderY_GF_toD = borderY_GI_toD / (SCREEN_H / 2);
	float border2Y_GF_toD = border2Y_GI_toD / (SCREEN_H / 2);
	
	// 上記の軸を、下から上へ伸びるようにした変更したもの
	float borderY_GF_toU = borderY_GF_toD * -1 + 1.0f;
	float border2Y_GF_toU = border2Y_GF_toD * -1 + 1.0f;
	
	float borderZ = start[1];
	float border2Z = start[2];

	// + ラスタスクロール
	// ピクセルに適応するテクスチャのUV座標を計算
	// カーブの境界によって道路は3つのエリアに分けられる
	// ピクセルがどのエリアに属するかを判定し、エリアごとに場合分けして計算を行う
	if (z - start[0] > 0 && z - start[1] < 0) {
		// 1番手前のカーブ
		uv.x += curve[0] * z;
	}
	else if (z - start[1] > 0 && z - start[2] < 0) {		
		// 2番目のカーブ
		// 前のカーブから影響を受けるので、若干式が複雑化する
		float rate = (yG - borderY_GF_toU) / (1.0f - borderY_GF_toU);
		rate = pow(rate, 2);
		if (rate < 0)
			rate = 0;
		if (rate > 1)
			rate = 1;
		if (borderZ < 0.00001f)
			rate = 1;

		uv.x += (curve[1] * z * rate) + (curve[0] * z * (1.0f - rate));	
	}
	else if (z - start[2] > 0) {
		// 3番目のカーブ
		// 前2つのカーブから影響を受けるため、それを加味して計算
		float rate1 = (yG - borderY_GF_toU) / (1.0f - borderY_GF_toU);
		rate1 = pow(rate1, 2);
		float rate2 = (yG - border2Y_GF_toU) / (1.0f - border2Y_GF_toU);
		rate2 = pow(rate2, 2);
		
		if (border2Z < 0.00001f) {
			rate1 = 0;
			rate2 = 1;
		}

		float deltaX1 = (curve[1] * z * rate1) + (curve[0] * z * (1.0f - rate1));
		uv.x += (curve[2] * z * rate2) + (deltaX1 * (1.0f - rate2));
	}

	// テクスチャカラーを取得
	vec4 texColor = texture(Texture0, uv);

	// 黒いところは道路外なので透過させる
	if (distance(texColor.rbg, vec3(0.0, 0.0, 0.0)) < 0.01)
		discard;

	// 色を出力
	FragColor = ((texColor * Color) + g_colorAdd);
}
