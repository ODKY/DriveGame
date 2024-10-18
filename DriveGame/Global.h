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
constexpr double DIM = 12.0;//4.0

// 遠くを隠すために、道路以外を少し下に下げる
constexpr int32 ALL_OFFSET_Y = 3;

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
//extern unique_ptr<Texture> imgBallonR;
//extern unique_ptr<Texture> imgTree01;
//extern unique_ptr<Texture> imgTree02;
//extern unique_ptr<Texture> imgGrass01;
constexpr int IMG_RED_BALLON = 0;
constexpr int IMG_YELLOW_BALLON = 1;
constexpr int IMG_BLUE_BALLON = 2;
constexpr int IMG_GRASS1 = 3;
constexpr int IMG_TREE1 = 4;
constexpr int IMG_TREE2 = 5;
constexpr int IMG_TRAFFIC_LIGHT = 6;
constexpr int IMG_ARROW1 = 7;

constexpr double SCALE_BALLON = 2.0;
constexpr double SCALE_GRASS1 = 1.0;
constexpr double SCALE_TREE1 = 1.0;
constexpr double SCALE_TREE2 = 3.0;
constexpr double SCALE_TRAFFIC_LIGHT = 2.0;
constexpr double SCALE_ARROW1 = 3.0;

extern unique_ptr<vector<TextureRegion>> imgRedCar;
extern unique_ptr<vector<TextureRegion>> imgBlackCar;
extern unique_ptr<vector<TextureRegion>> imgBlueCar;
constexpr int CAR_DEFAULT = 0;
constexpr int CAR_LEFT1 = 1;
constexpr int CAR_LEFT2 = 2;
constexpr int CAR_RIGHT1 = 3;
constexpr int CAR_RIGHT2 = 4;
extern unique_ptr<vector<TextureRegion>> imgObjects;

// シェーダー
extern unique_ptr<VertexShader> vertexShader;
extern unique_ptr<PixelShader> pixelShader;

// コンスタントバッファー
struct TimeStruct {
	float time;
	//float padding[3];
};
extern ConstantBuffer<TimeStruct> cbTime;

struct RoadData {
	float start0;
	float start1;
	float start2;
	float curve0;
	float curve1;
	float curve2;
};

struct CameraData {
	float cameraH;
};
extern ConstantBuffer<CameraData> cbCamera;

extern ConstantBuffer<RoadData> cbRoad;

inline void load_font() {
	//fontA = std::make_unique<Font>(FontMethod::Bitmap, 12, FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"msgothic.ttc", FontStyle::Bitmap);
	fontDeb = std::make_unique<Font>(FontMethod::Bitmap, 23, U"font/PixelMplus12-Regular.ttf", FontStyle::Bitmap);
	fontA = std::make_unique<Font>(FontMethod::Bitmap, 32, U"font/PixelMplus12-Regular.ttf", FontStyle::Bitmap);
}

inline void load_image() {
	imgMountIwate.reset(new Texture(U"./img/mount_iwate.png"));
	//imgBallonR.reset(new Texture(U"./img/ballon01.png"));
	//imgTree01.reset(new Texture(U"./img/tree01.png"));
	//imgTree02.reset(new Texture(U"./img/tree02.png"));
	//imgGrass01.reset(new Texture(U"./img/grass01.png"));
	Texture imgAll(U"./img/DriveGame.png");
	imgObjects.reset(new vector<TextureRegion>());
	imgObjects->push_back(imgAll(0, 0, 64, 64));	// RedBallon
	imgObjects->push_back(imgAll(64, 0, 64, 64));	// YellowBallon
	imgObjects->push_back(imgAll(128, 0, 64, 64));	// BlueBallonk
	imgObjects->push_back(imgAll(192, 0, 64, 64));	// Grass01
	imgObjects->push_back(imgAll(0, 64, 32 * 2, 32 * 4));	// tree01
	imgObjects->push_back(imgAll(32 * 2, 64, 32 * 4, 32 * 4));	// tree02
	imgObjects->push_back(imgAll(0, 32 * 6, 32 * 3, 32 * 6));	// rafficLight
	imgObjects->push_back(imgAll(32 * 3, 32 * 6, 32 * 1, 32 * 1));	// Arrow1

	imgRedCar.reset(new vector<TextureRegion>());
	imgBlackCar.reset(new vector<TextureRegion>());
	imgBlueCar.reset(new vector<TextureRegion>());
	imgRedCar->push_back(imgAll(256 + 96 * 0, 256, 96, 96));
	imgRedCar->push_back(imgAll(256 + 96 * 1, 256, 96, 96));
	imgRedCar->push_back(imgAll(256 + 96 * 2, 256, 96, 96));
	imgRedCar->push_back(imgAll(256 + 96 * 3, 256, 96, 96));
	imgRedCar->push_back(imgAll(256 + 96 * 4, 256, 96, 96));
	imgBlackCar->push_back(imgAll(256 + 96 * 0, 256 - 96, 96, 96));
	imgBlackCar->push_back(imgAll(256 + 96 * 1, 256 - 96, 96, 96));
	imgBlackCar->push_back(imgAll(256 + 96 * 2, 256 - 96, 96, 96));
	imgBlackCar->push_back(imgAll(256 + 96 * 3, 256 - 96, 96, 96));
	imgBlackCar->push_back(imgAll(256 + 96 * 4, 256 - 96, 96, 96));
	imgBlueCar->push_back(imgAll(256 + 96 * 0, 256 + 96, 96, 96));
	imgBlueCar->push_back(imgAll(256 + 96 * 1, 256 + 96, 96, 96));
	imgBlueCar->push_back(imgAll(256 + 96 * 2, 256 + 96, 96, 96));
	imgBlueCar->push_back(imgAll(256 + 96 * 3, 256 + 96, 96, 96));
	imgBlueCar->push_back(imgAll(256 + 96 * 4, 256 + 96, 96, 96));
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
	//cbRoad->start = 1.0f;
	//cbRoad->end= 1.0f;
	//cbRoad->curve = 5.0f;
}

