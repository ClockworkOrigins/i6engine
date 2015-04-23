#include "i6engine/rpg/components/HumanMovementComponent.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"

namespace i6engine {
namespace rpg {
namespace components {

	HumanMovementComponent::HumanMovementComponent(const int64_t id, const api::attributeMap & params) : MovementComponent(id, params) {
		_objComponentID = config::HumanMovementComponent;
	}

	HumanMovementComponent::~HumanMovementComponent() {
	}

	api::ComPtr HumanMovementComponent::createC(const int64_t id, const api::attributeMap & params) {
		return utils::make_shared<HumanMovementComponent, api::Component>(id, params);
	}

	void HumanMovementComponent::forward() {
		auto psc = _psc.get();
		psc->applyCentralForce(Vec3(0.0, 0.0, -30.0), true);
	}

	void HumanMovementComponent::backward() {
		auto psc = _psc.get();
		psc->applyCentralForce(Vec3(0.0, 0.0, 25.0), true);
	}

	void HumanMovementComponent::left() {
		auto psc = _psc.get();
		Quaternion rot(Vec3(0.0, 1.0, 0.0), 1.5 * PI / 180);
		psc->applyRotation(rot);
	}

	void HumanMovementComponent::right() {
		auto psc = _psc.get();
		Quaternion rot(Vec3(0.0, 1.0, 0.0), -1.5 * PI / 180);
		psc->applyRotation(rot);
	}

	api::attributeMap HumanMovementComponent::synchronize() const {
		return api::attributeMap();
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
