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
		// ワールド座標は Y=0 とした上でカーブの影響を計算する
		const Vec3 posC = camera.world_pos_to_camera_pos({ pos.x, 0, pos.z });
		const Vec2 posS = camera.camera_pos_to_screen_pos(posC);
		const Vec2 borderPosS = camera.camera_pos_to_screen_pos({ 0.0, -camera.get_y(), cbRoad->start1 });
		const Vec2 border2PosS = camera.camera_pos_to_screen_pos({ 0.0, -camera.get_y(), cbRoad->start2 });
		const double curve0 = cbRoad->curve0;
		const double curve1 = cbRoad->curve1;
		const double curve2 = cbRoad->curve2;

		//if (get_id() > 500) {
		//	Print << U"posC.z : " << posC.z;
		//}
		double deltaX = 0.0;
		if (posC.z < cbRoad->start1) {
			// 1つ目のカーブ境界より前
			deltaX = curve0 * posC.z;
		}
		else if (posC.z < cbRoad->start2) {
			// 2つ目のカーブ境界より前
			const double screenHeightHalf = SCREEN_H / 2;
			const double YfromB = SCREEN_H - posS.y;
			const double borderYfromB = SCREEN_H - borderPosS.y;
			double rate = (YfromB - borderYfromB) / (screenHeightHalf - borderYfromB);
			rate = pow(rate, 2.0);
			//Print << rate;
			//if (rate < 0.0)
			//	rate = 0.0;
			//if (rate > 1.0)
			//	rate = 1.0;
			if (cbRoad->start1 < 0.0)
				rate = 1;

			deltaX = (curve1 * posC.z * rate) + (curve0 * posC.z * (1.0 - rate));
			//if (get_id() > 500)
			//	Print << rate;
		}
		else {
			// 2つ目のカーブ境界以降
			const double screenHeightHalf = SCREEN_H / 2;
			const double YfromB = SCREEN_H - posS.y;
			const double border1YfromB = SCREEN_H - borderPosS.y;
			const double border2YfromB = SCREEN_H - border2PosS.y;
			const double border2YfromBorder1 = border2PosS.y - borderPosS.y;

			double rate1 = (YfromB - border1YfromB) / (screenHeightHalf - border1YfromB);
			rate1 = pow(rate1, 2.0);

			double rate2 = (YfromB - border2YfromB) / (screenHeightHalf - border2YfromB);
			rate2 = pow(rate2, 2.0);

			if (cbRoad->start2 < 0.0) {
				rate2 = 1;
				rate1 = 0;
			}
			
			double deltaX1 = (curve1 * posC.z * rate1) + (curve0 * posC.z * (1.0 - rate1));
			deltaX = (curve2 * posC.z * rate2) + (deltaX1 * (1 - rate2));
		}
		deltaX *= RENDER_TEXTURE_W;
		camera.draw_object(pos.movedBy(0.0, -img.size.y * scale / 2.0 + ALL_OFFSET_Y, 0.0), img, scale, { deltaX, 0.0 });
		

		//Line(0, 245, 1000, 245).draw(Palette::Red);
		//if (camera.in_viewport(camera.world_pos_to_camera_pos(pos)))
		//	Circle(camera.world_pos_to_screen_pos(pos), 3).draw(Palette::Cyan);
	}

	Vec2 pos2() const { return Vec2(pos.x, pos.y); }
	Point pos2I() const { return Point((int32)pos.x, (int32)pos.y); }
};
