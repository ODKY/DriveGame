#pragma once

#include "Global.h"

class Obj : public Object {

public:
	Obj(const Vec3& pos_, const Texture& img_) :
		Object(pos_),
		img(img_) {}

private:
	const Texture& img;

	bool update() override {
		return true;
	}

	void draw() const override {
		auto posFromCamera = world_pos_to_camera_pos(pos);
		if (posFromCamera.z > 2.0) {
			Point posDrawing = camera_pos_to_screen_pos(posFromCamera);
			img.scaled(calc_scale(posFromCamera)).drawAt(posDrawing);
		}
		Print << U"OBJECT: " << posFromCamera;
	}

	Vec2 pos2() const { return Vec2(pos.x, pos.y); }
	Point pos2I() const { return Point((int32)pos.x, (int32)pos.y); }
};
