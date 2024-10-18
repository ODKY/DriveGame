﻿#pragma once

#include "Global.h"

class Camera {
public:
	static constexpr double NEAR_PLANE = 0.2;
	static constexpr double FAR_PLANE = 50.0;
	static constexpr double HEIGHT = 145.0;

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
	Vec2 camera_pos_to_screen_pos(const Vec3& cameraToObject) const {
		double z = cameraToObject.z > 0 ? cameraToObject.z : 0.001;
		return Vec2{
			(cameraToObject.x / z) + screenCenter.x,
			(cameraToObject.y / z) + screenCenter.y,
		};
	}

	// 座標変換
	// ワールド座標系 -> デバイス座標系
	Vec2 world_pos_to_screen_pos(const Vec3& posInWorld) const {
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
	bool draw_object(const Vec3& posInWorld, const Texture& img, double scale = 1.0, const Vec2& offset = { 0.0, 0.0 }) const {
		Vec3 posFromCamera = world_pos_to_camera_pos(posInWorld);
		if (in_viewport(posFromCamera)) {
			img.scaled(calc_scale(posFromCamera) * scale).drawAt(camera_pos_to_screen_pos(posFromCamera).movedBy(offset));
			return true;
		}
		return false;
	}
	bool draw_object(const Vec3& posInWorld, const TextureRegion& img, double scale = 1.0, const Vec2& offset = { 0.0, 0.0 }) const {
		Vec3 posFromCamera = world_pos_to_camera_pos(posInWorld);
		if (in_viewport(posFromCamera)) {
			img.scaled(calc_scale(posFromCamera) * scale).drawAt(camera_pos_to_screen_pos(posFromCamera).movedBy(offset));
			return true;
		}
		return false;
	}

	// カメラが乗ってる道のカーブ値
	double get_cameras_curve() const {
		return cbRoad->curve0;
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
