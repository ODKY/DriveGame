#pragma once

#include "Global.h"
#include "Obj.hpp"
#include "Road.h"
#include "DistantView.hpp"
#include "Player.hpp"

class GS_Drive : public GameState {

	static constexpr Color FLOOR_COLOR_TOP{ 12, 92, 0 };
	static constexpr Color FLOOR_COLOR_BOTTOM{ 12, 252, 0 };

public:
	GS_Drive() :
		camera({ 0.0, -145.0, 0.0 }, { SCREEN_W, SCREEN_H }),
		road(*add_object(new Road(camera))) {
		perform_z_sort_every_frame(true);

		add_object(new Player(*imgBlackCar, camera));
		add_object(new DistantView(*imgMountIwate, camera));
		for (int i = 0; i < 50; ++i) {
			add_object(new Obj({ road.get_left_side_x() + 10, 0, i }, imgObjects->at(IMG_TRAFFIC_LIGHT), camera, 2));
			add_object(new Obj({ road.get_right_side_x() + 80, 0, i }, imgObjects->at(IMG_TREE1), camera));

			add_object(new Obj({ road.get_left_side_x() - 100, 0, (int32)i + 0.5}, imgObjects->at(IMG_ARROW1), camera, 3));
			add_object(new Obj({ road.get_right_side_x() + 180, 0, i + 0.5}, imgObjects->at(IMG_TREE2), camera, 3));
			add_object(new Obj({ road.get_left_side_x() - 280, 0, i }, imgObjects->at(IMG_GRASS1), camera));
			add_object(new Obj({ road.get_right_side_x() + 280, 0, i }, imgObjects->at(IMG_GRASS1), camera));
			add_object(new Obj({ road.get_left_side_x() - 380, 0, i + 0.5}, imgObjects->at(IMG_TREE1), camera));
			add_object(new Obj({ road.get_right_side_x() + 380, 0, i + 0.5}, imgObjects->at(IMG_TREE1), camera));
			add_object(new Obj({ road.get_left_side_x() + 300, 0, i }, imgObjects->at(IMG_TREE1), camera));
			add_object(new Obj({ road.get_right_side_x() + 480, 0, i }, imgObjects->at(IMG_TREE1), camera));

			if (i % 9 == 0)
				add_object(new Obj({ road.get_left_side_x() + random() % (int)road.get_road_width(), -180 - (int)(random() % 80), i}, imgObjects->at(IMG_RED_BALLON), camera, 2));
			if (i % 9 == 3)
				add_object(new Obj({ road.get_left_side_x() + random() % (int)road.get_road_width(), -180 - (int)(random() % 80), i }, imgObjects->at(IMG_BLUE_BALLON), camera, 2));
			if (i % 9 == 6)
				add_object(new Obj({ road.get_left_side_x() + random() % (int)road.get_road_width(), -180 - (int)(random() % 80), i }, imgObjects->at(IMG_YELLOW_BALLON), camera, 2));
		}
		add_object(new Obj({ road.get_left_side_x() + 250, 0, 50 }, imgObjects->at(IMG_TREE2), camera, 3));
	}

private:
	Camera camera;
	Road& road;

	void update() override {
		//double speed = 400.0;
		//if (KeyUp.pressed())
		//	camera.add_y(-speed *3* Scene::DeltaTime());
		//if (KeyRight.pressed())
		//	camera.add_x(speed * Scene::DeltaTime());
		//if (KeyDown.pressed())
		//	camera.add_y(speed *3* Scene::DeltaTime());
		//if (KeyLeft.pressed())
		//	camera.add_x(-speed * Scene::DeltaTime());

		//if (KeyW.pressed())
		//	camera.add_z(speed / 10 * Scene::DeltaTime());
		//if (KeyQ.pressed())
		//	camera.add_z(-speed / 10 * Scene::DeltaTime());
		//if (KeyS.pressed())
		//	camera.add_z(speed / 50 * Scene::DeltaTime());
		//if (KeyA.pressed())
		//	camera.add_z(-speed / 50 * Scene::DeltaTime());
		//if (KeyX.pressed())
		//	camera.add_z(speed / 250 * Scene::DeltaTime());
		//if (KeyZ.pressed())
		//	camera.add_z(-speed / 250 * Scene::DeltaTime());

		cbCamera->cameraH = -camera.get_y();

		Print << U"CAMERA_POS : " << camera.get_pos();
	}

	void draw() const override {
		Rect{ 0, SCREEN_CENTER.y, SCREEN_W, SCREEN_H/2 }.draw(
			Arg::top = FLOOR_COLOR_TOP, Arg::bottom = FLOOR_COLOR_BOTTOM
		);
	}
};
