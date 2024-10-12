#pragma once

#include "Global.h"
#include "Camera.hpp"

class Road : public Object {

	// 色
	static constexpr Color ROAD_COLOR{ 128, 128, 128 };
	static constexpr Color CENTER_LINE_COLOR{ 255, 255, 255 };
	static constexpr Color SIDE_LINE_COLOR_A{ 255, 80, 80};
	static constexpr Color SIDE_LINE_COLOR_B{ 80, 80, 255 };

	// 位置
	static constexpr int32 POS_X = 0;
	static constexpr int32 POS_Y = 0;

	// サイズ
	static constexpr int32 CENTER_LINE_WIDTH = 10;
	static constexpr int32 CENTER_LINE_SPAN = 5;
	static constexpr int32 SIDE_LINE_WIDTH = 15;
	static constexpr int32 SIDE_LINE_SPAN = 10;

	static constexpr int32 ROAD_WIDTH_A = 300;			// 道幅は途中で変更できるようにしたい

	static constexpr int32 FAR_Z = INT_MAX;

public:
	Road(const Camera& camera_) :
		Object({ 0.0, 0.0, INT_MAX }),
		camera(camera_),
		roadWidth(ROAD_WIDTH_A) {
	}

	double get_left_side_x() const {
		return POS_X - roadWidth / 2.0;
	}
	double get_right_side_x() const {
		return POS_X + roadWidth / 2.0;
	}

private:
	const Camera& camera;
	int32 roadWidth;

	bool update() override {
		return true;
	}

	void draw() const override {
		// 道路の頂点ワールド座標
		Vec3 roadNearL{ POS_X - roadWidth / 2.0, POS_Y, camera.get_z()};
		Vec3 roadNearR{ POS_X + roadWidth / 2.0, POS_Y, camera.get_z() };
		Vec3 roadFarL = roadNearL.movedBy(0.0, 0.0, FAR_Z);
		Vec3 roadFarR = roadNearR.movedBy(0.0, 0.0, FAR_Z);

		// 道路描画
		Quad{
			camera.world_pos_to_screen_pos(roadNearL),
			camera.world_pos_to_screen_pos(roadFarL),
			camera.world_pos_to_screen_pos(roadFarR),
			camera.world_pos_to_screen_pos(roadNearR)
		}.draw(ROAD_COLOR);

		// デバッグ用
		Circle{ camera.world_pos_to_screen_pos(roadNearL), 3 }.draw(Palette::Red);
		Circle{ camera.world_pos_to_screen_pos(roadNearR), 3 }.draw(Palette::Red);
		Circle{ camera.world_pos_to_screen_pos(roadFarL), 3 }.draw(Palette::Red);
		Circle{ camera.world_pos_to_screen_pos(roadFarR), 3 }.draw(Palette::Red);
	}

	Vec2 pos2() const { return Vec2(pos.x, pos.y); }
	Point pos2I() const { return Point((int32)pos.x, (int32)pos.y); }
};
