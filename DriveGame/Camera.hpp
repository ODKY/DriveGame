#pragma once

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

	// オブジェクトを左右にどれだけずらすべきか求める
	// カーブに沿った配置を行うために必要
	// カメラに持たせたのはいろんなところからアクセスしやすいからで意味はない
	// 開発終盤は時間なくてこういうことしがち
	double calc_delta_x(const Vec3& worldPos) const {
		auto pos = worldPos;
		// ワールド座標は Y=0 とした上でカーブの影響を計算する
		const Vec3 posC = world_pos_to_camera_pos({ pos.x, 0, pos.z });
		const Vec2 posS = camera_pos_to_screen_pos(posC);
		const Vec2 borderPosS = camera_pos_to_screen_pos({ 0.0, -get_y(), cbRoad->start1 });
		const Vec2 border2PosS = camera_pos_to_screen_pos({ 0.0, -get_y(), cbRoad->start2 });
		const double curve0 = cbRoad->curve0;
		const double curve1 = cbRoad->curve1;
		const double curve2 = cbRoad->curve2;

		//if (get_id() > 500) {
		//	Print << U"posC.z : " << posC.z;
		//}
		double deltaX = 0.0;
		if (posC.z < cbRoad->start1) {
			// 1つ目のカーブ境界より前
			deltaX = curve0 * posC.z;
		}
		else if (posC.z < cbRoad->start2) {
			// 2つ目のカーブ境界より前
			const double screenHeightHalf = SCREEN_H / 2;
			const double YfromB = SCREEN_H - posS.y;
			const double borderYfromB = SCREEN_H - borderPosS.y;
			double rate = (YfromB - borderYfromB) / (screenHeightHalf - borderYfromB);
			rate = pow(rate, 2.0);
			//Print << rate;
			//if (rate < 0.0)
			//	rate = 0.0;
			//if (rate > 1.0)
			//	rate = 1.0;
			if (cbRoad->start1 < 0.0)
				rate = 1;

			deltaX = (curve1 * posC.z * rate) + (curve0 * posC.z * (1.0 - rate));
			//if (get_id() > 500)
			//	Print << rate;
		}
		else {
			// 2つ目のカーブ境界以降
			const double screenHeightHalf = SCREEN_H / 2;
			const double YfromB = SCREEN_H - posS.y;
			const double border1YfromB = SCREEN_H - borderPosS.y;
			const double border2YfromB = SCREEN_H - border2PosS.y;
			const double border2YfromBorder1 = border2PosS.y - borderPosS.y;

			double rate1 = (YfromB - border1YfromB) / (screenHeightHalf - border1YfromB);
			rate1 = pow(rate1, 2.0);

			double rate2 = (YfromB - border2YfromB) / (screenHeightHalf - border2YfromB);
			rate2 = pow(rate2, 2.0);

			if (cbRoad->start2 < 0.0) {
				rate2 = 1;
				rate1 = 0;
			}

			double deltaX1 = (curve1 * posC.z * rate1) + (curve0 * posC.z * (1.0 - rate1));
			deltaX = (curve2 * posC.z * rate2) + (deltaX1 * (1 - rate2));
		}
		deltaX *= RENDER_TEXTURE_W;
		return deltaX;
	}

private:
	const Size screenSize;
	const Point screenCenter;
	Vec3 pos;
};
