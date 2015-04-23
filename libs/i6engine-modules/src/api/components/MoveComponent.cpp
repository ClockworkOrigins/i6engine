#include "i6engine/api/components/MoveComponent.h"

#include "i6engine/api/components/MovementComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6engine {
namespace api {

	MoveComponent::MoveComponent(const int64_t id, const attributeMap & params) : Component(id, params), _mc() {
		_objFamilyID = components::ComponentTypes::MoveComponent;
		_objComponentID = components::ComponentTypes::MoveComponent;
	}

	MoveComponent::~MoveComponent() {
	}

	ComPtr MoveComponent::createC(const int64_t id, const attributeMap & params) {
		return utils::make_shared<MoveComponent, Component>(id, params);
	}

	void MoveComponent::Init() {
		addTicker();
		_mc = getOwnerGO()->getGOC<MovementComponent>(components::ComponentTypes::MovementComponent);
	}

	void MoveComponent::Tick() {
	}

	void MoveComponent::Finalize() {
		removeTicker();
	}

	attributeMap MoveComponent::synchronize() const {
		return attributeMap();
	}

} /* namespace api */
} /* namespace i6engine */
