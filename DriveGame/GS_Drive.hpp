#pragma once

#include "Global.h"
#include "Obj.hpp"

class GS_Drive : public GameState {

public:
	GS_Drive() {
		perform_z_sort_every_time(true);
		add_object(new Obj({ 320, 200, 3 }, *imgTree01));
		add_object(new Obj({ 320, 200, 6 }, *imgTree01));
		add_object(new Obj({ 320, 200, 9 }, *imgTree01));
		add_object(new Obj({ 320, 200, 12 }, *imgTree01));
		add_object(new Obj({ 320, 200, 15 }, *imgTree01));
	}

private:

	void update() override {
		double speed = 50.0;
		if (KeyUp.pressed())
			cameraPos.y -= speed * Scene::DeltaTime();
		if (KeyRight.pressed())
			cameraPos.x += speed * Scene::DeltaTime();
		if (KeyDown.pressed())
			cameraPos.y += speed * Scene::DeltaTime();
		if (KeyLeft.pressed())
			cameraPos.x -= speed * Scene::DeltaTime();
		if (KeyS.pressed())
			cameraPos.z += speed / 10 * Scene::DeltaTime();
		if (KeyA.pressed())
			cameraPos.z -= speed / 10 * Scene::DeltaTime();
		Print << cameraPos;
	}

	void draw() const override {

	}
};
