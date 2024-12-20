﻿#include "GS_Drive.hpp"
#include "Player.hpp"

GS_Drive::GS_Drive() :
	camera({ 0.0, -145.0, 0.0 }, { SCREEN_W, SCREEN_H }),
	stageData(),
	road(*add_object(new Road(camera, stageData.get_lane_num(STAGE_ID)))),
	player(*add_object(new Player(*imgBlackCar, camera, road))),
	objectData(),
	objIdx(0),
	otherCarGauge(0) {

	perform_z_sort_every_frame(true);

	road.set_curve_pos(stageData.get_curve_data(STAGE_ID));
	objectData = stageData.get_object_data(STAGE_ID);

	add_object(new DistantView(*imgMountIwate, camera));
}

void GS_Drive::update() {

	// オブジェクトデータを参照し、一定距離内に入ったオブジェクトを実体化
	while (1) {
		if (objIdx < (int)objectData.size() && objectData.at(objIdx).pos.z < camera.get_z() + Camera::FAR_PLANE) {
			const auto& obj = objectData.at(objIdx);
			add_object(new Obj(obj.pos, imgObjects->at(obj.imgIdx), camera, obj.scale, obj.boxSize, obj.boxOffset, player));
			++objIdx;
		}
		else
			break;
	}

	// 他車の生成
	otherCarGauge += player.get_velocity().y;
	if (otherCarGauge > 10) {
		otherCarGauge -= 10;
		if (random() % 3 == 0) {
			int lane = random() % stageData.get_lane_num(STAGE_ID);
			double x = road.get_left_side_x() + Road::ALL_PADDING + lane * Road::LANE_WIDTH + Road::LANE_WIDTH / 2;
			if (random() % 2 == 0)
				add_object(new OtherCar(Vec3{ x, 0, camera.get_z() + Camera::FAR_PLANE }, *imgRedCar, camera, player));
			else
				add_object(new OtherCar(Vec3{ x, 0, camera.get_z() + Camera::FAR_PLANE }, *imgBlueCar, camera, player));
		}
	}

	// デバッグ用
	if (KeyT.down())
		isDisplayHitBox = !isDisplayHitBox;
}
