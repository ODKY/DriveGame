#pragma once

#include "Global.h"
#include "Camera.hpp"

class Obj : public Object {

public:
	Obj(const Vec3& pos_, const Texture& img_, const Camera& camera_, const double scale_ = 1.0) :
		Object(pos_),
		img(img_),
		scale(scale_),
		camera(camera_) {}

private:
	const Texture& img;
	const double scale;
	const Camera& camera;

	bool update() override {
		return true;
	}

	void draw() const override {
		const Vec3 posC = camera.world_pos_to_camera_pos(pos);
		const Vec2 posS = camera.camera_pos_to_screen_pos(posC);
		const Vec2 borderPosS = camera.camera_pos_to_screen_pos({ 0.0, -camera.get_y(), cbRoad->start1 });
		const double curve0 = cbRoad->curve0;
		const double curve1 = cbRoad->curve1;

		double deltaX = 0.0;
		if (posC.z < cbRoad->start1) {
			deltaX = curve0 * posC.z;
		}
		else {
			const double screenBottomYG = SCREEN_H / 2;
			//const double posSG_y = posS.y - SCREEN_H / 2;
			//const double borderPosSG_y = borderPosS.y - SCREEN_H / 2;
			double rate = ((SCREEN_H - posS.y) - (SCREEN_H - borderPosS.y)) / (screenBottomYG - (SCREEN_H - borderPosS.y));
			rate = pow(rate, 2.0);
			//Print << rate;
			if (rate < 0.0)
				rate = 0.0;
			if (rate > 1.0)
				rate = 1.0;
			if (cbRoad->start1 < 0.0)
				rate = 1;

			deltaX = (curve1 * posC.z * rate) + (curve0 * posC.z * (1.0 - rate));
			if (get_id() > 500)
				Print << rate;
		}
		deltaX *= RENDER_TEXTURE_W;
		camera.draw_object(pos.movedBy(0.0, -img.height() * scale / 2.0 + ALL_OFFSET_Y, 0.0), img, scale, { deltaX, 0.0 });

		Line(0, 245, 1000, 245).draw(Palette::Red);
		//if (camera.in_viewport(camera.world_pos_to_camera_pos(pos)))
		//	Circle(camera.world_pos_to_screen_pos(pos), 3).draw(Palette::Cyan);
	}

	Vec2 pos2() const { return Vec2(pos.x, pos.y); }
	Point pos2I() const { return Point((int32)pos.x, (int32)pos.y); }
};
