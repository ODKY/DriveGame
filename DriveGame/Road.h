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

public:
	// 1車線の幅
	static constexpr int32 LANE_WIDTH = 200;
private:
	
	// 通常の線のパラメータ
	static constexpr int32 LINE_WIDTH = 10;
	static constexpr double LINE_LENGTH = 0.7;
	static constexpr int32 LINE_NUM = 18;

	// 外側から2番目の線
	static constexpr int32 SIDE_LINE_IN_WIDTH = 12;
	static constexpr double SIDE_LINE_IN_LENGTH = 0.3;
	static constexpr int32 SIDE_LINE_IN_PADDING = 18;
	static constexpr int32 SIDE_LINE_IN_NUM = 25;

	// 一番外側の線
	static constexpr int32 SIDE_LINE_OUT_WIDTH = 10;

public:
	// 外側にあるやつ全体の幅
	static constexpr int32 ALL_PADDING = SIDE_LINE_IN_WIDTH + SIDE_LINE_IN_PADDING + SIDE_LINE_OUT_WIDTH;
private:

	// 車線の数と道路全体の幅
	static constexpr int32 LANE_NUM = 5;
	static constexpr int32 ROAD_WIDTH = LANE_WIDTH * LANE_NUM + ALL_PADDING * 2;

	static constexpr int32 FAR_Z = INT_MAX;

	// マジックナンバー
	static constexpr int IDX_START = 0;
	static constexpr int IDX_CURVE = 1;

public:
	Road(const Camera& camera_, const int32 laneNum_) :
		Object({ 0.0, 0.0, INT_MAX }),
		camera(camera_),
		laneNum(laneNum_),
		roadWidth(calc_road_width(laneNum)),
		renderTexture(RENDER_TEXTURE_W, SCREEN_H, Palette::Black),
		curveData(),
		index(0) {
		//set_curve_pos();
	}

	double get_left_side_x() const {
		return POS_X - roadWidth / 2.0;
	}
	double get_right_side_x() const {
		return POS_X + roadWidth / 2.0;
	}

	double get_road_width() const {
		return roadWidth;
	}

	static double calc_road_width(const int laneNum) {
		return LANE_WIDTH * laneNum + ALL_PADDING * 2;
	}

	// コピーすべきじゃないよね
	void set_curve_pos(const vector<array<double, 2>>& data) {
		curveData = data;
	}

private:
	const Camera& camera;
	int32 laneNum;
	int32 roadWidth;
	RenderTexture renderTexture;
	vector<array<double, 2>> curveData;
	int index;

	bool update() override {

		// シェーダーにカーブ位置と曲がり具合を送信する
		cbRoad->start0 = (float)camera.world_pos_to_camera_pos({ 0.0, 0.0, curveData[index][IDX_START] }).z;
		cbRoad->start1 = (float)camera.world_pos_to_camera_pos({ 0.0, 0.0, curveData[index + 1][IDX_START] }).z;
		cbRoad->start2 = (float)camera.world_pos_to_camera_pos({ 0.0, 0.0, curveData[index + 2][IDX_START] }).z;
		cbRoad->curve0 = curveData.at(index)[IDX_CURVE];
		cbRoad->curve1 = curveData.at(index + 1)[IDX_CURVE];
		cbRoad->curve2 = curveData.at(index + 2)[IDX_CURVE];

		// 一定ラインを超えたらカーブ情報を更新
		if (
			camera.get_z() > curveData.at(index + 1)[IDX_START]
			&& index < (int)curveData.size() - 3
			) {
			++index;
		}

		// 以下全部デバッグ
		//Print << U"START0: " << cbRoad->start0;
		//Print << U"START1: " << cbRoad->start1;
		//Print << U"START2: " << cbRoad->start2;
		//Print << U"CURVE0: " << cbRoad->curve0;
		//Print << U"CURVE1: " << cbRoad->curve1;
		//Print << U"CURVE2: " << cbRoad->curve2;

		//double curve;
		//if (camera.get_z() < curveData.at(index + 1)[IDX_START])
		//	curve = curveData.at(index)[IDX_CURVE];
		//else
		//	curve = curveData.at(index + 1)[IDX_CURVE];
		//Print << U"CURVE: " << curve;
		//Print << U"CURVE: " << camera.get_cameras_curve();

		Print << U"CurveAmount : " << curveAmount;

		return true;
	}

	void draw() const override;

	//void set_curve_pos() {

	//	// 前の前からの差は50以内で
	//	// 50以内だと変化が近くで起こるから見えちゃう
	//	curveData = {
	//		{ 0.0, -18 },
	//		{ 25.0, 0 },
	//		{ 50.0, 14 },
	//		{ 80.0, -7 },
	//		{ 90.0, 16 },
	//		{ 130.0, 0 },
	//		{ 155.0, -18 },
	//		{ 180.0, 18 },
	//		{ 205.0, -18 },
	//		{ 230.0, 0 },
	//		{ 275.0, 20 },
	//		{ 280.0, -20 },
	//		{ 325.0, 0 },
	//		{ INT_MAX - 1, 0 },
	//		{ INT_MAX, 0 },
	//	};
	//	for (auto& data : curveData)
	//		data[1] /= 1000;
	//}

	Vec2 pos2() const { return Vec2(pos.x, pos.y); }
	Point pos2I() const { return Point((int32)pos.x, (int32)pos.y); }
};
