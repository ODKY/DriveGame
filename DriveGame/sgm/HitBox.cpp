#include "HitBox.h"
#include "CollisionManager.h"

using namespace SGM2::Plus;

uint32_t HitBox::nextId = 1;

void HitBox::calc_collision() {
	cm.recalc_collision(*this);
}

// ヘッダの方に書いた通り、このコンポーネントの作成は少々面倒である
// 判定対象に応じたオーバロードが必要となり、その数は非常に多い
// 同じ内容を何度も書くのは手間なので、関数の内容は下記のマクロにまとめた
//
// あと、個別に指定するのが面倒だったので座標値にpoint型を使っている
// Vec2にした方が精度は高いかも

//#define CHECK_HIT_FUNCTION_CONTENT						\
//	bool isHit = false;									\
//	const auto& targetShapes = target.get_shapes();		\
//	for (const auto& shape : shapes)					\
//		for (const auto& targetShape : targetShapes) {	\
//			if (isHit) break;							\
//			if (shape.intersects(targetShape)) {		\
//				isHit = true;							\
//				break;									\
//			}											\
//		}												\
//	return isHit;										\

#define CHECK_HIT_FUNCTION_CONTENT												\
bool isHit = false;																\
const auto& targetShapes = target.get_shapes();									\
for (const auto& shape : shapes) {												\
	/* 位置の分だけずらして判定 */												\
	/* 本来の位置がoffsetとして働く*/											\
	auto movedShape = shape.movedBy(get_object_posI());							\
	for (const auto& targetShape : targetShapes) {								\
		if (isHit) break;														\
		auto movedTargetShape = targetShape.movedBy(target.get_object_posI());	\
		if (movedShape.intersects(movedTargetShape)) {							\
			isHit = true;														\
			break;																\
		}																		\
	}																			\
}																				\
return isHit;																	\


bool CircleHitBox::check_hit(const CircleHitBox& target) { CHECK_HIT_FUNCTION_CONTENT }
bool CircleHitBox::check_hit(const RectHitBox& target) { CHECK_HIT_FUNCTION_CONTENT }
bool CircleHitBox::check_hit(const LineHitBox& target) { CHECK_HIT_FUNCTION_CONTENT }

bool RectHitBox::check_hit(const CircleHitBox& target) { CHECK_HIT_FUNCTION_CONTENT }
bool RectHitBox::check_hit(const RectHitBox& target) { CHECK_HIT_FUNCTION_CONTENT }
bool RectHitBox::check_hit(const LineHitBox& target) { CHECK_HIT_FUNCTION_CONTENT }

bool LineHitBox::check_hit(const CircleHitBox& target) { CHECK_HIT_FUNCTION_CONTENT }
bool LineHitBox::check_hit(const RectHitBox& target) { CHECK_HIT_FUNCTION_CONTENT }
bool LineHitBox::check_hit(const LineHitBox& target) { CHECK_HIT_FUNCTION_CONTENT }

//bool CircleHitBox::check_hit(const RectHitBox& target) {
//	bool isHit = false;
//	const auto& targetShapes = target.get_shapes();
//	for (const auto& shape : shapes) {
//		auto movedShape = shape.movedBy(get_object_posI());
//		for (const auto& targetShape : targetShapes) {
//			if (isHit) break;
//			auto movedTargetShape = targetShape.movedBy(target.get_object_posI());
//			if (movedShape.intersects(movedTargetShape)) {
//				isHit = true;
//				break;
//			}
//		}
//	}
//	return isHit;
//}
