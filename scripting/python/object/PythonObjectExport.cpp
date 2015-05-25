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
#include "i6engine/api/components/BillboardComponent.h"
#include "i6engine/api/components/FollowComponent.h"
#include "i6engine/api/components/LifetimeComponent.h"
#include "i6engine/api/components/LuminousAppearanceComponent.h"
#include "i6engine/api/components/MeshAppearanceComponent.h"
#include "i6engine/api/components/MovableTextComponent.h"
#include "i6engine/api/components/MoveComponent.h"
#include "i6engine/api/components/MovementComponent.h"
#include "i6engine/api/components/MoverCircleComponent.h"
#include "i6engine/api/components/MoverInterpolateComponent.h"
#include "i6engine/api/components/MovingCameraComponent.h"
#include "i6engine/api/components/NetworkSenderComponent.h"
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
		std::list<i6engine::api::GOPtr> v = i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->getAllObjectsOfType(types);
		for (std::list<i6engine::api::GOPtr>::const_iterator it = v.begin(); it != v.end(); ++it) {
			l.append(*it);
		}
		return l;
	}

	i6engine::api::GOPtr getObject(const int64_t id) {
		return i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(id);
	}

	boost::python::list getGOList() {
		boost::python::list l;
		std::list<i6engine::api::GOPtr> v = i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->getGOList();
		for (std::list<i6engine::api::GOPtr>::const_iterator it = v.begin(); it != v.end(); ++it) {
			l.append(*it);
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
			if (boost::python::override Tick = this->get_override("Tick")) {
				boost::python::call<void>(Tick.ptr());
			}
			Component::Tick();
		}

		void default_Tick() {
			this->Component::Tick();
		}

		virtual void News(const i6engine::api::GameMessage::Ptr & msg) override {
			if (boost::python::override News = this->get_override("News")) {
				boost::python::call<void>(News.ptr(), msg);
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
			if (boost::python::override Finalize = this->get_override("Finalize")) {
				boost::python::call<void>(Finalize.ptr());
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
			if (boost::python::override howToAdd = this->get_override("howToAdd")) {
				return boost::python::call<std::pair<i6engine::api::AddStrategy, int64_t>>(howToAdd.ptr(), comp);
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

	i6engine::utils::sharedPtr<i6engine::api::CameraComponent, i6engine::api::Component> getCameraComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::CameraComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::CameraComponent, i6engine::api::Component> getCameraComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::CameraComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::LifetimeComponent, i6engine::api::Component> getLifetimeComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::LifetimeComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::LifetimeComponent, i6engine::api::Component> getLifetimeComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::LifetimeComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::LuminousAppearanceComponent, i6engine::api::Component> getLuminousAppearanceComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::LuminousAppearanceComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::LuminousAppearanceComponent, i6engine::api::Component> getLuminousAppearanceComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::LuminousAppearanceComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::MeshAppearanceComponent, i6engine::api::Component> getMeshAppearanceComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::MeshAppearanceComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::MeshAppearanceComponent, i6engine::api::Component> getMeshAppearanceComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::MeshAppearanceComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::MoverCircleComponent, i6engine::api::Component> getMoverCircleComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::MoverCircleComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::MoverCircleComponent, i6engine::api::Component> getMoverCircleComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::MoverCircleComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::MoverComponent, i6engine::api::Component> getMoverComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::MoverComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::MoverComponent, i6engine::api::Component> getMoverComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::MoverComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::MoverInterpolateComponent, i6engine::api::Component> getMoverInterpolateComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::MoverInterpolateComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::MoverInterpolateComponent, i6engine::api::Component> getMoverInterpolateComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::MoverInterpolateComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::MovingCameraComponent, i6engine::api::Component> getMovingCameraComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::MovingCameraComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::MovingCameraComponent, i6engine::api::Component> getMovingCameraComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::MovingCameraComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::NetworkSenderComponent, i6engine::api::Component> getNetworkSenderComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::NetworkSenderComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::NetworkSenderComponent, i6engine::api::Component> getNetworkSenderComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::NetworkSenderComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::ParticleEmitterComponent, i6engine::api::Component> getParticleEmitterComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::ParticleEmitterComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::ParticleEmitterComponent, i6engine::api::Component> getParticleEmitterComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::ParticleEmitterComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::PhysicalStateComponent, i6engine::api::Component> getPhysicalStateComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::PhysicalStateComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::PhysicalStateComponent, i6engine::api::Component> getPhysicalStateComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::PhysicalStateComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::ShatterComponent, i6engine::api::Component> getShatterComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::ShatterComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::ShatterComponent, i6engine::api::Component> getShatterComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::ShatterComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::SpawnpointComponent, i6engine::api::Component> getSpawnpointComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::SpawnpointComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::SpawnpointComponent, i6engine::api::Component> getSpawnpointComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::SpawnpointComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> getStaticStateComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::StaticStateComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> getStaticStateComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::StaticStateComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::TerrainAppearanceComponent, i6engine::api::Component> getTerrainAppearanceComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::TerrainAppearanceComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::TerrainAppearanceComponent, i6engine::api::Component> getTerrainAppearanceComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::TerrainAppearanceComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::SoundComponent, i6engine::api::Component> getSoundComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::SoundComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::SoundComponent, i6engine::api::Component> getSoundComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::SoundComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::SoundListenerComponent, i6engine::api::Component> getSoundListenerComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::SoundListenerComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::SoundListenerComponent, i6engine::api::Component> getSoundListenerComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::SoundListenerComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::BillboardComponent, i6engine::api::Component> getBillboardComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::BillboardComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::BillboardComponent, i6engine::api::Component> getBillboardComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::BillboardComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::FollowComponent, i6engine::api::Component> getFollowComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::FollowComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::FollowComponent, i6engine::api::Component> getFollowComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::FollowComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::MovableTextComponent, i6engine::api::Component> getMovableTextComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::MovableTextComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::MovableTextComponent, i6engine::api::Component> getMovableTextComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::MovableTextComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::WaypointComponent, i6engine::api::Component> getWaypointComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::WaypointComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::WaypointComponent, i6engine::api::Component> getWaypointComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::WaypointComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::NavigationComponent, i6engine::api::Component> getNavigationComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::NavigationComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::NavigationComponent, i6engine::api::Component> getNavigationComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::NavigationComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::WaynetNavigationComponent, i6engine::api::Component> getWaynetNavigationComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::WaynetNavigationComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::WaynetNavigationComponent, i6engine::api::Component> getWaynetNavigationComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::WaynetNavigationComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::MoveComponent, i6engine::api::Component> getMoveComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::MoveComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::MoveComponent, i6engine::api::Component> getMoveComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::MoveComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::MovementComponent, i6engine::api::Component> getMovementComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::MovementComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::MovementComponent, i6engine::api::Component> getMovementComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::MovementComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::ToggleWaynetComponent, i6engine::api::Component> getToggleWaynetComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::ToggleWaynetComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::ToggleWaynetComponent, i6engine::api::Component> getToggleWaynetComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::ToggleWaynetComponent>(type, identifier);
	}

	i6engine::utils::sharedPtr<i6engine::api::Point2PointConstraintComponent, i6engine::api::Component> getPoint2PointConstraintComponent(i6engine::api::GameObject * go, uint32_t type) {
		return go->getGOC<i6engine::api::Point2PointConstraintComponent>(type);
	}

	i6engine::utils::sharedPtr<i6engine::api::Point2PointConstraintComponent, i6engine::api::Component> getPoint2PointConstraintComponent(i6engine::api::GameObject * go, uint32_t type, const std::string & identifier) {
		return go->getGOC<i6engine::api::Point2PointConstraintComponent>(type, identifier);
	}

	struct CameraComponentWrapper : public i6engine::api::CameraComponent, public boost::python::wrapper<i6engine::api::CameraComponent> {
		CameraComponentWrapper() : CameraComponent(-1, i6engine::api::attributeMap()), boost::python::wrapper<i6engine::api::CameraComponent>() {
		}

		CameraComponentWrapper(const int64_t id, const attributeMap & params) : CameraComponent(id, params), boost::python::wrapper<i6engine::api::CameraComponent>() {
		}

		CameraComponentWrapper(const i6engine::api::CameraComponent & arg) : i6engine::api::CameraComponent(-1, i6engine::api::attributeMap()), boost::python::wrapper<i6engine::api::CameraComponent>() {
		}

		virtual void Tick() override {
			if (boost::python::override Tick = this->get_override("Tick")) {
				boost::python::call<void>(Tick.ptr());
			}
			Component::Tick();
		}

		void default_Tick() {
			this->Component::Tick();
		}

		virtual void News(const i6engine::api::GameMessage::Ptr & msg) override {
			if (boost::python::override News = this->get_override("News")) {
				boost::python::call<void>(News.ptr(), msg);
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
			if (boost::python::override Finalize = this->get_override("Finalize")) {
				boost::python::call<void>(Finalize.ptr());
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
			if (boost::python::override Tick = this->get_override("Tick")) {
				boost::python::call<void>(Tick.ptr());
			}
		}

		void default_Tick() {
			this->MovementComponent::Tick();
		}

		virtual void News(const i6engine::api::GameMessage::Ptr & msg) override {
			if (boost::python::override News = this->get_override("News")) {
				return boost::python::call<void>(News.ptr(), msg);
			}
			Component::News(msg);
		}

		void default_News(const i6engine::api::GameMessage::Ptr & msg) {
			this->Component::News(msg);
		}

		virtual void Init() override {
			MovementComponent::Init();
			if (boost::python::override Init = this->get_override("Init")) {
				boost::python::call<void>(Init.ptr());
			}
		}

		void default_Init() {
			this->MovementComponent::Init();
		}

		virtual void Finalize() override {
			MovementComponent::Finalize();
			if (boost::python::override Finalize = this->get_override("Finalize")) {
				return boost::python::call<void>(Finalize.ptr());
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
		.def("getCameraComponent", (i6engine::utils::sharedPtr<i6engine::api::CameraComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getCameraComponent)
		.def("getCameraComponent", (i6engine::utils::sharedPtr<i6engine::api::CameraComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getCameraComponent)
		.def("getLifetimeComponent", (i6engine::utils::sharedPtr<i6engine::api::LifetimeComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getLifetimeComponent)
		.def("getLifetimeComponent", (i6engine::utils::sharedPtr<i6engine::api::LifetimeComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getLifetimeComponent)
		.def("getLuminousAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::LuminousAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getLuminousAppearanceComponent)
		.def("getLuminousAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::LuminousAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getLuminousAppearanceComponent)
		.def("getMeshAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::MeshAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getMeshAppearanceComponent)
		.def("getMeshAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::MeshAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getMeshAppearanceComponent)
		.def("getMoverCircleComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverCircleComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getMoverCircleComponent)
		.def("getMoverCircleComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverCircleComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getMoverCircleComponent)
		.def("getMoverComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getMoverComponent)
		.def("getMoverComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getMoverComponent)
		.def("getMoverInterpolateComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverInterpolateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getMoverInterpolateComponent)
		.def("getMoverInterpolateComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverInterpolateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getMoverInterpolateComponent)
		.def("getMovingCameraComponent", (i6engine::utils::sharedPtr<i6engine::api::MovingCameraComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getMovingCameraComponent)
		.def("getMovingCameraComponent", (i6engine::utils::sharedPtr<i6engine::api::MovingCameraComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getMovingCameraComponent)
		.def("getNetworkSenderComponent", (i6engine::utils::sharedPtr<i6engine::api::NetworkSenderComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getNetworkSenderComponent)
		.def("getNetworkSenderComponent", (i6engine::utils::sharedPtr<i6engine::api::NetworkSenderComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getNetworkSenderComponent)
		.def("getParticleEmitterComponent", (i6engine::utils::sharedPtr<i6engine::api::ParticleEmitterComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getParticleEmitterComponent)
		.def("getParticleEmitterComponent", (i6engine::utils::sharedPtr<i6engine::api::ParticleEmitterComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getParticleEmitterComponent)
		.def("getPhysicalStateComponent", (i6engine::utils::sharedPtr<i6engine::api::PhysicalStateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getPhysicalStateComponent)
		.def("getPhysicalStateComponent", (i6engine::utils::sharedPtr<i6engine::api::PhysicalStateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getPhysicalStateComponent)
		.def("getShatterComponent", (i6engine::utils::sharedPtr<i6engine::api::ShatterComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getShatterComponent)
		.def("getShatterComponent", (i6engine::utils::sharedPtr<i6engine::api::ShatterComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getShatterComponent)
		.def("getSpawnpointComponent", (i6engine::utils::sharedPtr<i6engine::api::SpawnpointComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getSpawnpointComponent)
		.def("getSpawnpointComponent", (i6engine::utils::sharedPtr<i6engine::api::SpawnpointComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getSpawnpointComponent)
		.def("getStaticStateComponent", (i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getStaticStateComponent)
		.def("getStaticStateComponent", (i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getStaticStateComponent)
		.def("getTerrainAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::TerrainAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getTerrainAppearanceComponent)
		.def("getTerrainAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::TerrainAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getTerrainAppearanceComponent)
		.def("getSoundComponent", (i6engine::utils::sharedPtr<i6engine::api::SoundComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getSoundComponent)
		.def("getSoundComponent", (i6engine::utils::sharedPtr<i6engine::api::SoundComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getSoundComponent)
		.def("getSoundListenerComponent", (i6engine::utils::sharedPtr<i6engine::api::SoundListenerComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getSoundListenerComponent)
		.def("getSoundListenerComponent", (i6engine::utils::sharedPtr<i6engine::api::SoundListenerComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getSoundListenerComponent)
		.def("getBillboardComponent", (i6engine::utils::sharedPtr<i6engine::api::BillboardComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getBillboardComponent)
		.def("getBillboardComponent", (i6engine::utils::sharedPtr<i6engine::api::BillboardComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getBillboardComponent)
		.def("getFollowComponent", (i6engine::utils::sharedPtr<i6engine::api::FollowComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getFollowComponent)
		.def("getFollowComponent", (i6engine::utils::sharedPtr<i6engine::api::FollowComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getFollowComponent)
		.def("getMovableTextComponent", (i6engine::utils::sharedPtr<i6engine::api::MovableTextComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getMovableTextComponent)
		.def("getMovableTextComponent", (i6engine::utils::sharedPtr<i6engine::api::MovableTextComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getMovableTextComponent)
		.def("getWaypointComponent", (i6engine::utils::sharedPtr<i6engine::api::WaypointComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getWaypointComponent)
		.def("getWaypointComponent", (i6engine::utils::sharedPtr<i6engine::api::WaypointComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getWaypointComponent)
		.def("getNavigationComponent", (i6engine::utils::sharedPtr<i6engine::api::NavigationComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getNavigationComponent)
		.def("getNavigationComponent", (i6engine::utils::sharedPtr<i6engine::api::NavigationComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getNavigationComponent)
		.def("getWaynetNavigationComponent", (i6engine::utils::sharedPtr<i6engine::api::WaynetNavigationComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getWaynetNavigationComponent)
		.def("getWaynetNavigationComponent", (i6engine::utils::sharedPtr<i6engine::api::WaynetNavigationComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getWaynetNavigationComponent)
		.def("getMoveComponent", (i6engine::utils::sharedPtr<i6engine::api::MoveComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getMoveComponent)
		.def("getMoveComponent", (i6engine::utils::sharedPtr<i6engine::api::MoveComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getMoveComponent)
		.def("getMovementComponent", (i6engine::utils::sharedPtr<i6engine::api::MovementComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getMovementComponent)
		.def("getMovementComponent", (i6engine::utils::sharedPtr<i6engine::api::MovementComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getMovementComponent)
		.def("getToggleWaynetComponent", (i6engine::utils::sharedPtr<i6engine::api::ToggleWaynetComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getToggleWaynetComponent)
		.def("getToggleWaynetComponent", (i6engine::utils::sharedPtr<i6engine::api::ToggleWaynetComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getToggleWaynetComponent)
		.def("getPoint2PointConstraintComponent", (i6engine::utils::sharedPtr<i6engine::api::Point2PointConstraintComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::python::object::getPoint2PointConstraintComponent)
		.def("getPoint2PointConstraintComponent", (i6engine::utils::sharedPtr<i6engine::api::Point2PointConstraintComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::python::object::getPoint2PointConstraintComponent);
	
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
		.def("stopAnimation", &i6engine::api::MeshAppearanceComponent::stopAnimation);

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

	class_<i6engine::api::PhysicalStateComponent, i6engine::utils::sharedPtr<i6engine::api::PhysicalStateComponent, i6engine::api::Component>, boost::noncopyable>("PhysicalStateComponent", no_init)
		.def("getPosition", &i6engine::api::PhysicalStateComponent::getPosition)
		.def("setPosition", &i6engine::api::PhysicalStateComponent::setPosition)
		.def("getRotation", &i6engine::api::PhysicalStateComponent::getRotation)
		.def("setRotation", &i6engine::api::PhysicalStateComponent::setRotation)
		.def("applyRotation", &i6engine::api::PhysicalStateComponent::applyRotation)
		.def("applyCentralForce", &i6engine::api::PhysicalStateComponent::applyCentralForce)
		.def("applyForce", &i6engine::api::PhysicalStateComponent::applyForce)
		.def("getLinearVelocity", &i6engine::api::PhysicalStateComponent::getLinearVelocity)
		.def("reset", &i6engine::api::PhysicalStateComponent::reset);

	class_<i6engine::api::RayTestResult>("RayTestResult")
		.def_readonly("objID", &i6engine::api::RayTestResult::objID)
		.def_readonly("sourceID", &i6engine::api::RayTestResult::sourceID);

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
		.def(init<uint32_t, uint32_t, uint32_t>());

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
		.value("ComponentTypesCount", i6engine::api::components::ComponentTypes::ComponentTypesCount)
		.export_values();
}
