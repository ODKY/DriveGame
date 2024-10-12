#pragma once

#include "Global.h"

class Camera {
public:
	static constexpr double NEAR_PLANE = 0.2;
	static constexpr double FAR_PLANE = 50.0;

	constexpr Camera(const Vec3& pos_, const Size& screenSize_) :
		screenSize(screenSize_),
		screenCenter(screenSize.x / 2, screenSize.y / 2),
		pos(pos_) {
	}

	// 座標変換
	// ワールド座標系 -> カメラ座標系
	Vec3 world_pos_to_camera_pos(const Vec3& posInWorld) const {
		return posInWorld - pos;
	}

	// 座標変換
	// カメラ座標系 -> デバイス座標系
	Point camera_pos_to_screen_pos(const Vec3& cameraToObject) const {
		double z = cameraToObject.z != 0 ? cameraToObject.z : 0.001;
		return Point{
			(int32)(cameraToObject.x / z) + screenCenter.x,
			(int32)(cameraToObject.y / z) + screenCenter.y,
		};
	}

	// 座標変換
	// ワールド座標系 -> デバイス座標系
	Point world_pos_to_screen_pos(const Vec3& posInWorld) const {
		return camera_pos_to_screen_pos(world_pos_to_camera_pos(posInWorld));
	}

	// 拡大率取得
	double calc_scale(const Vec3& cameraToObject) const {
		double z = cameraToObject.z != 0 ? cameraToObject.z : 0.001;
		return 1.0 / z;
	}

	// ビューポートに入っているか
	// カメラ座標系で判定
	// ただし、前面と後面の判定しか行わない
	bool in_viewport(const Vec3& posFromCamera) const {
		return posFromCamera.z > NEAR_PLANE && posFromCamera.z < FAR_PLANE;
	}

	// ワールド座標と画像をもらってスクリーンに投影
	// 描画した場合は true を返す
	bool draw_object(const Vec3& posInWorld, const Texture& img) const {
		Vec3 posFromCamera = world_pos_to_camera_pos(posInWorld);
		if (in_viewport(posFromCamera)) {
			img.scaled(calc_scale(posFromCamera)).drawAt(camera_pos_to_screen_pos(posFromCamera));
			return true;
		}
		return false;
	}

	void set_pos(const Vec3& pos_) { pos = pos_; }
	void move(const Vec3& move) { pos += move; }
	void add_x(const double x) { pos.x += x; }
	void add_y(const double y) { pos.y += y; }
	void add_z(const double z) { pos.z += z; }
	const Vec3& get_pos() const { return pos; }
	double get_x() const { return pos.x; }
	double get_y() const { return pos.y; }
	double get_z() const { return pos.z; }

private:
	const Size screenSize;
	const Point screenCenter;
	Vec3 pos;
};
