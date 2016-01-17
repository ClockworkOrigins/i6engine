/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/AnimatedDirectionalLightComponent.h"
#include "i6engine/api/components/AnimatedSpotLightComponent.h"
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

namespace i6engine {
namespace python {
namespace object {

	boost::python::list getAllObjectsOfType(const std::string & types) {
		boost::python::list l;
		std::vector<i6engine::api::GOPtr> v = i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->getAllObjectsOfType(types);
		for (std::vector<i6engine::api::GOPtr>::const_iterator it = v.begin(); it != v.end(); ++it) {
			l.append(*it);
		}
		return l;
	}

	i6engine::api::GOPtr getObject(const int64_t id) {
		return i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(id);
	}

	boost::python::list getGOList() {
		boost::python::list l;
		std::unordered_map<int64_t, i6engine::api::GOPtr> v = i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->getGOMap();
		for (std::unordered_map<int64_t, i6engine::api::GOPtr>::const_iterator it = v.begin(); it != v.end(); ++it) {
			l.append(it->second);
		}
		return l;
	}

	void deleteAllObjectsOfType(const std::string & types) {
		i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->deleteAllObjectsOfType(types);
	}

	void createObject(const std::string & gTemplate, const i6engine::api::objects::GOTemplate & tmpl, uint32_t uuid, const bool sender) {
		i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->createObject(gTemplate, tmpl, uuid, sender);
	}

	void cleanUpAll() {
		i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->cleanUpAll();
	}

	void loadLevel(const std::string & file, const std::string & flags) {
		i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->loadLevel(file, flags);
	}

	uint32_t getFrameTime() {
		return i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->getFrameTime();
	}

	void createGO(const std::string & gTemplate, const i6engine::api::objects::GOTemplate & tmpl, uint32_t uuid, const bool sender, const std::string & func) {
		i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->createGO(gTemplate, tmpl, uuid, sender, [func](i6engine::api::GOPtr go) {
			i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func, go);
		});
	}

	void createComponent(int64_t goid, int64_t coid, const std::string & component, const attributeMap & params) {
		i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->createComponent(goid, coid, component, params);
	}

	void resetObjectSubSystem() {
		i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->resetSubSystem();
	}

	void pauseObject() {
		i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->pause();
	}

	void unpauseObject() {
		i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->unpause();
	}

	void rayTest(i6engine::api::PhysicalStateComponent * c, const Vec3 & from, const Vec3 & to, i6engine::api::PhysicalStateComponent::RayTestRepetition rtr, i6engine::api::PhysicalStateComponent::RayTestNotify rtn, const std::string & script, const std::string & func, const int64_t rayID) {
		c->rayTest(from, to, rtr, rtn, boost::make_shared<i6engine::api::GameMessage>(i6engine::api::messages::ScriptingMessageType, i6engine::api::scripting::ScrRayResult, i6engine::core::Method::Update, new i6engine::api::scripting::Scripting_RayResult_Update(script, func, rayID), i6engine::core::Subsystem::Unknown));
	}

	void insertPairInAttributeMap(i6engine::api::attributeMap * am, const std::string & key, const std::string & value) {
		am->insert(std::make_pair(key, value));
	}

	struct ComponentWrapper : public i6engine::api::Component, public boost::python::wrapper<i6engine::api::Component> {
		ComponentWrapper(const int64_t id, const attributeMap & params) : Component(id, params), boost::python::wrapper<i6engine::api::Component>() {
		}

		ComponentWrapper(const i6engine::api::Component & arg) : i6engine::api::Component(), boost::python::wrapper<i6engine::api::Component>() {
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

		virtual void News(const i6engine::api::GameMessage::Ptr & msg) override {
			if (boost::python::override n = this->get_override("News")) {
				boost::python::call<void>(n.ptr(), msg);
				return;
			}
			Component::News(msg);
		}

		void default_News(const i6engine::api::GameMessage::Ptr & msg) {
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

		virtual i6engine::api::attributeMap synchronize() const {
			return boost::python::call<i6engine::api::attributeMap>(this->get_override("synchronize").ptr());
		}

		virtual std::pair<i6engine::api::AddStrategy, int64_t> howToAdd(const i6engine::api::ComPtr & comp) const override {
			if (boost::python::override hta = this->get_override("howToAdd")) {
				return boost::python::call<std::pair<i6engine::api::AddStrategy, int64_t>>(hta.ptr(), comp);
			}
			return Component::howToAdd(comp);
		}

		std::pair<i6engine::api::AddStrategy, int64_t> default_howToAdd(const i6engine::api::ComPtr & comp) {
			return this->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return boost::python::call<std::string>(this->get_override("getTemplateName").ptr());
		}

		std::vector<i6engine::api::componentOptions> getComponentOptions() {
			return {};
		}

		void addTicker() {
			Component::addTicker();
		}

		void removeTicker() {
			Component::removeTicker();
		}
	};

	i6engine::utils::sharedPtr<i6engine::api::AnimatedDirectionalLightComponent, i6engine::api::Component> getAnimatedDirectionalLightComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::AnimatedDirectionalLightComponent>(i6engine::api::components::ComponentTypes::AnimatedDirectionalLightComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::AnimatedDirectionalLightComponent, i6engine::api::Component> getAnimatedDirectionalLightComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::AnimatedDirectionalLightComponent>(i6engine::api::components::ComponentTypes::AnimatedDirectionalLightComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::AnimatedLuminousAppearanceComponent, i6engine::api::Component> getAnimatedLuminousAppearanceComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::AnimatedLuminousAppearanceComponent>(i6engine::api::components::ComponentTypes::AnimatedLuminousAppearanceComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::AnimatedLuminousAppearanceComponent, i6engine::api::Component> getAnimatedLuminousAppearanceComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::AnimatedLuminousAppearanceComponent>(i6engine::api::components::ComponentTypes::AnimatedLuminousAppearanceComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::AnimatedSpotLightComponent, i6engine::api::Component> getAnimatedSpotLightComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::AnimatedSpotLightComponent>(i6engine::api::components::ComponentTypes::AnimatedSpotLightComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::AnimatedSpotLightComponent, i6engine::api::Component> getAnimatedSpotLightComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::AnimatedSpotLightComponent>(i6engine::api::components::ComponentTypes::AnimatedSpotLightComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::CameraComponent, i6engine::api::Component> getCameraComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::CameraComponent>(i6engine::api::components::ComponentTypes::CameraComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::CameraComponent, i6engine::api::Component> getCameraComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::CameraComponent>(i6engine::api::components::ComponentTypes::CameraComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::LifetimeComponent, i6engine::api::Component> getLifetimeComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::LifetimeComponent>(i6engine::api::components::ComponentTypes::LifetimeComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::LifetimeComponent, i6engine::api::Component> getLifetimeComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::LifetimeComponent>(i6engine::api::components::ComponentTypes::LifetimeComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::LuminousAppearanceComponent, i6engine::api::Component> getLuminousAppearanceComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::LuminousAppearanceComponent>(i6engine::api::components::ComponentTypes::LuminousAppearanceComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::LuminousAppearanceComponent, i6engine::api::Component> getLuminousAppearanceComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::LuminousAppearanceComponent>(i6engine::api::components::ComponentTypes::LuminousAppearanceComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::MeshAppearanceComponent, i6engine::api::Component> getMeshAppearanceComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::MeshAppearanceComponent>(i6engine::api::components::ComponentTypes::MeshAppearanceComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::MeshAppearanceComponent, i6engine::api::Component> getMeshAppearanceComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::MeshAppearanceComponent>(i6engine::api::components::ComponentTypes::MeshAppearanceComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::MoverCircleComponent, i6engine::api::Component> getMoverCircleComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::MoverCircleComponent>(i6engine::api::components::ComponentTypes::MoverCircleComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::MoverCircleComponent, i6engine::api::Component> getMoverCircleComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::MoverCircleComponent>(i6engine::api::components::ComponentTypes::MoverCircleComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::MoverComponent, i6engine::api::Component> getMoverComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::MoverComponent>(i6engine::api::components::ComponentTypes::MoverComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::MoverComponent, i6engine::api::Component> getMoverComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::MoverComponent>(i6engine::api::components::ComponentTypes::MoverComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::MoverInterpolateComponent, i6engine::api::Component> getMoverInterpolateComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::MoverInterpolateComponent>(i6engine::api::components::ComponentTypes::MoverInterpolateComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::MoverInterpolateComponent, i6engine::api::Component> getMoverInterpolateComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::MoverInterpolateComponent>(i6engine::api::components::ComponentTypes::MoverInterpolateComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::MovingCameraComponent, i6engine::api::Component> getMovingCameraComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::MovingCameraComponent>(i6engine::api::components::ComponentTypes::MovingCameraComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::MovingCameraComponent, i6engine::api::Component> getMovingCameraComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::MovingCameraComponent>(i6engine::api::components::ComponentTypes::MovingCameraComponent, identifier);
	}

#ifdef ISIXE_WITH_NETWORK
	i6engine::utils::sharedPtr<i6engine::api::NetworkSenderComponent, i6engine::api::Component> getNetworkSenderComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::NetworkSenderComponent>(i6engine::api::components::ComponentTypes::NetworkSenderComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::NetworkSenderComponent, i6engine::api::Component> getNetworkSenderComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::NetworkSenderComponent>(i6engine::api::components::ComponentTypes::NetworkSenderComponent, identifier);
	}
#endif

	i6engine::utils::sharedPtr<i6engine::api::ParticleEmitterComponent, i6engine::api::Component> getParticleEmitterComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::ParticleEmitterComponent>(i6engine::api::components::ComponentTypes::ParticleEmitterComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::ParticleEmitterComponent, i6engine::api::Component> getParticleEmitterComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::ParticleEmitterComponent>(i6engine::api::components::ComponentTypes::ParticleEmitterComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::PhysicalStateComponent, i6engine::api::Component> getPhysicalStateComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::PhysicalStateComponent>(i6engine::api::components::ComponentTypes::PhysicalStateComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::PhysicalStateComponent, i6engine::api::Component> getPhysicalStateComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::PhysicalStateComponent>(i6engine::api::components::ComponentTypes::PhysicalStateComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::ShatterComponent, i6engine::api::Component> getShatterComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::ShatterComponent>(i6engine::api::components::ComponentTypes::ShatterComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::ShatterComponent, i6engine::api::Component> getShatterComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::ShatterComponent>(i6engine::api::components::ComponentTypes::ShatterComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::SpawnpointComponent, i6engine::api::Component> getSpawnpointComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::SpawnpointComponent>(i6engine::api::components::ComponentTypes::SpawnpointComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::SpawnpointComponent, i6engine::api::Component> getSpawnpointComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::SpawnpointComponent>(i6engine::api::components::ComponentTypes::SpawnpointComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> getStaticStateComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::ComponentTypes::StaticStateComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> getStaticStateComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::ComponentTypes::StaticStateComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::TerrainAppearanceComponent, i6engine::api::Component> getTerrainAppearanceComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::TerrainAppearanceComponent>(i6engine::api::components::ComponentTypes::TerrainAppearanceComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::TerrainAppearanceComponent, i6engine::api::Component> getTerrainAppearanceComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::TerrainAppearanceComponent>(i6engine::api::components::ComponentTypes::TerrainAppearanceComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::SoundComponent, i6engine::api::Component> getSoundComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::SoundComponent>(i6engine::api::components::ComponentTypes::SoundComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::SoundComponent, i6engine::api::Component> getSoundComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::SoundComponent>(i6engine::api::components::ComponentTypes::SoundComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::SoundListenerComponent, i6engine::api::Component> getSoundListenerComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::SoundListenerComponent>(i6engine::api::components::ComponentTypes::SoundListenerComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::SoundListenerComponent, i6engine::api::Component> getSoundListenerComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::SoundListenerComponent>(i6engine::api::components::ComponentTypes::SoundListenerComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::BillboardComponent, i6engine::api::Component> getBillboardComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::BillboardComponent>(i6engine::api::components::ComponentTypes::BillboardComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::BillboardComponent, i6engine::api::Component> getBillboardComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::BillboardComponent>(i6engine::api::components::ComponentTypes::BillboardComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::FollowComponent, i6engine::api::Component> getFollowComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::FollowComponent>(i6engine::api::components::ComponentTypes::FollowComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::FollowComponent, i6engine::api::Component> getFollowComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::FollowComponent>(i6engine::api::components::ComponentTypes::FollowComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::MovableTextComponent, i6engine::api::Component> getMovableTextComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::MovableTextComponent>(i6engine::api::components::ComponentTypes::MovableTextComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::MovableTextComponent, i6engine::api::Component> getMovableTextComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::MovableTextComponent>(i6engine::api::components::ComponentTypes::MovableTextComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::WaypointComponent, i6engine::api::Component> getWaypointComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::WaypointComponent>(i6engine::api::components::ComponentTypes::WaypointComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::WaypointComponent, i6engine::api::Component> getWaypointComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::WaypointComponent>(i6engine::api::components::ComponentTypes::WaypointComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::NavigationComponent, i6engine::api::Component> getNavigationComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::NavigationComponent>(i6engine::api::components::ComponentTypes::NavigationComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::NavigationComponent, i6engine::api::Component> getNavigationComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::NavigationComponent>(i6engine::api::components::ComponentTypes::NavigationComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::WaynetNavigationComponent, i6engine::api::Component> getWaynetNavigationComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::WaynetNavigationComponent>(i6engine::api::components::ComponentTypes::WaynetNavigationComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::WaynetNavigationComponent, i6engine::api::Component> getWaynetNavigationComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::WaynetNavigationComponent>(i6engine::api::components::ComponentTypes::WaynetNavigationComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::MoveComponent, i6engine::api::Component> getMoveComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::MoveComponent>(i6engine::api::components::ComponentTypes::MoveComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::MoveComponent, i6engine::api::Component> getMoveComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::MoveComponent>(i6engine::api::components::ComponentTypes::MoveComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::MovementComponent, i6engine::api::Component> getMovementComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::MovementComponent>(i6engine::api::components::ComponentTypes::MovementComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::MovementComponent, i6engine::api::Component> getMovementComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::MovementComponent>(i6engine::api::components::ComponentTypes::MovementComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::ToggleWaynetComponent, i6engine::api::Component> getToggleWaynetComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::ToggleWaynetComponent>(i6engine::api::components::ComponentTypes::ToggleWaynetComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::ToggleWaynetComponent, i6engine::api::Component> getToggleWaynetComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::ToggleWaynetComponent>(i6engine::api::components::ComponentTypes::ToggleWaynetComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::Point2PointConstraintComponent, i6engine::api::Component> getPoint2PointConstraintComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::Point2PointConstraintComponent>(i6engine::api::components::ComponentTypes::Point2PointConstraintComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::Point2PointConstraintComponent, i6engine::api::Component> getPoint2PointConstraintComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::Point2PointConstraintComponent>(i6engine::api::components::ComponentTypes::Point2PointConstraintComponent, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::VelocityComponent, i6engine::api::Component> getVelocityComponent(i6engine::api::GameObject * go) {
		return go->getGOC<i6engine::api::VelocityComponent>(i6engine::api::components::ComponentTypes::VelocityComponent);
	}

	i6engine::utils::sharedPtr<i6engine::api::VelocityComponent, i6engine::api::Component> getVelocityComponent(i6engine::api::GameObject * go, const std::string & identifier) {
		return go->getGOC<i6engine::api::VelocityComponent>(i6engine::api::components::ComponentTypes::VelocityComponent, identifier);
	}

	struct CameraComponentWrapper : public i6engine::api::CameraComponent, public boost::python::wrapper<i6engine::api::CameraComponent> {
		CameraComponentWrapper() : CameraComponent(-1, i6engine::api::attributeMap()), boost::python::wrapper<i6engine::api::CameraComponent>() {
		}

		CameraComponentWrapper(const int64_t id, const attributeMap & params) : CameraComponent(id, params), boost::python::wrapper<i6engine::api::CameraComponent>() {
		}

		CameraComponentWrapper(const i6engine::api::CameraComponent & arg) : i6engine::api::CameraComponent(-1, i6engine::api::attributeMap()), boost::python::wrapper<i6engine::api::CameraComponent>() {
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

		virtual void News(const i6engine::api::GameMessage::Ptr & msg) override {
			if (boost::python::override n = this->get_override("News")) {
				boost::python::call<void>(n.ptr(), msg);
				return;
			}
			CameraComponent::News(msg);
		}

		void default_News(const i6engine::api::GameMessage::Ptr & msg) {
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

		virtual i6engine::api::attributeMap synchronize() const {
			i6engine::api::attributeMap params = CameraComponent::synchronize();
			i6engine::api::attributeMap params2 = boost::python::call<i6engine::api::attributeMap>(this->get_override("synchronize").ptr());
			params.insert(params2.begin(), params2.end());
			return params;
		}

		virtual std::pair<i6engine::api::AddStrategy, int64_t> howToAdd(const i6engine::api::ComPtr & comp) const override {
			return boost::python::call<std::pair<i6engine::api::AddStrategy, int64_t>>(this->get_override("howToAdd").ptr(), comp);
		}

		std::pair<i6engine::api::AddStrategy, int64_t> default_howToAdd(const i6engine::api::ComPtr & comp) {
			return this->CameraComponent::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return boost::python::call<std::string>(this->get_override("getTemplateName").ptr());
		}

		std::vector<i6engine::api::componentOptions> getComponentOptions() {
			return {};
		}

		void addTicker() {
			Component::addTicker();
		}

		void removeTicker() {
			Component::removeTicker();
		}
	};

	struct MovementComponentWrapper : public i6engine::api::MovementComponent, public boost::python::wrapper<i6engine::api::MovementComponent> {
		MovementComponentWrapper(const int64_t id, const attributeMap & params) : MovementComponent(id, params), boost::python::wrapper<i6engine::api::MovementComponent>() {
		}

		MovementComponentWrapper(const i6engine::api::MovementComponent & arg) : i6engine::api::MovementComponent(-1, i6engine::api::attributeMap()), boost::python::wrapper<i6engine::api::MovementComponent>() {
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

		virtual void News(const i6engine::api::GameMessage::Ptr & msg) override {
			if (boost::python::override n = this->get_override("News")) {
				return boost::python::call<void>(n.ptr(), msg);
			}
			Component::News(msg);
		}

		void default_News(const i6engine::api::GameMessage::Ptr & msg) {
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

		virtual i6engine::api::attributeMap synchronize() const {
			return boost::python::call<i6engine::api::attributeMap>(this->get_override("synchronize").ptr());
		}

		virtual std::pair<i6engine::api::AddStrategy, int64_t> howToAdd(const i6engine::api::ComPtr & comp) const override {
			return boost::python::call<std::pair<i6engine::api::AddStrategy, int64_t>>(this->get_override("howToAdd").ptr(), comp);
		}

		std::pair<i6engine::api::AddStrategy, int64_t> default_howToAdd(const i6engine::api::ComPtr & comp) {
			return this->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return boost::python::call<std::string>(this->get_override("getTemplateName").ptr());
		}

		std::vector<i6engine::api::componentOptions> getComponentOptions() {
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
	};

	struct MoverComponentWrapper : public i6engine::api::MoverComponent, public boost::python::wrapper<i6engine::api::MoverComponent> {
		MoverComponentWrapper(const int64_t id, const attributeMap & params) : MoverComponent(id, params), boost::python::wrapper<i6engine::api::MoverComponent>() {
		}

		MoverComponentWrapper(const i6engine::api::MoverComponent & arg) : i6engine::api::MoverComponent(-1, i6engine::api::attributeMap()), boost::python::wrapper<i6engine::api::MoverComponent>() {
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

		virtual void News(const i6engine::api::GameMessage::Ptr & msg) override {
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

		virtual i6engine::api::attributeMap synchronize() const {
			i6engine::api::attributeMap params = MoverComponent::synchronize();
			i6engine::api::attributeMap params2 = boost::python::call<i6engine::api::attributeMap>(this->get_override("synchronize").ptr());
			params.insert(params2.begin(), params2.end());
			return params;
		}

		virtual std::pair<i6engine::api::AddStrategy, int64_t> howToAdd(const i6engine::api::ComPtr & comp) const override {
			if (boost::python::override hta = this->get_override("howToAdd")) {
				return boost::python::call<std::pair<i6engine::api::AddStrategy, int64_t>>(hta.ptr(), comp);
			}
			return MoverComponent::howToAdd(comp);
		}

		std::pair<i6engine::api::AddStrategy, int64_t> default_howToAdd(const i6engine::api::ComPtr & comp) {
			return this->MoverComponent::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return boost::python::call<std::string>(this->get_override("getTemplateName").ptr());
		}

		std::vector<i6engine::api::componentOptions> getComponentOptions() {
			return {};
		}

		void start(Vec3 & startPos) override {
			boost::python::call<void>(this->get_override("start").ptr());
		}

		virtual void reset() override {
			boost::python::call<void>(this->get_override("reset").ptr());
		}

		void getNewPosition(const uint64_t time, Vec3 & pos, Quaternion & rot) override {
			boost::python::call<void>(this->get_override("getNewPosition").ptr(), time, pos, rot);
		}
	};

	struct NavigationComponentWrapper : public i6engine::api::NavigationComponent, public boost::python::wrapper<i6engine::api::NavigationComponent> {
		NavigationComponentWrapper(const int64_t id, const attributeMap & params) : NavigationComponent(id, params), boost::python::wrapper<i6engine::api::NavigationComponent>() {
		}

		NavigationComponentWrapper(const i6engine::api::NavigationComponent & arg) : i6engine::api::NavigationComponent(-1, i6engine::api::attributeMap()), boost::python::wrapper<i6engine::api::NavigationComponent>() {
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

		virtual void News(const i6engine::api::GameMessage::Ptr & msg) override {
			if (boost::python::override n = this->get_override("News")) {
				return boost::python::call<void>(n.ptr(), msg);
			}
			Component::News(msg);
		}

		void default_News(const i6engine::api::GameMessage::Ptr & msg) {
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

		virtual i6engine::api::attributeMap synchronize() const {
			return boost::python::call<i6engine::api::attributeMap>(this->get_override("synchronize").ptr());
		}

		virtual std::pair<i6engine::api::AddStrategy, int64_t> howToAdd(const i6engine::api::ComPtr & comp) const override {
			return boost::python::call<std::pair<i6engine::api::AddStrategy, int64_t>>(this->get_override("howToAdd").ptr(), comp);
		}

		std::pair<i6engine::api::AddStrategy, int64_t> default_howToAdd(const i6engine::api::ComPtr & comp) {
			return this->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return boost::python::call<std::string>(this->get_override("getTemplateName").ptr());
		}

		std::vector<i6engine::api::componentOptions> getComponentOptions() {
			return {};
		}

		std::vector<Vec3> getPath(const Vec3 & from, const Vec3 & to) const override {
			return boost::python::call<std::vector<Vec3>>(this->get_override("getPathPos").ptr(), from, to);
		}

		std::vector<Vec3> getPath(const Vec3 & from, const std::string & to) const override {
			return boost::python::call<std::vector<Vec3>>(this->get_override("getPathWP").ptr(), from, to);
		}
	};

	struct ShatterComponentWrapper : public i6engine::api::ShatterComponent, public boost::python::wrapper<i6engine::api::ShatterComponent> {
		ShatterComponentWrapper(const int64_t id, const attributeMap & params) : ShatterComponent(id, params), boost::python::wrapper<i6engine::api::ShatterComponent>() {
		}

		ShatterComponentWrapper(const i6engine::api::NavigationComponent & arg) : i6engine::api::ShatterComponent(-1, i6engine::api::attributeMap()), boost::python::wrapper<i6engine::api::ShatterComponent>() {
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

		virtual void News(const i6engine::api::GameMessage::Ptr & msg) override {
			if (boost::python::override n = this->get_override("News")) {
				boost::python::call<void>(n.ptr(), msg);
				return;
			}
			ShatterComponent::News(msg);
		}

		void default_News(const i6engine::api::GameMessage::Ptr & msg) {
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

		virtual i6engine::api::attributeMap synchronize() const {
			return boost::python::call<i6engine::api::attributeMap>(this->get_override("synchronize").ptr());
		}

		virtual std::pair<i6engine::api::AddStrategy, int64_t> howToAdd(const i6engine::api::ComPtr & comp) const override {
			return boost::python::call<std::pair<i6engine::api::AddStrategy, int64_t>>(this->get_override("howToAdd").ptr(), comp);
		}

		std::pair<i6engine::api::AddStrategy, int64_t> default_howToAdd(const i6engine::api::ComPtr & comp) {
			return this->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return boost::python::call<std::string>(this->get_override("getTemplateName").ptr());
		}

		std::vector<i6engine::api::componentOptions> getComponentOptions() {
			return {};
		}

		void shatter(const i6engine::api::GOPtr & other) override {
			boost::python::call<void>(this->get_override("shatter").ptr(), other);
		}
	};

	void accelerate(i6engine::api::VelocityComponent * c, const Vec3 & acceleration, i6engine::api::VelocityComponent::MaxSpeedHandling handling, const std::string & func) {
		c->accelerate(acceleration, handling, [func]() {
			if (!func.empty()) {
				i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func);
			}
		});
	}

	void accelerate(i6engine::api::VelocityComponent * c, const std::string & func) {
		c->accelerate([func]() {
			if (!func.empty()) {
				i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func);
			}
		});
	}

	void decelerate(i6engine::api::VelocityComponent * c, const Vec3 & deceleration, const std::string & func) {
		c->decelerate(deceleration, [func]() {
			if (!func.empty()) {
				i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func);
			}
		});
	}

	void decelerate(i6engine::api::VelocityComponent * c, const std::string & func) {
		c->decelerate([func]() {
			if (!func.empty()) {
				i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func);
			}
		});
	}

} /* namespace object */
} /* namespace python */
} /* namespace i6engine */

namespace boost {
namespace python {

	template<typename T, typename U>
	struct pointee<i6engine::utils::sharedPtr<T, U>> {
		typedef T type;
	};

} /* namespace python */
} /* namespace boost */

BOOST_PYTHON_MODULE(ScriptingObjectPython) {
	using namespace boost::python;

	class_<i6engine::api::GameObject, i6engine::api::GOPtr, boost::noncopyable>("GameObject", no_init)
		.def("getID", &i6engine::api::GameObject::getID)
		.def("getGOC", (i6engine::api::ComPtr(i6engine::api::GameObject::*)(uint32_t) const) &i6engine::api::GameObject::getGOC)
		.def("getGOC", (i6engine::api::ComPtr(i6engine::api::GameObject::*)(uint32_t, const std::string &) const) &i6engine::api::GameObject::getGOC)
		.def("getGOCID", &i6engine::api::GameObject::getGOCID)
		.def("getGOCList", &i6engine::api::GameObject::getGOCList)
		.def("getType", &i6engine::api::GameObject::getType)
		.def("setDie", &i6engine::api::GameObject::setDie)
		.def("getOwner", &i6engine::api::GameObject::getOwner)
		.def("getUUID", &i6engine::api::GameObject::getUUID)
		.def("getAnimatedDirectionalLightComponent", (i6engine::utils::sharedPtr<i6engine::api::AnimatedDirectionalLightComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getAnimatedDirectionalLightComponent)
		.def("getAnimatedDirectionalLightComponent", (i6engine::utils::sharedPtr<i6engine::api::AnimatedDirectionalLightComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getAnimatedDirectionalLightComponent)
		.def("getAnimatedLuminousAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::AnimatedLuminousAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getAnimatedLuminousAppearanceComponent)
		.def("getAnimatedLuminousAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::AnimatedLuminousAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getAnimatedLuminousAppearanceComponent)
		.def("getAnimatedSpotLightComponent", (i6engine::utils::sharedPtr<i6engine::api::AnimatedSpotLightComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getAnimatedSpotLightComponent)
		.def("getAnimatedSpotLightComponent", (i6engine::utils::sharedPtr<i6engine::api::AnimatedSpotLightComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getAnimatedSpotLightComponent)
		.def("getCameraComponent", (i6engine::utils::sharedPtr<i6engine::api::CameraComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getCameraComponent)
		.def("getCameraComponent", (i6engine::utils::sharedPtr<i6engine::api::CameraComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getCameraComponent)
		.def("getLifetimeComponent", (i6engine::utils::sharedPtr<i6engine::api::LifetimeComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getLifetimeComponent)
		.def("getLifetimeComponent", (i6engine::utils::sharedPtr<i6engine::api::LifetimeComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getLifetimeComponent)
		.def("getLuminousAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::LuminousAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getLuminousAppearanceComponent)
		.def("getLuminousAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::LuminousAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getLuminousAppearanceComponent)
		.def("getMeshAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::MeshAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getMeshAppearanceComponent)
		.def("getMeshAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::MeshAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getMeshAppearanceComponent)
		.def("getMoverCircleComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverCircleComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getMoverCircleComponent)
		.def("getMoverCircleComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverCircleComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getMoverCircleComponent)
		.def("getMoverComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getMoverComponent)
		.def("getMoverComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getMoverComponent)
		.def("getMoverInterpolateComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverInterpolateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getMoverInterpolateComponent)
		.def("getMoverInterpolateComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverInterpolateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getMoverInterpolateComponent)
		.def("getMovingCameraComponent", (i6engine::utils::sharedPtr<i6engine::api::MovingCameraComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getMovingCameraComponent)
		.def("getMovingCameraComponent", (i6engine::utils::sharedPtr<i6engine::api::MovingCameraComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getMovingCameraComponent)
#ifdef ISIXE_WITH_NETWORK
		.def("getNetworkSenderComponent", (i6engine::utils::sharedPtr<i6engine::api::NetworkSenderComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getNetworkSenderComponent)
		.def("getNetworkSenderComponent", (i6engine::utils::sharedPtr<i6engine::api::NetworkSenderComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getNetworkSenderComponent)
#endif
		.def("getParticleEmitterComponent", (i6engine::utils::sharedPtr<i6engine::api::ParticleEmitterComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getParticleEmitterComponent)
		.def("getParticleEmitterComponent", (i6engine::utils::sharedPtr<i6engine::api::ParticleEmitterComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getParticleEmitterComponent)
		.def("getPhysicalStateComponent", (i6engine::utils::sharedPtr<i6engine::api::PhysicalStateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getPhysicalStateComponent)
		.def("getPhysicalStateComponent", (i6engine::utils::sharedPtr<i6engine::api::PhysicalStateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getPhysicalStateComponent)
		.def("getShatterComponent", (i6engine::utils::sharedPtr<i6engine::api::ShatterComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getShatterComponent)
		.def("getShatterComponent", (i6engine::utils::sharedPtr<i6engine::api::ShatterComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getShatterComponent)
		.def("getSpawnpointComponent", (i6engine::utils::sharedPtr<i6engine::api::SpawnpointComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getSpawnpointComponent)
		.def("getSpawnpointComponent", (i6engine::utils::sharedPtr<i6engine::api::SpawnpointComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getSpawnpointComponent)
		.def("getStaticStateComponent", (i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getStaticStateComponent)
		.def("getStaticStateComponent", (i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getStaticStateComponent)
		.def("getTerrainAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::TerrainAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getTerrainAppearanceComponent)
		.def("getTerrainAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::TerrainAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getTerrainAppearanceComponent)
		.def("getSoundComponent", (i6engine::utils::sharedPtr<i6engine::api::SoundComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getSoundComponent)
		.def("getSoundComponent", (i6engine::utils::sharedPtr<i6engine::api::SoundComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getSoundComponent)
		.def("getSoundListenerComponent", (i6engine::utils::sharedPtr<i6engine::api::SoundListenerComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getSoundListenerComponent)
		.def("getSoundListenerComponent", (i6engine::utils::sharedPtr<i6engine::api::SoundListenerComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getSoundListenerComponent)
		.def("getBillboardComponent", (i6engine::utils::sharedPtr<i6engine::api::BillboardComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getBillboardComponent)
		.def("getBillboardComponent", (i6engine::utils::sharedPtr<i6engine::api::BillboardComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getBillboardComponent)
		.def("getFollowComponent", (i6engine::utils::sharedPtr<i6engine::api::FollowComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getFollowComponent)
		.def("getFollowComponent", (i6engine::utils::sharedPtr<i6engine::api::FollowComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getFollowComponent)
		.def("getMovableTextComponent", (i6engine::utils::sharedPtr<i6engine::api::MovableTextComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getMovableTextComponent)
		.def("getMovableTextComponent", (i6engine::utils::sharedPtr<i6engine::api::MovableTextComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getMovableTextComponent)
		.def("getWaypointComponent", (i6engine::utils::sharedPtr<i6engine::api::WaypointComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getWaypointComponent)
		.def("getWaypointComponent", (i6engine::utils::sharedPtr<i6engine::api::WaypointComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getWaypointComponent)
		.def("getNavigationComponent", (i6engine::utils::sharedPtr<i6engine::api::NavigationComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getNavigationComponent)
		.def("getNavigationComponent", (i6engine::utils::sharedPtr<i6engine::api::NavigationComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getNavigationComponent)
		.def("getWaynetNavigationComponent", (i6engine::utils::sharedPtr<i6engine::api::WaynetNavigationComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getWaynetNavigationComponent)
		.def("getWaynetNavigationComponent", (i6engine::utils::sharedPtr<i6engine::api::WaynetNavigationComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getWaynetNavigationComponent)
		.def("getMoveComponent", (i6engine::utils::sharedPtr<i6engine::api::MoveComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getMoveComponent)
		.def("getMoveComponent", (i6engine::utils::sharedPtr<i6engine::api::MoveComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getMoveComponent)
		.def("getMovementComponent", (i6engine::utils::sharedPtr<i6engine::api::MovementComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getMovementComponent)
		.def("getMovementComponent", (i6engine::utils::sharedPtr<i6engine::api::MovementComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getMovementComponent)
		.def("getToggleWaynetComponent", (i6engine::utils::sharedPtr<i6engine::api::ToggleWaynetComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getToggleWaynetComponent)
		.def("getToggleWaynetComponent", (i6engine::utils::sharedPtr<i6engine::api::ToggleWaynetComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getToggleWaynetComponent)
		.def("getPoint2PointConstraintComponent", (i6engine::utils::sharedPtr<i6engine::api::Point2PointConstraintComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getPoint2PointConstraintComponent)
		.def("getPoint2PointConstraintComponent", (i6engine::utils::sharedPtr<i6engine::api::Point2PointConstraintComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getPoint2PointConstraintComponent)
		.def("getVelocityComponent", (i6engine::utils::sharedPtr<i6engine::api::VelocityComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *)) &i6engine::python::object::getVelocityComponent)
		.def("getVelocityComponent", (i6engine::utils::sharedPtr<i6engine::api::VelocityComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, const std::string &)) &i6engine::python::object::getVelocityComponent);
	
	enum_<i6engine::api::AddStrategy>("AddStrategy")
		.value("ADD", i6engine::api::AddStrategy::ADD)
		.value("REPLACE", i6engine::api::AddStrategy::REPLACE)
		.value("REPLACE_DIS", i6engine::api::AddStrategy::REPLACE_DIS)
		.value("REJECT", i6engine::api::AddStrategy::REJECT)
		.export_values();

	class_<std::pair<i6engine::api::AddStrategy, int64_t>>("AddStrategyPair")
		.def(init<>())
		.def_readwrite("first", &std::pair<i6engine::api::AddStrategy, int64_t>::first)
		.def_readwrite("second", &std::pair<i6engine::api::AddStrategy, int64_t>::second);

	class_<i6engine::python::object::ComponentWrapper, i6engine::api::ComPtr, boost::noncopyable>("Component", no_init)
		.def("getOwnerGO", &i6engine::api::Component::getOwnerGO)
		.def("getComponentID", &i6engine::api::Component::getComponentID)
		.def("getFamilyID", &i6engine::api::Component::getFamilyID)
		.def("getIdentifier", &i6engine::api::Component::getIdentifier)
		.def("Tick", &i6engine::api::Component::Tick, &i6engine::python::object::ComponentWrapper::default_Tick)
		.def("setDie", &i6engine::api::Component::setDie)
		.def("getID", &i6engine::api::Component::getID)
		.def("News", &i6engine::api::Component::News, &i6engine::python::object::ComponentWrapper::default_News)
		.def("Init", pure_virtual(&i6engine::python::object::ComponentWrapper::Init))
		.def("Finalize", &i6engine::api::Component::Finalize, &i6engine::python::object::ComponentWrapper::default_Finalize)
		.def("synchronize", pure_virtual(&i6engine::python::object::ComponentWrapper::synchronize))
		.def("setSync", &i6engine::api::Component::setSync)
		.def("getSync", &i6engine::api::Component::getSync)
		//.def("howToAdd", &i6engine::api::Component::howToAdd, &i6engine::python::object::ComponentWrapper::default_howToAdd)
		.def("getTemplateName", pure_virtual(&i6engine::python::object::ComponentWrapper::getTemplateName))
		.def("addTicker", &i6engine::python::object::ComponentWrapper::addTicker)
		.def("removeTicker", &i6engine::python::object::ComponentWrapper::removeTicker);

	class_<i6engine::api::AnimatedDirectionalLightComponent, i6engine::utils::sharedPtr<i6engine::api::AnimatedDirectionalLightComponent, i6engine::api::Component>, boost::noncopyable>("AnimatedDirectionalLightComponent", no_init)
		.def("synchronize", &i6engine::api::AnimatedDirectionalLightComponent::synchronize)
		.def("getTemplateName", &i6engine::api::AnimatedDirectionalLightComponent::getTemplateName);

	class_<i6engine::api::AnimatedLuminousAppearanceComponent, i6engine::utils::sharedPtr<i6engine::api::AnimatedLuminousAppearanceComponent, i6engine::api::Component>, boost::noncopyable>("AnimatedLuminousAppearanceComponent", no_init)
		.def("synchronize", &i6engine::api::LuminousAppearanceComponent::synchronize)
		.def("getTemplateName", &i6engine::api::LuminousAppearanceComponent::getTemplateName);

	class_<i6engine::api::AnimatedSpotLightComponent, i6engine::utils::sharedPtr<i6engine::api::AnimatedSpotLightComponent, i6engine::api::Component>, boost::noncopyable>("AnimatedSpotLightComponent", no_init)
		.def("synchronize", &i6engine::api::AnimatedDirectionalLightComponent::synchronize)
		.def("getTemplateName", &i6engine::api::AnimatedDirectionalLightComponent::getTemplateName);

	class_<i6engine::api::BillboardComponent, i6engine::utils::sharedPtr<i6engine::api::BillboardComponent, i6engine::api::Component>, boost::noncopyable>("BillboardComponent", no_init)
		.def("createOrUpdateBillboard", &i6engine::api::BillboardComponent::createOrUpdateBillboard)
		.def("deleteBillboard", &i6engine::api::BillboardComponent::deleteBillboard)
		.def("synchronize", &i6engine::api::BillboardComponent::synchronize)
		.def("getTemplateName", &i6engine::api::BillboardComponent::getTemplateName);

	class_<i6engine::python::object::CameraComponentWrapper, i6engine::utils::sharedPtr<i6engine::api::CameraComponent, i6engine::api::Component>, boost::noncopyable>("CameraComponent", no_init)
		.def("Tick", &i6engine::api::Component::Tick, &i6engine::python::object::CameraComponentWrapper::default_Tick)
		.def("News", &i6engine::api::Component::News, &i6engine::python::object::CameraComponentWrapper::default_News)
		.def("Init", &i6engine::python::object::CameraComponentWrapper::Init)
		.def("Finalize", &i6engine::api::Component::Finalize, &i6engine::python::object::CameraComponentWrapper::default_Finalize)
		.def("synchronize", &i6engine::python::object::CameraComponentWrapper::synchronize)
		//.def("howToAdd", &i6engine::api::Component::howToAdd, &i6engine::python::object::CameraComponentWrapper::default_howToAdd)
		.def("getTemplateName", &i6engine::python::object::CameraComponentWrapper::getTemplateName)
		.def("addTicker", &i6engine::python::object::CameraComponentWrapper::addTicker)
		.def("removeTicker", &i6engine::python::object::CameraComponentWrapper::removeTicker)
		.def("setPosition", &i6engine::api::CameraComponent::setPosition)
		.def("setLookAt", &i6engine::api::CameraComponent::setLookAt)
		.def("setNearClip", &i6engine::api::CameraComponent::setNearClip)
		.def("setAspectRatio", &i6engine::api::CameraComponent::setAspectRatio)
		.def("setFOVy", &i6engine::api::CameraComponent::setFOVy)
		.def("setFrustumExtends", &i6engine::api::CameraComponent::setFrustumExtends)
		.def("setViewportDimension", &i6engine::api::CameraComponent::setViewportDimension)
		.def("setViewportBackground", &i6engine::api::CameraComponent::setViewportBackground)
		.def("getPosition", &i6engine::api::CameraComponent::getPosition)
		.def("getLookAt", &i6engine::api::CameraComponent::getLookAt)
		.def("enableCompositor", &i6engine::api::CameraComponent::enableCompositor);

	class_<i6engine::api::FollowComponent, i6engine::utils::sharedPtr<i6engine::api::FollowComponent, i6engine::api::Component>, boost::noncopyable>("FollowComponent", no_init)
		.def("synchronize", &i6engine::api::FollowComponent::synchronize)
		.def("getTemplateName", &i6engine::api::FollowComponent::getTemplateName);

	class_<i6engine::api::LifetimeComponent, i6engine::utils::sharedPtr<i6engine::api::LifetimeComponent, i6engine::api::Component>, boost::noncopyable>("LifetimeComponent", no_init)
		.def("synchronize", &i6engine::api::LifetimeComponent::synchronize)
		.def("getTemplateName", &i6engine::api::LifetimeComponent::getTemplateName)
		.def("instantKill", &i6engine::api::LifetimeComponent::instantKill);

	class_<i6engine::api::LuminousAppearanceComponent, i6engine::utils::sharedPtr<i6engine::api::LuminousAppearanceComponent, i6engine::api::Component>, boost::noncopyable>("LuminousAppearanceComponent", no_init)
		.def("synchronize", &i6engine::api::LuminousAppearanceComponent::synchronize)
		.def("getTemplateName", &i6engine::api::LuminousAppearanceComponent::getTemplateName)
		.def("setLightType", &i6engine::api::LuminousAppearanceComponent::setLightType)
		.def("getLightType", &i6engine::api::LuminousAppearanceComponent::getLightType)
		.def("setDiffuseColor", &i6engine::api::LuminousAppearanceComponent::setDiffuseColor)
		.def("getDiffuseColor", &i6engine::api::LuminousAppearanceComponent::getDiffuseColor)
		.def("setSpecularColor", &i6engine::api::LuminousAppearanceComponent::setSpecularColor)
		.def("getSpecularColor", &i6engine::api::LuminousAppearanceComponent::getSpecularColor)
		.def("setAttenuation", &i6engine::api::LuminousAppearanceComponent::setAttenuation)
		.def("getAttenuation", &i6engine::api::LuminousAppearanceComponent::getAttenuation)
		.def("setDirection", &i6engine::api::LuminousAppearanceComponent::setDirection)
		.def("getDirection", &i6engine::api::LuminousAppearanceComponent::getDirection);

	enum_<i6engine::api::LuminousAppearanceComponent::LightType>("LightType")
		.value("POINT", i6engine::api::LuminousAppearanceComponent::LightType::POINT)
		.value("DIRECTIONAL", i6engine::api::LuminousAppearanceComponent::LightType::DIRECTIONAL)
		.value("SPOT", i6engine::api::LuminousAppearanceComponent::LightType::SPOT)
		.export_values();

	class_<i6engine::api::MeshAppearanceComponent, i6engine::utils::sharedPtr<i6engine::api::MeshAppearanceComponent, i6engine::api::Component>, boost::noncopyable>("MeshAppearanceComponent", no_init)
		.def("synchronize", &i6engine::api::MeshAppearanceComponent::synchronize)
		.def("getTemplateName", &i6engine::api::MeshAppearanceComponent::getTemplateName)
		.def("getPosition", &i6engine::api::MeshAppearanceComponent::getPosition)
		.def("setVisibility", &i6engine::api::MeshAppearanceComponent::setVisibility)
		.def("setMesh", &i6engine::api::MeshAppearanceComponent::setMesh)
		.def("setCustomParameter", &i6engine::api::MeshAppearanceComponent::setCustomParameter)
		.def("getVisibility", &i6engine::api::MeshAppearanceComponent::getVisibility)
		.def("getMesh", &i6engine::api::MeshAppearanceComponent::getMesh)
		.def("setMaterial", &i6engine::api::MeshAppearanceComponent::setMaterial)
		.def("playAnimation", &i6engine::api::MeshAppearanceComponent::playAnimation)
		.def("setAnimationSpeed", &i6engine::api::MeshAppearanceComponent::setAnimationSpeed)
		.def("stopAnimation", &i6engine::api::MeshAppearanceComponent::stopAnimation)
		.def("drawBoundingBox", &i6engine::api::MeshAppearanceComponent::drawBoundingBox)
		.def("removeBoundingBox", &i6engine::api::MeshAppearanceComponent::removeBoundingBox)
		.def("attachGameObjectToBone", &i6engine::api::MeshAppearanceComponent::attachGameObjectToBone)
		.def("detachGameObjectFromBone", &i6engine::api::MeshAppearanceComponent::detachGameObjectFromBone);

	class_<i6engine::api::MovableTextComponent, i6engine::utils::sharedPtr<i6engine::api::MovableTextComponent, i6engine::api::Component>, boost::noncopyable>("MovableTextComponent", no_init)
		.def("synchronize", &i6engine::api::MovableTextComponent::synchronize)
		.def("getTemplateName", &i6engine::api::MovableTextComponent::getTemplateName)
		.def("setText", &i6engine::api::MovableTextComponent::setText);

	class_<i6engine::api::MoveComponent, i6engine::utils::sharedPtr<i6engine::api::MoveComponent, i6engine::api::Component>, boost::noncopyable>("MoveComponent", no_init)
		.def("synchronize", &i6engine::api::MoveComponent::synchronize)
		.def("getTemplateName", &i6engine::api::MoveComponent::getTemplateName)
		.def("navigate", (void(i6engine::api::MoveComponent::*)(const Vec3 &)) &i6engine::api::MoveComponent::navigate)
		.def("navigate", (void(i6engine::api::MoveComponent::*)(const std::string &)) &i6engine::api::MoveComponent::navigate);

	class_<i6engine::python::object::MovementComponentWrapper, i6engine::utils::sharedPtr<i6engine::api::MovementComponent, i6engine::api::Component>, boost::noncopyable>("MovementComponent", no_init)
		.def("Tick", &i6engine::api::MovementComponent::Tick, &i6engine::python::object::MovementComponentWrapper::default_Tick)
		.def("News", &i6engine::api::MovementComponent::News, &i6engine::python::object::MovementComponentWrapper::default_News)
		.def("Init", &i6engine::python::object::MovementComponentWrapper::Init)
		.def("Finalize", &i6engine::api::MovementComponent::Finalize, &i6engine::python::object::MovementComponentWrapper::default_Finalize)
		.def("synchronize", pure_virtual(&i6engine::python::object::MovementComponentWrapper::synchronize))
		//.def("howToAdd", &i6engine::api::Component::howToAdd, &i6engine::python::object::MovementComponentWrapper::default_howToAdd)
		.def("getTemplateName", pure_virtual(&i6engine::python::object::MovementComponentWrapper::getTemplateName))
		.def("forward", pure_virtual(&i6engine::python::object::MovementComponentWrapper::forward))
		.def("backward", pure_virtual(&i6engine::python::object::MovementComponentWrapper::backward))
		.def("left", pure_virtual(&i6engine::python::object::MovementComponentWrapper::left))
		.def("right", pure_virtual(&i6engine::python::object::MovementComponentWrapper::right));

	class_<i6engine::api::MoverCircleComponent, i6engine::utils::sharedPtr<i6engine::api::MoverCircleComponent, i6engine::api::Component>, boost::noncopyable>("MoverCircleComponent", no_init)
		.def("synchronize", &i6engine::api::MoverCircleComponent::synchronize)
		.def("getTemplateName", &i6engine::api::MoverCircleComponent::getTemplateName)
		.def("setPositioning", &i6engine::api::MoverCircleComponent::setPositioning)
		.def("setCircleParameters", &i6engine::api::MoverCircleComponent::setCircleParameters)
		.def("start", &i6engine::api::MoverCircleComponent::start)
		.def("getCircleAxis", &i6engine::api::MoverCircleComponent::getCircleAxis)
		.def("getCircleRadius", &i6engine::api::MoverCircleComponent::getCircleRadius)
		.def("reset", &i6engine::api::MoverCircleComponent::reset);

	class_<i6engine::python::object::MoverComponentWrapper, i6engine::utils::sharedPtr<i6engine::api::MoverComponent, i6engine::api::Component>, boost::noncopyable>("MoverComponent", no_init)
		.def("Tick", &i6engine::api::MoverComponent::Tick, &i6engine::python::object::MoverComponentWrapper::default_Tick)
		.def("News", &i6engine::python::object::MoverComponentWrapper::News)
		.def("Init", &i6engine::python::object::MoverComponentWrapper::Init)
		.def("Finalize", &i6engine::api::MoverComponent::Finalize, &i6engine::python::object::MoverComponentWrapper::default_Finalize)
		.def("synchronize", &i6engine::python::object::MoverComponentWrapper::synchronize)
		//.def("howToAdd", &i6engine::api::Component::howToAdd, &i6engine::python::object::MoverComponentWrapper::default_howToAdd)
		.def("getTemplateName", &i6engine::python::object::MoverComponentWrapper::getTemplateName)
		.def("start", &i6engine::python::object::MoverComponentWrapper::start)
		.def("reset", &i6engine::python::object::MoverComponentWrapper::reset);

	enum_<i6engine::api::MoverComponent::Positioning>("Positioning")
		.value("POSITIONING_ABSOLUTE", i6engine::api::MoverComponent::Positioning::POSITIONING_ABSOLUTE)
		.value("POSITIONING_RELATIVE", i6engine::api::MoverComponent::Positioning::POSITIONING_RELATIVE)
		.export_values();

	class_<i6engine::api::MoverInterpolateComponent, i6engine::utils::sharedPtr<i6engine::api::MoverInterpolateComponent, i6engine::api::Component>, boost::noncopyable>("MoverInterpolateComponent", no_init)
		.def("synchronize", &i6engine::api::MoverInterpolateComponent::synchronize)
		.def("getTemplateName", &i6engine::api::MoverInterpolateComponent::getTemplateName)
		.def("addKeyFrame", &i6engine::api::MoverInterpolateComponent::addKeyFrame)
		.def("removeKeyFrame", &i6engine::api::MoverInterpolateComponent::removeKeyFrame)
		.def("getKeyframe", &i6engine::api::MoverInterpolateComponent::getKeyframe)
		.def("setMode", &i6engine::api::MoverInterpolateComponent::setMode)
		.def("setOpenTime", &i6engine::api::MoverInterpolateComponent::setOpenTime)
		.def("setWay", &i6engine::api::MoverInterpolateComponent::setWay)
		.def("getWay", &i6engine::api::MoverInterpolateComponent::getWay)
		.def("start", &i6engine::api::MoverInterpolateComponent::start)
		.def("reset", &i6engine::api::MoverInterpolateComponent::reset);

	enum_<i6engine::api::MoverInterpolateComponent::Mode>("Mode")
		.value("TWOSTATE_TOGGLE", i6engine::api::MoverInterpolateComponent::Mode::TWOSTATE_TOGGLE)
		.value("TWOSTATE_OPENTIME", i6engine::api::MoverInterpolateComponent::Mode::TWOSTATE_OPENTIME)
		.value("NSTATE_LOOP", i6engine::api::MoverInterpolateComponent::Mode::NSTATE_LOOP)
		.value("ONCE", i6engine::api::MoverInterpolateComponent::Mode::ONCE)
		.export_values();
		
	enum_<i6engine::api::MoverInterpolateComponent::Way>("Way")
		.value("LINEAR", i6engine::api::MoverInterpolateComponent::Way::LINEAR)
		.value("BEZIER", i6engine::api::MoverInterpolateComponent::Way::BEZIER)
		.export_values();

	class_<i6engine::api::MovingCameraComponent, i6engine::utils::sharedPtr<i6engine::api::MovingCameraComponent, i6engine::api::Component>, boost::noncopyable>("MovingCameraComponent", no_init)
		.def("getTemplateName", &i6engine::api::MoverCircleComponent::getTemplateName),

	class_<i6engine::python::object::NavigationComponentWrapper, i6engine::utils::sharedPtr<i6engine::api::NavigationComponent, i6engine::api::Component>, boost::noncopyable>("NavigationComponent", no_init)
		.def("Tick", &i6engine::api::Component::Tick, &i6engine::python::object::NavigationComponentWrapper::default_Tick)
		.def("News", &i6engine::api::Component::News, &i6engine::python::object::NavigationComponentWrapper::default_News)
		.def("Init", &i6engine::python::object::NavigationComponentWrapper::Init)
		.def("Finalize", &i6engine::api::Component::Finalize, &i6engine::python::object::NavigationComponentWrapper::default_Finalize)
		.def("synchronize", &i6engine::python::object::NavigationComponentWrapper::synchronize)
		//.def("howToAdd", &i6engine::api::Component::howToAdd, &i6engine::python::object::NavigationComponentWrapper::default_howToAdd)
		.def("getTemplateName", &i6engine::python::object::NavigationComponentWrapper::getTemplateName)
		.def("getPathPos", (std::vector<Vec3>(i6engine::python::object::NavigationComponentWrapper::*)(const Vec3 &, const Vec3 &) const) &i6engine::python::object::NavigationComponentWrapper::getPath)
		.def("getPathWP", (std::vector<Vec3>(i6engine::python::object::NavigationComponentWrapper::*)(const Vec3 &, const std::string &) const) &i6engine::python::object::NavigationComponentWrapper::getPath);

#ifdef ISIXE_WITH_NETWORK
	class_<i6engine::api::NetworkSenderComponent, i6engine::utils::sharedPtr<i6engine::api::NetworkSenderComponent, i6engine::api::Component>, boost::noncopyable>("NetworkSenderComponent", no_init)
		.def("getTemplateName", &i6engine::api::NetworkSenderComponent::getTemplateName);
#endif

	class_<i6engine::api::ParticleEmitterComponent, i6engine::utils::sharedPtr<i6engine::api::ParticleEmitterComponent, i6engine::api::Component>, boost::noncopyable>("ParticleEmitterComponent", no_init)
		.def("synchronize", &i6engine::api::ParticleEmitterComponent::synchronize)
		.def("getTemplateName", &i6engine::api::ParticleEmitterComponent::getTemplateName);

	enum_<i6engine::api::ResponseType::ResponseType>("ResponseType")
		.value("NONE", i6engine::api::ResponseType::ResponseType::NONE)
		.value("STATIC", i6engine::api::ResponseType::ResponseType::STATIC)
		.value("GHOST", i6engine::api::ResponseType::ResponseType::GHOST)
		.value("TRIGGER", i6engine::api::ResponseType::ResponseType::TRIGGER)
		.export_values();

	enum_<i6engine::api::ShatterInterest>("ShatterInterest")
		.value("NONE", i6engine::api::ShatterInterest::NONE)
		.value("START", i6engine::api::ShatterInterest::START)
		.value("END", i6engine::api::ShatterInterest::END)
		.value("ALWAYS", i6engine::api::ShatterInterest::ALWAYS)
		.export_values();

	class_<i6engine::api::PhysicalStateComponent, i6engine::utils::sharedPtr<i6engine::api::PhysicalStateComponent, i6engine::api::Component>, boost::noncopyable>("PhysicalStateComponent", no_init)
		.def("getPosition", &i6engine::api::PhysicalStateComponent::getPosition)
		.def("setPosition", &i6engine::api::PhysicalStateComponent::setPosition)
		.def("getRotation", &i6engine::api::PhysicalStateComponent::getRotation)
		.def("setRotation", &i6engine::api::PhysicalStateComponent::setRotation)
		.def("getScale", &i6engine::api::PhysicalStateComponent::getScale)
		.def("setScale", &i6engine::api::PhysicalStateComponent::setScale)
		.def("setCollisionFlags", &i6engine::api::PhysicalStateComponent::setCollisionFlags)
		.def("getCollisionFlags", &i6engine::api::PhysicalStateComponent::getCollisionFlags)
		.def("reset", &i6engine::api::PhysicalStateComponent::reset)
		.def("setCollisionShape", &i6engine::api::PhysicalStateComponent::setCollisionShape)
		.def("applyRotation", &i6engine::api::PhysicalStateComponent::applyRotation)
		.def("getLinearVelocity", &i6engine::api::PhysicalStateComponent::getLinearVelocity)
		.def("setLinearVelocity", &i6engine::api::PhysicalStateComponent::setLinearVelocity)
		.def("applyCentralForce", &i6engine::api::PhysicalStateComponent::applyCentralForce)
		.def("applyForce", &i6engine::api::PhysicalStateComponent::applyForce)
		.def("setShatterInterest", &i6engine::api::PhysicalStateComponent::setShatterInterest)
		.def("setGravity", &i6engine::api::PhysicalStateComponent::setGravity)
		.def("synchronize", &i6engine::api::PhysicalStateComponent::synchronize)
		.def("rayTest", &i6engine::api::PhysicalStateComponent::rayTest)
		.def("getTemplateName", &i6engine::api::PhysicalStateComponent::getTemplateName)
		.def("addPosition", &i6engine::api::PhysicalStateComponent::addPosition);
	
	enum_<i6engine::api::PhysicalStateComponent::ShapeType>("ShapeType")
		.value("PLANE", i6engine::api::PhysicalStateComponent::ShapeType::PLANE)
		.value("BOX", i6engine::api::PhysicalStateComponent::ShapeType::BOX)
		.value("SPHERE", i6engine::api::PhysicalStateComponent::ShapeType::SPHERE)
		.value("FILE", i6engine::api::PhysicalStateComponent::ShapeType::FILE)
		.export_values();

	enum_<i6engine::api::PhysicalStateComponent::RayTestRepetition>("RayTestRepetition")
		.value("STOP", i6engine::api::PhysicalStateComponent::RayTestRepetition::STOP)
		.value("ONCE", i6engine::api::PhysicalStateComponent::RayTestRepetition::ONCE)
		.value("PERIODIC", i6engine::api::PhysicalStateComponent::RayTestRepetition::PERIODIC)
		.export_values();

	enum_<i6engine::api::PhysicalStateComponent::RayTestNotify>("RayTestNotify")
		.value("ALWAYS", i6engine::api::PhysicalStateComponent::RayTestNotify::ALWAYS)
		.value("CHANGE", i6engine::api::PhysicalStateComponent::RayTestNotify::CHANGE)
		.value("FOUND", i6engine::api::PhysicalStateComponent::RayTestNotify::FOUND)
		.value("NOTFOUND", i6engine::api::PhysicalStateComponent::RayTestNotify::NOTFOUND)
		.value("OBJECTCHANGE", i6engine::api::PhysicalStateComponent::RayTestNotify::OBJECTCHANGE)
		.export_values();

	class_<i6engine::api::RayTestResult>("RayTestResult")
		.def(init<>())
		.def_readonly("objID", &i6engine::api::RayTestResult::objID)
		.def_readonly("sourceID", &i6engine::api::RayTestResult::sourceID)
		.def_readonly("collisionPoint", &i6engine::api::RayTestResult::collisionPoint);

	class_<i6engine::api::Point2PointConstraintComponent, i6engine::utils::sharedPtr<i6engine::api::Point2PointConstraintComponent, i6engine::api::Component>, boost::noncopyable>("Point2PointConstraintComponent", no_init)
		.def("synchronize", &i6engine::api::Point2PointConstraintComponent::synchronize)
		.def("getTemplateName", &i6engine::api::Point2PointConstraintComponent::getTemplateName);

	class_<i6engine::python::object::ShatterComponentWrapper, i6engine::utils::sharedPtr<i6engine::api::ShatterComponent, i6engine::api::Component>, boost::noncopyable>("ShatterComponent", no_init)
		.def("Tick", &i6engine::api::Component::Tick, &i6engine::python::object::ShatterComponentWrapper::default_Tick)
		.def("News", &i6engine::api::ShatterComponent::News, &i6engine::python::object::ShatterComponentWrapper::default_News)
		.def("Init", &i6engine::api::ShatterComponent::Init, &i6engine::python::object::ShatterComponentWrapper::Init)
		.def("Finalize", &i6engine::api::Component::Finalize, &i6engine::python::object::ShatterComponentWrapper::default_Finalize)
		.def("synchronize", &i6engine::python::object::CameraComponentWrapper::synchronize)
		//.def("howToAdd", &i6engine::api::Component::howToAdd, &i6engine::python::object::ShatterComponentWrapper::default_howToAdd)
		.def("getTemplateName", &i6engine::python::object::ShatterComponentWrapper::getTemplateName)
		.def("shatter", pure_virtual(&i6engine::python::object::ShatterComponentWrapper::shatter))
		.def("resetRespawn", &i6engine::api::ShatterComponent::resetRespawn);

	class_<i6engine::api::SoundComponent, i6engine::utils::sharedPtr<i6engine::api::SoundComponent, i6engine::api::Component>, boost::noncopyable>("SoundComponent", no_init)
		.def("synchronize", &i6engine::api::SoundComponent::synchronize)
		.def("getTemplateName", &i6engine::api::SoundComponent::getTemplateName);

	class_<i6engine::api::SoundListenerComponent, i6engine::utils::sharedPtr<i6engine::api::SoundListenerComponent, i6engine::api::Component>, boost::noncopyable>("SoundListenerComponent", no_init)
		.def("synchronize", &i6engine::api::SoundListenerComponent::synchronize)
		.def("getTemplateName", &i6engine::api::SoundListenerComponent::getTemplateName);

	class_<i6engine::api::SpawnpointComponent, i6engine::utils::sharedPtr<i6engine::api::SpawnpointComponent, i6engine::api::Component>, boost::noncopyable>("SpawnpointComponent", no_init)
		.def("synchronize", &i6engine::api::SpawnpointComponent::synchronize)
		.def("getTemplateName", &i6engine::api::SpawnpointComponent::getTemplateName)
		.def("addSpawntype", &i6engine::api::SpawnpointComponent::addSpawntype)
		.def("addSpawntypes", &i6engine::api::SpawnpointComponent::addSpawntypes)
		.def("removeSpawntype", &i6engine::api::SpawnpointComponent::removeSpawntype)
		.def("containsSpawntype", &i6engine::api::SpawnpointComponent::containsSpawntype)
		.def("available", &i6engine::api::SpawnpointComponent::available)
		.def("setState", &i6engine::api::SpawnpointComponent::setState);

	class_<i6engine::api::StaticStateComponent, i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component>, boost::noncopyable>("StaticStateComponent", no_init)
		.def("synchronize", &i6engine::api::StaticStateComponent::synchronize)
		.def("getTemplateName", &i6engine::api::StaticStateComponent::getTemplateName)
		.def("setPosition", &i6engine::api::StaticStateComponent::setPosition)
		.def("setRotation", &i6engine::api::StaticStateComponent::setRotation)
		.def("setScale", &i6engine::api::StaticStateComponent::setScale)
		.def("getPosition", &i6engine::api::StaticStateComponent::getPosition)
		.def("getRotation", &i6engine::api::StaticStateComponent::getRotation)
		.def("getScale", &i6engine::api::StaticStateComponent::getScale);

	class_<i6engine::api::TerrainAppearanceComponent, i6engine::utils::sharedPtr<i6engine::api::TerrainAppearanceComponent, i6engine::api::Component>, boost::noncopyable>("TerrainAppearanceComponent", no_init)
		.def("synchronize", &i6engine::api::TerrainAppearanceComponent::synchronize)
		.def("getTemplateName", &i6engine::api::TerrainAppearanceComponent::getTemplateName)
		.def("getHeightmap", &i6engine::api::TerrainAppearanceComponent::getHeightmap)
		.def("getSize", &i6engine::api::TerrainAppearanceComponent::getSize);

	class_<i6engine::api::ToggleWaynetComponent, i6engine::utils::sharedPtr<i6engine::api::ToggleWaynetComponent, i6engine::api::Component>, boost::noncopyable>("ToggleWaynetComponent", no_init)
		.def("synchronize", &i6engine::api::ToggleWaynetComponent::synchronize)
		.def("getTemplateName", &i6engine::api::ToggleWaynetComponent::getTemplateName)
		.def("enable", &i6engine::api::ToggleWaynetComponent::enable);

	class_<i6engine::api::VelocityComponent, i6engine::utils::sharedPtr<i6engine::api::VelocityComponent, i6engine::api::Component>, boost::noncopyable>("VelocityComponent", no_init)
		.def("synchronize", &i6engine::api::VelocityComponent::synchronize)
		.def("getTemplateName", &i6engine::api::VelocityComponent::getTemplateName)
		.def("accelerate", (void(*)(i6engine::api::VelocityComponent *, const Vec3 &, i6engine::api::VelocityComponent::MaxSpeedHandling, const std::string &)) &i6engine::python::object::accelerate)
		.def("accelerate", (void(*)(i6engine::api::VelocityComponent *, const std::string &)) &i6engine::python::object::accelerate)
		.def("decelerate", (void(*)(i6engine::api::VelocityComponent *, const Vec3 &, const std::string &)) &i6engine::python::object::decelerate)
		.def("decelerate", (void(*)(i6engine::api::VelocityComponent *, const std::string &)) &i6engine::python::object::decelerate)
		.def("setMaxSpeed", &i6engine::api::VelocityComponent::setMaxSpeed)
		.def("setResistanceCoefficient", &i6engine::api::VelocityComponent::setResistanceCoefficient)
		.def("setWindage", &i6engine::api::VelocityComponent::setWindage);

	enum_<i6engine::api::VelocityComponent::MaxSpeedHandling>("MaxSpeedHandling")
		.value("KeepSpeed", i6engine::api::VelocityComponent::MaxSpeedHandling::KeepSpeed)
		.value("StopAcceleration", i6engine::api::VelocityComponent::MaxSpeedHandling::StopAcceleration)
		.export_values();

	class_<i6engine::api::WaynetNavigationComponent, i6engine::utils::sharedPtr<i6engine::api::WaynetNavigationComponent, i6engine::api::Component>, boost::noncopyable>("WaynetNavigationComponent", no_init)
		.def("synchronize", &i6engine::api::WaynetNavigationComponent::synchronize)
		.def("getTemplateName", &i6engine::api::WaynetNavigationComponent::getTemplateName)
		.def("getPathPos", (std::vector<Vec3>(i6engine::api::WaynetNavigationComponent::*)(const Vec3 &, const Vec3 &) const) &i6engine::api::WaynetNavigationComponent::getPath)
		.def("getPathWP", (std::vector<Vec3>(i6engine::api::WaynetNavigationComponent::*)(const Vec3 &, const std::string &) const) &i6engine::api::WaynetNavigationComponent::getPath);

	class_<i6engine::api::WaypointComponent, i6engine::utils::sharedPtr<i6engine::api::WaypointComponent, i6engine::api::Component>, boost::noncopyable>("WaypointComponent", no_init)
		.def("synchronize", &i6engine::api::WaypointComponent::synchronize)
		.def("getTemplateName", &i6engine::api::WaypointComponent::getTemplateName)
		.def("getName", &i6engine::api::WaypointComponent::getName)
		.def("getConnections", &i6engine::api::WaypointComponent::getConnections)
		.def("isConnected", &i6engine::api::WaypointComponent::isConnected)
		.def("addConnection", &i6engine::api::WaypointComponent::addConnection)
		.def("removeConnection", &i6engine::api::WaypointComponent::removeConnection);

	def("getObject", &i6engine::python::object::getObject);
	def("getAllObjectsOfType", &i6engine::python::object::getAllObjectsOfType);
	def("getGOList", &i6engine::python::object::getGOList);
	def("deleteAllObjectsOfType", &i6engine::python::object::deleteAllObjectsOfType);
	def("createObject", &i6engine::python::object::createObject);
	def("cleanUpAll", &i6engine::python::object::cleanUpAll);
	def("loadLevel", &i6engine::python::object::loadLevel);
	def("getFrameTime", &i6engine::python::object::getFrameTime);
	def("createGO", &i6engine::python::object::createGO);
	def("createComponent", &i6engine::python::object::createComponent);
	def("resetObjectSubSystem", &i6engine::python::object::resetObjectSubSystem);
	def("pauseObject", &i6engine::python::object::pauseObject);
	def("unpauseObject", &i6engine::python::object::unpauseObject);
	def("rayTest", &i6engine::python::object::rayTest);

	class_<i6engine::api::objects::GOTemplateComponent>("GOTemplateComponent")
		.def(init<const std::string &, const i6engine::api::attributeMap &, const std::string &, bool, bool>())
		.def_readwrite("template", &i6engine::api::objects::GOTemplateComponent::_template)
		.def_readwrite("id", &i6engine::api::objects::GOTemplateComponent::_id)
		.def_readwrite("params", &i6engine::api::objects::GOTemplateComponent::_params)
		.def_readwrite("owner", &i6engine::api::objects::GOTemplateComponent::_owner)
		.def_readwrite("identifier", &i6engine::api::objects::GOTemplateComponent::_identifier)
		.def_readwrite("deleted", &i6engine::api::objects::GOTemplateComponent::_deleted);

	class_<i6engine::api::objects::GOTemplate>("GOTemplate")
		.def(init<>())
		.def_readwrite("type", &i6engine::api::objects::GOTemplate::_type)
		.def_readwrite("components", &i6engine::api::objects::GOTemplate::_components);

	class_<i6engine::api::attributeMap>("attributeMap")
		.def(init<>())
		.def("insert", &i6engine::python::object::insertPairInAttributeMap);

	class_<std::vector<i6engine::api::objects::GOTemplateComponent>>("GOTemplateComponentVector")
		.def(init<>())
		.def("push_back", (void(std::vector<i6engine::api::objects::GOTemplateComponent>::*)(const i6engine::api::objects::GOTemplateComponent &)) &std::vector<i6engine::api::objects::GOTemplateComponent>::push_back);

	class_<i6engine::api::CollisionGroup>("CollisionGroup")
		.def(init<>())
		.def(init<uint32_t, uint32_t, uint32_t>())
		.def(init<const std::string &>())
		.def_readwrite("responseType", &i6engine::api::CollisionGroup::responseType)
		.def_readwrite("crashType", &i6engine::api::CollisionGroup::crashType)
		.def_readwrite("crashMask", &i6engine::api::CollisionGroup::crashMask);

	enum_<i6engine::api::components::ComponentTypes>("ComponentTypes")
		.value("CameraComponent", i6engine::api::components::ComponentTypes::CameraComponent)
		.value("LifetimeComponent", i6engine::api::components::ComponentTypes::LifetimeComponent)
		.value("LuminousAppearanceComponent", i6engine::api::components::ComponentTypes::LuminousAppearanceComponent)
		.value("MeshAppearanceComponent", i6engine::api::components::ComponentTypes::MeshAppearanceComponent)
		.value("MoverCircleComponent", i6engine::api::components::ComponentTypes::MoverCircleComponent)
		.value("MoverComponent", i6engine::api::components::ComponentTypes::MoverComponent)
		.value("MoverInterpolateComponent", i6engine::api::components::ComponentTypes::MoverInterpolateComponent)
		.value("MovingCameraComponent", i6engine::api::components::ComponentTypes::MovingCameraComponent)
		.value("NetworkSenderComponent", i6engine::api::components::ComponentTypes::NetworkSenderComponent)
		.value("ParticleEmitterComponent", i6engine::api::components::ComponentTypes::ParticleEmitterComponent)
		.value("PhysicalStateComponent", i6engine::api::components::ComponentTypes::PhysicalStateComponent)
		.value("ShatterComponent", i6engine::api::components::ComponentTypes::ShatterComponent)
		.value("SpawnpointComponent", i6engine::api::components::ComponentTypes::SpawnpointComponent)
		.value("StaticStateComponent", i6engine::api::components::ComponentTypes::StaticStateComponent)
		.value("TerrainAppearanceComponent", i6engine::api::components::ComponentTypes::TerrainAppearanceComponent)
		.value("SoundComponent", i6engine::api::components::ComponentTypes::SoundComponent)
		.value("SoundListenerComponent", i6engine::api::components::ComponentTypes::SoundListenerComponent)
		.value("BillboardComponent", i6engine::api::components::ComponentTypes::BillboardComponent)
		.value("FollowComponent", i6engine::api::components::ComponentTypes::FollowComponent)
		.value("MovableTextComponent", i6engine::api::components::ComponentTypes::MovableTextComponent)
		.value("WaypointComponent", i6engine::api::components::ComponentTypes::WaypointComponent)
		.value("NavigationComponent", i6engine::api::components::ComponentTypes::NavigationComponent)
		.value("WaynetNavigationComponent", i6engine::api::components::ComponentTypes::WaynetNavigationComponent)
		.value("MoveComponent", i6engine::api::components::ComponentTypes::MoveComponent)
		.value("MovementComponent", i6engine::api::components::ComponentTypes::MovementComponent)
		.value("ToggleWaynetComponent", i6engine::api::components::ComponentTypes::ToggleWaynetComponent)
		.value("Point2PointConstraintComponent", i6engine::api::components::ComponentTypes::Point2PointConstraintComponent)
		.value("VelocityComponent", i6engine::api::components::ComponentTypes::VelocityComponent)
		.value("ComponentTypesCount", i6engine::api::components::ComponentTypes::ComponentTypesCount)
		.export_values();
}
