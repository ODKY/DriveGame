#include "Global.h"
#include "GS_Drive.hpp"

void Main() {
	try {
		load_font();
		load_image();
		Window::Resize(SCREEN_W, SCREEN_H);
		Scene::SetBackground(Color(0, 0, 0));

		gsm.reset(GameStateManager::create_instance(new GS_Drive()));
		Stopwatch stopwatch;

		while (System::Update() && gsm) {
			stopwatch.restart();
			ClearPrint();
			Print << U"FPS: " << (int)(1.0 / Scene::DeltaTime() + 0.5);
			if (!gsm->update())
				break;
			Print << U"{0: >3}"_fmt(stopwatch.ms()) << U" ms";
		}
		return;
	}
	catch (const std::exception& e) {
		System::MessageBoxOK(U"exception", Unicode::Widen(e.what()), MessageBoxStyle::Error);
		throw;
	}
	return;
}
