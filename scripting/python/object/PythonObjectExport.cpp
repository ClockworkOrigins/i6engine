/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/AnimatedDirectionalLightComponent.h"
#include "i6engine/api/components/AnimatedSpotLightComponent.h"
#include "i6engine/api/components/AnimationControllerComponent.h"
#include "i6engine/api/components/BillboardComponent.h"
#include "i6engine/api/components/FollowComponent.h"
#include "i6engine/api/components/LifetimeComponent.h"
#include "i6engine/api/components/MeshAppearanceComponent.h"
#include "i6engine/api/components/MovableTextComponent.h"
#include "i6engine/api/components/MoveComponent.h"
#include "i6engine/api/components/MovementComponent.h"
#include "i6engine/api/components/MoverCircleComponent.h"
#include "i6engine/api/components/MoverInterpolateComponent.h"
#include "i6engine/api/components/MovingCameraComponent.h"

#ifdef ISIXE_WITH_NETWORK
	#include "i6engine/api/components/NetworkSenderComponent.h"
#endif

#include "i6engine/api/components/ParticleEmitterComponent.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/components/Point2PointConstraintComponent.h"
#include "i6engine/api/components/ShatterComponent.h"
#include "i6engine/api/components/SoundComponent.h"
#include "i6engine/api/components/SoundListenerComponent.h"
#include "i6engine/api/components/SpawnpointComponent.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/components/TerrainAppearanceComponent.h"
#include "i6engine/api/components/ToggleWaynetComponent.h"
#include "i6engine/api/components/VelocityComponent.h"
#include "i6engine/api/components/WaynetNavigationComponent.h"
#include "i6engine/api/components/WaypointComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/ScriptingConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/facades/ScriptingFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "boost/python.hpp"

namespace i6e {
namespace python {
namespace object {

	boost::python::list getAllObjectsOfType(const std::string & types) {
		boost::python::list l;
		std::vector<i6e::api::GOPtr> v = i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->getAllObjectsOfType(types);
		for (std::vector<i6e::api::GOPtr>::const_iterator it = v.begin(); it != v.end(); ++it) {
			l.append(*it);
		}
		return l;
	}

	i6e::api::GOPtr getObject(const int64_t id) {
		return i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(id);
	}

	boost::python::list getGOList() {
		boost::python::list l;
		std::unordered_map<int64_t, i6e::api::GOPtr> v = i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->getGOMap();
		for (std::unordered_map<int64_t, i6e::api::GOPtr>::const_iterator it = v.begin(); it != v.end(); ++it) {
			l.append(it->second);
		}
		return l;
	}

	void deleteAllObjectsOfType(const std::string & types) {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->deleteAllObjectsOfType(types);
	}

	void createObject(const std::string & gTemplate, const i6e::api::objects::GOTemplate & tmpl, uint32_t uuid, const bool sender) {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->createObject(gTemplate, tmpl, uuid, sender);
	}

	void cleanUpAll() {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->cleanUpAll();
	}

	void loadLevel(const std::string & file, const std::string & flags) {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->loadLevel(file, flags);
	}

	void loadLevel(const std::string & file, const std::string & flags, const std::string & resourcesFile) {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->loadLevel(file, flags, resourcesFile);
	}

	void loadLevelCallbackFunc(const std::string & file, const std::string & flags, const std::string & func) {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->loadLevel(file, flags, [func](uint16_t value) {
			i6e::api::EngineController::GetSingletonPtr()->getScriptingFacade()->callFunction<void>(func, value);
		});
	}

	void loadLevelCallbackScript(const std::string & file, const std::string & flags, const std::string & script, const std::string & func) {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->loadLevel(file, flags, [script, func](uint16_t value) {
			i6e::api::EngineController::GetSingletonPtr()->getScriptingFacade()->callScript<void>(script, func, value);
		});
	}

	void loadLevelCallbackFunc(const std::string & file, const std::string & flags, const std::string & resourcesFile, const std::string & func) {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->loadLevel(file, flags, resourcesFile, [func](uint16_t value) {
			i6e::api::EngineController::GetSingletonPtr()->getScriptingFacade()->callFunction<void>(func, value);
		});
	}

	void loadLevelCallbackScript(const std::string & file, const std::string & flags, const std::string & resourcesFile, const std::string & script, const std::string & func) {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->loadLevel(file, flags, resourcesFile, [script, func](uint16_t value) {
			i6e::api::EngineController::GetSingletonPtr()->getScriptingFacade()->callScript<void>(script, func, value);
		});
	}

	uint32_t getFrameTime() {
		return i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->getFrameTime();
	}

	void createGO(const std::string & gTemplate, const i6e::api::objects::GOTemplate & tmpl, uint32_t uuid, const bool sender, const std::string & func) {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->createGO(gTemplate, tmpl, uuid, sender, [func](i6e::api::GOPtr go) {
			i6e::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func, go);
		});
	}

	void createComponent(int64_t goid, int64_t coid, const std::string & component, const i6e::api::attributeMap & params) {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->createComponent(goid, coid, component, params);
	}

	void createComponentCallback(int64_t goid, int64_t coid, const std::string & component, const i6e::api::attributeMap & params, const std::string & script, const std::string & func) {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->createComponentCallback(goid, coid, component, params, [script, func](i6e::api::ComPtr c) {
			i6e::api::EngineController::GetSingleton().getScriptingFacade()->callScript<void>(script, func, c);
		});
	}

	void createComponentCallback(int64_t goid, int64_t coid, const std::string & component, const i6e::api::attributeMap & params, const std::string & func) {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->createComponentCallback(goid, coid, component, params, [func](i6e::api::ComPtr c) {
			i6e::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func, c);
		});
	}

	void resetObjectSubSystem() {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->resetSubSystem();
	}

	void pauseObject() {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->pause();
	}

	void unpauseObject() {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->unpause();
	}

	void rayTest(i6e::api::PhysicalStateComponent * c, const Vec3 & from, const Vec3 & to, i6e::api::PhysicalStateComponent::RayTestRepetition rtr, i6e::api::PhysicalStateComponent::RayTestNotify rtn, const std::string & script, const std::string & func, const int64_t rayID) {
		c->rayTest(from, to, rtr, rtn, boost::make_shared<i6e::api::GameMessage>(i6e::api::messages::ScriptingMessageType, i6e::api::scripting::ScrRayResult, i6e::core::Method::Update, new i6e::api::scripting::Scripting_RayResult_Update(script, func, rayID), i6e::core::Subsystem::Unknown));
	}

	void insertPairInAttributeMap(i6e::api::attributeMap * am, const std::string & key, const std::string & value) {
		am->insert(std::make_pair(key, value));
	}

	struct ComponentWrapper : public i6e::api::Component, public boost::python::wrapper<i6e::api::Component> {
		ComponentWrapper(const int64_t id, const i6e::api::attributeMap & params) : Component(id, params), boost::python::wrapper<i6e::api::Component>() {
		}

		ComponentWrapper(const i6e::api::Component & arg) : i6e::api::Component(), boost::python::wrapper<i6e::api::Component>() {
		}

		virtual void Tick() override {
			if (boost::python::override t = this->get_override("Tick")) {
				boost::python::call<void>(t.ptr());
				return;
			}
			Component::Tick();
		}

		void default_Tick() {
			this->Component::Tick();
		}

		virtual void News(const i6e::api::GameMessage::Ptr & msg) override {
			if (boost::python::override n = this->get_override("News")) {
				boost::python::call<void>(n.ptr(), msg);
				return;
			}
			Component::News(msg);
		}

		void default_News(const i6e::api::GameMessage::Ptr & msg) {
			this->Component::News(msg);
		}

		virtual void Init() override {
			boost::python::call<void>(this->get_override("Init").ptr());
		}

		virtual void Finalize() override {
			if (boost::python::override f = this->get_override("Finalize")) {
				boost::python::call<void>(f.ptr());
				return;
			}
			Component::Finalize();
		}

		void default_Finalize() {
			this->Component::Finalize();
		}

		virtual i6e::api::attributeMap synchronize() const {
			return boost::python::call<i6e::api::attributeMap>(this->get_override("synchronize").ptr());
		}

		virtual std::pair<i6e::api::AddStrategy, int64_t> howToAdd(const i6e::api::ComPtr & comp) const override {
			if (boost::python::override hta = this->get_override("howToAdd")) {
				return boost::python::call<std::pair<i6e::api::AddStrategy, int64_t>>(hta.ptr(), comp);
			}
			return Component::howToAdd(comp);
		}

		std::pair<i6e::api::AddStrategy, int64_t> default_howToAdd(const i6e::api::ComPtr & comp) {
			return this->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return boost::python::call<std::string>(this->get_override("getTemplateName").ptr());
		}

		std::vector<i6e::api::componentOptions> getComponentOptions() {
			return {};
		}

		void addTicker() {
			Component::addTicker();
		}

		void removeTicker() {
			Component::removeTicker();
		}
	};

	i6e::utils::sharedPtr<i6e::api::AnimatedDirectionalLightComponent, i6e::api::Component> getAnimatedDirectionalLightComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::AnimatedDirectionalLightComponent>(i6e::api::components::ComponentTypes::AnimatedDirectionalLightComponent);
	}

	i6e::utils::sharedPtr<i6e::api::AnimatedDirectionalLightComponent, i6e::api::Component> getAnimatedDirectionalLightComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::AnimatedDirectionalLightComponent>(i6e::api::components::ComponentTypes::AnimatedDirectionalLightComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::AnimatedLuminousAppearanceComponent, i6e::api::Component> getAnimatedLuminousAppearanceComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::AnimatedLuminousAppearanceComponent>(i6e::api::components::ComponentTypes::AnimatedLuminousAppearanceComponent);
	}

	i6e::utils::sharedPtr<i6e::api::AnimatedLuminousAppearanceComponent, i6e::api::Component> getAnimatedLuminousAppearanceComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::AnimatedLuminousAppearanceComponent>(i6e::api::components::ComponentTypes::AnimatedLuminousAppearanceComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::AnimatedSpotLightComponent, i6e::api::Component> getAnimatedSpotLightComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::AnimatedSpotLightComponent>(i6e::api::components::ComponentTypes::AnimatedSpotLightComponent);
	}

	i6e::utils::sharedPtr<i6e::api::AnimatedSpotLightComponent, i6e::api::Component> getAnimatedSpotLightComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::AnimatedSpotLightComponent>(i6e::api::components::ComponentTypes::AnimatedSpotLightComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::AnimationControllerComponent, i6e::api::Component> getAnimationControllerComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::AnimationControllerComponent>(i6e::api::components::ComponentTypes::AnimationControllerComponent);
	}

	i6e::utils::sharedPtr<i6e::api::AnimationControllerComponent, i6e::api::Component> getAnimationControllerComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::AnimationControllerComponent>(i6e::api::components::ComponentTypes::AnimationControllerComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::CameraComponent, i6e::api::Component> getCameraComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::CameraComponent>(i6e::api::components::ComponentTypes::CameraComponent);
	}

	i6e::utils::sharedPtr<i6e::api::CameraComponent, i6e::api::Component> getCameraComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::CameraComponent>(i6e::api::components::ComponentTypes::CameraComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::LifetimeComponent, i6e::api::Component> getLifetimeComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::LifetimeComponent>(i6e::api::components::ComponentTypes::LifetimeComponent);
	}

	i6e::utils::sharedPtr<i6e::api::LifetimeComponent, i6e::api::Component> getLifetimeComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::LifetimeComponent>(i6e::api::components::ComponentTypes::LifetimeComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::LuminousAppearanceComponent, i6e::api::Component> getLuminousAppearanceComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::LuminousAppearanceComponent>(i6e::api::components::ComponentTypes::LuminousAppearanceComponent);
	}

	i6e::utils::sharedPtr<i6e::api::LuminousAppearanceComponent, i6e::api::Component> getLuminousAppearanceComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::LuminousAppearanceComponent>(i6e::api::components::ComponentTypes::LuminousAppearanceComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::MeshAppearanceComponent, i6e::api::Component> getMeshAppearanceComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::MeshAppearanceComponent>(i6e::api::components::ComponentTypes::MeshAppearanceComponent);
	}

	i6e::utils::sharedPtr<i6e::api::MeshAppearanceComponent, i6e::api::Component> getMeshAppearanceComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::MeshAppearanceComponent>(i6e::api::components::ComponentTypes::MeshAppearanceComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::MoverCircleComponent, i6e::api::Component> getMoverCircleComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::MoverCircleComponent>(i6e::api::components::ComponentTypes::MoverCircleComponent);
	}

	i6e::utils::sharedPtr<i6e::api::MoverCircleComponent, i6e::api::Component> getMoverCircleComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::MoverCircleComponent>(i6e::api::components::ComponentTypes::MoverCircleComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::MoverComponent, i6e::api::Component> getMoverComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::MoverComponent>(i6e::api::components::ComponentTypes::MoverComponent);
	}

	i6e::utils::sharedPtr<i6e::api::MoverComponent, i6e::api::Component> getMoverComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::MoverComponent>(i6e::api::components::ComponentTypes::MoverComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::MoverInterpolateComponent, i6e::api::Component> getMoverInterpolateComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::MoverInterpolateComponent>(i6e::api::components::ComponentTypes::MoverInterpolateComponent);
	}

	i6e::utils::sharedPtr<i6e::api::MoverInterpolateComponent, i6e::api::Component> getMoverInterpolateComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::MoverInterpolateComponent>(i6e::api::components::ComponentTypes::MoverInterpolateComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::MovingCameraComponent, i6e::api::Component> getMovingCameraComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::MovingCameraComponent>(i6e::api::components::ComponentTypes::MovingCameraComponent);
	}

	i6e::utils::sharedPtr<i6e::api::MovingCameraComponent, i6e::api::Component> getMovingCameraComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::MovingCameraComponent>(i6e::api::components::ComponentTypes::MovingCameraComponent, identifier);
	}

#ifdef ISIXE_WITH_NETWORK
	i6e::utils::sharedPtr<i6e::api::NetworkSenderComponent, i6e::api::Component> getNetworkSenderComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::NetworkSenderComponent>(i6e::api::components::ComponentTypes::NetworkSenderComponent);
	}

	i6e::utils::sharedPtr<i6e::api::NetworkSenderComponent, i6e::api::Component> getNetworkSenderComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::NetworkSenderComponent>(i6e::api::components::ComponentTypes::NetworkSenderComponent, identifier);
	}
#endif

	i6e::utils::sharedPtr<i6e::api::ParticleEmitterComponent, i6e::api::Component> getParticleEmitterComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::ParticleEmitterComponent>(i6e::api::components::ComponentTypes::ParticleEmitterComponent);
	}

	i6e::utils::sharedPtr<i6e::api::ParticleEmitterComponent, i6e::api::Component> getParticleEmitterComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::ParticleEmitterComponent>(i6e::api::components::ComponentTypes::ParticleEmitterComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::PhysicalStateComponent, i6e::api::Component> getPhysicalStateComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::PhysicalStateComponent>(i6e::api::components::ComponentTypes::PhysicalStateComponent);
	}

	i6e::utils::sharedPtr<i6e::api::PhysicalStateComponent, i6e::api::Component> getPhysicalStateComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::PhysicalStateComponent>(i6e::api::components::ComponentTypes::PhysicalStateComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::ShatterComponent, i6e::api::Component> getShatterComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::ShatterComponent>(i6e::api::components::ComponentTypes::ShatterComponent);
	}

	i6e::utils::sharedPtr<i6e::api::ShatterComponent, i6e::api::Component> getShatterComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::ShatterComponent>(i6e::api::components::ComponentTypes::ShatterComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::SpawnpointComponent, i6e::api::Component> getSpawnpointComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::SpawnpointComponent>(i6e::api::components::ComponentTypes::SpawnpointComponent);
	}

	i6e::utils::sharedPtr<i6e::api::SpawnpointComponent, i6e::api::Component> getSpawnpointComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::SpawnpointComponent>(i6e::api::components::ComponentTypes::SpawnpointComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component> getStaticStateComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::StaticStateComponent>(i6e::api::components::ComponentTypes::StaticStateComponent);
	}

	i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component> getStaticStateComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::StaticStateComponent>(i6e::api::components::ComponentTypes::StaticStateComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::TerrainAppearanceComponent, i6e::api::Component> getTerrainAppearanceComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::TerrainAppearanceComponent>(i6e::api::components::ComponentTypes::TerrainAppearanceComponent);
	}

	i6e::utils::sharedPtr<i6e::api::TerrainAppearanceComponent, i6e::api::Component> getTerrainAppearanceComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::TerrainAppearanceComponent>(i6e::api::components::ComponentTypes::TerrainAppearanceComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::SoundComponent, i6e::api::Component> getSoundComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::SoundComponent>(i6e::api::components::ComponentTypes::SoundComponent);
	}

	i6e::utils::sharedPtr<i6e::api::SoundComponent, i6e::api::Component> getSoundComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::SoundComponent>(i6e::api::components::ComponentTypes::SoundComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::SoundListenerComponent, i6e::api::Component> getSoundListenerComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::SoundListenerComponent>(i6e::api::components::ComponentTypes::SoundListenerComponent);
	}

	i6e::utils::sharedPtr<i6e::api::SoundListenerComponent, i6e::api::Component> getSoundListenerComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::SoundListenerComponent>(i6e::api::components::ComponentTypes::SoundListenerComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::BillboardComponent, i6e::api::Component> getBillboardComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::BillboardComponent>(i6e::api::components::ComponentTypes::BillboardComponent);
	}

	i6e::utils::sharedPtr<i6e::api::BillboardComponent, i6e::api::Component> getBillboardComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::BillboardComponent>(i6e::api::components::ComponentTypes::BillboardComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::FollowComponent, i6e::api::Component> getFollowComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::FollowComponent>(i6e::api::components::ComponentTypes::FollowComponent);
	}

	i6e::utils::sharedPtr<i6e::api::FollowComponent, i6e::api::Component> getFollowComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::FollowComponent>(i6e::api::components::ComponentTypes::FollowComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::MovableTextComponent, i6e::api::Component> getMovableTextComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::MovableTextComponent>(i6e::api::components::ComponentTypes::MovableTextComponent);
	}

	i6e::utils::sharedPtr<i6e::api::MovableTextComponent, i6e::api::Component> getMovableTextComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::MovableTextComponent>(i6e::api::components::ComponentTypes::MovableTextComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::WaypointComponent, i6e::api::Component> getWaypointComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::WaypointComponent>(i6e::api::components::ComponentTypes::WaypointComponent);
	}

	i6e::utils::sharedPtr<i6e::api::WaypointComponent, i6e::api::Component> getWaypointComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::WaypointComponent>(i6e::api::components::ComponentTypes::WaypointComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::NavigationComponent, i6e::api::Component> getNavigationComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::NavigationComponent>(i6e::api::components::ComponentTypes::NavigationComponent);
	}

	i6e::utils::sharedPtr<i6e::api::NavigationComponent, i6e::api::Component> getNavigationComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::NavigationComponent>(i6e::api::components::ComponentTypes::NavigationComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::WaynetNavigationComponent, i6e::api::Component> getWaynetNavigationComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::WaynetNavigationComponent>(i6e::api::components::ComponentTypes::WaynetNavigationComponent);
	}

	i6e::utils::sharedPtr<i6e::api::WaynetNavigationComponent, i6e::api::Component> getWaynetNavigationComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::WaynetNavigationComponent>(i6e::api::components::ComponentTypes::WaynetNavigationComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::MoveComponent, i6e::api::Component> getMoveComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::MoveComponent>(i6e::api::components::ComponentTypes::MoveComponent);
	}

	i6e::utils::sharedPtr<i6e::api::MoveComponent, i6e::api::Component> getMoveComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::MoveComponent>(i6e::api::components::ComponentTypes::MoveComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::MovementComponent, i6e::api::Component> getMovementComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::MovementComponent>(i6e::api::components::ComponentTypes::MovementComponent);
	}

	i6e::utils::sharedPtr<i6e::api::MovementComponent, i6e::api::Component> getMovementComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::MovementComponent>(i6e::api::components::ComponentTypes::MovementComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::ToggleWaynetComponent, i6e::api::Component> getToggleWaynetComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::ToggleWaynetComponent>(i6e::api::components::ComponentTypes::ToggleWaynetComponent);
	}

	i6e::utils::sharedPtr<i6e::api::ToggleWaynetComponent, i6e::api::Component> getToggleWaynetComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::ToggleWaynetComponent>(i6e::api::components::ComponentTypes::ToggleWaynetComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::Point2PointConstraintComponent, i6e::api::Component> getPoint2PointConstraintComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::Point2PointConstraintComponent>(i6e::api::components::ComponentTypes::Point2PointConstraintComponent);
	}

	i6e::utils::sharedPtr<i6e::api::Point2PointConstraintComponent, i6e::api::Component> getPoint2PointConstraintComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::Point2PointConstraintComponent>(i6e::api::components::ComponentTypes::Point2PointConstraintComponent, identifier);
	}

	i6e::utils::sharedPtr<i6e::api::VelocityComponent, i6e::api::Component> getVelocityComponent(i6e::api::GameObject * go) {
		return go->getGOC<i6e::api::VelocityComponent>(i6e::api::components::ComponentTypes::VelocityComponent);
	}

	i6e::utils::sharedPtr<i6e::api::VelocityComponent, i6e::api::Component> getVelocityComponent(i6e::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6e::api::VelocityComponent>(i6e::api::components::ComponentTypes::VelocityComponent, identifier);
	}

	struct CameraComponentWrapper : public i6e::api::CameraComponent, public boost::python::wrapper<i6e::api::CameraComponent> {
		CameraComponentWrapper() : CameraComponent(-1, i6e::api::attributeMap()), boost::python::wrapper<i6e::api::CameraComponent>() {
		}

		CameraComponentWrapper(const int64_t id, const i6e::api::attributeMap & params) : CameraComponent(id, params), boost::python::wrapper<i6e::api::CameraComponent>() {
		}

		CameraComponentWrapper(const i6e::api::CameraComponent & arg) : i6e::api::CameraComponent(-1, i6e::api::attributeMap()), boost::python::wrapper<i6e::api::CameraComponent>() {
		}

		virtual void Tick() override {
			if (boost::python::override t = this->get_override("Tick")) {
				boost::python::call<void>(t.ptr());
				return;
			}
			Component::Tick();
		}

		void default_Tick() {
			this->Component::Tick();
		}

		virtual void News(const i6e::api::GameMessage::Ptr & msg) override {
			if (boost::python::override n = this->get_override("News")) {
				boost::python::call<void>(n.ptr(), msg);
				return;
			}
			CameraComponent::News(msg);
		}

		void default_News(const i6e::api::GameMessage::Ptr & msg) {
			this->CameraComponent::News(msg);
		}

		virtual void Init() override {
			CameraComponent::Init();
			boost::python::call<void>(this->get_override("Init").ptr());
		}

		virtual void Finalize() override {
			CameraComponent::Finalize();
			if (boost::python::override f = this->get_override("Finalize")) {
				boost::python::call<void>(f.ptr());
				return;
			}
		}

		void default_Finalize() {
			this->CameraComponent::Finalize();
		}

		virtual i6e::api::attributeMap synchronize() const {
			i6e::api::attributeMap params = CameraComponent::synchronize();
			i6e::api::attributeMap params2 = boost::python::call<i6e::api::attributeMap>(this->get_override("synchronize").ptr());
			params.insert(params2.begin(), params2.end());
			return params;
		}

		virtual std::pair<i6e::api::AddStrategy, int64_t> howToAdd(const i6e::api::ComPtr & comp) const override {
			return boost::python::call<std::pair<i6e::api::AddStrategy, int64_t>>(this->get_override("howToAdd").ptr(), comp);
		}

		std::pair<i6e::api::AddStrategy, int64_t> default_howToAdd(const i6e::api::ComPtr & comp) {
			return this->CameraComponent::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return boost::python::call<std::string>(this->get_override("getTemplateName").ptr());
		}

		std::vector<i6e::api::componentOptions> getComponentOptions() {
			return {};
		}

		void addTicker() {
			Component::addTicker();
		}

		void removeTicker() {
			Component::removeTicker();
		}
	};

	struct MovementComponentWrapper : public i6e::api::MovementComponent, public boost::python::wrapper<i6e::api::MovementComponent> {
		MovementComponentWrapper(const int64_t id, const i6e::api::attributeMap & params) : MovementComponent(id, params), boost::python::wrapper<i6e::api::MovementComponent>() {
		}

		MovementComponentWrapper(const i6e::api::MovementComponent & arg) : i6e::api::MovementComponent(-1, i6e::api::attributeMap()), boost::python::wrapper<i6e::api::MovementComponent>() {
		}

		virtual void Tick() override {
			MovementComponent::Tick();
			if (boost::python::override t = this->get_override("Tick")) {
				boost::python::call<void>(t.ptr());
				return;
			}
		}

		void default_Tick() {
			this->MovementComponent::Tick();
		}

		virtual void News(const i6e::api::GameMessage::Ptr & msg) override {
			if (boost::python::override n = this->get_override("News")) {
				return boost::python::call<void>(n.ptr(), msg);
			}
			Component::News(msg);
		}

		void default_News(const i6e::api::GameMessage::Ptr & msg) {
			this->Component::News(msg);
		}

		virtual void Init() override {
			MovementComponent::Init();
			if (boost::python::override i = this->get_override("Init")) {
				boost::python::call<void>(i.ptr());
				return;
			}
		}

		void default_Init() {
			this->MovementComponent::Init();
		}

		virtual void Finalize() override {
			MovementComponent::Finalize();
			if (boost::python::override f = this->get_override("Finalize")) {
				boost::python::call<void>(f.ptr());
				return;
			}
		}

		void default_Finalize() {
			this->MovementComponent::Finalize();
		}

		virtual i6e::api::attributeMap synchronize() const {
			return boost::python::call<i6e::api::attributeMap>(this->get_override("synchronize").ptr());
		}

		virtual std::pair<i6e::api::AddStrategy, int64_t> howToAdd(const i6e::api::ComPtr & comp) const override {
			return boost::python::call<std::pair<i6e::api::AddStrategy, int64_t>>(this->get_override("howToAdd").ptr(), comp);
		}

		std::pair<i6e::api::AddStrategy, int64_t> default_howToAdd(const i6e::api::ComPtr & comp) {
			return this->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return boost::python::call<std::string>(this->get_override("getTemplateName").ptr());
		}

		std::vector<i6e::api::componentOptions> getComponentOptions() {
			return {};
		}

		virtual void forward() override {
			boost::python::call<void>(this->get_override("forward").ptr());
		}

		virtual void backward() override {
			boost::python::call<void>(this->get_override("backward").ptr());
		}

		virtual void left() override {
			boost::python::call<void>(this->get_override("left").ptr());
		}

		virtual void right() override {
			boost::python::call<void>(this->get_override("right").ptr());
		}

		virtual void stop() override {
			boost::python::call<void>(this->get_override("stop").ptr());
		}
	};

	struct MoverComponentWrapper : public i6e::api::MoverComponent, public boost::python::wrapper<i6e::api::MoverComponent> {
		MoverComponentWrapper(const int64_t id, const i6e::api::attributeMap & params) : MoverComponent(id, params), boost::python::wrapper<i6e::api::MoverComponent>() {
		}

		MoverComponentWrapper(const i6e::api::MoverComponent & arg) : i6e::api::MoverComponent(-1, i6e::api::attributeMap()), boost::python::wrapper<i6e::api::MoverComponent>() {
		}

		virtual void Tick() override {
			if (boost::python::override t = this->get_override("Tick")) {
				boost::python::call<void>(t.ptr());
				return;
			}
			MoverComponent::Tick();
		}

		void default_Tick() {
			this->MoverComponent::Tick();
		}

		virtual void News(const i6e::api::GameMessage::Ptr & msg) override {
			boost::python::call<void>(this->get_override("News").ptr(), msg);
		}

		virtual void Init() override {
			if (boost::python::override i = this->get_override("Init")) {
				boost::python::call<void>(i.ptr());
				return;
			}
			MoverComponent::Init();
		}

		void default_Init() {
			this->MoverComponent::Init();
		}

		virtual void Finalize() override {
			if (boost::python::override f = this->get_override("Finalize")) {
				boost::python::call<void>(f.ptr());
				return;
			}
			MoverComponent::Finalize();
		}

		void default_Finalize() {
			this->MoverComponent::Finalize();
		}

		virtual i6e::api::attributeMap synchronize() const {
			i6e::api::attributeMap params = MoverComponent::synchronize();
			i6e::api::attributeMap params2 = boost::python::call<i6e::api::attributeMap>(this->get_override("synchronize").ptr());
			params.insert(params2.begin(), params2.end());
			return params;
		}

		virtual std::pair<i6e::api::AddStrategy, int64_t> howToAdd(const i6e::api::ComPtr & comp) const override {
			if (boost::python::override hta = this->get_override("howToAdd")) {
				return boost::python::call<std::pair<i6e::api::AddStrategy, int64_t>>(hta.ptr(), comp);
			}
			return MoverComponent::howToAdd(comp);
		}

		std::pair<i6e::api::AddStrategy, int64_t> default_howToAdd(const i6e::api::ComPtr & comp) {
			return this->MoverComponent::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return boost::python::call<std::string>(this->get_override("getTemplateName").ptr());
		}

		std::vector<i6e::api::componentOptions> getComponentOptions() {
			return {};
		}

		void start(Vec3 & startPos) override {
			boost::python::call<void>(this->get_override("start").ptr(), startPos);
		}

		virtual void reset() override {
			boost::python::call<void>(this->get_override("reset").ptr());
		}

		void getNewPosition(const uint64_t time, Vec3 & pos, Quaternion & rot) override {
			boost::python::call<void>(this->get_override("getNewPosition").ptr(), time, pos, rot);
		}
	};

	struct NavigationComponentWrapper : public i6e::api::NavigationComponent, public boost::python::wrapper<i6e::api::NavigationComponent> {
		NavigationComponentWrapper(const int64_t id, const i6e::api::attributeMap & params) : NavigationComponent(id, params), boost::python::wrapper<i6e::api::NavigationComponent>() {
		}

		NavigationComponentWrapper(const i6e::api::NavigationComponent & arg) : i6e::api::NavigationComponent(-1, i6e::api::attributeMap()), boost::python::wrapper<i6e::api::NavigationComponent>() {
		}

		virtual void Tick() override {
			if (boost::python::override t = this->get_override("Tick")) {
				boost::python::call<void>(t.ptr());
				return;
			}
			Component::Tick();
		}

		void default_Tick() {
			this->Component::Tick();
		}

		virtual void News(const i6e::api::GameMessage::Ptr & msg) override {
			if (boost::python::override n = this->get_override("News")) {
				return boost::python::call<void>(n.ptr(), msg);
			}
			Component::News(msg);
		}

		void default_News(const i6e::api::GameMessage::Ptr & msg) {
			this->Component::News(msg);
		}

		virtual void Init() override {
			boost::python::call<void>(this->get_override("Init").ptr());
		}

		virtual void Finalize() override {
			if (boost::python::override f = this->get_override("Finalize")) {
				boost::python::call<void>(f.ptr());
				return;
			}
			Component::Finalize();
		}

		void default_Finalize() {
			this->NavigationComponent::Finalize();
		}

		virtual i6e::api::attributeMap synchronize() const {
			return boost::python::call<i6e::api::attributeMap>(this->get_override("synchronize").ptr());
		}

		virtual std::pair<i6e::api::AddStrategy, int64_t> howToAdd(const i6e::api::ComPtr & comp) const override {
			return boost::python::call<std::pair<i6e::api::AddStrategy, int64_t>>(this->get_override("howToAdd").ptr(), comp);
		}

		std::pair<i6e::api::AddStrategy, int64_t> default_howToAdd(const i6e::api::ComPtr & comp) {
			return this->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return boost::python::call<std::string>(this->get_override("getTemplateName").ptr());
		}

		std::vector<i6e::api::componentOptions> getComponentOptions() {
			return {};
		}

		std::vector<Vec3> getPath(const Vec3 & from, const Vec3 & to) const override {
			return boost::python::call<std::vector<Vec3>>(this->get_override("getPathPos").ptr(), from, to);
		}

		std::vector<Vec3> getPath(const Vec3 & from, const std::string & to) const override {
			return boost::python::call<std::vector<Vec3>>(this->get_override("getPathWP").ptr(), from, to);
		}
	};

	struct ShatterComponentWrapper : public i6e::api::ShatterComponent, public boost::python::wrapper<i6e::api::ShatterComponent> {
		ShatterComponentWrapper(const int64_t id, const i6e::api::attributeMap & params) : ShatterComponent(id, params), boost::python::wrapper<i6e::api::ShatterComponent>() {
		}

		ShatterComponentWrapper(const i6e::api::NavigationComponent & arg) : i6e::api::ShatterComponent(-1, i6e::api::attributeMap()), boost::python::wrapper<i6e::api::ShatterComponent>() {
		}

		virtual void Tick() override {
			if (boost::python::override t = this->get_override("Tick")) {
				boost::python::call<void>(t.ptr());
				return;
			}
			Component::Tick();
		}

		void default_Tick() {
			this->Component::Tick();
		}

		virtual void News(const i6e::api::GameMessage::Ptr & msg) override {
			if (boost::python::override n = this->get_override("News")) {
				boost::python::call<void>(n.ptr(), msg);
				return;
			}
			ShatterComponent::News(msg);
		}

		void default_News(const i6e::api::GameMessage::Ptr & msg) {
			this->ShatterComponent::News(msg);
		}

		virtual void Init() override {
			if (boost::python::override i = this->get_override("Init")) {
				boost::python::call<void>(i.ptr());
				return;
			}
			Component::Init();
		}

		void default_Init() {
			this->ShatterComponent::Init();
		}

		virtual void Finalize() override {
			if (boost::python::override f = this->get_override("Finalize")) {
				boost::python::call<void>(f.ptr());
				return;
			}
			Component::Finalize();
		}

		void default_Finalize() {
			this->Component::Finalize();
		}

		virtual i6e::api::attributeMap synchronize() const {
			return boost::python::call<i6e::api::attributeMap>(this->get_override("synchronize").ptr());
		}

		virtual std::pair<i6e::api::AddStrategy, int64_t> howToAdd(const i6e::api::ComPtr & comp) const override {
			return boost::python::call<std::pair<i6e::api::AddStrategy, int64_t>>(this->get_override("howToAdd").ptr(), comp);
		}

		std::pair<i6e::api::AddStrategy, int64_t> default_howToAdd(const i6e::api::ComPtr & comp) {
			return this->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return boost::python::call<std::string>(this->get_override("getTemplateName").ptr());
		}

		std::vector<i6e::api::componentOptions> getComponentOptions() {
			return {};
		}

		void shatter(const i6e::api::GOPtr & other) override {
			boost::python::call<void>(this->get_override("shatter").ptr(), other);
		}
	};

	void accelerate(i6e::api::VelocityComponent * c, const Vec3 & acceleration, i6e::api::VelocityComponent::MaxSpeedHandling handling, const std::string & func) {
		c->accelerate(acceleration, handling, [func]() {
			if (!func.empty()) {
				i6e::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func);
			}
		});
	}

	void accelerate(i6e::api::VelocityComponent * c, const std::string & func) {
		c->accelerate([func]() {
			if (!func.empty()) {
				i6e::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func);
			}
		});
	}

	void decelerate(i6e::api::VelocityComponent * c, const Vec3 & deceleration, i6e::api::VelocityComponent::DecelerationHandling handling, const std::string & func) {
		c->decelerate(deceleration, handling, [func]() {
			if (!func.empty()) {
				i6e::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func);
			}
		});
	}

	void decelerate(i6e::api::VelocityComponent * c, const std::string & func) {
		c->decelerate([func]() {
			if (!func.empty()) {
				i6e::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func);
			}
		});
	}

	void addAnimationFrameEvent(i6e::api::MeshAppearanceComponent * c, uint64_t frameTime, const std::string & func) {
		c->addAnimationFrameEvent(frameTime, [func]() {
			if (!func.empty()) {
				i6e::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func);
			}
		});
	}

	void addAnimationFrameEvent(i6e::api::MeshAppearanceComponent * c, uint64_t frameTime, const std::string & script, const std::string & func) {
		c->addAnimationFrameEvent(frameTime, [script, func]() {
			if (!script.empty() && !func.empty()) {
				i6e::api::EngineController::GetSingleton().getScriptingFacade()->callScript<void>(script, func);
			}
		});
	}

	void addAnimationFrameEvent(i6e::api::AnimationControllerComponent * c, const std::string & animation, uint64_t frameTime, const std::string & func) {
		c->addAnimationFrameEvent(animation, frameTime, [func]() {
			if (!func.empty()) {
				i6e::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func);
			}
		});
	}

	void addAnimationFrameEvent(i6e::api::AnimationControllerComponent * c, const std::string & animation, uint64_t frameTime, const std::string & script, const std::string & func) {
		c->addAnimationFrameEvent(animation, frameTime, [script, func]() {
			if (!script.empty() && !func.empty()) {
				i6e::api::EngineController::GetSingleton().getScriptingFacade()->callScript<void>(script, func);
			}
		});
	}

} /* namespace object */
} /* namespace python */
} /* namespace i6e */

namespace boost {
namespace python {

	template<typename T, typename U>
	struct pointee<i6e::utils::sharedPtr<T, U>> {
		typedef T type;
	};

} /* namespace python */
} /* namespace boost */

BOOST_PYTHON_MODULE(ScriptingObjectPython) {
	using namespace boost::python;

	class_<i6e::api::GameObject, i6e::api::GOPtr, boost::noncopyable>("GameObject", no_init)
		.def("getID", &i6e::api::GameObject::getID)
		.def("getGOC", (i6e::api::ComPtr(i6e::api::GameObject::*)(uint32_t) const) &i6e::api::GameObject::getGOC)
		.def("getGOC", (i6e::api::ComPtr(i6e::api::GameObject::*)(uint32_t, const std::string &) const) &i6e::api::GameObject::getGOC)
		.def("getGOCID", &i6e::api::GameObject::getGOCID)
		.def("getGOCList", &i6e::api::GameObject::getGOCList)
		.def("getType", &i6e::api::GameObject::getType)
		.def("setDie", &i6e::api::GameObject::setDie)
		.def("getOwner", &i6e::api::GameObject::getOwner)
		.def("getUUID", &i6e::api::GameObject::getUUID)
		.def("getAnimatedDirectionalLightComponent", (i6e::utils::sharedPtr<i6e::api::AnimatedDirectionalLightComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getAnimatedDirectionalLightComponent)
		.def("getAnimatedDirectionalLightComponent", (i6e::utils::sharedPtr<i6e::api::AnimatedDirectionalLightComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getAnimatedDirectionalLightComponent)
		.def("getAnimatedLuminousAppearanceComponent", (i6e::utils::sharedPtr<i6e::api::AnimatedLuminousAppearanceComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getAnimatedLuminousAppearanceComponent)
		.def("getAnimatedLuminousAppearanceComponent", (i6e::utils::sharedPtr<i6e::api::AnimatedLuminousAppearanceComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getAnimatedLuminousAppearanceComponent)
		.def("getAnimatedSpotLightComponent", (i6e::utils::sharedPtr<i6e::api::AnimatedSpotLightComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getAnimatedSpotLightComponent)
		.def("getAnimatedSpotLightComponent", (i6e::utils::sharedPtr<i6e::api::AnimatedSpotLightComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getAnimatedSpotLightComponent)
		.def("getAnimationControllerComponent", (i6e::utils::sharedPtr<i6e::api::AnimationControllerComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getAnimationControllerComponent)
		.def("getAnimationControllerComponent", (i6e::utils::sharedPtr<i6e::api::AnimationControllerComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getAnimationControllerComponent)
		.def("getCameraComponent", (i6e::utils::sharedPtr<i6e::api::CameraComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getCameraComponent)
		.def("getCameraComponent", (i6e::utils::sharedPtr<i6e::api::CameraComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getCameraComponent)
		.def("getLifetimeComponent", (i6e::utils::sharedPtr<i6e::api::LifetimeComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getLifetimeComponent)
		.def("getLifetimeComponent", (i6e::utils::sharedPtr<i6e::api::LifetimeComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getLifetimeComponent)
		.def("getLuminousAppearanceComponent", (i6e::utils::sharedPtr<i6e::api::LuminousAppearanceComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getLuminousAppearanceComponent)
		.def("getLuminousAppearanceComponent", (i6e::utils::sharedPtr<i6e::api::LuminousAppearanceComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getLuminousAppearanceComponent)
		.def("getMeshAppearanceComponent", (i6e::utils::sharedPtr<i6e::api::MeshAppearanceComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getMeshAppearanceComponent)
		.def("getMeshAppearanceComponent", (i6e::utils::sharedPtr<i6e::api::MeshAppearanceComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getMeshAppearanceComponent)
		.def("getMoverCircleComponent", (i6e::utils::sharedPtr<i6e::api::MoverCircleComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getMoverCircleComponent)
		.def("getMoverCircleComponent", (i6e::utils::sharedPtr<i6e::api::MoverCircleComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getMoverCircleComponent)
		.def("getMoverComponent", (i6e::utils::sharedPtr<i6e::api::MoverComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getMoverComponent)
		.def("getMoverComponent", (i6e::utils::sharedPtr<i6e::api::MoverComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getMoverComponent)
		.def("getMoverInterpolateComponent", (i6e::utils::sharedPtr<i6e::api::MoverInterpolateComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getMoverInterpolateComponent)
		.def("getMoverInterpolateComponent", (i6e::utils::sharedPtr<i6e::api::MoverInterpolateComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getMoverInterpolateComponent)
		.def("getMovingCameraComponent", (i6e::utils::sharedPtr<i6e::api::MovingCameraComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getMovingCameraComponent)
		.def("getMovingCameraComponent", (i6e::utils::sharedPtr<i6e::api::MovingCameraComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getMovingCameraComponent)
#ifdef ISIXE_WITH_NETWORK
		.def("getNetworkSenderComponent", (i6e::utils::sharedPtr<i6e::api::NetworkSenderComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getNetworkSenderComponent)
		.def("getNetworkSenderComponent", (i6e::utils::sharedPtr<i6e::api::NetworkSenderComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getNetworkSenderComponent)
#endif
		.def("getParticleEmitterComponent", (i6e::utils::sharedPtr<i6e::api::ParticleEmitterComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getParticleEmitterComponent)
		.def("getParticleEmitterComponent", (i6e::utils::sharedPtr<i6e::api::ParticleEmitterComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getParticleEmitterComponent)
		.def("getPhysicalStateComponent", (i6e::utils::sharedPtr<i6e::api::PhysicalStateComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getPhysicalStateComponent)
		.def("getPhysicalStateComponent", (i6e::utils::sharedPtr<i6e::api::PhysicalStateComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getPhysicalStateComponent)
		.def("getShatterComponent", (i6e::utils::sharedPtr<i6e::api::ShatterComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getShatterComponent)
		.def("getShatterComponent", (i6e::utils::sharedPtr<i6e::api::ShatterComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getShatterComponent)
		.def("getSpawnpointComponent", (i6e::utils::sharedPtr<i6e::api::SpawnpointComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getSpawnpointComponent)
		.def("getSpawnpointComponent", (i6e::utils::sharedPtr<i6e::api::SpawnpointComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getSpawnpointComponent)
		.def("getStaticStateComponent", (i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getStaticStateComponent)
		.def("getStaticStateComponent", (i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getStaticStateComponent)
		.def("getTerrainAppearanceComponent", (i6e::utils::sharedPtr<i6e::api::TerrainAppearanceComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getTerrainAppearanceComponent)
		.def("getTerrainAppearanceComponent", (i6e::utils::sharedPtr<i6e::api::TerrainAppearanceComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getTerrainAppearanceComponent)
		.def("getSoundComponent", (i6e::utils::sharedPtr<i6e::api::SoundComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getSoundComponent)
		.def("getSoundComponent", (i6e::utils::sharedPtr<i6e::api::SoundComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getSoundComponent)
		.def("getSoundListenerComponent", (i6e::utils::sharedPtr<i6e::api::SoundListenerComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getSoundListenerComponent)
		.def("getSoundListenerComponent", (i6e::utils::sharedPtr<i6e::api::SoundListenerComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getSoundListenerComponent)
		.def("getBillboardComponent", (i6e::utils::sharedPtr<i6e::api::BillboardComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getBillboardComponent)
		.def("getBillboardComponent", (i6e::utils::sharedPtr<i6e::api::BillboardComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getBillboardComponent)
		.def("getFollowComponent", (i6e::utils::sharedPtr<i6e::api::FollowComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getFollowComponent)
		.def("getFollowComponent", (i6e::utils::sharedPtr<i6e::api::FollowComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getFollowComponent)
		.def("getMovableTextComponent", (i6e::utils::sharedPtr<i6e::api::MovableTextComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getMovableTextComponent)
		.def("getMovableTextComponent", (i6e::utils::sharedPtr<i6e::api::MovableTextComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getMovableTextComponent)
		.def("getWaypointComponent", (i6e::utils::sharedPtr<i6e::api::WaypointComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getWaypointComponent)
		.def("getWaypointComponent", (i6e::utils::sharedPtr<i6e::api::WaypointComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getWaypointComponent)
		.def("getNavigationComponent", (i6e::utils::sharedPtr<i6e::api::NavigationComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getNavigationComponent)
		.def("getNavigationComponent", (i6e::utils::sharedPtr<i6e::api::NavigationComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getNavigationComponent)
		.def("getWaynetNavigationComponent", (i6e::utils::sharedPtr<i6e::api::WaynetNavigationComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getWaynetNavigationComponent)
		.def("getWaynetNavigationComponent", (i6e::utils::sharedPtr<i6e::api::WaynetNavigationComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getWaynetNavigationComponent)
		.def("getMoveComponent", (i6e::utils::sharedPtr<i6e::api::MoveComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getMoveComponent)
		.def("getMoveComponent", (i6e::utils::sharedPtr<i6e::api::MoveComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getMoveComponent)
		.def("getMovementComponent", (i6e::utils::sharedPtr<i6e::api::MovementComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getMovementComponent)
		.def("getMovementComponent", (i6e::utils::sharedPtr<i6e::api::MovementComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getMovementComponent)
		.def("getToggleWaynetComponent", (i6e::utils::sharedPtr<i6e::api::ToggleWaynetComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getToggleWaynetComponent)
		.def("getToggleWaynetComponent", (i6e::utils::sharedPtr<i6e::api::ToggleWaynetComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getToggleWaynetComponent)
		.def("getPoint2PointConstraintComponent", (i6e::utils::sharedPtr<i6e::api::Point2PointConstraintComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getPoint2PointConstraintComponent)
		.def("getPoint2PointConstraintComponent", (i6e::utils::sharedPtr<i6e::api::Point2PointConstraintComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getPoint2PointConstraintComponent)
		.def("getVelocityComponent", (i6e::utils::sharedPtr<i6e::api::VelocityComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::python::object::getVelocityComponent)
		.def("getVelocityComponent", (i6e::utils::sharedPtr<i6e::api::VelocityComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::python::object::getVelocityComponent);
	
	enum_<i6e::api::AddStrategy>("AddStrategy")
		.value("ADD", i6e::api::AddStrategy::ADD)
		.value("REPLACE", i6e::api::AddStrategy::REPLACE)
		.value("REPLACE_DIS", i6e::api::AddStrategy::REPLACE_DIS)
		.value("REJECT", i6e::api::AddStrategy::REJECT)
		.export_values();

	class_<std::pair<i6e::api::AddStrategy, int64_t>>("AddStrategyPair")
		.def(init<>())
		.def_readwrite("first", &std::pair<i6e::api::AddStrategy, int64_t>::first)
		.def_readwrite("second", &std::pair<i6e::api::AddStrategy, int64_t>::second);

	class_<i6e::python::object::ComponentWrapper, i6e::api::ComPtr, boost::noncopyable>("Component", no_init)
		.def("getOwnerGO", &i6e::api::Component::getOwnerGO)
		.def("getComponentID", &i6e::api::Component::getComponentID)
		.def("getFamilyID", &i6e::api::Component::getFamilyID)
		.def("getIdentifier", &i6e::api::Component::getIdentifier)
		.def("Tick", &i6e::api::Component::Tick, &i6e::python::object::ComponentWrapper::default_Tick)
		.def("setDie", &i6e::api::Component::setDie)
		.def("getID", &i6e::api::Component::getID)
		.def("News", &i6e::api::Component::News, &i6e::python::object::ComponentWrapper::default_News)
		.def("Init", pure_virtual(&i6e::python::object::ComponentWrapper::Init))
		.def("Finalize", &i6e::api::Component::Finalize, &i6e::python::object::ComponentWrapper::default_Finalize)
		.def("synchronize", pure_virtual(&i6e::python::object::ComponentWrapper::synchronize))
		.def("setSync", &i6e::api::Component::setSync)
		.def("getSync", &i6e::api::Component::getSync)
		//.def("howToAdd", &i6e::api::Component::howToAdd, &i6e::python::object::ComponentWrapper::default_howToAdd)
		.def("getTemplateName", pure_virtual(&i6e::python::object::ComponentWrapper::getTemplateName))
		.def("addTicker", &i6e::python::object::ComponentWrapper::addTicker)
		.def("removeTicker", &i6e::python::object::ComponentWrapper::removeTicker);

	class_<i6e::api::AnimatedDirectionalLightComponent, i6e::utils::sharedPtr<i6e::api::AnimatedDirectionalLightComponent, i6e::api::Component>, boost::noncopyable>("AnimatedDirectionalLightComponent", no_init)
		.def("synchronize", &i6e::api::AnimatedDirectionalLightComponent::synchronize)
		.def("getTemplateName", &i6e::api::AnimatedDirectionalLightComponent::getTemplateName);

	class_<i6e::api::AnimatedLuminousAppearanceComponent, i6e::utils::sharedPtr<i6e::api::AnimatedLuminousAppearanceComponent, i6e::api::Component>, boost::noncopyable>("AnimatedLuminousAppearanceComponent", no_init)
		.def("synchronize", &i6e::api::LuminousAppearanceComponent::synchronize)
		.def("getTemplateName", &i6e::api::LuminousAppearanceComponent::getTemplateName);

	class_<i6e::api::AnimatedSpotLightComponent, i6e::utils::sharedPtr<i6e::api::AnimatedSpotLightComponent, i6e::api::Component>, boost::noncopyable>("AnimatedSpotLightComponent", no_init)
		.def("synchronize", &i6e::api::AnimatedDirectionalLightComponent::synchronize)
		.def("getTemplateName", &i6e::api::AnimatedDirectionalLightComponent::getTemplateName);

	class_<i6e::api::AnimationControllerComponent, i6e::utils::sharedPtr<i6e::api::AnimationControllerComponent, i6e::api::Component>, boost::noncopyable>("AnimationControllerComponent", no_init)
		.def("addAnimationFrameEvent", (void(*)(i6e::api::AnimationControllerComponent*, const std::string &, uint64_t, const std::string &)) &i6e::python::object::addAnimationFrameEvent)
		.def("addAnimationFrameEvent", (void(*)(i6e::api::AnimationControllerComponent*, const std::string &, uint64_t, const std::string &, const std::string &)) &i6e::python::object::addAnimationFrameEvent)
		.def("playAnimation", &i6e::api::AnimationControllerComponent::playAnimation)
		.def("setAnimationSpeed", &i6e::api::AnimationControllerComponent::setAnimationSpeed)
		.def("stopAnimation", &i6e::api::AnimationControllerComponent::stopAnimation)
		.def("synchronize", &i6e::api::AnimationControllerComponent::synchronize)
		.def("getTemplateName", &i6e::api::AnimationControllerComponent::getTemplateName);

	class_<i6e::api::BillboardComponent, i6e::utils::sharedPtr<i6e::api::BillboardComponent, i6e::api::Component>, boost::noncopyable>("BillboardComponent", no_init)
		.def("createOrUpdateBillboard", &i6e::api::BillboardComponent::createOrUpdateBillboard)
		.def("deleteBillboard", &i6e::api::BillboardComponent::deleteBillboard)
		.def("synchronize", &i6e::api::BillboardComponent::synchronize)
		.def("getTemplateName", &i6e::api::BillboardComponent::getTemplateName);

	class_<i6e::python::object::CameraComponentWrapper, i6e::utils::sharedPtr<i6e::api::CameraComponent, i6e::api::Component>, boost::noncopyable>("CameraComponent", no_init)
		.def("Tick", &i6e::api::Component::Tick, &i6e::python::object::CameraComponentWrapper::default_Tick)
		.def("News", &i6e::api::Component::News, &i6e::python::object::CameraComponentWrapper::default_News)
		.def("Init", &i6e::python::object::CameraComponentWrapper::Init)
		.def("Finalize", &i6e::api::Component::Finalize, &i6e::python::object::CameraComponentWrapper::default_Finalize)
		.def("synchronize", &i6e::python::object::CameraComponentWrapper::synchronize)
		//.def("howToAdd", &i6e::api::Component::howToAdd, &i6e::python::object::CameraComponentWrapper::default_howToAdd)
		.def("getTemplateName", &i6e::python::object::CameraComponentWrapper::getTemplateName)
		.def("addTicker", &i6e::python::object::CameraComponentWrapper::addTicker)
		.def("removeTicker", &i6e::python::object::CameraComponentWrapper::removeTicker)
		.def("setPosition", &i6e::api::CameraComponent::setPosition)
		.def("setLookAt", &i6e::api::CameraComponent::setLookAt)
		.def("setNearClip", &i6e::api::CameraComponent::setNearClip)
		.def("setAspectRatio", &i6e::api::CameraComponent::setAspectRatio)
		.def("setFOVy", &i6e::api::CameraComponent::setFOVy)
		.def("setFrustumExtends", &i6e::api::CameraComponent::setFrustumExtends)
		.def("setViewportDimension", &i6e::api::CameraComponent::setViewportDimension)
		.def("setViewportBackground", &i6e::api::CameraComponent::setViewportBackground)
		.def("getPosition", &i6e::api::CameraComponent::getPosition)
		.def("getLookAt", &i6e::api::CameraComponent::getLookAt)
		.def("enableCompositor", &i6e::api::CameraComponent::enableCompositor);

	class_<i6e::api::FollowComponent, i6e::utils::sharedPtr<i6e::api::FollowComponent, i6e::api::Component>, boost::noncopyable>("FollowComponent", no_init)
		.def("synchronize", &i6e::api::FollowComponent::synchronize)
		.def("getTemplateName", &i6e::api::FollowComponent::getTemplateName);

	class_<i6e::api::LifetimeComponent, i6e::utils::sharedPtr<i6e::api::LifetimeComponent, i6e::api::Component>, boost::noncopyable>("LifetimeComponent", no_init)
		.def("synchronize", &i6e::api::LifetimeComponent::synchronize)
		.def("getTemplateName", &i6e::api::LifetimeComponent::getTemplateName)
		.def("instantKill", &i6e::api::LifetimeComponent::instantKill);

	class_<i6e::api::LuminousAppearanceComponent, i6e::utils::sharedPtr<i6e::api::LuminousAppearanceComponent, i6e::api::Component>, boost::noncopyable>("LuminousAppearanceComponent", no_init)
		.def("synchronize", &i6e::api::LuminousAppearanceComponent::synchronize)
		.def("getTemplateName", &i6e::api::LuminousAppearanceComponent::getTemplateName)
		.def("setLightType", &i6e::api::LuminousAppearanceComponent::setLightType)
		.def("getLightType", &i6e::api::LuminousAppearanceComponent::getLightType)
		.def("setDiffuseColor", &i6e::api::LuminousAppearanceComponent::setDiffuseColor)
		.def("getDiffuseColor", &i6e::api::LuminousAppearanceComponent::getDiffuseColor)
		.def("setSpecularColor", &i6e::api::LuminousAppearanceComponent::setSpecularColor)
		.def("getSpecularColor", &i6e::api::LuminousAppearanceComponent::getSpecularColor)
		.def("setAttenuation", &i6e::api::LuminousAppearanceComponent::setAttenuation)
		.def("getAttenuation", &i6e::api::LuminousAppearanceComponent::getAttenuation)
		.def("setDirection", &i6e::api::LuminousAppearanceComponent::setDirection)
		.def("getDirection", &i6e::api::LuminousAppearanceComponent::getDirection);

	enum_<i6e::api::LuminousAppearanceComponent::LightType>("LightType")
		.value("POINT", i6e::api::LuminousAppearanceComponent::LightType::POINT)
		.value("DIRECTIONAL", i6e::api::LuminousAppearanceComponent::LightType::DIRECTIONAL)
		.value("SPOT", i6e::api::LuminousAppearanceComponent::LightType::SPOT)
		.export_values();

	class_<i6e::api::MeshAppearanceComponent, i6e::utils::sharedPtr<i6e::api::MeshAppearanceComponent, i6e::api::Component>, boost::noncopyable>("MeshAppearanceComponent", no_init)
		.def("synchronize", &i6e::api::MeshAppearanceComponent::synchronize)
		.def("getTemplateName", &i6e::api::MeshAppearanceComponent::getTemplateName)
		.def("getPosition", &i6e::api::MeshAppearanceComponent::getPosition)
		.def("setVisibility", &i6e::api::MeshAppearanceComponent::setVisibility)
		.def("setMesh", &i6e::api::MeshAppearanceComponent::setMesh)
		.def("setCustomParameter", &i6e::api::MeshAppearanceComponent::setCustomParameter)
		.def("getVisibility", &i6e::api::MeshAppearanceComponent::getVisibility)
		.def("getMesh", &i6e::api::MeshAppearanceComponent::getMesh)
		.def("setMaterial", &i6e::api::MeshAppearanceComponent::setMaterial)
		.def("playAnimation", &i6e::api::MeshAppearanceComponent::playAnimation)
		.def("setAnimationSpeed", &i6e::api::MeshAppearanceComponent::setAnimationSpeed)
		.def("stopAnimation", &i6e::api::MeshAppearanceComponent::stopAnimation)
		.def("drawBoundingBox", &i6e::api::MeshAppearanceComponent::drawBoundingBox)
		.def("removeBoundingBox", &i6e::api::MeshAppearanceComponent::removeBoundingBox)
		.def("attachGameObjectToBone", &i6e::api::MeshAppearanceComponent::attachGameObjectToBone)
		.def("detachGameObjectFromBone", &i6e::api::MeshAppearanceComponent::detachGameObjectFromBone)
		.def("addAnimationFrameEvent", (void(*)(i6e::api::MeshAppearanceComponent*, uint64_t, const std::string &)) &i6e::python::object::addAnimationFrameEvent)
		.def("addAnimationFrameEvent", (void(*)(i6e::api::MeshAppearanceComponent*, uint64_t, const std::string &, const std::string &)) &i6e::python::object::addAnimationFrameEvent)
		.def("getBoneTransform", &i6e::api::MeshAppearanceComponent::getBoneTransform)
		.def("setShadowCasting", &i6e::api::MeshAppearanceComponent::setShadowCasting);

	class_<i6e::api::MovableTextComponent, i6e::utils::sharedPtr<i6e::api::MovableTextComponent, i6e::api::Component>, boost::noncopyable>("MovableTextComponent", no_init)
		.def("synchronize", &i6e::api::MovableTextComponent::synchronize)
		.def("getTemplateName", &i6e::api::MovableTextComponent::getTemplateName)
		.def("setText", &i6e::api::MovableTextComponent::setText);

	class_<i6e::api::MoveComponent, i6e::utils::sharedPtr<i6e::api::MoveComponent, i6e::api::Component>, boost::noncopyable>("MoveComponent", no_init)
		.def("synchronize", &i6e::api::MoveComponent::synchronize)
		.def("getTemplateName", &i6e::api::MoveComponent::getTemplateName)
		.def("navigate", (void(i6e::api::MoveComponent::*)(const Vec3 &)) &i6e::api::MoveComponent::navigate)
		.def("navigate", (void(i6e::api::MoveComponent::*)(const std::string &)) &i6e::api::MoveComponent::navigate);

	class_<i6e::python::object::MovementComponentWrapper, i6e::utils::sharedPtr<i6e::api::MovementComponent, i6e::api::Component>, boost::noncopyable>("MovementComponent", no_init)
		.def("Tick", &i6e::api::MovementComponent::Tick, &i6e::python::object::MovementComponentWrapper::default_Tick)
		.def("News", &i6e::api::MovementComponent::News, &i6e::python::object::MovementComponentWrapper::default_News)
		.def("Init", &i6e::python::object::MovementComponentWrapper::Init)
		.def("Finalize", &i6e::api::MovementComponent::Finalize, &i6e::python::object::MovementComponentWrapper::default_Finalize)
		.def("synchronize", pure_virtual(&i6e::python::object::MovementComponentWrapper::synchronize))
		//.def("howToAdd", &i6e::api::Component::howToAdd, &i6e::python::object::MovementComponentWrapper::default_howToAdd)
		.def("getTemplateName", pure_virtual(&i6e::python::object::MovementComponentWrapper::getTemplateName))
		.def("forward", pure_virtual(&i6e::python::object::MovementComponentWrapper::forward))
		.def("backward", pure_virtual(&i6e::python::object::MovementComponentWrapper::backward))
		.def("left", pure_virtual(&i6e::python::object::MovementComponentWrapper::left))
		.def("right", pure_virtual(&i6e::python::object::MovementComponentWrapper::right))
		.def("stop", pure_virtual(&i6e::python::object::MovementComponentWrapper::stop));

	class_<i6e::api::MoverCircleComponent, i6e::utils::sharedPtr<i6e::api::MoverCircleComponent, i6e::api::Component>, boost::noncopyable>("MoverCircleComponent", no_init)
		.def("synchronize", &i6e::api::MoverCircleComponent::synchronize)
		.def("getTemplateName", &i6e::api::MoverCircleComponent::getTemplateName)
		.def("setPositioning", &i6e::api::MoverCircleComponent::setPositioning)
		.def("setCircleParameters", &i6e::api::MoverCircleComponent::setCircleParameters)
		.def("start", &i6e::api::MoverCircleComponent::start)
		.def("getCircleAxis", &i6e::api::MoverCircleComponent::getCircleAxis)
		.def("getCircleRadius", &i6e::api::MoverCircleComponent::getCircleRadius)
		.def("reset", &i6e::api::MoverCircleComponent::reset);

	class_<i6e::python::object::MoverComponentWrapper, i6e::utils::sharedPtr<i6e::api::MoverComponent, i6e::api::Component>, boost::noncopyable>("MoverComponent", no_init)
		.def("Tick", &i6e::api::MoverComponent::Tick, &i6e::python::object::MoverComponentWrapper::default_Tick)
		.def("News", &i6e::python::object::MoverComponentWrapper::News)
		.def("Init", &i6e::python::object::MoverComponentWrapper::Init)
		.def("Finalize", &i6e::api::MoverComponent::Finalize, &i6e::python::object::MoverComponentWrapper::default_Finalize)
		.def("synchronize", &i6e::python::object::MoverComponentWrapper::synchronize)
		//.def("howToAdd", &i6e::api::Component::howToAdd, &i6e::python::object::MoverComponentWrapper::default_howToAdd)
		.def("getTemplateName", &i6e::python::object::MoverComponentWrapper::getTemplateName)
		.def("start", &i6e::python::object::MoverComponentWrapper::start)
		.def("reset", &i6e::python::object::MoverComponentWrapper::reset);

	enum_<i6e::api::MoverComponent::Positioning>("Positioning")
		.value("POSITIONING_ABSOLUTE", i6e::api::MoverComponent::Positioning::POSITIONING_ABSOLUTE)
		.value("POSITIONING_RELATIVE", i6e::api::MoverComponent::Positioning::POSITIONING_RELATIVE)
		.export_values();

	class_<i6e::api::MoverInterpolateComponent, i6e::utils::sharedPtr<i6e::api::MoverInterpolateComponent, i6e::api::Component>, boost::noncopyable>("MoverInterpolateComponent", no_init)
		.def("synchronize", &i6e::api::MoverInterpolateComponent::synchronize)
		.def("getTemplateName", &i6e::api::MoverInterpolateComponent::getTemplateName)
		.def("addKeyFrame", &i6e::api::MoverInterpolateComponent::addKeyFrame)
		.def("removeKeyFrame", &i6e::api::MoverInterpolateComponent::removeKeyFrame)
		.def("getKeyframe", &i6e::api::MoverInterpolateComponent::getKeyframe)
		.def("setMode", &i6e::api::MoverInterpolateComponent::setMode)
		.def("setOpenTime", &i6e::api::MoverInterpolateComponent::setOpenTime)
		.def("setWay", &i6e::api::MoverInterpolateComponent::setWay)
		.def("getWay", &i6e::api::MoverInterpolateComponent::getWay)
		.def("start", &i6e::api::MoverInterpolateComponent::start)
		.def("reset", &i6e::api::MoverInterpolateComponent::reset);

	enum_<i6e::api::MoverInterpolateComponent::Mode>("Mode")
		.value("TWOSTATE_TOGGLE", i6e::api::MoverInterpolateComponent::Mode::TWOSTATE_TOGGLE)
		.value("TWOSTATE_OPENTIME", i6e::api::MoverInterpolateComponent::Mode::TWOSTATE_OPENTIME)
		.value("NSTATE_LOOP", i6e::api::MoverInterpolateComponent::Mode::NSTATE_LOOP)
		.value("ONCE", i6e::api::MoverInterpolateComponent::Mode::ONCE)
		.export_values();
		
	enum_<i6e::api::MoverInterpolateComponent::Way>("Way")
		.value("LINEAR", i6e::api::MoverInterpolateComponent::Way::LINEAR)
		.value("BEZIER", i6e::api::MoverInterpolateComponent::Way::BEZIER)
		.export_values();

	class_<i6e::api::MovingCameraComponent, i6e::utils::sharedPtr<i6e::api::MovingCameraComponent, i6e::api::Component>, boost::noncopyable>("MovingCameraComponent", no_init)
		.def("getTemplateName", &i6e::api::MoverCircleComponent::getTemplateName),

	class_<i6e::python::object::NavigationComponentWrapper, i6e::utils::sharedPtr<i6e::api::NavigationComponent, i6e::api::Component>, boost::noncopyable>("NavigationComponent", no_init)
		.def("Tick", &i6e::api::Component::Tick, &i6e::python::object::NavigationComponentWrapper::default_Tick)
		.def("News", &i6e::api::Component::News, &i6e::python::object::NavigationComponentWrapper::default_News)
		.def("Init", &i6e::python::object::NavigationComponentWrapper::Init)
		.def("Finalize", &i6e::api::Component::Finalize, &i6e::python::object::NavigationComponentWrapper::default_Finalize)
		.def("synchronize", &i6e::python::object::NavigationComponentWrapper::synchronize)
		//.def("howToAdd", &i6e::api::Component::howToAdd, &i6e::python::object::NavigationComponentWrapper::default_howToAdd)
		.def("getTemplateName", &i6e::python::object::NavigationComponentWrapper::getTemplateName)
		.def("getPathPos", (std::vector<Vec3>(i6e::python::object::NavigationComponentWrapper::*)(const Vec3 &, const Vec3 &) const) &i6e::python::object::NavigationComponentWrapper::getPath)
		.def("getPathWP", (std::vector<Vec3>(i6e::python::object::NavigationComponentWrapper::*)(const Vec3 &, const std::string &) const) &i6e::python::object::NavigationComponentWrapper::getPath);

#ifdef ISIXE_WITH_NETWORK
	class_<i6e::api::NetworkSenderComponent, i6e::utils::sharedPtr<i6e::api::NetworkSenderComponent, i6e::api::Component>, boost::noncopyable>("NetworkSenderComponent", no_init)
		.def("getTemplateName", &i6e::api::NetworkSenderComponent::getTemplateName);
#endif

	class_<i6e::api::ParticleEmitterComponent, i6e::utils::sharedPtr<i6e::api::ParticleEmitterComponent, i6e::api::Component>, boost::noncopyable>("ParticleEmitterComponent", no_init)
		.def("synchronize", &i6e::api::ParticleEmitterComponent::synchronize)
		.def("getTemplateName", &i6e::api::ParticleEmitterComponent::getTemplateName);

	enum_<i6e::api::ResponseType::ResponseType>("ResponseType")
		.value("NONE", i6e::api::ResponseType::ResponseType::NONE)
		.value("STATIC", i6e::api::ResponseType::ResponseType::STATIC)
		.value("GHOST", i6e::api::ResponseType::ResponseType::GHOST)
		.value("TRIGGER", i6e::api::ResponseType::ResponseType::TRIGGER)
		.export_values();

	enum_<i6e::api::ShatterInterest>("ShatterInterest")
		.value("NONE", i6e::api::ShatterInterest::NONE)
		.value("START", i6e::api::ShatterInterest::START)
		.value("END", i6e::api::ShatterInterest::END)
		.value("ALWAYS", i6e::api::ShatterInterest::ALWAYS)
		.export_values();

	class_<i6e::api::PhysicalStateComponent, i6e::utils::sharedPtr<i6e::api::PhysicalStateComponent, i6e::api::Component>, boost::noncopyable>("PhysicalStateComponent", no_init)
		.def("getPosition", &i6e::api::PhysicalStateComponent::getPosition)
		.def("setPosition", &i6e::api::PhysicalStateComponent::setPosition)
		.def("getRotation", &i6e::api::PhysicalStateComponent::getRotation)
		.def("setRotation", &i6e::api::PhysicalStateComponent::setRotation)
		.def("getScale", &i6e::api::PhysicalStateComponent::getScale)
		.def("setScale", &i6e::api::PhysicalStateComponent::setScale)
		.def("setCollisionFlags", &i6e::api::PhysicalStateComponent::setCollisionFlags)
		.def("getCollisionFlags", &i6e::api::PhysicalStateComponent::getCollisionFlags)
		.def("reset", &i6e::api::PhysicalStateComponent::reset)
		.def("setCollisionShape", &i6e::api::PhysicalStateComponent::setCollisionShape)
		.def("applyRotation", &i6e::api::PhysicalStateComponent::applyRotation)
		.def("getLinearVelocity", &i6e::api::PhysicalStateComponent::getLinearVelocity)
		.def("setLinearVelocity", &i6e::api::PhysicalStateComponent::setLinearVelocity)
		.def("applyCentralForce", &i6e::api::PhysicalStateComponent::applyCentralForce)
		.def("applyForce", &i6e::api::PhysicalStateComponent::applyForce)
		.def("setShatterInterest", &i6e::api::PhysicalStateComponent::setShatterInterest)
		.def("setGravity", &i6e::api::PhysicalStateComponent::setGravity)
		.def("synchronize", &i6e::api::PhysicalStateComponent::synchronize)
		.def("rayTest", &i6e::api::PhysicalStateComponent::rayTest)
		.def("getTemplateName", &i6e::api::PhysicalStateComponent::getTemplateName)
		.def("addPosition", &i6e::api::PhysicalStateComponent::addPosition);
	
	enum_<i6e::api::PhysicalStateComponent::ShapeType>("ShapeType")
		.value("PLANE", i6e::api::PhysicalStateComponent::ShapeType::PLANE)
		.value("BOX", i6e::api::PhysicalStateComponent::ShapeType::BOX)
		.value("SPHERE", i6e::api::PhysicalStateComponent::ShapeType::SPHERE)
		.value("FILE", i6e::api::PhysicalStateComponent::ShapeType::FILE)
		.export_values();

	enum_<i6e::api::PhysicalStateComponent::RayTestRepetition>("RayTestRepetition")
		.value("STOP", i6e::api::PhysicalStateComponent::RayTestRepetition::STOP)
		.value("ONCE", i6e::api::PhysicalStateComponent::RayTestRepetition::ONCE)
		.value("PERIODIC", i6e::api::PhysicalStateComponent::RayTestRepetition::PERIODIC)
		.export_values();

	enum_<i6e::api::PhysicalStateComponent::RayTestNotify>("RayTestNotify")
		.value("ALWAYS", i6e::api::PhysicalStateComponent::RayTestNotify::ALWAYS)
		.value("CHANGE", i6e::api::PhysicalStateComponent::RayTestNotify::CHANGE)
		.value("FOUND", i6e::api::PhysicalStateComponent::RayTestNotify::FOUND)
		.value("NOTFOUND", i6e::api::PhysicalStateComponent::RayTestNotify::NOTFOUND)
		.value("OBJECTCHANGE", i6e::api::PhysicalStateComponent::RayTestNotify::OBJECTCHANGE)
		.export_values();

	class_<i6e::api::RayTestResult>("RayTestResult")
		.def(init<>())
		.def_readonly("objID", &i6e::api::RayTestResult::objID)
		.def_readonly("sourceID", &i6e::api::RayTestResult::sourceID)
		.def_readonly("collisionPoint", &i6e::api::RayTestResult::collisionPoint);

	class_<i6e::api::Point2PointConstraintComponent, i6e::utils::sharedPtr<i6e::api::Point2PointConstraintComponent, i6e::api::Component>, boost::noncopyable>("Point2PointConstraintComponent", no_init)
		.def("synchronize", &i6e::api::Point2PointConstraintComponent::synchronize)
		.def("getTemplateName", &i6e::api::Point2PointConstraintComponent::getTemplateName);

	class_<i6e::python::object::ShatterComponentWrapper, i6e::utils::sharedPtr<i6e::api::ShatterComponent, i6e::api::Component>, boost::noncopyable>("ShatterComponent", no_init)
		.def("Tick", &i6e::api::Component::Tick, &i6e::python::object::ShatterComponentWrapper::default_Tick)
		.def("News", &i6e::api::ShatterComponent::News, &i6e::python::object::ShatterComponentWrapper::default_News)
		.def("Init", &i6e::api::ShatterComponent::Init, &i6e::python::object::ShatterComponentWrapper::Init)
		.def("Finalize", &i6e::api::Component::Finalize, &i6e::python::object::ShatterComponentWrapper::default_Finalize)
		.def("synchronize", &i6e::python::object::CameraComponentWrapper::synchronize)
		//.def("howToAdd", &i6e::api::Component::howToAdd, &i6e::python::object::ShatterComponentWrapper::default_howToAdd)
		.def("getTemplateName", &i6e::python::object::ShatterComponentWrapper::getTemplateName)
		.def("shatter", pure_virtual(&i6e::python::object::ShatterComponentWrapper::shatter))
		.def("resetRespawn", &i6e::api::ShatterComponent::resetRespawn);

	class_<i6e::api::SoundComponent, i6e::utils::sharedPtr<i6e::api::SoundComponent, i6e::api::Component>, boost::noncopyable>("SoundComponent", no_init)
		.def("synchronize", &i6e::api::SoundComponent::synchronize)
		.def("getTemplateName", &i6e::api::SoundComponent::getTemplateName);

	class_<i6e::api::SoundListenerComponent, i6e::utils::sharedPtr<i6e::api::SoundListenerComponent, i6e::api::Component>, boost::noncopyable>("SoundListenerComponent", no_init)
		.def("synchronize", &i6e::api::SoundListenerComponent::synchronize)
		.def("getTemplateName", &i6e::api::SoundListenerComponent::getTemplateName);

	class_<i6e::api::SpawnpointComponent, i6e::utils::sharedPtr<i6e::api::SpawnpointComponent, i6e::api::Component>, boost::noncopyable>("SpawnpointComponent", no_init)
		.def("synchronize", &i6e::api::SpawnpointComponent::synchronize)
		.def("getTemplateName", &i6e::api::SpawnpointComponent::getTemplateName)
		.def("addSpawntype", &i6e::api::SpawnpointComponent::addSpawntype)
		.def("addSpawntypes", &i6e::api::SpawnpointComponent::addSpawntypes)
		.def("removeSpawntype", &i6e::api::SpawnpointComponent::removeSpawntype)
		.def("containsSpawntype", &i6e::api::SpawnpointComponent::containsSpawntype)
		.def("available", &i6e::api::SpawnpointComponent::available)
		.def("setState", &i6e::api::SpawnpointComponent::setState);

	class_<i6e::api::StaticStateComponent, i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component>, boost::noncopyable>("StaticStateComponent", no_init)
		.def("synchronize", &i6e::api::StaticStateComponent::synchronize)
		.def("getTemplateName", &i6e::api::StaticStateComponent::getTemplateName)
		.def("setPosition", &i6e::api::StaticStateComponent::setPosition)
		.def("setRotation", &i6e::api::StaticStateComponent::setRotation)
		.def("setScale", &i6e::api::StaticStateComponent::setScale)
		.def("getPosition", &i6e::api::StaticStateComponent::getPosition)
		.def("getRotation", &i6e::api::StaticStateComponent::getRotation)
		.def("getScale", &i6e::api::StaticStateComponent::getScale);

	class_<i6e::api::TerrainAppearanceComponent, i6e::utils::sharedPtr<i6e::api::TerrainAppearanceComponent, i6e::api::Component>, boost::noncopyable>("TerrainAppearanceComponent", no_init)
		.def("synchronize", &i6e::api::TerrainAppearanceComponent::synchronize)
		.def("getTemplateName", &i6e::api::TerrainAppearanceComponent::getTemplateName)
		.def("getHeightmap", &i6e::api::TerrainAppearanceComponent::getHeightmap)
		.def("getSize", &i6e::api::TerrainAppearanceComponent::getSize);

	class_<i6e::api::ToggleWaynetComponent, i6e::utils::sharedPtr<i6e::api::ToggleWaynetComponent, i6e::api::Component>, boost::noncopyable>("ToggleWaynetComponent", no_init)
		.def("synchronize", &i6e::api::ToggleWaynetComponent::synchronize)
		.def("getTemplateName", &i6e::api::ToggleWaynetComponent::getTemplateName)
		.def("enable", &i6e::api::ToggleWaynetComponent::enable);

	class_<i6e::api::VelocityComponent, i6e::utils::sharedPtr<i6e::api::VelocityComponent, i6e::api::Component>, boost::noncopyable>("VelocityComponent", no_init)
		.def("synchronize", &i6e::api::VelocityComponent::synchronize)
		.def("getTemplateName", &i6e::api::VelocityComponent::getTemplateName)
		.def("accelerate", (void(*)(i6e::api::VelocityComponent *, const Vec3 &, i6e::api::VelocityComponent::MaxSpeedHandling, const std::string &)) &i6e::python::object::accelerate)
		.def("accelerate", (void(*)(i6e::api::VelocityComponent *, const std::string &)) &i6e::python::object::accelerate)
		.def("decelerate", (void(*)(i6e::api::VelocityComponent *, const Vec3 &, i6e::api::VelocityComponent::DecelerationHandling, const std::string &)) &i6e::python::object::decelerate)
		.def("decelerate", (void(*)(i6e::api::VelocityComponent *, const std::string &)) &i6e::python::object::decelerate)
		.def("setMaxSpeed", &i6e::api::VelocityComponent::setMaxSpeed)
		.def("setResistanceCoefficient", &i6e::api::VelocityComponent::setResistanceCoefficient)
		.def("setWindage", &i6e::api::VelocityComponent::setWindage);

	enum_<i6e::api::VelocityComponent::MaxSpeedHandling>("MaxSpeedHandling")
		.value("KeepSpeed", i6e::api::VelocityComponent::MaxSpeedHandling::KeepSpeed)
		.value("StopAcceleration", i6e::api::VelocityComponent::MaxSpeedHandling::StopAcceleration)
		.export_values();

	enum_<i6e::api::VelocityComponent::DecelerationHandling>("DecelerationHandling")
		.value("Backward", i6e::api::VelocityComponent::DecelerationHandling::Backward)
		.value("StopDeceleration", i6e::api::VelocityComponent::DecelerationHandling::StopDeceleration)
		.export_values();

	class_<i6e::api::WaynetNavigationComponent, i6e::utils::sharedPtr<i6e::api::WaynetNavigationComponent, i6e::api::Component>, boost::noncopyable>("WaynetNavigationComponent", no_init)
		.def("synchronize", &i6e::api::WaynetNavigationComponent::synchronize)
		.def("getTemplateName", &i6e::api::WaynetNavigationComponent::getTemplateName)
		.def("getPathPos", (std::vector<Vec3>(i6e::api::WaynetNavigationComponent::*)(const Vec3 &, const Vec3 &) const) &i6e::api::WaynetNavigationComponent::getPath)
		.def("getPathWP", (std::vector<Vec3>(i6e::api::WaynetNavigationComponent::*)(const Vec3 &, const std::string &) const) &i6e::api::WaynetNavigationComponent::getPath);

	class_<i6e::api::WaypointComponent, i6e::utils::sharedPtr<i6e::api::WaypointComponent, i6e::api::Component>, boost::noncopyable>("WaypointComponent", no_init)
		.def("synchronize", &i6e::api::WaypointComponent::synchronize)
		.def("getTemplateName", &i6e::api::WaypointComponent::getTemplateName)
		.def("getName", &i6e::api::WaypointComponent::getName)
		.def("getConnections", &i6e::api::WaypointComponent::getConnections)
		.def("isConnected", &i6e::api::WaypointComponent::isConnected)
		.def("addConnection", &i6e::api::WaypointComponent::addConnection)
		.def("removeConnection", &i6e::api::WaypointComponent::removeConnection);

	def("getObject", &i6e::python::object::getObject);
	def("getAllObjectsOfType", &i6e::python::object::getAllObjectsOfType);
	def("getGOList", &i6e::python::object::getGOList);
	def("deleteAllObjectsOfType", &i6e::python::object::deleteAllObjectsOfType);
	def("createObject", &i6e::python::object::createObject);
	def("cleanUpAll", &i6e::python::object::cleanUpAll);
	def("loadLevel", (void(*)(const std::string &, const std::string &)) &i6e::python::object::loadLevel);
	def("loadLevel", (void(*)(const std::string &, const std::string &, const std::string &)) &i6e::python::object::loadLevel);
	def("loadLevelCallbackFunc", (void(*)(const std::string &, const std::string &, const std::string &)) &i6e::python::object::loadLevelCallbackFunc);
	def("loadLevelCallbackScript", (void(*)(const std::string &, const std::string &, const std::string &, const std::string &)) &i6e::python::object::loadLevelCallbackScript);
	def("loadLevelCallbackFunc", (void(*)(const std::string &, const std::string &, const std::string &, const std::string &)) &i6e::python::object::loadLevelCallbackFunc);
	def("loadLevelCallbackScript", (void(*)(const std::string &, const std::string &, const std::string &, const std::string &, const std::string &)) &i6e::python::object::loadLevelCallbackScript);
	def("getFrameTime", &i6e::python::object::getFrameTime);
	def("createGO", &i6e::python::object::createGO);
	def("createComponent", &i6e::python::object::createComponent);
	def("createComponentCallback", (void(*)(int64_t, int64_t, const std::string &, const i6e::api::attributeMap &, const std::string &)) &i6e::python::object::createComponentCallback);
	def("createComponentCallback", (void(*)(int64_t, int64_t, const std::string &, const i6e::api::attributeMap &, const std::string &, const std::string &)) &i6e::python::object::createComponentCallback);
	def("resetObjectSubSystem", &i6e::python::object::resetObjectSubSystem);
	def("pauseObject", &i6e::python::object::pauseObject);
	def("unpauseObject", &i6e::python::object::unpauseObject);
	def("rayTest", &i6e::python::object::rayTest);

	class_<i6e::api::objects::GOTemplateComponent>("GOTemplateComponent")
		.def(init<const std::string &, const i6e::api::attributeMap &, const std::string &, bool, bool>())
		.def_readwrite("template", &i6e::api::objects::GOTemplateComponent::_template)
		.def_readwrite("id", &i6e::api::objects::GOTemplateComponent::_id)
		.def_readwrite("params", &i6e::api::objects::GOTemplateComponent::_params)
		.def_readwrite("owner", &i6e::api::objects::GOTemplateComponent::_owner)
		.def_readwrite("identifier", &i6e::api::objects::GOTemplateComponent::_identifier)
		.def_readwrite("deleted", &i6e::api::objects::GOTemplateComponent::_deleted);

	class_<i6e::api::objects::GOTemplate>("GOTemplate")
		.def(init<>())
		.def_readwrite("type", &i6e::api::objects::GOTemplate::_type)
		.def_readwrite("components", &i6e::api::objects::GOTemplate::_components);

	class_<i6e::api::attributeMap>("attributeMap")
		.def(init<>())
		.def("insert", &i6e::python::object::insertPairInAttributeMap);

	class_<std::vector<i6e::api::objects::GOTemplateComponent>>("GOTemplateComponentVector")
		.def(init<>())
		.def("push_back", (void(std::vector<i6e::api::objects::GOTemplateComponent>::*)(const i6e::api::objects::GOTemplateComponent &)) &std::vector<i6e::api::objects::GOTemplateComponent>::push_back);

	class_<i6e::api::CollisionGroup>("CollisionGroup")
		.def(init<>())
		.def(init<uint32_t, uint32_t, uint32_t>())
		.def(init<const std::string &>())
		.def_readwrite("responseType", &i6e::api::CollisionGroup::responseType)
		.def_readwrite("crashType", &i6e::api::CollisionGroup::crashType)
		.def_readwrite("crashMask", &i6e::api::CollisionGroup::crashMask);

	enum_<i6e::api::components::ComponentTypes>("ComponentTypes")
		.value("CameraComponent", i6e::api::components::ComponentTypes::CameraComponent)
		.value("LifetimeComponent", i6e::api::components::ComponentTypes::LifetimeComponent)
		.value("LuminousAppearanceComponent", i6e::api::components::ComponentTypes::LuminousAppearanceComponent)
		.value("MeshAppearanceComponent", i6e::api::components::ComponentTypes::MeshAppearanceComponent)
		.value("MoverCircleComponent", i6e::api::components::ComponentTypes::MoverCircleComponent)
		.value("MoverComponent", i6e::api::components::ComponentTypes::MoverComponent)
		.value("MoverInterpolateComponent", i6e::api::components::ComponentTypes::MoverInterpolateComponent)
		.value("MovingCameraComponent", i6e::api::components::ComponentTypes::MovingCameraComponent)
		.value("NetworkSenderComponent", i6e::api::components::ComponentTypes::NetworkSenderComponent)
		.value("ParticleEmitterComponent", i6e::api::components::ComponentTypes::ParticleEmitterComponent)
		.value("PhysicalStateComponent", i6e::api::components::ComponentTypes::PhysicalStateComponent)
		.value("ShatterComponent", i6e::api::components::ComponentTypes::ShatterComponent)
		.value("SpawnpointComponent", i6e::api::components::ComponentTypes::SpawnpointComponent)
		.value("StaticStateComponent", i6e::api::components::ComponentTypes::StaticStateComponent)
		.value("TerrainAppearanceComponent", i6e::api::components::ComponentTypes::TerrainAppearanceComponent)
		.value("SoundComponent", i6e::api::components::ComponentTypes::SoundComponent)
		.value("SoundListenerComponent", i6e::api::components::ComponentTypes::SoundListenerComponent)
		.value("BillboardComponent", i6e::api::components::ComponentTypes::BillboardComponent)
		.value("FollowComponent", i6e::api::components::ComponentTypes::FollowComponent)
		.value("MovableTextComponent", i6e::api::components::ComponentTypes::MovableTextComponent)
		.value("WaypointComponent", i6e::api::components::ComponentTypes::WaypointComponent)
		.value("NavigationComponent", i6e::api::components::ComponentTypes::NavigationComponent)
		.value("WaynetNavigationComponent", i6e::api::components::ComponentTypes::WaynetNavigationComponent)
		.value("MoveComponent", i6e::api::components::ComponentTypes::MoveComponent)
		.value("MovementComponent", i6e::api::components::ComponentTypes::MovementComponent)
		.value("ToggleWaynetComponent", i6e::api::components::ComponentTypes::ToggleWaynetComponent)
		.value("Point2PointConstraintComponent", i6e::api::components::ComponentTypes::Point2PointConstraintComponent)
		.value("VelocityComponent", i6e::api::components::ComponentTypes::VelocityComponent)
		.value("ComponentTypesCount", i6e::api::components::ComponentTypes::ComponentTypesCount)
		.export_values();
}
