#pragma once

#include "Global.h"
#include "Camera.hpp"

class Obj : public Object {

public:
	Obj(const Vec3& pos_, const Texture& img_, const Camera& camera_) :
		Object(pos_),
		img(img_),
		camera(camera_) {}

private:
	const Texture& img;
	const Camera& camera;

	bool update() override {
		return true;
	}

	void draw() const override {
		camera.draw_object(pos.movedBy(0.0, -img.height() / 2.0, 0.0), img);

		//if (camera.in_viewport(camera.world_pos_to_camera_pos(pos)))
		//	Circle(camera.world_pos_to_screen_pos(pos), 3).draw(Palette::Cyan);
	}

	Vec2 pos2() const { return Vec2(pos.x, pos.y); }
	Point pos2I() const { return Point((int32)pos.x, (int32)pos.y); }
};
