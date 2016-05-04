#include "i6engine/api/EngineController.h"

#include "MyApp.cpp"

int main() {
	// create Application
	MyApp myApp;

	// register basics
	i6e::api::EngineController::GetSingleton().registerDefault(false);
	i6e::api::EngineController::GetSingleton().registerApplication(myApp);

	// start
	i6e::api::EngineController::GetSingleton().start();
}

