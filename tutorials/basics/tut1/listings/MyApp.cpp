#include "i6engine/api/Application.h"

#include <iostream>

class MyApp : public i6engine::api::Application {
public:
	MyApp() : i6engine::api::Application() {}

	~MyApp() {}

	void Initialize() {
		std::cout << "Initialize() invoked\n" << std::endl;
	}

	void AfterInitialize() {
		std::cout << "AfterInitialize() invoked\n" << std::endl;
	}

	void Tick() {
		std::cout << "Tick() invoked\n" << std::endl;
	}

	void ShutdownRequest() {
		std::cout << "ShutdownRequest() invoked\n" << std::endl;
	}

	void Finalize() {
		std::cout << "Finalize() invoked\n" << std::endl;
	}
};
