#include "i6engine/api/EngineController.h"

#include "MyApp.cpp"

int main() {
	// create Application
	MyApp myApp;

	// register basics
	i6engine::api::EngineController::GetSingleton().registerDefault(false);
	i6engine::api::EngineController::GetSingleton().registerApplication(myApp);

	// start
	i6engine::api::EngineController::GetSingleton().start();
}

