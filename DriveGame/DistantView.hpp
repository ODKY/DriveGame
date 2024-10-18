#pragma once

#include "Global.h"
#include "Camera.hpp"

class DistantView : public Object {

	static constexpr Color SKY_COLOR_TOP{ 0, 70, 255 };
	static constexpr Color SKY_COLOR_BOTTOM{ 15, 120, 255 };

	static constexpr double DEFAULT_X = -900.0;

public:
	DistantView(const Texture& img_, const Camera& camera_) :
		Object({ DEFAULT_X, 0.0, INT_MAX -1 }),
		img(img_),
		camera(camera_) {}

private:
	const Texture& img;
	const Camera& camera;

	bool update() override {
		pos.x = DEFAULT_X - curveAmount * 73000;
		return true;
	}

	// すげー雑だけど、時間ないから3つ並べて描画します
	// 左右に見切れたときの対策
	// これだけ置いておけばまず大丈夫なはず
	void draw() const override {
		Rect{ 0, 0, SCREEN_W, SCREEN_H / 2 }.draw(
			Arg::top = SKY_COLOR_TOP, Arg::bottom = SKY_COLOR_BOTTOM
		);
		double scale = (SCREEN_H / 2.0) / img.height();
		img.scaled(scale).draw(pos.x, ALL_OFFSET_Y);
		img.scaled(scale).draw(pos.x - img.width() * scale, ALL_OFFSET_Y);
		img.scaled(scale).draw(pos.x + img.width() * scale, ALL_OFFSET_Y);
	}

	Vec2 pos2() const { return Vec2(pos.x, pos.y); }
	Point pos2I() const { return Point((int32)pos.x, (int32)pos.y); }
};
