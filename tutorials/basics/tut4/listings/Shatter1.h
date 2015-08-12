#ifndef SH1
#define SH1

#include <iostream>

#include "api/components/ShatterComponent.h"

class Shatter1 : public i6engine::api::ShatterComponent {
public:
	Shatter1(int64_t id, const attributeMap & params) : ShatterComponent(id, params) {
	}

	void shatter(const i6engine::api::GOPtr & other) {
		std::cout << "Shatter1" << std::endl;
	}
};

#endif
