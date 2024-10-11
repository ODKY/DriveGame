#pragma once

#include "sgm.h"
#include "Position.hpp"
#include <Siv3D.hpp>

/*
* <案A : コンポーネントに複数の図形を持たせる> (ここでの実装)
* 判定を変形させる際、いちいち図形と位置をセットせねばならない
* その分処理コストが発生する
* 毎回設定しなおすので操作は楽
*
* <案B : 1つの図形ごとにコンポーネントを作る>
* 相対位置をずらす、変形させる、deactivateさせる、等で判定の形状をいじる
* 処理コストは減るが、操作が面倒になる
*/

/*
* 判定用に図形を足していくわけだが、
* 中央（Positionコンポーネントの座標）からずれる場合はoffsetを指定する
*/

namespace SGM2::Plus {

	class CollisionManager;

	// 当たり判定には Siv3D のCircleやRectといった図形を使用する
	// しかし、これらのクラスは共通の親を持たず、1つの配列にまとめることができない
	// したがって、このコンポーネントは図形ごとに分けて作成せざるを得なかった
	// Circle判定を持つコンポーネント、Rect判定を持つコンポーネント、と言ったように
	class CircleHitBox;
	class RectHitBox;
	class LineHitBox;

	class HitBox : public Component {
		friend CollisionManager;

	public:

		//■//■//■//■//■// セッター //■//■//■//■//■//

		constexpr void add_tag(const string& tag) { tags.push_back(tag); }

		constexpr void reset_target_group_id(const vector<uint32_t>& list) {
			targetIdList = list;
			std::sort(targetIdList.begin(), targetIdList.end());
			targetIdList.erase(std::unique(targetIdList.begin(), targetIdList.end()), targetIdList.end());
		}



		//■//■//■//■//■// ゲッター //■//■//■//■//■//

		constexpr const vector<const HitBox*>& get_others_hitbox() const { return hitList; }

		// ベクトル実体を返すので、ちょっとコピーの負荷があるかも
		// ポインタで返した方いいかな？
		// でも、動的確保の方が遅くなる可能性もある？
		constexpr const vector<const HitBox*> get_others_hitbox_by_tag(const string& tag) const {
			vector<const HitBox*> hitBoxes;
			std::copy_if(hitList.begin(), hitList.end(), std::back_inserter(hitBoxes), [&tag](const HitBox* hitBox) {
				return std::ranges::contains(hitBox->get_tags(), tag);
			});
			return hitBoxes;
		}

		constexpr bool is_hit() const { return hitList.empty() ? false : true; }

		constexpr const vector<string>& get_tags() const { return tags; }

		constexpr vector<string> get_others_tag() const {
			vector<string> tags;
			for (const auto& other : hitList)
				tags.insert(tags.end(), other->tags.begin(), other->tags.end());
			std::sort(tags.begin(), tags.end());
			tags.erase(std::unique(tags.begin(), tags.end()), tags.end());
			return tags;
		}

		constexpr bool others_tag_contain(const string& tag_) const {
			for (const auto& other : hitList)
				for (const auto& tag : other->tags)
					if (tag_ == tag)
						return true;
			return false;
		}

		const Vec2& get_object_posD() const { return objPos->pos; }

		const Point& get_object_posI() const { return Point((int)objPos->pos.x, (int)objPos->pos.y); }

		const uint32_t get_object_id() const { return objectId; }



		//■//■//■//■//■// 計算 //■//■//■//■//■//

		void calc_collision();



	protected:
		//■//■//■//■//■// 特殊関数 //■//■//■//■//■//

		HitBox(CollisionManager& cm_, Object& obj_, const bool checkFlag_ = true) :
			Component(obj_),
			id(nextId),
			objectId(obj_.get_id()),
			targetIdList({ 0 }),
			hitList(),
			checkFlag(checkFlag_),
			tags(),
			cm(cm_) {
			++nextId;
			objPos = object.get_component_cshare<Position>();
			if (!objPos)
				throw std::runtime_error("CollisionDetector : Positionコンポーネントがないです");
		}

		virtual ~HitBox() {}



	private:
		//■//■//■//■//■// 当たり判定関数 //■//■//■//■//■//

		virtual bool check_hit(const CircleHitBox&) = 0;
		virtual bool check_hit(const RectHitBox&) = 0;
		virtual bool check_hit(const LineHitBox&) = 0;



		//■//■//■//■//■// メンバ変数 //■//■//■//■//■//

		static uint32_t nextId;

		uint32_t id;
		uint32_t objectId;
		shared_ptr<const Position> objPos;
		vector<uint32_t> targetIdList;				// 接触判定を行う相手のグループID（自身のグループIDはキーとしてManagerに記憶される）
	protected:
		vector<const HitBox*> hitList;				// 接触しているHitBox
		const bool checkFlag;						// これがfalseだと、check_hitが呼ばれない
	private:
		vector<string> tags;
		CollisionManager& cm;
	};








	class CircleHitBox : public HitBox {
		friend CollisionManager;

	public:
		constexpr const vector<Circle>& get_shapes() const { return shapes; }

		constexpr void add_shape(const double& radius, const Vec2& offset = { 0, 0 }) {
			//add_shape_pos(offset);
			shapes.push_back(Circle(offset, radius));
		}
		constexpr void clear() {
			//clear_pos();
			shapes.clear();
		}

	private:
		CircleHitBox(CollisionManager& cm_, Object& obj_, const bool checkFlag_ = true) : HitBox(cm_, obj_, checkFlag_) {}
		bool check_hit(const CircleHitBox& target) override;
		bool check_hit(const RectHitBox& target) override;
		bool check_hit(const LineHitBox& target) override;
		constexpr void update() override {

		}

		vector<Circle> shapes;
	};



	class RectHitBox : public HitBox {
		friend CollisionManager;

	public:
		constexpr const vector<Rect>& get_shapes() const { return shapes; }

		constexpr void add_shape(const Size& size, const Point& offset = { 0, 0 }) {
			//add_shape_pos(offset);
			shapes.push_back(Rect(Arg::center(offset), size));
		}
		constexpr void clear() {
			//clear_pos();
			shapes.clear();
		}

	private:
		RectHitBox(CollisionManager& cm_, Object& obj_, const bool checkFlag_ = true) : HitBox(cm_, obj_, checkFlag_) {}
		bool check_hit(const CircleHitBox& target) override;
		bool check_hit(const RectHitBox& target) override;
		bool check_hit(const LineHitBox& target) override;

		vector<Rect> shapes;
	};



	class LineHitBox : public HitBox {
		friend CollisionManager;

	public:
		constexpr const vector<Line>& get_shapes() const { return shapes; }

		constexpr void add_shape(const Vec2& start, const Vec2& end, const Vec2& offset = { 0, 0 }) {
			//add_shape_pos(shapePos_);
			shapes.push_back(Line(start + offset, end + offset));
		}
		constexpr void clear() {
			//clear_pos();
			shapes.clear();
		}

	private:
		LineHitBox(CollisionManager& cm_, Object& obj_, const bool checkFlag_ = true) : HitBox(cm_, obj_, checkFlag_) {}
		bool check_hit(const CircleHitBox& target) override;
		bool check_hit(const RectHitBox& target) override;
		bool check_hit(const LineHitBox& target) override;

		vector<Line> shapes;
	};
}
