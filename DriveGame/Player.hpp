#pragma once

#include "Global.h"
#include "Camera.hpp"

class Player : public Object {

	static constexpr Vec3 POS{ 0.0 , 0.0, 1.0 };
	static constexpr Size IMG_SIZE{ 96, 96 };

public:
	Player(const vector<TextureRegion>& imgs_, Camera& camera_) :
		Object(POS),
		imgs(imgs_),
		scale(1.0),
		camera(camera_),
		imgIdx(CAR_DEFAULT),
		offset() {}

private:
	const vector<TextureRegion>& imgs;
	const double scale;
	Camera& camera;
	int imgIdx;
	Vec2 offset;

	bool update() override {
		int32 direction = 0;
		offset = { 0.0, 0.0 };

		// 移動
		double speed = 400.0;
		if (KeyUp.pressed())
			pos.y += (-speed * 3 * Scene::DeltaTime());
		if (KeyRight.pressed()) {
			pos.x += (speed * Scene::DeltaTime());
			++direction;
		}
		if (KeyDown.pressed())
			pos.y += (speed * 3 * Scene::DeltaTime());
		if (KeyLeft.pressed()) {
			pos.x += (-speed * Scene::DeltaTime());
			--direction;
		}

		if (KeyW.pressed())
			pos.z += (speed / 10 * Scene::DeltaTime());
		if (KeyQ.pressed())
			pos.z += (-speed / 10 * Scene::DeltaTime());
		if (KeyS.pressed())
			pos.z += (speed / 50 * Scene::DeltaTime());
		if (KeyA.pressed())
			pos.z += (-speed / 50 * Scene::DeltaTime());
		if (KeyX.pressed())
			pos.z += (speed / 250 * Scene::DeltaTime());
		if (KeyZ.pressed())
			pos.z += (-speed / 250 * Scene::DeltaTime());

		// カーブにいるかどうか
		double curve = 0;
		double zFromCamera = camera.world_pos_to_camera_pos(pos).z;
		if (zFromCamera < cbRoad->start1)
			curve = cbRoad->curve0;

		if (curve > 0.01)
			++direction;
		else if (curve < -0.01)
			--direction;
		Print << U"PLAYER_POS : " << pos;
		Print << U"NOW CURVE : " << curve;

		// 向きに応じた絵の選択
		switch (direction) {
		case 0:
			imgIdx = CAR_DEFAULT;
			break;
		case 1:
			imgIdx = CAR_RIGHT1;
			offset = { -8.0, 0.0 };
			break;
		case 2:
			imgIdx = CAR_RIGHT2;
			offset = { -12.0, 0.0 };
			break;
		case -1:
			imgIdx = CAR_LEFT1;
			offset = { 8.0, 0.0 };
			break;
		case -2:
			imgIdx = CAR_LEFT2;
			offset = { 12.0, 0.0 };
			break;
		default:
			break;
		}

		// カメラを自分の後ろに配置
		camera.set_pos({ pos.x, pos.y - Camera::HEIGHT, pos.z - 0.7 });

		return true;
	}

	void draw() const override {
		const TextureRegion& img = imgs.at(imgIdx);
		Print << offset;
		camera.draw_object(pos.movedBy(offset.x, -IMG_SIZE.y * scale / 2.0 + ALL_OFFSET_Y, 0.0), img, scale);
	}
};
