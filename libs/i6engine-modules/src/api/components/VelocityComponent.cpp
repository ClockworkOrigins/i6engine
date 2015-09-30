#include "i6engine/api/components/VelocityComponent.h"

#include "i6engine/api/configs/ComponentConfig.h"

namespace i6engine {
namespace api {

	VelocityComponent::VelocityComponent(const int64_t id, const attributeMap & params) : Component(id, params), _acceleration(), _deceleration(), _maxSpeed(), _resistanceCoefficient(0.6), _windage(0.8), _handling(MaxSpeedHandling::KeepSpeed) {
		Component::_objFamilyID = components::ComponentTypes::VelocityComponent;
		Component::_objComponentID = components::ComponentTypes::VelocityComponent;

		parseAttribute<true>(params, "acceleration", _acceleration);
		parseAttribute<false>(params, "deceleration", _deceleration);
		parseAttribute<true>(params, "maxSpeed", _maxSpeed);
		parseAttribute<false>(params, "resistanceCoefficient", _resistanceCoefficient);
		parseAttribute<false>(params, "windage", _windage);
		parseAttribute<false>(params, "handling", _handling);
	}

	VelocityComponent::~VelocityComponent() {
	}

	ComPtr VelocityComponent::createC(const int64_t id, const attributeMap & params) {
		return utils::make_shared<VelocityComponent, Component>(id, params);
	}

	void VelocityComponent::Init() {
	}

	void VelocityComponent::Finalize() {
	}

	attributeMap VelocityComponent::synchronize() const {
		attributeMap params;
		params.insert(std::make_pair("acceleration", std::to_string(_acceleration)));
		params.insert(std::make_pair("deceleration", std::to_string(_deceleration)));
		params.insert(std::make_pair("maxSpeed", std::to_string(_maxSpeed)));
		params.insert(std::make_pair("resistanceCoefficient", std::to_string(_resistanceCoefficient)));
		params.insert(std::make_pair("windage", std::to_string(_windage)));
		params.insert(std::make_pair("handling", std::to_string(int(_handling))));
		return params;
	}

	void VelocityComponent::accelerate(double acceleration, MaxSpeedHandling handling, const std::function<void(void)> & callback) {
		_acceleration = acceleration;
		_handling = handling;
		// TODO: (Daniel) implement
	}

	void VelocityComponent::decelerate(double deceleration, const std::function<void(void)> & callback) {
		_deceleration = deceleration;
		// TODO: (Daniel) implement
	}

	void VelocityComponent::setMaxSpeed(double maxSpeed) {
		_maxSpeed = maxSpeed;
		// TODO: (Daniel) implement
	}

	void VelocityComponent::setResistanceCoefficient(double coeff) {
		_resistanceCoefficient = coeff;
		// TODO: (Daniel) implement
	}

	void VelocityComponent::setWindage(double windage) {
		_windage = windage;
		// TODO: (Daniel) implement
	}

} /* namespace api */
} /* namespace i6engine */
