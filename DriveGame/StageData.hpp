#pragma once

#include "Global.h"
#include "Road.h"

struct ObjectData {
	ObjectData(Vec3 pos_, ObjData data_):
		pos(pos_), imgIdx(data_.index), scale(data_.scale), boxSize(data_.boxSize), boxOffset(data_.boxOffset) {}
	ObjectData(Vec3 pos_, int idx_, int scale_, Size boxSize_, Point boxOffset_) :
		pos(pos_), imgIdx(idx_), scale(scale_), boxSize(boxSize_), boxOffset(boxOffset_) {}

	Vec3 pos{ 0.0, 0.0, 0.0 };
	int imgIdx = 1.0;
	double scale = 1.0;
	Size boxSize;
	Point boxOffset;
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
				{ 0.0, 0 },
				{ 50.0, 7 },// 50 R
				{ 70.0, 0 },
				{ 120.0, -7 },// 120 L
				{ 150.0, 0 },
				{ 180.0, 16 },// 180 R
				{ 200.0, -7 },// 200 L
				{ 240 + 0.0, 0 },
				{ 240 + 25.0, 0 },
				{ 240 + 50.0, 14 },// 290 R
				{ 240 + 80.0, -7 },// 320 L
				{ 240 + 90.0, 16 },// 330 R
				{ 240 + 130.0, 0 },
				{ 240 + 155.0, -18 },// 240+155 L
				{ 240 + 180.0, 18 },// 240+180 R
				{ 240 + 205.0, -18 },// 240+205 L
				{ 240 + 230.0, 0 },
				{ 240 + 275.0, 20 },// 240+275 R
				{ 240 + 280.0, -20 },// 240+280 L
				{ 240 + 325.0, 0 },
				{ INT_MAX - 1, 0 },
				{ INT_MAX, 0 },
			};
			auto& stage0 = objectData[0];
			//stage0.push_back({ { leftSideX + 180, 0, 3 }, TREE1 });
			
			// 草とまばらな木
			for (int i = 0; i < 1000; ++i) {
				for (int j = 1; j < 7; ++j) {
					stage0.push_back({ { leftSideX - 240 * j, 0, i + 0.2}, GRASS2 });
					stage0.push_back({ { leftSideX - 410 * j, 0, i + 0.7}, GRASS2 });
					stage0.push_back({ { rightSideX + 240 * j, 0, i + 0.0}, GRASS2 });
					stage0.push_back({ { rightSideX +  410 * j, 0, i + 0.5}, GRASS2 });
				}

				if (i % 4 == 0) {
					stage0.push_back({ { leftSideX - 230 * (int)(random() % 6 + 1), 0, i}, TREE2 });
					stage0.push_back({ { rightSideX + 230 * (int)(random() % 6 + 1), 0, i}, TREE2 });
				}
			}

			// 信号機
			for (int i = 0; i < 1000; i += 200) {
				stage0.push_back({ { leftSideX + 10, 0, i }, TRAFFIC_LIGHT });
			}

			// カーブの矢印
			for (int i = 50; i < 70; i+=3) {
				stage0.push_back({ { leftSideX - 100, 0, i }, ARROW_R });
			}
			for (int i = 120; i < 150; i += 3) {
				stage0.push_back({ { rightSideX + 100, 0, i }, ARROW_L });
			}
			for (int i = 180; i < 200; i += 3) {
				stage0.push_back({ { leftSideX - 100, 0, i }, ARROW_R });
			}
			for (int i = 200; i < 240; i += 3) {
				stage0.push_back({ { rightSideX + 100, 0, i }, ARROW_L });
			}
			for (int i = 290; i < 320; i += 3) {
				stage0.push_back({ { leftSideX - 100, 0, i }, ARROW_R });
			}
			for (int i = 320; i < 330; i += 3) {
				stage0.push_back({ { rightSideX + 100, 0, i }, ARROW_L });
			}
			for (int i = 330; i < 360; i += 3) {
				stage0.push_back({ { leftSideX - 100, 0, i }, ARROW_R });
			}

			for (int i = 240; i < 265; ++i) {
				//stage0.push_back({ { leftSideX + 10, 0, i }, TRAFFIC_LIGHT });
				stage0.push_back({ { rightSideX + 180, 0, i }, TREE2 });
			}
			for (int i = 265; i < 315; ++i) {
				//stage0.push_back({ { leftSideX - 300, 0, i+0.5 }, TREE2 });
				stage0.push_back({ { rightSideX + 100, 0, i }, GRASS1 });
				int idx = random() % 3;
				stage0.push_back({ { leftSideX + random() % (int)roadWidth, -230 - (int)(random() % 300), i }, BALLONS[idx] });
				stage0.push_back({ { rightSideX + 180, 0, i }, TREE2 });
			}
			for (int i = 330; i < 540; ++i) {
				stage0.push_back({ { leftSideX - 200, 0, i }, TREE2 });
				stage0.push_back({ { rightSideX + 200, 0, i }, TREE2 });
			}

			for (int j = 0; j < 18; ++j) {
				stage0.push_back({ { leftSideX + 63 * j, 0, 600}, GOAL });
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
