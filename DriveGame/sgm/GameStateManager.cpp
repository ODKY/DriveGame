#include "GameStateManager.h"
#include "GameState.h"

namespace SGM2 {

	int GameStateManager::instanceNum = 0;

	GameStateManager::GameStateManager(GameState* initialState) :
		gameStateStack(),
		nextGameStateId(GameState::FIRST_ID),
		transitionQueue() {
		++instanceNum;
		nextGameStateId++;	// インスタンス生成メソッドで最初のIDが使用されるため、ここでインクリメントしておく
		gameStateStack.emplace_back(std::move(initialState));
	}

	GameStateManager::~GameStateManager() {
		--instanceNum;
	}

	//GameStateManager::GameStateManager(const GameStateManager&) = delete;
	//GameStateManager& GameStateManager::operator=(const GameStateManager&) = delete;
	//GameStateManager::GameStateManager(const GameStateManager&&) = delete;
	//GameStateManager& GameStateManager::operator=(const GameStateManager&&) = delete;

	GameStateManager* GameStateManager::create_instance(GameState* initialState) {
		if (instanceNum != 0)
			return nullptr;
		initialState->set_id(GameState::FIRST_ID);	// これは静的メソッドなので、当然メンバ変数にはアクセスできない
		return new GameStateManager(initialState);	//std::make_unique<GameStateManager>(initialState);
	}

	GameStateManager::GameStateManager(GameStateManager&& r) noexcept :
		gameStateStack(std::move(r.gameStateStack)),
		nextGameStateId(r.nextGameStateId) {
		r.gameStateStack.clear();
	}

	
	GameStateManager& GameStateManager::operator=(GameStateManager&& r) noexcept {
		if (this != &r) {
			gameStateStack = std::move(r.gameStateStack);
			nextGameStateId = r.nextGameStateId;
			r.gameStateStack.clear();
		}
		return *this;
	}

	void GameStateManager::reserve_transition(const Object& caller, GameState* destGameState, const bool isPop, const bool deactivateFlag) {
		auto* p = new TransitionData();
		p->callersGameStateId = caller.get_game_state_id();
		p->dest = destGameState;
		p->isPop = isPop;
		p->deactivateFlag = deactivateFlag;
		transitionQueue.push(p);
	}

	bool GameStateManager::update() {
		if (gameStateStack.empty())
			return false;
		for (auto& gameState : gameStateStack)
			if (gameState->is_active())
				gameState->trigger_update();
		transition();
		return true;
	}

	bool GameStateManager::is_top_state(const int callersGameStateId) const {
		if (gameStateStack.size() <= 1)
			return true;
		if (gameStateStack[gameStateStack.size() - 1]->get_id() == callersGameStateId)
			return true;
		return false;
	}

	// キューに遷移依頼たまっていた場合、順番に全ての依頼を処理する
	// 呼び出し元がトップステートでなければ、上のステートを全てポップ
	// プッシュなしの場合、自動的にトップステートが活性化される
	// 新たにプッシュした場合も、そのステートは活性化される
	void GameStateManager::transition() {
		while (!transitionQueue.empty()) {
			auto p = transitionQueue.front();
			transitionQueue.pop();
			while (!is_top_state(p->callersGameStateId))
				gameStateStack.pop_back();
			if (!gameStateStack.empty()) {
				if (p->isPop)
					gameStateStack.pop_back();
				else if (p->deactivateFlag)
					gameStateStack[gameStateStack.size() - 1]->deactivate();	// ポップなしならトップステートを非活性状態にする（基本的には）
			}
			if (p->dest) {
				p->dest->set_id(nextGameStateId++);
				p->dest->activate();
				gameStateStack.emplace_back(p->dest);
			}
			else
				if (!gameStateStack.empty())
					gameStateStack.at(gameStateStack.size() - 1)->activate();	// プッシュなしの場合、トップステートは自動的に活性化
		}
	}

#ifdef _DEBUG
	vector<unique_ptr<GameState>>& GameStateManager::DEBUG_get_game_state_stack() {
		return gameStateStack;
	}
#endif

	// pushと言いつつIDのインクリメントもしている
	// 名前から予測できない挙動を含む関数はバグの原因になるので避けるべきだ
	// よって削除した
	// 
	//void GameStateManager::push_gs(GameState* gameState) {
	//	gameState->set_id(nextGameStateId++);
	//	gameStateStack.emplace_back(gameState);
	//}

	//void GameStateManager::pop_gs() {
	//	gameStateStack.pop_back();
	//}
}
