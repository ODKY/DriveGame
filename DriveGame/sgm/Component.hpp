/*****************************************************************//**
 * @file   Component.h
 * @brief  派生して"コンポーネント"を作成できます
 * 
 * @author yo746
 * @date   January 2024
 *********************************************************************/
#pragma once

#include "std.h"

namespace SGM2 {

	class Object;

	/**
	 * @brief コンポーネントを作成するためのクラスです
	 */
	class Component {

		friend class Object;

	public:
		constexpr Component(Object& obj_, const int priority_ = 0) :
			object(obj_),
			isActive(true),
			isSorted(false),
			priority(priority_) {
		}

		virtual ~Component() = default;

		void activate() {
			isActive = true;
		}

		void deactivate() {
			isActive = false;
		}

		int get_priority() const { return priority; }

		bool is_active() const { return isActive; }

		bool is_sorted() const { return isSorted; }

	private:
		/**
		 * @brief 更新関数
		 * @details
		 * 毎フレーム親オブジェクトからコールされます。
		 * 
		 * @param obj
		 * コール元の親オブジェクト
		 */
		virtual void update() {}

		/**
		 * @brief 自身のクラス名を取得
		 * @details
		 * 実行時型情報を用いてクラス名を返却する。
		 * 派生した場合は派生クラス名を返すため、コンポーネントの判別に利用可能。
		 * Object の get_componet で利用している。
		 * 
		 * @return クラス名
		 */
		string get_type_name() const { return std::string(typeid(*this).name()); }

		
		//void set_priority(const int priority_) {
		//	priority = priority_;
		//}

	protected:
		Object& object;
	private:
		bool isActive;
		bool isSorted;
		const int priority;
	};
}

