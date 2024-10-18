#pragma once

#include "Global.h"
#include "Camera.hpp"

class Obj : public Object {

public:
	Obj(const Vec3& pos_, const TextureRegion& img_, const Camera& camera_, const double scale_ = 1.0) :
		Object(pos_),
		img(img_),
		scale(scale_),
		camera(camera_) {}

private:
	const TextureRegion& img;
	const double scale;
	const Camera& camera;

	bool update() override {
		if (pos.z < camera.get_z())
			return false;
			//pos.z += 50;
		return true;
	}

	void draw() const override {
		double deltaX = camera.calc_delta_x(pos);
		camera.draw_object(pos.movedBy(0.0, -img.size.y * scale / 2.0 + ALL_OFFSET_Y, 0.0), img, scale, { deltaX, 0.0 });
		
		//Line(0, 245, 1000, 245).draw(Palette::Red);
		//if (camera.in_viewport(camera.world_pos_to_camera_pos(pos)))
		//	Circle(camera.world_pos_to_screen_pos(pos), 3).draw(Palette::Cyan);
	}

	

	Vec2 pos2() const { return Vec2(pos.x, pos.y); }
	Point pos2I() const { return Point((int32)pos.x, (int32)pos.y); }
};
