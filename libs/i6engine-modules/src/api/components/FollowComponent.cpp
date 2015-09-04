#include "i6engine/api/components/FollowComponent.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6engine {
namespace api {

	FollowComponent::FollowComponent(const int64_t id, const attributeMap & params) : Component(id, params), _ownPSC(), _targetPSC(), _lastTime(), _speed() {
		Component::_objFamilyID = components::ComponentTypes::FollowComponent;
		Component::_objComponentID = components::ComponentTypes::FollowComponent;

		int64_t targetID;
		parseAttribute<true>(params, "targetID", targetID);
		parseAttribute<true>(params, "speed", _speed);

		auto go = EngineController::GetSingleton().getObjectFacade()->getObject(targetID);
		if (go != nullptr) {
			_targetPSC = go->getGOC<PhysicalStateComponent>(components::ComponentTypes::PhysicalStateComponent);
		}
	}

	FollowComponent::~FollowComponent() {
	}

	ComPtr FollowComponent::createC(const int64_t id, const attributeMap & params) {
		ISIXE_THROW_API_COND("FollowComponent", "Parameter 'targetID' not set!", params.find("targetID") != params.end());
		ISIXE_THROW_API_COND("FollowComponent", "Parameter 'speed' not set!", params.find("speed") != params.end());
		return utils::make_shared<FollowComponent, Component>(id, params);
	}

	void FollowComponent::Init() {
		_ownPSC = getOwnerGO()->getGOC<PhysicalStateComponent>(components::ComponentTypes::PhysicalStateComponent);
		_lastTime = EngineController::GetSingleton().getCurrentTime();
		addTicker();
	}

	void FollowComponent::Finalize() {
		removeTicker();
	}

	void FollowComponent::Tick() {
		uint64_t cT = EngineController::GetSingleton().getCurrentTime();

		auto ownPSC = _ownPSC.get();
		auto targetPSC = _targetPSC.get();

		auto go = getOwnerGO();

		if (go == nullptr) {
			return;
		}

		if (targetPSC == nullptr) {
			go->setDie();
			return;
		}

		Vec3 dir = targetPSC->getPosition() - ownPSC->getPosition();

		if (dir.length() < 0.2) {
			go->setDie();
			return;
		}

		dir = dir.normalize();

		Vec3 dirY = dir;
		dirY.setY(0);

		Vec3 dirZ = dir;
		dirZ.setZ(0);
		dirZ.setX(0);

		Vec3 projectileDir = math::rotateVector(Vec3(0, 0, -1), ownPSC->getRotation());
		double angleY = Vec3::crossAngleSigned(projectileDir, dirY, Vec3(0, 1, 0));
		double angleZ = Vec3::crossAngleSigned(projectileDir, dirZ, Vec3(1, 0, 1).normalize());

		Quaternion rotY(Vec3(0, 1, 0), angleY);
		Quaternion rotZ(Vec3(1, 0, 1).normalize(), angleZ);
		Quaternion rot = rotY * rotZ;

		ownPSC->setPosition(ownPSC->getPosition() + (targetPSC->getPosition() - ownPSC->getPosition()).normalize() * (_speed * (cT - _lastTime) / 1000000.0), 2);
		ownPSC->applyRotation(rot);
		_lastTime = cT;
	}

	attributeMap FollowComponent::synchronize() const {
		attributeMap params;
		int64_t targetID = (_targetPSC != nullptr) ? _targetPSC.get()->getOwnerGO()->getID() : -1;
		params.insert(std::make_pair("targetID", std::to_string(targetID)));
		params.insert(std::make_pair("speed", std::to_string(_speed)));
		return params;
	}

} /* namespace api */
} /* namespace i6engine */
