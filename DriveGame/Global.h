#pragma once

#include <Siv3D.hpp> // Siv3D v0.6.14
#include <random>
#include "sgmPlus.h"

using namespace SGM2;
using namespace SGM2::Plus;

constexpr int32 SCREEN_W = 640;
constexpr int32 SCREEN_H = 480;
//constexpr Point SCREEN_CENTER = { SCREEN_W / 2, SCREEN_H / 2 };

// 乱数生成器
std::random_device seedGenerator;
std::mt19937 random(seedGenerator());

// フォント
unique_ptr<Font> fontA;
unique_ptr<Font> fontDeb;

// ゲームステートマネージャー
unique_ptr<GameStateManager> gsm;

// 画像
unique_ptr<Texture> imgBallonR;
unique_ptr<Texture> imgTree01;

// カメラ位置
//Vec3 cameraPos = DEFAULT_CAMERA_POS_IN_WORLD;

inline void load_font() {
	//fontA = std::make_unique<Font>(FontMethod::Bitmap, 12, FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"msgothic.ttc", FontStyle::Bitmap);
	fontDeb = std::make_unique<Font>(FontMethod::Bitmap, 23, U"font/PixelMplus12-Regular.ttf", FontStyle::Bitmap);
	fontA = std::make_unique<Font>(FontMethod::Bitmap, 32, U"font/PixelMplus12-Regular.ttf", FontStyle::Bitmap);
}

inline void load_image() {
	imgBallonR.reset(new Texture(U"./img/ballon01.png"));
	imgTree01.reset(new Texture(U"./img/tree01.png"));
}

//// 座標変換
//// ワールド座標系 -> カメラ座標系
//inline Vec3 world_pos_to_camera_pos(const Vec3& posInWorld) {
//	return posInWorld - cameraPos;
//}
//
//// 座標変換
//// カメラ座標系 -> デバイス座標系
//inline Point camera_pos_to_screen_pos(const Vec3& cameraToObject) {
//	return Point {
//		(int32)(cameraToObject.x / cameraToObject.z) + SCREEN_CENTER.x,
//		(int32)(cameraToObject.y / cameraToObject.z) + SCREEN_CENTER.y,
//	};
//}
//
//// 拡大率取得
//inline double calc_scale(const Vec3& cameraToObject) {
//	return 3.0 / cameraToObject.z;
//}
