#include "i6engine/api/components/MovementComponent.h"

#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/components/VelocityComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6engine {
namespace api {

	MovementComponent::MovementComponent(const int64_t id, const attributeMap & params) : Component(id, params), _psc(), _vc() {
		_objFamilyID = components::ComponentTypes::MovementComponent;
	}

	MovementComponent::~MovementComponent() {
	}

	void MovementComponent::Init() {
		addTicker();
		_psc = getOwnerGO()->getGOC<PhysicalStateComponent>(components::ComponentTypes::PhysicalStateComponent);
		_vc = getOwnerGO()->getGOC<VelocityComponent>(components::ComponentTypes::VelocityComponent);
	}

	void MovementComponent::Tick() {
	}

	void MovementComponent::Finalize() {
		removeTicker();
	}

} /* namespace api */
} /* namespace i6engine */
