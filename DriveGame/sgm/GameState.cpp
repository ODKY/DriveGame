
#include "GameState.h"
#include "Object.hpp"

using namespace SGM2;

// 自身にidがセットされるのは遷移するとき
// よって、コンストラクタからの add_object では無効なIDをセットすることになってしまう
// 対策として、GameState へのIDセット時に、オブジェクトのIDも指定し直している
//
// ↑オブジェクトにGSのポインタを持たせたので、現在はIDを渡していません
void GameState::add_object(shared_ptr<Object> obj) {
	if (obj != nullptr) {
		//obj->set_game_state_id(id);
		obj->gameState = this;
		objects->push_back(obj);
		objectsDraw->push_back(obj);
		objectIsAdded = true;
	}
}

void GameState::add_object(Object* obj) {
	add_object(shared_ptr<Object>(obj));
}

// Zソートはフラグが立っていれば毎回行われる
// 前から描画するので降順
// Zが小さいほど手前に描画される
void GameState::z_sort() {
	std::sort(objectsDraw->begin(), objectsDraw->end(), [](const auto& v0, const auto& v1) {
		return v0->get_z() > v1->get_z();
	});
}

void GameState::trigger_update() {

	// 優先度によるオブジェクトのソート
	// Objectクラスにも似たような実装があるので、詳しくはそちらを参照
	if (objectIsAdded) {

		// 後ろから更新するので昇順に
		std::sort(objects->begin(), objects->end(), [](const auto& v0, const auto& v1) {
			return v0->get_priority() < v1->get_priority();
		});
		z_sort();

		for (auto& obj : *objects)
			obj->isSorted = true;
		objectIsAdded = false;
	}

	if (flag_performZSoortEveryTime)
		z_sort();

	// オブジェクトの更新
	// Objectクラスにも似たような実装があるので、詳しくはそちらを参照
	// 削除するので後ろから
	// 後ろからやるのならis_sortedの判定いらない？
	// いや、いるな。GSから追加されることもあるし
	for (int i = (int)objects->size() - 1; i >= 0; --i) {
		auto& obj = objects->at(i);
		if (obj->is_active() && obj->is_sorted())
			if (!obj->trigger_update()) {
				objects->erase(objects->begin() + i);
				objectsDraw->erase(objectsDraw->begin() + i);
			}
	}
	//for (int i = 0; i < (int)objects->size(); ++i) {
	//	auto& obj = objects->at(i);
	//	if (obj->is_active() && obj->is_sorted())
	//		obj->trigger_update();
	//}

	// オブジェクトの削除
	//for (int i = (int)objects->size() - 1; i >= 0; --i)
	//	if ((*objects)[i]->should_be_deleted())
	//		objects->erase(objects->begin() + i);

	// GSの更新と描画
	update();
	draw();

	// オブジェクトの描画
	for (const auto& obj : *objectsDraw)
		if (obj->isSorted)
			obj->draw();

	// エフェクト描画
	// エフェクトとオブジェクトの前後関係はまだ制御できない
	// できるようにした方がいいかもね
	effects->update();

	//for (const auto& obj : *objects)
	//	if (obj->isSorted)
	//		obj->draw();

#include <Siv3D.hpp>
	Print << U"objects num 0 : " << objects->size();
	Print << U"objects num 1 : " << objectsDraw->size();
}
