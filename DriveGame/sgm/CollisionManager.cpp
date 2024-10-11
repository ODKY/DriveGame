
#include "CollisionManager.h"
#include "HitBox.h"

namespace SGM2::Plus {

	shared_ptr<CircleHitBox> CollisionManager::create_circle_hit_box(Object& obj_, const bool checkFlag_, const uint32_t groupId_) {
		shared_ptr<CircleHitBox> collisionDetector(new CircleHitBox(*this, obj_, checkFlag_));
		circleHitBoxes[groupId_].push_back(collisionDetector);
		return collisionDetector;
	}

	shared_ptr<RectHitBox> CollisionManager::create_rect_hit_box(Object& obj_, const bool checkFlag_, const uint32_t groupId_) {
		shared_ptr<RectHitBox> collisionDetector(new RectHitBox(*this, obj_, checkFlag_));
		rectHitBoxes[groupId_].push_back(collisionDetector);
		return collisionDetector;
	}

	shared_ptr<LineHitBox> CollisionManager::create_line_hit_box(Object& obj_, const bool checkFlag_, const uint32_t groupId_ ) {
		shared_ptr<LineHitBox> collisionDetector(new LineHitBox(*this, obj_, checkFlag_));
		lineHitBoxes[groupId_].push_back(collisionDetector);
		return collisionDetector;
	}

	// 再計算時にはHitBoxを持つオブジェクトが削除されている可能性がある
	// 削除判定を行いつつ処理
	// updateからコールするcalc_collisionは別にあるので注意
	void CollisionManager::recalc_collision(HitBox& hitBox) const {

		// リストの初期化
		hitBox.hitList.clear();

		// 判定対象グループを走査して、ヒットしているかを調べる
		for (const auto& otherId : hitBox.targetIdList) {
			if (circleHitBoxes.contains(otherId))
				for (auto& other : circleHitBoxes.at(otherId))
					if (other.use_count() > 1)
						if (hitBox.check_hit(*other))
							hitBox.hitList.push_back(other.get());
			if (rectHitBoxes.contains(otherId))
				for (auto& other : rectHitBoxes.at(otherId))
					if (other.use_count() > 1)
						if (hitBox.check_hit(*other))
							hitBox.hitList.push_back(other.get());
			if (lineHitBoxes.contains(otherId))
				for (auto& other : lineHitBoxes.at(otherId))
					if (other.use_count() > 1)
						if (hitBox.check_hit(*other))
							hitBox.hitList.push_back(other.get());
		}

		// リストから自分を除外する
		for (int i = hitBox.hitList.size() - 1; i >= 0; --i) {
			auto& hitList = hitBox.hitList;
			const auto& other = hitList.at(i);
			if (hitBox.id == other->id) {
				hitList.erase(hitList.begin() + i);
				break;
			}
		}
	}

	void CollisionManager::draw() const {
		for (auto& pair : circleHitBoxes)
			for (auto& hitbox : pair.second) {
				Color colorFill, colorEdge;
				if (hitbox->checkFlag) {
					colorFill = Color{ 128, 128, 0, 128 };
					colorEdge = Color{ 255, 255, 0, 128 };
				}
				else {
					colorFill = Color{ 0, 128, 0, 128 };
					colorEdge = Color{ 0, 255, 0, 128 };
				}
				for (auto& shape : hitbox->get_shapes())
					shape.movedBy(hitbox->get_object_posI()).draw(colorFill).drawFrame(2, 0, colorEdge);
			}
		for (auto& pair : rectHitBoxes)
			for (auto& hitbox : pair.second) {
				Color colorFill, colorEdge;
				if (hitbox->checkFlag) {
					colorFill = Color{ 128, 128, 0, 128 };
					colorEdge = Color{ 255, 255, 0, 128 };
				}
				else {
					colorFill = Color{ 0, 128, 0, 128 };
					colorEdge = Color{ 0, 255, 0, 128 };
				}
				for (auto& shape : hitbox->get_shapes())
					shape.movedBy(hitbox->get_object_posI()).draw(colorFill).drawFrame(2, 0, colorEdge);
			}
		for (auto& pair : lineHitBoxes)
			for (auto& hitbox : pair.second) {
				Color colorEdge;
				if (hitbox->checkFlag) {
					colorEdge = Color{ 255, 255, 0, 128 };
				}
				else {
					colorEdge = Color{ 0, 255, 0, 128 };
				}
				for (auto& shape : hitbox->get_shapes())
					shape.movedBy(hitbox->get_object_posI()).draw(3, colorEdge);
			}
	}

	// 図形ごとにやってるのは仕方ないのかもしれない
	// ただ、同じ判定を2回ずつやっているのは明らかに無駄である
	// 遅そうならその辺の改善もしたい
	bool CollisionManager::update() {
		// 所有者のいないヒットボックスを削除
		for (auto& pair : circleHitBoxes)
			for (int i = (int)pair.second.size() - 1; i >= 0; --i) {
				auto& hitboxes = pair.second;
				if (hitboxes.at(i).use_count() <= 1)
					hitboxes.erase(hitboxes.begin() + i);
			}
		for (auto& pair : rectHitBoxes)
			for (int i = (int)pair.second.size() - 1; i >= 0; --i) {
				auto& hitboxes = pair.second;
				if (hitboxes.at(i).use_count() <= 1)
					hitboxes.erase(hitboxes.begin() + i);
			}
		for (auto& pair : lineHitBoxes)
			for (int i = (int)pair.second.size() - 1; i >= 0; --i) {
				auto& hitboxes = pair.second;
				if (hitboxes.at(i).use_count() <= 1)
					hitboxes.erase(hitboxes.begin() + i);
			}

		//// ヒットリストを初期化
		//for (auto& pair : circleHitBoxes)
		//	for (auto& hitbox : pair.second)
		//		hitbox->hitList.clear();
		//for (auto& pair : rectHitBoxes)
		//	for (auto& hitbox : pair.second)
		//		hitbox->hitList.clear();
		//for (auto& pair : lineHitBoxes)
		//	for (auto& hitbox : pair.second)
		//		hitbox->hitList.clear();

		//for (auto& pair : circleHitBoxes)
		//	for (auto& hitbox : pair.second) {
		//		for (auto& shape : hitbox->get_shapes())
		//	}


		// 各グループの配列を取り出す
		for (auto& pair : circleHitBoxes) {
			// 配列内の各要素を取得
			for (auto& hitbox : pair.second) {
				if (!hitbox->checkFlag)
					break;
				calc_collision(*hitbox);
			}
		}

		// 各グループの配列を取り出す
		for (auto& pair : rectHitBoxes) {
			// 配列内の各要素を取得
			for (auto& hitbox : pair.second) {
				if (!hitbox->checkFlag)
					continue;
				calc_collision(*hitbox);
			}
		}

		// 各グループの配列を取り出す
		for (auto& pair : lineHitBoxes) {
			// 配列内の各要素を取得
			for (auto& hitbox : pair.second) {
				if (!hitbox->checkFlag)
					break;
				calc_collision(*hitbox);
			}
		}
		return true;
	}

	void CollisionManager::calc_collision(HitBox& hitBox) const {

		// リストの初期化
		hitBox.hitList.clear();

		// 判定対象グループを走査して、ヒットしているかを調べる
		for (const auto& otherId : hitBox.targetIdList) {
			if (circleHitBoxes.contains(otherId))
				for (auto& other : circleHitBoxes.at(otherId))
					if (hitBox.check_hit(*other))
						hitBox.hitList.push_back(other.get());
			if (rectHitBoxes.contains(otherId))
				for (auto& other : rectHitBoxes.at(otherId))
					if (hitBox.check_hit(*other))
						hitBox.hitList.push_back(other.get());
			if (lineHitBoxes.contains(otherId))
				for (auto& other : lineHitBoxes.at(otherId))
					if (hitBox.check_hit(*other))
						hitBox.hitList.push_back(other.get());
		}

		// リストから自分を除外する
		for (int i = hitBox.hitList.size() - 1; i >= 0; --i) {
			auto& hitList = hitBox.hitList;
			const auto& other = hitList.at(i);
			if (hitBox.id == other->id) {
				hitList.erase(hitList.begin() + i);
				break;
			}
		}
	}
}
