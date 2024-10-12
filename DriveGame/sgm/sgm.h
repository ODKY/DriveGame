#pragma once

//             //////////////////
//            //****************//
//           //*                *//
//          //*       SGM2       *//
//         //*                    *//
//          //*     Ver 1.02     *//
//           //*                *//
//            //****************//
//             //////////////////

#include "std.h"
#include "Component.hpp"
#include "Object.h"
#include "Position.hpp"
#include "GameState.h"
#include "GameStateManager.h"

#define SUPER_GAME_MAKER2

// 変更箇所
//
// 
// ● Ver 1.01 -> 1.02
//
// ドライブゲームと並行して作成
// 
// ・Positionを(x, y)から(x, y, z)に変更
// 
// ・GameStateが同じオブジェクトを2つの配列で保持するようにした
//		update用に優先度でソートするもの、draw用にZ値でソートするもの、の2つ
//		同じものを2つずつ持つのは、無駄が多いしバグが怖い
//		まあ、仕方ないってことで
//		Zソートのタイミングは、priority同様オブジェクトを追加したときのみ
//		設定を変えれば、毎フレームソートすることもできる
// 
// ・PositionをObjectに標準装備
//		描画でZ値が必要になったため
//
// ・add_objectがshared_ptrを返すようにした
//		元の戻り値はvoidだった
//		add_componentはshared_ptrを返していたのに
//
// 
//
// ● Ver 1.00 -> 1.01
//
// カラーゲームの開発と並行して作成
//
// ・がっつり変えすぎて何したか覚えてない
//		1.00と1.01は完全に別物
//		そもそも1.00は DXLib ベースだし
//		当たり判定システム入れたり、ちょっと機能を減らしたり
//		そんな感じかな？
//
// 
//
// ● SGM（Super Game Maker） -> SGM2
//
// ・SGMは開発が頓挫したためSGM2に移行
//		描画関係やカメラ機能はSGMの方が充実していた
//		というか、現在（Ver1.02）はカメラとかないし
//
