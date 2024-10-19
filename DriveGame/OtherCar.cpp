#include "OtherCar.hpp"
#include "Player.hpp"

bool OtherCar::update() {
	pos.z += SPEED * Scene::DeltaTime();

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

	// 当たり判定の位置調整
	int32 deltaX = camera.calc_delta_x(pos);
	auto posS = camera.world_pos_to_screen_pos(pos);
	hitbox.pos = { (int32)posS.x - hitbox.w / 2 + deltaX, (int32)posS.y - 20 };

	// 当たり判定
	if (hitbox.intersects(player.get_hit_box()))
		isHit = true;

	if (pos.z < camera.get_z() || pos.z > camera.get_z() + 100)
		return false;
	return true;
}
