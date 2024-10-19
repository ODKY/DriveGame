#pragma once

#include "Global.h"
#include "Camera.hpp"
//#include "Player.hpp"

class Player;

class OtherCar : public Object {

	static constexpr double SPEED = 4.2;

	static constexpr Size HIT_BOX_SIZE{ 84, 3 };

public:
	OtherCar(const Vec3& pos_, const vector<TextureRegion>& imgs_, const Camera& camera_, const Player& player_) :
		Object(pos_),
		imgs(imgs_),
		scale(1.0),
		camera(camera_),
		player(player_),
		offsetX(0),
		imgIdx(CAR_DEFAULT),
		hitbox(HIT_BOX_SIZE) {}

private:
	const vector<TextureRegion>& imgs;
	const double scale;
	const Camera& camera;
	const Player& player;
	double offsetX;
	int imgIdx;
	Rect hitbox;

	bool update() override;

	void draw() const override {
		double deltaX = camera.calc_delta_x(pos);
		//int imgIdx = CAR_DEFAULT;
		camera.draw_object(pos.movedBy(offsetX, -imgs.at(imgIdx).size.y * scale / 2.0 + ALL_OFFSET_Y, 0.0), imgs.at(imgIdx), scale, {deltaX, 0.0});
		if (isDisplayHitBox)
			hitbox.draw(Color(255, 0, 0, 128));

		//Line(0, 245, 1000, 245).draw(Palette::Red);
		//if (camera.in_viewport(camera.world_pos_to_camera_pos(pos)))
		//	Circle(camera.world_pos_to_screen_pos(pos), 3).draw(Palette::Cyan);
	}



	Vec2 pos2() const { return Vec2(pos.x, pos.y); }
	Point pos2I() const { return Point((int32)pos.x, (int32)pos.y); }
};
