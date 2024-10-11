#pragma once

#include "std.h"
#include "GameState.h"
#include "Object.hpp"

namespace SGM2 {

	class GameStateManager {

	public:
		static GameStateManager* create_instance(GameState* initialState);

		GameStateManager(GameStateManager&&) noexcept;
		GameStateManager& operator=(GameStateManager&&) noexcept;
		~GameStateManager();

		void reserve_transition(const Object& caller, GameState* destGameState, const bool isPop = true, const bool deactivateFlag = true);
		bool update();	// 終了状態（スタックが空）でfalseを返す

	private:
		struct TransitionData {
			int callersGameStateId = 0;
			GameState* dest = nullptr;
			bool isPop = true;
			bool deactivateFlag = true;
		};

		GameStateManager(GameState* initialState);
		GameStateManager(const GameStateManager&) = delete;
		GameStateManager& operator=(const GameStateManager&) = delete;

		bool is_top_state(const int callersGameStateId) const;
		void transition();

		vector<unique_ptr<GameState>> gameStateStack;
		queue<TransitionData*> transitionQueue;
		int nextGameStateId;

		static int instanceNum;

	public:
		// --- DEBUG用関数---//　
		vector<unique_ptr<GameState>>& DEBUG_get_game_state_stack();
	};
}
