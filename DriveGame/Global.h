#pragma once

#include <Siv3D.hpp> // Siv3D v0.6.14
#include <random>
#include "sgmPlus.h"

using namespace SGM2;
using namespace SGM2::Plus;

// サイズ
constexpr int32 SCREEN_W = 640;
constexpr int32 SCREEN_H = 480;
constexpr Point SCREEN_CENTER = { SCREEN_W / 2, SCREEN_H / 2 };

constexpr int32 RENDER_TEXTURE_W = SCREEN_W * 1.5;
constexpr int32 RENDER_TEXTURE_W_HALF = RENDER_TEXTURE_W / 2;
constexpr int32 RENDER_TEXTURE_W_QUARTER = RENDER_TEXTURE_W_HALF / 2;

// カーブ時の歪み計算で使う値
// シェーダー側と揃えるように
constexpr double DIM = 4.0;

// 乱数生成器
extern std::random_device seedGenerator;
extern std::mt19937 random;

// フォント
extern unique_ptr<Font> fontA;
extern unique_ptr<Font> fontDeb;

// ゲームステートマネージャー
extern unique_ptr<GameStateManager> gsm;

// 画像
extern unique_ptr<Texture> imgMountIwate;
extern unique_ptr<Texture> imgBallonR;
extern unique_ptr<Texture> imgTree01;
extern unique_ptr<Texture> imgTree02;
extern unique_ptr<Texture> imgGrass01;

// シェーダー
extern unique_ptr<VertexShader> vertexShader;
extern unique_ptr<PixelShader> pixelShader;

// コンスタントバッファー
struct TimeStruct {
	float time;
};
extern ConstantBuffer<TimeStruct> cbTime;

struct RoadData {
	float start;
	float end;
	float curve;
};
extern ConstantBuffer<RoadData> cbRoad;

inline void load_font() {
	//fontA = std::make_unique<Font>(FontMethod::Bitmap, 12, FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"msgothic.ttc", FontStyle::Bitmap);
	fontDeb = std::make_unique<Font>(FontMethod::Bitmap, 23, U"font/PixelMplus12-Regular.ttf", FontStyle::Bitmap);
	fontA = std::make_unique<Font>(FontMethod::Bitmap, 32, U"font/PixelMplus12-Regular.ttf", FontStyle::Bitmap);
}

inline void load_image() {
	imgMountIwate.reset(new Texture(U"./img/mount_iwate.png"));
	imgBallonR.reset(new Texture(U"./img/ballon01.png"));
	imgTree01.reset(new Texture(U"./img/tree01.png"));
	imgTree02.reset(new Texture(U"./img/tree02.png"));
	imgGrass01.reset(new Texture(U"./img/grass01.png"));
}

inline bool load_shader() {
	vertexShader.reset(new VertexShader(HLSL{ U"./shader/road.hlsl", U"vs_main" }));
	pixelShader.reset(new PixelShader(HLSL{ U"./shader/road.hlsl", U"ps_main" }));

	if (*vertexShader && *pixelShader)
		return true;
	return false;
}

inline void init() {
	cbTime->time = 0.0f;
	cbRoad->start = 1.0f;
	cbRoad->end= 1.0f;
	cbRoad->curve = 5.0f;
}

