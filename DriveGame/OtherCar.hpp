#pragma once

#include "Global.h"
#include "Camera.hpp"
#include "Player.hpp"

class OtherCar : public Object {

	static constexpr double SPEED = 4.2;

public:
	OtherCar(const Vec3& pos_, const vector<TextureRegion>& imgs_, const Camera& camera_, const Player& player_) :
		Object(pos_),
		imgs(imgs_),
		scale(1.0),
		camera(camera_),
		player(player_),
		offsetX(0),
		imgIdx(CAR_DEFAULT) {}

private:
	const vector<TextureRegion>& imgs;
	const double scale;
	const Camera& camera;
	const Player& player;
	double offsetX;
	int imgIdx;

	bool update() override {
		pos.z += SPEED * Scene::DeltaTime();
		Print << SPEED * Scene::DeltaTime() * 1000;

		int direction = 0;

		// プレイヤーとの位置関係
		if (pos.x < player.get_pos().x)
			++direction;
		else
			--direction;

		// カーブ情報
		double curve = 0;
		double zFromCamera = camera.world_pos_to_camera_pos(pos).z;
		if (zFromCamera < cbRoad->start1)
			curve = cbRoad->curve0;
		else if (zFromCamera < cbRoad->start2)
			curve = cbRoad->curve1;
		else
			curve = cbRoad->curve2;

		if (curve > 0.01)
			++direction;
		else if (curve < -0.01)
			--direction;

		// 向きに応じた絵の選択
		switch (direction) {
		case 0:
			imgIdx = CAR_DEFAULT;
			break;
		case 1:
			imgIdx = CAR_RIGHT1;
			offsetX = -8.0;
			break;
		case 2:
			imgIdx = CAR_RIGHT2;
			offsetX = -12.0;
			break;
		case -1:
			imgIdx = CAR_LEFT1;
			offsetX = 8.0;
			break;
		case -2:
			imgIdx = CAR_LEFT2;
			offsetX = 12.0;
			break;
		default:
			break;
		}

		if (pos.z < camera.get_z() || pos.z > camera.get_z() + 100)
			return false;
		return true;
	}

	void draw() const override {
		double deltaX = camera.calc_delta_x(pos);
		//int imgIdx = CAR_DEFAULT;
		camera.draw_object(pos.movedBy(offsetX, -imgs.at(imgIdx).size.y * scale / 2.0 + ALL_OFFSET_Y, 0.0), imgs.at(imgIdx), scale, {deltaX, 0.0});

		//Line(0, 245, 1000, 245).draw(Palette::Red);
		//if (camera.in_viewport(camera.world_pos_to_camera_pos(pos)))
		//	Circle(camera.world_pos_to_screen_pos(pos), 3).draw(Palette::Cyan);
	}



	Vec2 pos2() const { return Vec2(pos.x, pos.y); }
	Point pos2I() const { return Point((int32)pos.x, (int32)pos.y); }
};
