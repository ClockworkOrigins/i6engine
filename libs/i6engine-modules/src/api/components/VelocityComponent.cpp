#include "i6engine/api/components/VelocityComponent.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/PhysicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6engine {
namespace api {

	VelocityComponent::VelocityComponent(const int64_t id, const attributeMap & params) : Component(id, params), _acceleration(), _deceleration(), _maxSpeed(), _resistanceCoefficient(0.6), _windage(0.8), _handling(MaxSpeedHandling::KeepSpeed), _decelerationHandling(DecelerationHandling::StopDeceleration) {
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

	void VelocityComponent::Init() {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyVelocityComponent, core::Method::Create, new physics::Physics_VelocityComponent_Create(getID(), _objOwnerID, _maxSpeed, _resistanceCoefficient, _windage), core::Subsystem::Object));
	}

	void VelocityComponent::Finalize() {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyVelocityComponent, core::Method::Delete, new physics::Physics_VelocityComponent_Delete(getID(), _objOwnerID), core::Subsystem::Object));
	}

	attributeMap VelocityComponent::synchronize() const {
		attributeMap params;
		writeAttribute(params, "acceleration", _acceleration);
		writeAttribute(params, "maxSpeed", _maxSpeed);
		if (_deceleration.isValid()) {
			writeAttribute(params, "deceleration", _deceleration);
		}
		if (std::abs(_resistanceCoefficient - 0.6) > DBL_EPSILON) {
			writeAttribute(params, "resistanceCoefficient", _resistanceCoefficient);
		}
		if (std::abs(_windage - 0.8) > DBL_EPSILON) {
			writeAttribute(params, "windage", _windage);
		}
		if (_handling != MaxSpeedHandling::KeepSpeed) {
			writeAttribute(params, "handling", _handling);
		}
		return params;
	}

	void VelocityComponent::accelerate(const Vec3 & acceleration, MaxSpeedHandling handling, const std::function<void(void)> & callback) {
		_acceleration = acceleration;
		_handling = handling;

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyAccelerate, core::Method::Update, new physics::Physics_Accelerate_Update(getID(), _objOwnerID, _acceleration, _handling, callback), core::Subsystem::Object));
	}

	void VelocityComponent::decelerate(const Vec3 & deceleration, DecelerationHandling handling, const std::function<void(void)> & callback) {
		_deceleration = deceleration;
		_decelerationHandling = handling;

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyDecelerate, core::Method::Update, new physics::Physics_Decelerate_Update(getID(), _objOwnerID, _deceleration, _decelerationHandling, callback), core::Subsystem::Object));
	}

	void VelocityComponent::stopAcceleration() const {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyStopAcceleration, core::Method::Update, new physics::Physics_StopAcceleration_Update(getID(), _objOwnerID), core::Subsystem::Object));
	}

	void VelocityComponent::setMaxSpeed(double maxSpeed) {
		_maxSpeed = maxSpeed;

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyMaxSpeed, core::Method::Update, new physics::Physics_SetMaxSpeed_Update(getID(), _objOwnerID, _maxSpeed), core::Subsystem::Object));
	}

	void VelocityComponent::setResistanceCoefficient(double coeff) {
		_resistanceCoefficient = coeff;

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyResistanceCoefficient, core::Method::Update, new physics::Physics_SetResistanceCoefficient_Update(getID(), _objOwnerID, _resistanceCoefficient), core::Subsystem::Object));
	}

	void VelocityComponent::setWindage(double windage) {
		_windage = windage;

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyWindage, core::Method::Update, new physics::Physics_SetWindage_Update(getID(), _objOwnerID, _windage), core::Subsystem::Object));
	}

} /* namespace api */
} /* namespace i6engine */
