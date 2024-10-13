#pragma once

#include "Global.h"
#include "Camera.hpp"

class Road : public Object {

	// 色
	static constexpr Color ROAD_COLOR{ 128, 128, 128 };

	static constexpr Color CENTER_LINE_COLOR{ 255, 255, 255 };

	static constexpr Color SIDE_LINE_IN_COLOR_A{ 100, 100, 100 };//{ 255, 80, 80};
	static constexpr Color SIDE_LINE_IN_COLOR_B{ 255, 255, 255 };//{ 80, 80, 255 };

	static constexpr Color SIDE_LINE_OUT_COLOR{ 255, 255, 255 };

	// 位置
	static constexpr int32 POS_X = 0;
	static constexpr int32 POS_Y = 0;

	// サイズ
	static constexpr int32 CENTER_LINE_WIDTH = 18;
	static constexpr double CENTER_LINE_LENGTH = 0.7;
	static constexpr int32 CENTER_LINE_NUM = 18;

	static constexpr int32 SIDE_LINE_IN_WIDTH = 23;
	static constexpr double SIDE_LINE_IN_LENGTH = 0.3;
	static constexpr int32 SIDE_LINE_IN_PADDING = 30;
	static constexpr int32 SIDE_LINE_IN_NUM = 25;

	static constexpr int32 SIDE_LINE_OUT_WIDTH = 15;

	static constexpr int32 ROAD_WIDTH_A = 600;			// 道幅は途中で変更できるようにしたい

	static constexpr int32 FAR_Z = INT_MAX;

public:
	Road(const Camera& camera_) :
		Object({ 0.0, 0.0, INT_MAX - 1 }),
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
		// 道路
		Vec3 roadNearL{ POS_X - roadWidth / 2.0, POS_Y, camera.get_z()};
		Vec3 roadNearR{ POS_X + roadWidth / 2.0, POS_Y, camera.get_z() };
		Vec3 roadFarL = roadNearL.movedBy(0.0, 0.0, FAR_Z);
		Vec3 roadFarR = roadNearR.movedBy(0.0, 0.0, FAR_Z);
		Quad{
			camera.world_pos_to_screen_pos(roadNearL),
			camera.world_pos_to_screen_pos(roadFarL),
			camera.world_pos_to_screen_pos(roadFarR),
			camera.world_pos_to_screen_pos(roadNearR)
		}.draw(ROAD_COLOR);

		// 左線（内側）のベース
		Vec3 leftInNearL{ POS_X - roadWidth / 2.0 + SIDE_LINE_IN_PADDING, POS_Y, camera.get_z()};
		Vec3 leftInNearR{ leftInNearL.x + SIDE_LINE_IN_WIDTH, POS_Y, camera.get_z() };
		Vec3 leftInFarL = leftInNearL.movedBy(0.0, 0.0, FAR_Z);
		Vec3 leftInFarR = leftInNearR.movedBy(0.0, 0.0, FAR_Z);
		Quad{
			camera.world_pos_to_screen_pos(leftInNearL),
			camera.world_pos_to_screen_pos(leftInFarL),
			camera.world_pos_to_screen_pos(leftInFarR),
			camera.world_pos_to_screen_pos(leftInNearR)
		}.draw(SIDE_LINE_IN_COLOR_A);

		// 右線（内側）のベース
		Vec3 rightInNearR{ POS_X + roadWidth / 2.0 - SIDE_LINE_IN_PADDING, POS_Y, camera.get_z() };
		Vec3 rightInNearL{ rightInNearR.x - SIDE_LINE_IN_WIDTH, POS_Y, camera.get_z() };
		Vec3 rightInFarR = rightInNearR.movedBy(0.0, 0.0, FAR_Z);
		Vec3 rightInFarL = rightInNearL.movedBy(0.0, 0.0, FAR_Z);
		Quad{
			camera.world_pos_to_screen_pos(rightInNearL),
			camera.world_pos_to_screen_pos(rightInFarL),
			camera.world_pos_to_screen_pos(rightInFarR),
			camera.world_pos_to_screen_pos(rightInNearR)
		}.draw(SIDE_LINE_IN_COLOR_A);

		// 左線（外側）のベース
		Vec3 leftOutNearL{ POS_X - roadWidth / 2.0, POS_Y, camera.get_z() };
		Vec3 leftOutNearR{ leftOutNearL.x + SIDE_LINE_OUT_WIDTH, POS_Y, camera.get_z() };
		Vec3 leftOutFarL = leftOutNearL.movedBy(0.0, 0.0, FAR_Z);
		Vec3 leftOutFarR = leftOutNearR.movedBy(0.0, 0.0, FAR_Z);
		Quad{
			camera.world_pos_to_screen_pos(leftOutNearL),
			camera.world_pos_to_screen_pos(leftOutFarL),
			camera.world_pos_to_screen_pos(leftOutFarR),
			camera.world_pos_to_screen_pos(leftOutNearR)
		}.draw(SIDE_LINE_OUT_COLOR);

		// 右線（外側）のベース
		Vec3 rightOutNearR{ POS_X + roadWidth / 2.0, POS_Y, camera.get_z() };
		Vec3 rightOutNearL{ rightOutNearR.x - SIDE_LINE_OUT_WIDTH, POS_Y, camera.get_z() };
		Vec3 rightOutFarR = rightOutNearR.movedBy(0.0, 0.0, FAR_Z);
		Vec3 rightOutFarL = rightOutNearL.movedBy(0.0, 0.0, FAR_Z);
		Quad{
			camera.world_pos_to_screen_pos(rightOutNearL),
			camera.world_pos_to_screen_pos(rightOutFarL),
			camera.world_pos_to_screen_pos(rightOutFarR),
			camera.world_pos_to_screen_pos(rightOutNearR)
		}.draw(SIDE_LINE_OUT_COLOR);


		// 中央線描画
		for (int i = 0; i < CENTER_LINE_NUM; ++i) {

			// 線の長さを L としたときに、2L - (進行距離 % 2L) + 進行距離 - L を線のスタートとしている
			// 最後の -L はよくわからないが無いとだめ
			// 進行距離に足す値を徐々に減らすことで、カメラとの相対距離を減らす、つまり近づくようにしている
			// 余りの計算は少数だとできないため、rate倍して整数化してから求めている
			// 0で割らないよう、tmpを用意した
			const double rate = 10000.0;
			int32 tmp = (int32)(CENTER_LINE_LENGTH * 2 * rate);
			if (tmp == 0)
				tmp = 1;
			const double remainder = (int32)(camera.get_z() * rate) % tmp / rate;
			const double start = CENTER_LINE_LENGTH * 2 - remainder + camera.get_z() - CENTER_LINE_LENGTH;
			const double z = start + CENTER_LINE_LENGTH * i * 2;

			// 中央線の頂点ワールド座標
			const Vec3 centerNearL{ POS_X - CENTER_LINE_WIDTH / 2.0, POS_Y, z };
			const Vec3 centerNearR{ POS_X + CENTER_LINE_WIDTH / 2.0, POS_Y, z };
			const Vec3 centerFarL = centerNearL.movedBy(0.0, 0.0, CENTER_LINE_LENGTH);
			const Vec3 centerFarR = centerNearR.movedBy(0.0, 0.0, CENTER_LINE_LENGTH);

			// 中央線描画
			Quad{
				camera.world_pos_to_screen_pos(centerNearL),
				camera.world_pos_to_screen_pos(centerFarL),
				camera.world_pos_to_screen_pos(centerFarR),
				camera.world_pos_to_screen_pos(centerNearR)
			}.draw(CENTER_LINE_COLOR);
		}

		// 両脇（内側）の線
		for (int i = 0; i < SIDE_LINE_IN_NUM; ++i) {

			const double rate = 10000.0;
			int32 tmp = (int32)(SIDE_LINE_IN_LENGTH * 2 * rate);
			if (tmp == 0)
				tmp = 1;
			const double remainder = (int32)(camera.get_z() * rate) % tmp / rate;
			const double start = SIDE_LINE_IN_LENGTH * 2 - remainder + camera.get_z() - SIDE_LINE_IN_LENGTH;
			const double z = start + SIDE_LINE_IN_LENGTH * i * 2;

			// 左線の頂点ワールド座標
			const Vec3 leftNearL{ POS_X - roadWidth / 2.0 + SIDE_LINE_IN_PADDING, POS_Y, z };
			const Vec3 leftNearR{ leftNearL.x + SIDE_LINE_IN_WIDTH, POS_Y, z };
			const Vec3 leftFarL = leftNearL.movedBy(0.0, 0.0, SIDE_LINE_IN_LENGTH);
			const Vec3 leftFarR = leftNearR.movedBy(0.0, 0.0, SIDE_LINE_IN_LENGTH);

			// 左線描画
			Quad{
				camera.world_pos_to_screen_pos(leftNearL),
				camera.world_pos_to_screen_pos(leftFarL),
				camera.world_pos_to_screen_pos(leftFarR),
				camera.world_pos_to_screen_pos(leftNearR)
			}.draw(SIDE_LINE_IN_COLOR_B);

			// 右線の頂点ワールド座標
			const Vec3 rightNearR{ POS_X + roadWidth / 2.0 - SIDE_LINE_IN_PADDING, POS_Y, z };
			const Vec3 rightNearL{ rightNearR.x - SIDE_LINE_IN_WIDTH, POS_Y, z };
			const Vec3 rightFarR = rightNearR.movedBy(0.0, 0.0, SIDE_LINE_IN_LENGTH);
			const Vec3 rightFarL = rightNearL.movedBy(0.0, 0.0, SIDE_LINE_IN_LENGTH);

			// 右線描画
			Quad{
				camera.world_pos_to_screen_pos(rightNearL),
				camera.world_pos_to_screen_pos(rightFarL),
				camera.world_pos_to_screen_pos(rightFarR),
				camera.world_pos_to_screen_pos(rightNearR)
			}.draw(SIDE_LINE_IN_COLOR_B);
		}

		// デバッグ用
		//Circle{ camera.world_pos_to_screen_pos(roadNearL), 3 }.draw(Palette::Red);
		//Circle{ camera.world_pos_to_screen_pos(roadNearR), 3 }.draw(Palette::Red);
		//Circle{ camera.world_pos_to_screen_pos(roadFarL), 3 }.draw(Palette::Red);
		//Circle{ camera.world_pos_to_screen_pos(roadFarR), 3 }.draw(Palette::Red);
	}

	Vec2 pos2() const { return Vec2(pos.x, pos.y); }
	Point pos2I() const { return Point((int32)pos.x, (int32)pos.y); }
};
