#pragma once

#include "std.h"
#include "Component.hpp"
#include "GameState.h"
#include "Position.hpp"

namespace SGM2 {

	class GameState;

	// 描画時にZソートする都合上、Postionコンポーネントのみデフォで追加済み
	class Object {

		// メンバ関数まで指定したかったが、ヘッダと実装を分けるのが面倒なのもあってクラス指定です
		// 多分、クラスの定義が見えないとメンバ関数のfriend指定ができない
		friend class GameState;

	public:
		static constexpr int FIRST_OBJECT_ID = 1;
		//static constexpr int INVALID_GAME_STATE_ID = 0;

		//■//■//■//■//■// 特殊関数 //■//■//■//■//■//
		
		Object(const Vec3& pos_ = { 0, 0, 0 }, const int priority_ = 0) noexcept :
			gameState(nullptr),
			id(nextId),
			//gameSstateId(INVALID_GAME_STATE_ID),
			priority(priority_),
			isActive(true),
			isSorted(false),
			componentIsAdded(false),
			tags(),
			components(),
			pos(add_component(new Position(*this, pos_))->pos) {
			++nextId;
		}

		explicit Object(const int priority_ = 0, const Vec3& pos_ = { 0, 0, 0 }) noexcept :
			Object(pos_, priority_) {}

		virtual ~Object() = default;



		//■//■//■//■//■// コンポーネント操作 //■//■//■//■//■//

		//void add_component(shared_ptr<Component> com) {
		//	components.push_back(com);
		//	componentIsAdded = true;
		//}

		//void add_component(Component* com) {
		//	add_component(shared_ptr<Component>(com));
		//}

		template<typename T>
		shared_ptr<T> add_component(shared_ptr<T> com) {
			components.push_back(com);
			componentIsAdded = true;
			return com;
		}

		template<typename T>
		shared_ptr<T> add_component(T* com) {
			return add_component<T>(shared_ptr<T>(com));
		}

		template<typename T>
		void delete_component() {
			for (int i = 0; i < (int)components.size(); ++i)
				if (typeid(T).name() == components[i]->get_type_name()) {
					components.erase(components.begin() + i);
					return;
				}
		}



		//■//■//■//■//■// オブジェクト操作 //■//■//■//■//■//

		//void set_priority(const int priority_) {
		//	priority = priority_;
		//}

		void activate() {
			isActive = true;
		}

		void deactivate() {
			isActive = false;
		}

		void add_tag(const string& tag) {
			tags.push_back(tag);
		}



		//■//■//■//■//■// ゲッター //■//■//■//■//■//

		int get_id() const { return id; }

		int get_game_state_id() const { return gameState->get_id(); }

		string get_type_name() const { return std::string(typeid(*this).name()); }

		int get_components_num() const { return (int)components.size(); }

		int get_priority() const { return priority; }

		bool is_active() const { return isActive; }

		bool is_sorted() const { return isSorted; }

		const vector<string>& get_tags() const { return tags; }

		const Vec3& get_pos() const { return pos; }

		double get_z() const { return pos.z; }

		template<typename T>
		weak_ptr<T> get_component() {
			for (int i = 0; i < (int)components.size(); ++i)
				if (typeid(T).name() == components[i]->get_type_name())
					return std::static_pointer_cast<T>(components[i]);
			return weak_ptr<T>();//nullptr;
		}

		template<typename T>
		shared_ptr<const T> get_component_cshare() const {
			for (int i = 0; i < (int)components.size(); ++i)
				if (typeid(T).name() == components[i]->get_type_name())
					return std::static_pointer_cast<T>(components[i]);
			return shared_ptr<T>();//nullptr;
		}



	private:
		//■//■//■//■//■// 仮想関数 //■//■//■//■//■//

		virtual bool update() { return true; }

		virtual void draw() const {}

		//virtual bool should_be_deleted() const { return false; }



		//■//■//■//■//■// セッター //■//■//■//■//■//

		//void set_game_state_id(const int id) {
		//	gameSstateId = id;
		//}



		//■//■//■//■//■// 更新関数 //■//■//■//■//■//

		void update_components() {

			// コンポーネントが追加されていれば優先度に従ってソートする
			// コンポーネントのupdateからコンポーネントを追加した場合、次のフレームまではソートされない
			// 従って、追加分の更新処理は次フレームからの開始とする
			// priorityに反する順序での処理を防ぐためである
			if (componentIsAdded) {
				std::sort(components.begin(), components.end(), [](const auto& v0, const auto& v1) {
					return v0->get_priority() > v1->get_priority();
				});
				for (auto& component : components)
					component->isSorted = true;
				componentIsAdded = false;
			}

			// コンポーネント内でコンポーネントが付加された場合、範囲forではうまくいかない
			//		for (auto& com : components)
			// sizeを先に取得すれば、処理中に追加されたコンポーネントは処理せずに済む
			// ただし、途中で削除が発生した場合に範囲外アクセスする危険がある
			//		int length = (int)components.size();
			// 現在はフラグ操作で対処している
			for (int i = 0; i < (int)components.size(); ++i) {
				auto& component = components.at(i);
				if (component->is_active() && component->is_sorted())
					component->update();
			}
		}

		bool trigger_update() {
			update_components();
			return update();
		}

		static int nextId;

	protected:
		GameState* gameState;
	private:
		const int id;
		//int gameSstateId;
		const int priority;
		bool isActive;
		bool isSorted;
		bool componentIsAdded;
		vector<string> tags;
		vector<shared_ptr<Component>> components;
	protected:
		Vec3& pos;
	};
}
