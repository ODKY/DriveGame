/*****************************************************************//**
 * @file   GameState.h
 * @brief  
 * 
 * @author yo746
 * @date   January 2024
 *********************************************************************/
#pragma once

#include "std.h"
#include <Siv3D.hpp>

namespace SGM2 {

	class GameStateManager;
	class Object;

	class GameState {

		friend class GameStateManager;

	public:
		static constexpr int INVALID_ID = 0;
		static constexpr int FIRST_ID = 1;

		//■//■//■//■//■// 特殊関数 //■//■//■//■//■//

		GameState() :
			id(INVALID_ID),
			isActive(true),
			objectIsAdded(false),
			objects(std::make_unique<vector<shared_ptr<Object>>>()),
			effects(std::make_unique<Effect>()) {
		}

		GameState(GameState&& r) noexcept :
			id(r.id),
			isActive(r.isActive),
			objectIsAdded(r.objectIsAdded),
			objects(std::move(r.objects)),
			effects(std::move(r.effects)) {
			r.objects.reset();
			r.effects.reset();
		}

		virtual ~GameState() = default;



		//■//■//■//■//■// オブジェクト操作 //■//■//■//■//■//

		void add_object(shared_ptr<Object> obj);

		void add_object(Object* obj);

		weak_ptr<Object> get_object_by_id(const int id_) {
			auto it = std::find_if(objects->begin(), objects->end(), [id_](const auto& v) {
				return v->get_id() == id_;
			});
			if (it != objects->end())
				return *it;
			return weak_ptr<Object>();
		}



		//■//■//■//■//■// ゲッター //■//■//■//■//■//

		int get_id() const { return id; }

		string get_type_name() const { return std::string(typeid(*this).name()); }

		bool is_active() const { return isActive; }

		int get_objects_num() const { return (int)objects->size(); }



	private:
		//■//■//■//■//■// 仮想関数 //■//■//■//■//■//

		virtual void update() {}

		virtual void draw() const {}



		//■//■//■//■//■// 更新関数 //■//■//■//■//■//

		void trigger_update();



		//■//■//■//■//■// 活性状態操作 //■//■//■//■//■//

		void activate() {
			isActive = true;
		}

		void deactivate() {
			isActive = false;
		}



		//■//■//■//■//■// セッター //■//■//■//■//■//

		void set_id(const int id_) {
			id = id_;
			//for (auto& obj : *objects)
			//	obj->set_game_state_id(id_);
		}



		int id;				// 1はじまりとする
		bool isActive;
		bool objectIsAdded;
		unique_ptr<vector<shared_ptr<Object>>> objects;	// ムーブできるようにポインタ化しておく

	public:
		unique_ptr<Effect> effects;

#ifdef _DEBUG
	public:
		vector<shared_ptr<Object>>* DEBUG_get_object_container() {
			return objects.get();
		}
#endif
	};
}

