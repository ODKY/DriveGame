#pragma once

#include "Global.h"
#include "Road.h"

struct ObjectData {
	Vec3 pos{ 0.0, 0.0, 0.0 };
	int imgIdx = 1.0;
	double scale_ = 1.0;
};

class StageData {

	static constexpr int STAGE_NUM = 1;

public:
	StageData() : laneNum(STAGE_NUM), curveData(STAGE_NUM), objectData(STAGE_NUM) {
		set_stage_data();
	}

private:
	vector<int> laneNum;
	vector<vector<array<double, 2>>> curveData;
	vector<vector<ObjectData>> objectData;

	void set_stage_data() {
		{
			// STAGE 0
			laneNum[0] = 5;
			double roadWidth = Road::calc_road_width(laneNum[0]);
			double leftSideX = -roadWidth / 2.0;
			double rightSideX = roadWidth / 2.0;
			curveData[0] = {
				{ 0.0, -18 },
				{ 25.0, 0 },
				{ 50.0, 14 },
				{ 80.0, -7 },
				{ 90.0, 16 },
				{ 130.0, 0 },
				{ 155.0, -18 },
				{ 180.0, 18 },
				{ 205.0, -18 },
				{ 230.0, 0 },
				{ 275.0, 20 },
				{ 280.0, -20 },
				{ 325.0, 0 },
				{ INT_MAX - 1, 0 },
				{ INT_MAX, 0 },
			};
			auto& stage0 = objectData[0];
			for (int i = 0; i < 25; ++i) {
				stage0.push_back({ { leftSideX + 10, 0, i }, IMG_TRAFFIC_LIGHT, SCALE_TRAFFIC_LIGHT });
				stage0.push_back({ { leftSideX + 80, 0, i }, IMG_TREE1, SCALE_TREE1 });
			}
		}

		// カーブの値を調整
		for (auto& list : curveData)
			for (auto& value : list)
				value[1] /= 1000.0;
	}
};
