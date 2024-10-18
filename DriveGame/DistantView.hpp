#pragma once

#include "Global.h"
#include "Camera.hpp"

class DistantView : public Object {

	static constexpr Color SKY_COLOR_TOP{ 0, 70, 255 };
	static constexpr Color SKY_COLOR_BOTTOM{ 15, 120, 255 };

public:
	DistantView(const Texture& img_, const Camera& camera_) :
		Object({ 0.0, 0.0, INT_MAX -1 }),
		img(img_),
		camera(camera_) {}

private:
	const Texture& img;
	const Camera& camera;

	bool update() override {
		return true;
	}

	void draw() const override {
		Rect{ 0, 0, SCREEN_W, SCREEN_H / 2 }.draw(
			Arg::top = SKY_COLOR_TOP, Arg::bottom = SKY_COLOR_BOTTOM
		);
		img.scaled((SCREEN_H / 2.0) / img.height()).draw(-230, ALL_OFFSET_Y);
	}

	Vec2 pos2() const { return Vec2(pos.x, pos.y); }
	Point pos2I() const { return Point((int32)pos.x, (int32)pos.y); }
};
