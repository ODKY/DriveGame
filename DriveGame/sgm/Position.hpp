#pragma once

#include "sgm.h"
#include <Siv3D.hpp>

namespace SGM2 {

	//class Position : public Component {
	//public:
	//	constexpr Position(Object& obj, const int x = 0, const int y = 0) : Component(obj), pos(x, y) {}
	//	constexpr Position(Object& obj, const Vec2& pos_) : Component(obj), pos(pos_) {}
	//	
	//	Vec2 pos;
	//};

	//class Position3 : public Position {
	//public:
	//	constexpr Position3(Object& obj, const int x = 0, const int y = 0, const int z = 0) :
	//		Position(obj, x, y), z(z) {}

	//	constexpr Position3(Object& obj, const Vec3& pos_) :
	//		Position(obj, pos_.x, pos_.y), z(pos_.z) {}

	//	constexpr Vec3 get_pos() const {
	//		return Vec3(pos.x, pos.y, z);
	//	}

	//	constexpr void set_pos(const Vec3& pos_) {
	//		pos.x = pos_.x;
	//		pos.y = pos_.y;
	//		z = pos_.z;
	//	}

	//	double z;
	//};

	// Z値が大きいほど奥に描画される
	class Position : public Component {
	public:
		constexpr Position(Object& obj, const int x = 0, const int y = 0, const int z = 0) :
			Component(obj), pos(x, y, z) {}

		constexpr Position(Object& obj, const Vec3& pos_) :
			Component(obj), pos(pos_) {}

		Vec3 pos;
	};
}
