#pragma once

#include "sgm.h"
#include <Siv3D.hpp>

namespace SGM2::Plus {

	class HitBox;
	class CircleHitBox;
	class RectHitBox;
	class LineHitBox;

	// ★★★ // ★★★ // ★★★ <<< 注意 >>> ★★★ // ★★★ // ★★★ //
	// ● 1
	// 接触判定は最初に行われるべきなので、Priorityはできるだけ高く設定すべきだ
	// 接触状態に応じて処理分岐するオブジェクトは多いと思うので
	// ● 2
	// createしたhitboxはCollisionManagerに格納される
	// ただし、オブジェクト側にも所有権を持たせないと削除されてしまうの注意
	// ちゃんとコンポーネントとして持たせていればよい
	// というか、そうしなきゃだめ
	//
	// 非常に面倒ではあるが、図形ごとにやっている
	// siv3Dでやるなら仕方ないような気もする
	class CollisionManager : public Object {
	public:
		explicit CollisionManager(const int priority = INT_MAX) : Object(priority) {}

		shared_ptr<CircleHitBox> create_circle_hit_box(Object& obj_, const bool checkFlag_ = true, const uint32_t groupId_ = 0);
		shared_ptr<RectHitBox> create_rect_hit_box(Object& obj_, const bool checkFlag_ = true, const uint32_t groupId_ = 0);
		shared_ptr<LineHitBox> create_line_hit_box(Object& obj_, const bool checkFlag_ = true, const uint32_t groupId_ = 0);

		// これを呼び出せば即座に当たり判定の計算を行える
		// 判定を行いながら位置を微調整したいときや、生成位置を調整したいときなど、
		// いろいろな場面で利用可能
		void recalc_collision(HitBox& hitBox) const;

	private:
		unordered_map<int, vector<shared_ptr<CircleHitBox>>> circleHitBoxes;
		unordered_map<int, vector<shared_ptr<RectHitBox>>> rectHitBoxes;
		unordered_map<int, vector<shared_ptr<LineHitBox>>> lineHitBoxes;

		// 自動で全ヒットボックスの当たり判定を行う
		bool update() override;
		void draw() const override;
		void calc_collision(HitBox& hitBox) const;
	};
}
