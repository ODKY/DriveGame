#pragma once

#include "Global.h"
#include "Obj.hpp"
#include "Road.h"
#include "DistantView.hpp"
//#include "Player.hpp"
#include "StageData.hpp"
#include "OtherCar.hpp"

class GS_Drive : public GameState {

	static constexpr Color FLOOR_COLOR_TOP{ 12, 92, 0 };
	static constexpr Color FLOOR_COLOR_BOTTOM{ 12, 252, 0 };

	static constexpr int STAGE_ID = 0;

public:
	GS_Drive();

private:
	Camera camera;
	StageData stageData;
	Road& road;
	Player& player;
	vector<ObjectData> objectData;
	int objIdx;
	double otherCarGauge;

	void update() override;

	void draw() const override {

		// 地面の描画
		Rect{ 0, SCREEN_CENTER.y, SCREEN_W, SCREEN_H/2 }.draw(
			Arg::top = FLOOR_COLOR_TOP, Arg::bottom = FLOOR_COLOR_BOTTOM
		);

		if (isHit)
			Rect{ 0, SCREEN_CENTER.y, SCREEN_W, SCREEN_H / 2 }.draw(Palette::Yellow);
		isHit = false;
	}
};
