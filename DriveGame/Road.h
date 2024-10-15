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

	// マジックナンバー
	static constexpr int IDX_START = 0;
	static constexpr int IDX_CURVE = 1;

public:
	Road(const Camera& camera_) :
		Object({ 0.0, 0.0, INT_MAX }),
		camera(camera_),
		roadWidth(ROAD_WIDTH_A),
		renderTexture(RENDER_TEXTURE_W, SCREEN_H, Palette::Black),
		curveData(),
		index(0) {
		set_curve_pos();
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
	RenderTexture renderTexture;
	vector<array<double, 2>> curveData;
	int index;

	bool update() override {

		// シェーダーにカーブ位置と曲がり具合を送信する
		//const Vec3 cameraToStart = camera.world_pos_to_camera_pos({ 0.0, 0.0, curveData[index][IDX_START] });
		//cbRoad->start0 = (float)(cameraToStart.z / Camera::FAR_PLANE);
		//const Vec3 cameraToStart1 = camera.world_pos_to_camera_pos({ 0.0, 0.0, curveData[index + 1][IDX_START] });
		//cbRoad->start1 = (float)(cameraToStart1.z / Camera::FAR_PLANE);

		cbRoad->start0 = (float)camera.world_pos_to_camera_pos({ 0.0, 0.0, curveData[index][IDX_START] }).z;
		cbRoad->start1 = (float)camera.world_pos_to_camera_pos({ 0.0, 0.0, curveData[index + 1][IDX_START] }).z;
		cbRoad->curve0 = curveData.at(index)[IDX_CURVE];
		cbRoad->curve1 = curveData.at(index + 1)[IDX_CURVE];

		//cbRoad->start0 = camera.world_pos_to_screen_pos({ 0.0, POS_Y, curveData[index][IDX_START] }).y;
		//if (cbRoad->start0 > 0 && cbRoad->start0 < SCREEN_H) {
		//	cbRoad->start0 /= SCREEN_H;						// 正規化
		//	cbRoad->start0 = cbRoad->start0 * -1.0f + 1.0f;	// 上を正に
		//	cbRoad->start0 *= 2.0f;								// 0～0.5の範囲になるので0～1.0に
		//}
		//else
		//	cbRoad->start0 = 0.0f;
		//cbRoad->curve0 = curveData.at(index)[IDX_CURVE];

		//cbRoad->start1 = camera.world_pos_to_screen_pos({ 0.0, POS_Y, curveData[index + 1][IDX_START] }).y;
		//if (cbRoad->start1 > 0 && cbRoad->start1 < SCREEN_H) {
		//	cbRoad->start1 /= SCREEN_H;
		//	cbRoad->start1 = cbRoad->start1 * -1.0f + 1.0f;
		//	cbRoad->start1 *= 2.0f;
		//}
		//else
		//	cbRoad->start1 = 0.0f;
		//cbRoad->curve1 = curveData.at(index + 1)[IDX_CURVE];

		Print << U"START0: " << cbRoad->start0;
		Print << U"START1: " << cbRoad->start1;

		Print << U"START1_: " << camera.world_pos_to_screen_pos({ 0.0, 0.0, curveData[index + 1][IDX_START] }).y;
		Print << U"START1_ - 240: " << camera.world_pos_to_screen_pos({ 0.0, 0.0, curveData[index + 1][IDX_START] }).y - 240;

		double curve;
		if (camera.get_z() < curveData.at(index + 1)[IDX_START])
			curve = curveData.at(index)[IDX_CURVE];
		else
			curve = curveData.at(index + 1)[IDX_CURVE];
		Print << U"CURVE:" << curve;

		if (
			camera.get_z() > curveData.at(index + 1)[IDX_START]
			&& index < (int)curveData.size() - 2
			) {
			++index;
		}
		return true;
	}

	void draw() const override;

	void set_curve_pos() {
		//curveData = {
		//	{ 50.0, 80.0, 1500.0 },
		//	{ 130.0, 230.0, -0},
		//	//{ 130.0, 230.0, -1526.0 }
		//};

		// 前の前からの差は50以内で
		curveData = {
			{ 0.0, -13 },
			{ 25.0, 0 },
			{ 50.0, 14 },
			{ 80.0, -7 },
			//{ 50.0, 0.01301 },
			//{ 0.0, -2000.0 },
			//{ 20.0, 1500.0 },
			//{ 80, 0.0 },
			//{ 130, -1500 },
			//{ 230, 0.0 },
		};
	}

	Vec2 pos2() const { return Vec2(pos.x, pos.y); }
	Point pos2I() const { return Point((int32)pos.x, (int32)pos.y); }
};
