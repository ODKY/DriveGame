#pragma once

#include "Global.h"
#include "Camera.hpp"
#include "Road.h"
//#include "GS_Drive.hpp"

class Player : public Object {

	static constexpr Vec3 POS{ 0.0 , 0.0, 1.0 };
	static constexpr Size IMG_SIZE{ 96, 96 };

	static constexpr double MAX_SPEED_Z = 0.21;
	static constexpr double DEACC_BORDER = 0.03;
	static constexpr double CREEP_BORDER = 0.01;
	static constexpr double DIRT_BORDER = 0.09;
	static constexpr double ACC = 0.045;
	static constexpr double DEACC = -0.02;
	static constexpr double BRAKE = -0.2;
	static constexpr double DIRT = -0.07;
	static constexpr double XSPEED = 300;

	static constexpr Size HIT_BOX_SIZE{ 84, 35 };

public:
	Player(const vector<TextureRegion>& imgs_, Camera& camera_, const Road& road_) :
		Object(POS),
		imgs(imgs_),
		scale(1.0),
		camera(camera_),
		road(road_),
		imgIdx(CAR_DEFAULT),
		offset(),
		velocity(),
		hitbox(HIT_BOX_SIZE),
		time(0.0) {
	}

	const Vec2& get_velocity() const {
		return velocity;
	}

	const Vec3& get_pos() const {
		return pos;
	}

	const Rect& get_hit_box() const {
		return hitbox;
	}

private:
	const vector<TextureRegion>& imgs;
	const double scale;
	Camera& camera;
	const Road& road;
	int imgIdx;
	Vec2 offset;
	Vec2 velocity;
	Rect hitbox;
	double time;

	bool update() override {

		int32 direction = 0;
		offset = { 0.0, 0.0 };

		// 入力を取得
		auto gamepad = Gamepad(0);
		bool buttonA = gamepad.buttons.at(0).pressed();
		bool buttonB = gamepad.buttons.at(2).pressed();
		bool buttonStart = gamepad.buttons.at(7).pressed();
		double inputX = gamepad.axes[0];
		double inputY = gamepad.axes[1];
		if (inputX > -0.5 && inputX < 0.5)
			inputX = 0;
		if (inputY > -0.5 && inputY < 0.5)
			inputY = 0;
		//Print << U"A: " << buttonA;
		//Print << U"B: " << buttonB;
		//Print << U"XY: " << Vec2(inputX, inputY);

		if (buttonStart) {
			isStart = false;
			countDown = 4.5;
			aaa = false;
			isGoal = false;
			gsm->reserve_transition(*this, nullptr, true);
			return true;
		}

		// いろいろ
		if (!isStart && aaa)
			return true;
		aaa = true;

		if (pos.z > 600) {
			isGoal = true;
		}

		if (isGoal)
			return true;

		if (!isGoal)
			time += Scene::DeltaTime();

		// 速度算出
		if (inputX > 0.5) {
			velocity.x = XSPEED * Scene::DeltaTime();
			++direction;
		}
		else if (inputX < -0.5) {
			velocity.x = -XSPEED * Scene::DeltaTime();
			--direction;
		}
		if (buttonA) {
			velocity.y += ACC * Scene::DeltaTime();
		}
		if (buttonB) {
			velocity.y += BRAKE * Scene::DeltaTime();
		}

		// 減速
		if (velocity.y > DEACC_BORDER)
			velocity.y += DEACC * Scene::DeltaTime();

		// クリープ現象
		if (velocity.y < CREEP_BORDER)
			velocity.y += ACC * Scene::DeltaTime() / 10;

		// ダート
		if (pos.x + hitbox.w / 2 > road.get_right_side_x() || pos.x - hitbox.w / 2 < road.get_left_side_x())
			if (velocity.y > DIRT_BORDER)
				velocity.y += DIRT * Scene::DeltaTime();

		// 速度調整
		if (velocity.y > MAX_SPEED_Z)
			velocity.y = MAX_SPEED_Z;
		if (velocity.y < 0)
			velocity.y = 0;

		// カーブ情報
		double curve = 0;
		double zFromCamera = camera.world_pos_to_camera_pos(pos).z;
		if (zFromCamera < cbRoad->start1)
			curve = cbRoad->curve0;
		else
			curve = cbRoad->curve1;

		if (curve > 0.01)
			++direction;
		else if (curve < -0.01)
			--direction;
		Print << U"PLAYER_POS : " << pos;
		Print << U"NOW CURVE : " << curve;

		if (isHit) {
			velocity.x = 0;
			velocity.y = 0;
		}

		// 移動
		pos.x += velocity.x;
		pos.z += velocity.y;
		curveAmount += velocity.y * camera.get_cameras_curve() * Scene::DeltaTime();

		// カーブの膨らみ
		pos.x += velocity.y * 130000 * -curve * Scene::DeltaTime();

		Print << U"velocity : " << velocity;
		int32 v = (int32)(velocity.y * 1000);
		Print << U"速度 : " << v << U"Km/h";
		velocity.x = 0;

		direction += debug_control();

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
		if (!isGoal)
			camera.set_pos({ pos.x, pos.y - Camera::HEIGHT, pos.z - 0.7 });

		cbCamera->cameraH = -camera.get_y();

		// 当たり判定の位置調整
		auto posS = camera.world_pos_to_screen_pos(pos);
		hitbox.pos = { (int32)posS.x - hitbox.w / 2, (int32)posS.y - 90 };

		return true;
	}

	void draw() const override {
		// スタート
		if (!isStart) {
			if (KeyR.pressed())
				isStart = true;
			countDown -= Scene::DeltaTime();
			if (countDown >= 3.4) {
				//(*fontA)(U"3").drawAt(100, Scene::Center() + Point{ 3, 3 }, Palette::Black);
				//(*fontA)(U"3").drawAt(100, Scene::Center(), Palette::Yellow);
			}
			else if (countDown >= 0.5) {
				(*fontA)(U"{:.0f}"_fmt(countDown)).drawAt(100, Scene::Center() + Point{ 3, 3 }, Palette::Black);
				(*fontA)(U"{:.0f}"_fmt(countDown)).drawAt(100, Scene::Center(), Palette::Yellow);
			}
			else if (countDown >= -0.5) {
				(*fontA)(U"GO").drawAt(100, Scene::Center() + Point{ 3, 3 }, Palette::Black);
				(*fontA)(U"GO").drawAt(100, Scene::Center(), Palette::Yellow);
			}
			else isStart = true;
		}
		if (isGoal) {
			(*fontA)(U"GOAL").drawAt(100, Scene::Center() + Point(0, -70) + Point{3, 3}, Palette::Black);
			(*fontA)(U"GOAL").drawAt(100, Scene::Center() + Point(0, -70), Palette::Yellow);
			(*fontA)(U"{:.2f}s"_fmt(time)).drawAt(100, Scene::Center() + Point(0, 40) + Point{ 3, 3 }, Palette::Black);
			(*fontA)(U"{:.2f}s"_fmt(time)).drawAt(100, Scene::Center() + Point(0, 40), Palette::Yellow);
		}

		const TextureRegion& img = imgs.at(imgIdx);
		Print << offset;

		// 速度出力
		int32 v = (int32)(velocity.y * 1000);
		Point vPos{ 470, 13 };
		Rect{ vPos.x - 15, vPos.y - 5, 160, 55 }.draw({ 0, 0, 0, 0.4}).drawFrame(3, Palette::Black);
		(*fontA)(U"{:0>3}Km/h"_fmt(v)).draw(vPos + Point{ 3, 3 }, Palette::Black);
		(*fontA)(U"{:0>3}Km/h"_fmt(v)).draw(vPos, Palette::Yellow);

		// 走行距離出力
		int32 d = (int32)(get_z() * 2) - 2;
		Point dPos{ 80, 13 };
		Rect{ dPos.x - 15, dPos.y - 5, 125, 55 }.draw({ 0, 0, 0, 0.4 }).drawFrame(3, Palette::Black);
		(*fontA)(U"{:0>4}m"_fmt(d)).draw(dPos + Point{ 3, 3 }, Palette::Black);
		(*fontA)(U"{:0>4}m"_fmt(d)).draw(dPos, Palette::Yellow);

		// タイム出力
		Point tPos{ 260, 13 };
		Rect{ tPos.x - 15, tPos.y - 5, 160, 55 }.draw({ 0, 0, 0, 0.4 }).drawFrame(3, Palette::Black);
		if (time < 10.0) {
			(*fontA)(U"00{:0>3.2f}s"_fmt(time)).draw(tPos + Point{ 3, 3 }, Palette::Black);
			(*fontA)(U"00{:0>3.2f}s"_fmt(time)).draw(tPos, Palette::Yellow);
		}
		else if (time < 100.0) {
			(*fontA)(U"0{:0>3.2f}s"_fmt(time)).draw(tPos + Point{ 3, 3 }, Palette::Black);
			(*fontA)(U"0{:0>3.2f}s"_fmt(time)).draw(tPos, Palette::Yellow);
		}
		else {
			(*fontA)(U"{:0>3.2f}s"_fmt(time)).draw(tPos + Point{ 3, 3 }, Palette::Black);
			(*fontA)(U"{:0>3.2f}s"_fmt(time)).draw(tPos, Palette::Yellow);
		}

		// 車描画
		camera.draw_object(pos.movedBy(offset.x, -IMG_SIZE.y * scale / 2.0 + ALL_OFFSET_Y, 0.0), img, scale);
		if (isDisplayHitBox)
			hitbox.draw(Color(0, 255, 0, 128));
		//auto posS = camera.world_pos_to_screen_pos(pos);
		//Rect{ Arg::center((int32)posS.x, (int32)posS.y - 80), HIT_BOX_SIZE }.draw(Color(0, 255, 0, 128));
	}

	int debug_control() {

		// 移動
		int direction = 0;
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

		if (KeyW.pressed()) {
			double deltaZ = (speed / 10 * Scene::DeltaTime());
			pos.z += deltaZ;
			curveAmount += deltaZ * camera.get_cameras_curve();

		}
		if (KeyQ.pressed())
			pos.z += (-speed / 10 * Scene::DeltaTime());
		if (KeyS.pressed()) {
			double deltaZ = (speed / 50 * Scene::DeltaTime());
			pos.z += deltaZ;
			curveAmount += deltaZ * camera.get_cameras_curve();
		}
		if (KeyA.pressed())
			pos.z += (-speed / 50 * Scene::DeltaTime());
		if (KeyX.pressed()) {
			double deltaZ = (speed / 250 * Scene::DeltaTime());
			pos.z += deltaZ;
			curveAmount += deltaZ * camera.get_cameras_curve();
		}
		if (KeyZ.pressed())
			pos.z += (-speed / 250 * Scene::DeltaTime());

		return direction;
	}
};
