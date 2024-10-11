
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
		objects->emplace_back(obj);
		objectIsAdded = true;
	}
}

void GameState::add_object(Object* obj) {
	add_object(shared_ptr<Object>(obj));
}

#include <Siv3D.hpp>
Stopwatch stopwatch5;

void GameState::trigger_update() {

	// 優先度によるオブジェクトのソート
	// Objectクラスにも似たような実装があるので、詳しくはそちらを参照
	// 後ろから更新するので昇順に
	if (objectIsAdded) {
		std::sort(objects->begin(), objects->end(), [](const auto& v0, const auto& v1) {
			return v0->get_priority() < v1->get_priority();
		});
		for (auto& obj : *objects)
			obj->isSorted = true;
		objectIsAdded = false;
	}

	// オブジェクトの更新
	// Objectクラスにも似たような実装があるので、詳しくはそちらを参照
	// 削除するので後ろから
	// 後ろからやるのならis_sortedの判定いらない？
	// いや、いるな。GSから追加されることもあるし
	for (int i = (int)objects->size() - 1; i >= 0; --i) {
		auto& obj = objects->at(i);
		if (obj->is_active() && obj->is_sorted())
			if (!obj->trigger_update())
				objects->erase(objects->begin() + i);
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

	stopwatch5.restart();
	// オブジェクトの描画
	for (int i = (int)objects->size() - 1; i >= 0; --i)
			if (objects->at(i)->isSorted)
				objects->at(i)->draw();

	// エフェクト描画
	// エフェクトとオブジェクトの前後関係はまだ制御できない
	// できるようにした方がいいかもね
	effects->update();

	//for (const auto& obj : *objects)
	//	if (obj->isSorted)
	//		obj->draw();
	Print << U"objects draw : {0: >3}"_fmt(stopwatch5.ms()) << U" ms";
}
