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
	vector<array<double, 3>> curveData;
	int index;

	bool update() override {
		if (camera.get_z() > 0.0) {
			const Vec3 cameraToStart = camera.world_pos_to_camera_pos({ 0.0, 0.0, curveData[index][0] });
			cbRoad->start = (float)(cameraToStart.z / Camera::FAR_PLANE);

			const Vec3 cameraToEnd = camera.world_pos_to_camera_pos({ 0.0, 0.0, curveData[index][1] });
			cbRoad->end = (float)(cameraToEnd.z / Camera::FAR_PLANE);

			cbRoad->curve = curveData.at(index)[2];

			if (
				camera.get_z() > curveData.at(index)[1]
				&& index < (int)curveData.size() - 1
				) {
				++index;
			}
		}
		return true;
	}

	void draw() const override;

	void set_curve_pos() {
		curveData.push_back({ 50.0, 80.0, 5.0 });
		curveData.push_back({ 130.0, 230.0, -2.0 });
	}

	Vec2 pos2() const { return Vec2(pos.x, pos.y); }
	Point pos2I() const { return Point((int32)pos.x, (int32)pos.y); }
};
