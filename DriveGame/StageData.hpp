#pragma once

#include "Global.h"
#include "Road.h"

struct ObjectData {
	ObjectData(Vec3 pos_, ImgPair pair_):
		pos(pos_), imgIdx(pair_.index), scale(pair_.scale) {}
	ObjectData(Vec3 pos_, int idx_, int scale_) :
		pos(pos_), imgIdx(idx_), scale(scale_) {}

	Vec3 pos{ 0.0, 0.0, 0.0 };
	int imgIdx = 1.0;
	double scale = 1.0;
};

class StageData {

	static constexpr int STAGE_NUM = 1;

public:
	StageData() : laneNum(STAGE_NUM), curveData(STAGE_NUM), objectData(STAGE_NUM) {
		set_stage_data();
	}

	int get_lane_num(const int stageNo) {
		return laneNum.at(stageNo);
	}

	const vector<array<double, 2>>& get_curve_data(const int stageNo) const {
		return curveData.at(stageNo);
	}

	const vector<ObjectData>& get_object_data(const int stageNo) const {
		return objectData.at(stageNo);
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
				stage0.push_back({ { leftSideX + 10, 0, i }, TRAFFIC_LIGHT });
				stage0.push_back({ { leftSideX - 100, 0, i }, TREE1 });
				stage0.push_back({ { leftSideX - 150, 0, i+0.5 }, TREE1 });
				stage0.push_back({ { rightSideX + 180, 0, i }, TREE2 });
			}
			for (int i = 25; i < 75; ++i) {
				stage0.push_back({ { leftSideX - 100, 0, i }, ARROW });
				stage0.push_back({ { leftSideX - 300, 0, i+0.5 }, TREE2 });
				stage0.push_back({ { rightSideX + 100, 0, i }, GRASS1 });
				int idx = random() % 3;
				stage0.push_back({ { leftSideX + random() % (int)roadWidth, -180 - (int)(random() % 80), i }, BALLONS[idx] });
				stage0.push_back({ { rightSideX + 180, 0, i }, TREE2 });
			}
			for (int i = 75; i < 300; ++i) {
				stage0.push_back({ { leftSideX - 180, 0, i }, TREE2 });
				stage0.push_back({ { rightSideX + 180, 0, i }, TREE2 });
			}
		}

		// カーブの値を調整
		for (auto& list : curveData)
			for (auto& value : list)
				value[1] /= 1000.0;

		// オブジェクトをz値ソート
		// 手前のもの（zが小さいもの）から昇順に
		for (auto& vec : objectData) {
			std::sort(vec.begin(), vec.end(), [](const auto& v1, const auto& v2) {
				return v1.pos.z < v2.pos.z;
			});
		}
	}
};
