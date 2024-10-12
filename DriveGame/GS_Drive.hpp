#pragma once

#include "Global.h"
#include "Obj.hpp"
#include "Road.hpp"

class GS_Drive : public GameState {

public:
	GS_Drive() :
		camera({ 0.0, -50.0, 0.0 }, {SCREEN_W, SCREEN_H}),
		road(*add_object(new Road(camera))) {
		perform_z_sort_every_frame(true);

		for (int i = 0; i < 50; ++i) {
			add_object(new Obj({ road.get_left_side_x() - 80, 0, i }, *imgTree01, camera));
			add_object(new Obj({ road.get_right_side_x() + 80, 0, i }, *imgTree01, camera));
		}
	}

private:
	Camera camera;
	Road& road;

	void update() override {
		double speed = 300.0;
		if (KeyUp.pressed())
			camera.add_y(-speed * Scene::DeltaTime());
		if (KeyRight.pressed())
			camera.add_x(speed * Scene::DeltaTime());
		if (KeyDown.pressed())
			camera.add_y(speed * Scene::DeltaTime());
		if (KeyLeft.pressed())
			camera.add_x(-speed * Scene::DeltaTime());
		if (KeyS.pressed())
			camera.add_z(speed / 30 * Scene::DeltaTime());
		if (KeyA.pressed())
			camera.add_z(-speed / 30 * Scene::DeltaTime());
		Print << camera.get_pos();
	}

	void draw() const override {

	}
};
