#pragma once

#include <Siv3D.hpp> // Siv3D v0.6.14
#include <random>
#include "sgmPlus.h"

constexpr int32 SCREEN_W = 640;
constexpr int32 SCREEN_H = 480;

// 乱数生成器
std::random_device seedGenerator;
std::mt19937 random(seedGenerator());

// フォント
unique_ptr<Font> fontA;
unique_ptr<Font> fontDeb;

using namespace SGM2;
using namespace SGM2::Plus;

unique_ptr<GameStateManager> gsm;

inline void set_font() {
	//fontA = std::make_unique<Font>(FontMethod::Bitmap, 12, FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"msgothic.ttc", FontStyle::Bitmap);
	fontDeb = std::make_unique<Font>(FontMethod::Bitmap, 23, U"font/PixelMplus12-Regular.ttf", FontStyle::Bitmap);
	fontA = std::make_unique<Font>(FontMethod::Bitmap, 32, U"font/PixelMplus12-Regular.ttf", FontStyle::Bitmap);
}
