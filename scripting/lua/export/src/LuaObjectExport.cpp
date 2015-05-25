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

#include "LuaObjectExport.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/math/i6eMath.h"

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

#include "i6engine/luabind/operator.hpp"

namespace i6engine {
namespace lua {
namespace object {

	std::list<i6engine::api::GOPtr> getAllObjectsOfType(const std::string & types) {
		return i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->getAllObjectsOfType(types);
	}

	i6engine::api::GOPtr getObject(const int64_t id) {
		return i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(id);
	}

	std::list<i6engine::api::GOPtr> getGOList() {
		return i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->getGOList();
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

	void registerCTemplate(const std::string & GOCType, const std::string & func) {
		i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->registerCTemplate(GOCType, [func](int64_t id, const i6engine::api::attributeMap & params) {
			return utils::sharedPtr<i6engine::api::Component, i6engine::api::Component>(i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<i6engine::api::Component *>(func, id, params)->get());
		});
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

	struct ComponentWrapper : public i6engine::api::Component, public luabind::wrap_base {
		ComponentWrapper(const int64_t id, const attributeMap & params) : Component(id, params), luabind::wrap_base() {
		}

		virtual void Tick() override {
			luabind::call_member<void>(this, "Tick");
		}

		static void default_Tick(i6engine::api::Component * ptr) {
			ptr->Component::Tick();
		}

		virtual void News(const i6engine::api::GameMessage::Ptr & msg) override {
			luabind::call_member<void>(this, "News", msg);
		}

		static void default_News(i6engine::api::Component * ptr, const i6engine::api::GameMessage::Ptr & msg) {
			ptr->Component::News(msg);
		}

		virtual void Init() override {
			luabind::call_member<void>(this, "Init");
		}

		virtual void Finalize() override {
			luabind::call_member<void>(this, "Finalize");
		}

		static void default_Finalize(i6engine::api::Component * ptr) {
			ptr->Component::Finalize();
		}

		virtual i6engine::api::attributeMap synchronize() const {
			return luabind::call_member<i6engine::api::attributeMap>(this, "synchronize");
		}

		virtual std::pair<i6engine::api::AddStrategy, int64_t> howToAdd(const i6engine::api::ComPtr & comp) const override {
			return luabind::call_member<std::pair<i6engine::api::AddStrategy, int64_t>>(this, "howToAdd", comp);
		}

		static std::pair<i6engine::api::AddStrategy, int64_t> default_howToAdd(i6engine::api::Component * ptr, const i6engine::api::ComPtr & comp) {
			return ptr->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return luabind::call_member<std::string>(this, "getTemplateName");
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

	struct CameraComponentWrapper : public i6engine::api::CameraComponent, public luabind::wrap_base {
		CameraComponentWrapper(const int64_t id, const attributeMap & params) : CameraComponent(id, params), luabind::wrap_base() {
		}

		virtual void Tick() override {
			luabind::call_member<void>(this, "Tick");
		}

		static void default_Tick(i6engine::api::CameraComponent * ptr) {
			ptr->Component::Tick();
		}

		virtual void News(const i6engine::api::GameMessage::Ptr & msg) override {
			luabind::call_member<void>(this, "News", msg);
		}

		static void default_News(i6engine::api::CameraComponent * ptr, const i6engine::api::GameMessage::Ptr & msg) {
			ptr->CameraComponent::News(msg);
		}

		virtual void Init() override {
			CameraComponent::Init();
			luabind::call_member<void>(this, "Init");
		}

		virtual void Finalize() override {
			CameraComponent::Finalize();
			luabind::call_member<void>(this, "Finalize");
		}

		static void default_Finalize(i6engine::api::CameraComponent * ptr) {
			ptr->CameraComponent::Finalize();
		}

		virtual i6engine::api::attributeMap synchronize() const {
			i6engine::api::attributeMap params = CameraComponent::synchronize();
			i6engine::api::attributeMap params2 = luabind::call_member<i6engine::api::attributeMap>(this, "synchronize");
			params.insert(params2.begin(), params2.end());
			return params;
		}

		virtual std::pair<i6engine::api::AddStrategy, int64_t> howToAdd(const i6engine::api::ComPtr & comp) const override {
			return luabind::call_member<std::pair<i6engine::api::AddStrategy, int64_t>>(this, "howToAdd", comp);
		}

		static std::pair<i6engine::api::AddStrategy, int64_t> default_howToAdd(i6engine::api::CameraComponent * ptr, const i6engine::api::ComPtr & comp) {
			return ptr->CameraComponent::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return luabind::call_member<std::string>(this, "getTemplateName");
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

	int getLightType(i6engine::api::LuminousAppearanceComponent * c) {
		return int(c->getLightType());
	}

	struct MovementComponentWrapper : public i6engine::api::MovementComponent, public luabind::wrap_base {
		MovementComponentWrapper(const int64_t id, const attributeMap & params) : MovementComponent(id, params), luabind::wrap_base() {
		}

		virtual void Tick() override {
			MovementComponent::Tick();
			luabind::call_member<void>(this, "Tick");
		}

		static void default_Tick(i6engine::api::MovementComponent * ptr) {
			ptr->MovementComponent::Tick();
		}

		virtual void News(const i6engine::api::GameMessage::Ptr & msg) override {
			luabind::call_member<void>(this, "News", msg);
		}

		static void default_News(i6engine::api::MovementComponent * ptr, const i6engine::api::GameMessage::Ptr & msg) {
			ptr->Component::News(msg);
		}

		virtual void Init() override {
			MovementComponent::Init();
			luabind::call_member<void>(this, "Init");
		}

		static void default_Init(i6engine::api::MovementComponent * ptr) {
			ptr->MovementComponent::Init();
		}

		virtual void Finalize() override {
			MovementComponent::Finalize();
			luabind::call_member<void>(this, "Finalize");
		}

		static void default_Finalize(i6engine::api::MovementComponent * ptr) {
			ptr->MovementComponent::Finalize();
		}

		virtual i6engine::api::attributeMap synchronize() const {
			return luabind::call_member<i6engine::api::attributeMap>(this, "synchronize");
		}

		virtual std::pair<i6engine::api::AddStrategy, int64_t> howToAdd(const i6engine::api::ComPtr & comp) const override {
			return luabind::call_member<std::pair<i6engine::api::AddStrategy, int64_t>>(this, "howToAdd", comp);
		}

		static std::pair<i6engine::api::AddStrategy, int64_t> default_howToAdd(i6engine::api::MovementComponent * ptr, const i6engine::api::ComPtr & comp) {
			return ptr->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return luabind::call_member<std::string>(this, "getTemplateName");
		}

		std::vector<i6engine::api::componentOptions> getComponentOptions() {
			return {};
		}

		virtual void forward() override {
			luabind::call_member<void>(this, "forward");
		}

		virtual void backward() override {
			luabind::call_member<void>(this, "backward");
		}

		virtual void left() override {
			luabind::call_member<void>(this, "left");
		}

		virtual void right() override {
			luabind::call_member<void>(this, "right");
		}
	};

	struct MoverComponentWrapper : public i6engine::api::MoverComponent, public luabind::wrap_base {
		MoverComponentWrapper(const int64_t id, const attributeMap & params) : MoverComponent(id, params), luabind::wrap_base() {
		}

		virtual void Tick() override {
			MoverComponent::Tick();
			luabind::call_member<void>(this, "Tick");
		}

		static void default_Tick(i6engine::api::MoverComponent * ptr) {
			ptr->MoverComponent::Tick();
		}

		virtual void News(const i6engine::api::GameMessage::Ptr & msg) override {
			luabind::call_member<void>(this, "News", msg);
		}

		virtual void Init() override {
			MoverComponent::Init();
			luabind::call_member<void>(this, "Init");
		}

		static void default_Init(i6engine::api::MoverComponent * ptr) {
			ptr->MoverComponent::Init();
		}

		virtual void Finalize() override {
			MoverComponent::Finalize();
			luabind::call_member<void>(this, "Finalize");
		}

		static void default_Finalize(i6engine::api::MoverComponent * ptr) {
			ptr->MoverComponent::Finalize();
		}

		virtual i6engine::api::attributeMap synchronize() const {
			i6engine::api::attributeMap params = MoverComponent::synchronize();
			i6engine::api::attributeMap params2 = luabind::call_member<i6engine::api::attributeMap>(this, "synchronize");
			params.insert(params2.begin(), params2.end());
			return params;
		}

		static i6engine::api::attributeMap default_synchronize(i6engine::api::MoverComponent * ptr) {
			return ptr->MoverComponent::synchronize();
		}

		virtual std::pair<i6engine::api::AddStrategy, int64_t> howToAdd(const i6engine::api::ComPtr & comp) const override {
			return luabind::call_member<std::pair<i6engine::api::AddStrategy, int64_t>>(this, "howToAdd", comp);
		}

		static std::pair<i6engine::api::AddStrategy, int64_t> default_howToAdd(i6engine::api::MoverComponent * ptr, const i6engine::api::ComPtr & comp) {
			return ptr->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return luabind::call_member<std::string>(this, "getTemplateName");
		}

		std::vector<i6engine::api::componentOptions> getComponentOptions() {
			return {};
		}

		void start(Vec3 & startPos) override {
			luabind::call_member<void>(this, "start");
		}

		virtual void reset() override {
			luabind::call_member<void>(this, "reset");
		}

		void getNewPosition(const uint64_t time, Vec3 & pos, Quaternion & rot) override {
			luabind::call_member<void>(this, "getNewPosition", time, pos, rot);
		}
	};

	int getWay(i6engine::api::MoverInterpolateComponent * c) {
		return int(c->getWay());
	}

	struct NavigationComponentWrapper : public i6engine::api::NavigationComponent, public luabind::wrap_base {
		NavigationComponentWrapper(const int64_t id, const attributeMap & params) : NavigationComponent(id, params), luabind::wrap_base() {
		}

		virtual void Tick() override {
			luabind::call_member<void>(this, "Tick");
		}

		static void default_Tick(i6engine::api::NavigationComponent * ptr) {
			ptr->Component::Tick();
		}

		virtual void News(const i6engine::api::GameMessage::Ptr & msg) override {
			luabind::call_member<void>(this, "News", msg);
		}

		static void default_News(i6engine::api::NavigationComponent * ptr, const i6engine::api::GameMessage::Ptr & msg) {
			ptr->Component::News(msg);
		}

		virtual void Init() override {
			luabind::call_member<void>(this, "Init");
		}

		virtual void Finalize() override {
			luabind::call_member<void>(this, "Finalize");
		}

		static void default_Finalize(i6engine::api::NavigationComponent * ptr) {
			ptr->NavigationComponent::Finalize();
		}

		virtual i6engine::api::attributeMap synchronize() const {
			return luabind::call_member<i6engine::api::attributeMap>(this, "synchronize");
		}

		virtual std::pair<i6engine::api::AddStrategy, int64_t> howToAdd(const i6engine::api::ComPtr & comp) const override {
			return luabind::call_member<std::pair<i6engine::api::AddStrategy, int64_t>>(this, "howToAdd", comp);
		}

		static std::pair<i6engine::api::AddStrategy, int64_t> default_howToAdd(i6engine::api::NavigationComponent * ptr, const i6engine::api::ComPtr & comp) {
			return ptr->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return luabind::call_member<std::string>(this, "getTemplateName");
		}

		std::vector<i6engine::api::componentOptions> getComponentOptions() {
			return {};
		}

		std::vector<Vec3> getPath(const Vec3 & from, const Vec3 & to) const override {
			return luabind::call_member<std::vector<Vec3>>(this, "getPathPos", from, to);
		}

		std::vector<Vec3> getPath(const Vec3 & from, const std::string & to) const override {
			return luabind::call_member<std::vector<Vec3>>(this, "getPathWP", from, to);
		}
	};

} /* namespace object */
} /* namespace lua */
} /* namespace i6engine */

using namespace luabind;

scope registerObject() {
	return
		class_<i6engine::api::GameObject, i6engine::api::GOPtr>("GameObject")
			.def(constructor<>())
			.def("getID", &i6engine::api::GameObject::getID)
			.def("getGOC", (i6engine::api::ComPtr(i6engine::api::GameObject::*)(uint32_t)const) &i6engine::api::GameObject::getGOC)
			.def("getGOC", (i6engine::api::ComPtr(i6engine::api::GameObject::*)(uint32_t, const std::string &)const) &i6engine::api::GameObject::getGOC)
			.def("getGOCID", &i6engine::api::GameObject::getGOCID)
			.def("getGOCList", &i6engine::api::GameObject::getGOCList)
			.def("getType", &i6engine::api::GameObject::getType)
			.def("setDie", &i6engine::api::GameObject::setDie)
			.def("getOwner", &i6engine::api::GameObject::getOwner)
			.def("getUUID", &i6engine::api::GameObject::getUUID)
			.def("getCameraComponent", (i6engine::utils::sharedPtr<i6engine::api::CameraComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getCameraComponent)
			.def("getCameraComponent", (i6engine::utils::sharedPtr<i6engine::api::CameraComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getCameraComponent)
			.def("getLifetimeComponent", (i6engine::utils::sharedPtr<i6engine::api::LifetimeComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getLifetimeComponent)
			.def("getLifetimeComponent", (i6engine::utils::sharedPtr<i6engine::api::LifetimeComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getLifetimeComponent)
			.def("getLuminousAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::LuminousAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getLuminousAppearanceComponent)
			.def("getLuminousAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::LuminousAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getLuminousAppearanceComponent)
			.def("getMeshAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::MeshAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getMeshAppearanceComponent)
			.def("getMeshAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::MeshAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getMeshAppearanceComponent)
			.def("getMoverCircleComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverCircleComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getMoverCircleComponent)
			.def("getMoverCircleComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverCircleComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getMoverCircleComponent)
			.def("getMoverComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getMoverComponent)
			.def("getMoverComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getMoverComponent)
			.def("getMoverInterpolateComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverInterpolateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getMoverInterpolateComponent)
			.def("getMoverInterpolateComponent", (i6engine::utils::sharedPtr<i6engine::api::MoverInterpolateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getMoverInterpolateComponent)
			.def("getMovingCameraComponent", (i6engine::utils::sharedPtr<i6engine::api::MovingCameraComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getMovingCameraComponent)
			.def("getMovingCameraComponent", (i6engine::utils::sharedPtr<i6engine::api::MovingCameraComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getMovingCameraComponent)
			.def("getNetworkSenderComponent", (i6engine::utils::sharedPtr<i6engine::api::NetworkSenderComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getNetworkSenderComponent)
			.def("getNetworkSenderComponent", (i6engine::utils::sharedPtr<i6engine::api::NetworkSenderComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getNetworkSenderComponent)
			.def("getParticleEmitterComponent", (i6engine::utils::sharedPtr<i6engine::api::ParticleEmitterComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getParticleEmitterComponent)
			.def("getParticleEmitterComponent", (i6engine::utils::sharedPtr<i6engine::api::ParticleEmitterComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getParticleEmitterComponent)
			.def("getPhysicalStateComponent", (i6engine::utils::sharedPtr<i6engine::api::PhysicalStateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getPhysicalStateComponent)
			.def("getPhysicalStateComponent", (i6engine::utils::sharedPtr<i6engine::api::PhysicalStateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getPhysicalStateComponent)
			.def("getShatterComponent", (i6engine::utils::sharedPtr<i6engine::api::ShatterComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getShatterComponent)
			.def("getShatterComponent", (i6engine::utils::sharedPtr<i6engine::api::ShatterComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getShatterComponent)
			.def("getSpawnpointComponent", (i6engine::utils::sharedPtr<i6engine::api::SpawnpointComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getSpawnpointComponent)
			.def("getSpawnpointComponent", (i6engine::utils::sharedPtr<i6engine::api::SpawnpointComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getSpawnpointComponent)
			.def("getStaticStateComponent", (i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getStaticStateComponent)
			.def("getStaticStateComponent", (i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getStaticStateComponent)
			.def("getTerrainAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::TerrainAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getTerrainAppearanceComponent)
			.def("getTerrainAppearanceComponent", (i6engine::utils::sharedPtr<i6engine::api::TerrainAppearanceComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getTerrainAppearanceComponent)
			.def("getSoundComponent", (i6engine::utils::sharedPtr<i6engine::api::SoundComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getSoundComponent)
			.def("getSoundComponent", (i6engine::utils::sharedPtr<i6engine::api::SoundComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getSoundComponent)
			.def("getSoundListenerComponent", (i6engine::utils::sharedPtr<i6engine::api::SoundListenerComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getSoundListenerComponent)
			.def("getSoundListenerComponent", (i6engine::utils::sharedPtr<i6engine::api::SoundListenerComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getSoundListenerComponent)
			.def("getBillboardComponent", (i6engine::utils::sharedPtr<i6engine::api::BillboardComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getBillboardComponent)
			.def("getBillboardComponent", (i6engine::utils::sharedPtr<i6engine::api::BillboardComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getBillboardComponent)
			.def("getFollowComponent", (i6engine::utils::sharedPtr<i6engine::api::FollowComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getFollowComponent)
			.def("getFollowComponent", (i6engine::utils::sharedPtr<i6engine::api::FollowComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getFollowComponent)
			.def("getMovableTextComponent", (i6engine::utils::sharedPtr<i6engine::api::MovableTextComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getMovableTextComponent)
			.def("getMovableTextComponent", (i6engine::utils::sharedPtr<i6engine::api::MovableTextComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getMovableTextComponent)
			.def("getWaypointComponent", (i6engine::utils::sharedPtr<i6engine::api::WaypointComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getWaypointComponent)
			.def("getWaypointComponent", (i6engine::utils::sharedPtr<i6engine::api::WaypointComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getWaypointComponent)
			.def("getNavigationComponent", (i6engine::utils::sharedPtr<i6engine::api::NavigationComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getNavigationComponent)
			.def("getNavigationComponent", (i6engine::utils::sharedPtr<i6engine::api::NavigationComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getNavigationComponent)
			.def("getWaynetNavigationComponent", (i6engine::utils::sharedPtr<i6engine::api::WaynetNavigationComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getWaynetNavigationComponent)
			.def("getWaynetNavigationComponent", (i6engine::utils::sharedPtr<i6engine::api::WaynetNavigationComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getWaynetNavigationComponent)
			.def("getMoveComponent", (i6engine::utils::sharedPtr<i6engine::api::MoveComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getMoveComponent)
			.def("getMoveComponent", (i6engine::utils::sharedPtr<i6engine::api::MoveComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getMoveComponent)
			.def("getMovementComponent", (i6engine::utils::sharedPtr<i6engine::api::MovementComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getMovementComponent)
			.def("getMovementComponent", (i6engine::utils::sharedPtr<i6engine::api::MovementComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getMovementComponent)
			.def("getToggleWaynetComponent", (i6engine::utils::sharedPtr<i6engine::api::ToggleWaynetComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getToggleWaynetComponent)
			.def("getToggleWaynetComponent", (i6engine::utils::sharedPtr<i6engine::api::ToggleWaynetComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getToggleWaynetComponent)
			.def("getPoint2PointConstraintComponent", (i6engine::utils::sharedPtr<i6engine::api::Point2PointConstraintComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t)) &i6engine::lua::object::getPoint2PointConstraintComponent)
			.def("getPoint2PointConstraintComponent", (i6engine::utils::sharedPtr<i6engine::api::Point2PointConstraintComponent, i6engine::api::Component>(*)(i6engine::api::GameObject *, uint32_t, const std::string &)) &i6engine::lua::object::getPoint2PointConstraintComponent),

		class_<i6engine::api::AddStrategy>("AddStrategy")
			.def(constructor<>())
			.enum_("AddStrategy")
			[
				value("ADD", int(i6engine::api::AddStrategy::ADD)),
				value("REPLACE", int(i6engine::api::AddStrategy::REPLACE)),
				value("REPLACE_DIS", int(i6engine::api::AddStrategy::REPLACE_DIS)),
				value("REJECT", int(i6engine::api::AddStrategy::REJECT))
			],

		class_<i6engine::api::Component, i6engine::lua::object::ComponentWrapper, i6engine::api::ComPtr>("Component")
			.def(constructor<int64_t, const i6engine::api::attributeMap &>())
			.def("getOwnerGO", &i6engine::api::Component::getOwnerGO)
			.def("getComponentID", &i6engine::api::Component::getComponentID)
			.def("getFamilyID", &i6engine::api::Component::getFamilyID)
			.def("getIdentifier", &i6engine::api::Component::getIdentifier)
			.def("Tick", &i6engine::api::Component::Tick, &i6engine::lua::object::ComponentWrapper::default_Tick)
			.def("setDie", &i6engine::api::Component::setDie)
			.def("getID", &i6engine::api::Component::getID)
			.def("News", &i6engine::api::Component::News, &i6engine::lua::object::ComponentWrapper::default_News)
			.def("Init", &i6engine::lua::object::ComponentWrapper::Init)
			.def("Finalize", &i6engine::api::Component::Finalize, &i6engine::lua::object::ComponentWrapper::default_Finalize)
			.def("synchronize", &i6engine::lua::object::ComponentWrapper::synchronize)
			.def("setSync", &i6engine::api::Component::setSync)
			.def("getSync", &i6engine::api::Component::getSync)
			.def("howToAdd", &i6engine::api::Component::howToAdd, &i6engine::lua::object::ComponentWrapper::default_howToAdd)
			.def("getTemplateName", &i6engine::lua::object::ComponentWrapper::getTemplateName)
			.def("addTicker", &i6engine::lua::object::ComponentWrapper::addTicker)
			.def("removeTicker", &i6engine::lua::object::ComponentWrapper::removeTicker),

		class_<i6engine::api::BillboardComponent, i6engine::api::Component, i6engine::utils::sharedPtr<i6engine::api::BillboardComponent, i6engine::api::Component>>("BillboardComponent")
			.def(constructor<int64_t, const i6engine::api::attributeMap &>())
			.def("createOrUpdateBillboard", &i6engine::api::BillboardComponent::createOrUpdateBillboard)
			.def("deleteBillboard", &i6engine::api::BillboardComponent::deleteBillboard)
			.def("synchronize", &i6engine::api::BillboardComponent::synchronize)
			.def("getTemplateName", &i6engine::api::BillboardComponent::getTemplateName),

		class_<i6engine::api::CameraComponent, i6engine::lua::object::CameraComponentWrapper, i6engine::utils::sharedPtr<i6engine::api::CameraComponent, i6engine::api::Component>>("CameraComponent")
			.def(constructor<int64_t, const i6engine::api::attributeMap &>())
			.def("Tick", &i6engine::api::Component::Tick, &i6engine::lua::object::CameraComponentWrapper::default_Tick)
			.def("News", &i6engine::api::Component::News, &i6engine::lua::object::CameraComponentWrapper::default_News)
			.def("Init", &i6engine::lua::object::CameraComponentWrapper::Init)
			.def("Finalize", &i6engine::api::Component::Finalize, &i6engine::lua::object::CameraComponentWrapper::default_Finalize)
			.def("synchronize", &i6engine::lua::object::CameraComponentWrapper::synchronize)
			.def("howToAdd", &i6engine::api::Component::howToAdd, &i6engine::lua::object::CameraComponentWrapper::default_howToAdd)
			.def("getTemplateName", &i6engine::lua::object::CameraComponentWrapper::getTemplateName)
			.def("addTicker", &i6engine::lua::object::CameraComponentWrapper::addTicker)
			.def("removeTicker", &i6engine::lua::object::CameraComponentWrapper::removeTicker)
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
			.def("enableCompositor", &i6engine::api::CameraComponent::enableCompositor),

		class_<i6engine::api::FollowComponent, i6engine::api::Component, i6engine::utils::sharedPtr<i6engine::api::FollowComponent, i6engine::api::Component>>("FollowComponent")
			.def(constructor<int64_t, const i6engine::api::attributeMap &>())
			.def("synchronize", &i6engine::api::FollowComponent::synchronize)
			.def("getTemplateName", &i6engine::api::FollowComponent::getTemplateName),

		class_<i6engine::api::LifetimeComponent, i6engine::api::Component, i6engine::utils::sharedPtr<i6engine::api::LifetimeComponent, i6engine::api::Component>>("LifetimeComponent")
			.def(constructor<int64_t, const i6engine::api::attributeMap &>())
			.def("synchronize", &i6engine::api::LifetimeComponent::synchronize)
			.def("getTemplateName", &i6engine::api::LifetimeComponent::getTemplateName)
			.def("instantKill", &i6engine::api::LifetimeComponent::instantKill),

		class_<i6engine::api::LuminousAppearanceComponent, i6engine::api::Component, i6engine::utils::sharedPtr<i6engine::api::LuminousAppearanceComponent, i6engine::api::Component>>("LuminousAppearanceComponent")
			.def(constructor<int64_t, const i6engine::api::attributeMap &>())
			.def("synchronize", &i6engine::api::LuminousAppearanceComponent::synchronize)
			.def("getTemplateName", &i6engine::api::LuminousAppearanceComponent::getTemplateName)
			.enum_("LightType")
			[
				value("POINT", int(i6engine::api::LuminousAppearanceComponent::LightType::POINT)),
				value("DIRECTIONAL", int(i6engine::api::LuminousAppearanceComponent::LightType::DIRECTIONAL)),
				value("SPOT", int(i6engine::api::LuminousAppearanceComponent::LightType::SPOT))
			]
			.def("setLightType", &i6engine::api::LuminousAppearanceComponent::setLightType)
			.def("getLightType", &i6engine::lua::object::getLightType)
			.def("setDiffuseColor", &i6engine::api::LuminousAppearanceComponent::setDiffuseColor)
			.def("getDiffuseColor", &i6engine::api::LuminousAppearanceComponent::getDiffuseColor)
			.def("setSpecularColor", &i6engine::api::LuminousAppearanceComponent::setSpecularColor)
			.def("getSpecularColor", &i6engine::api::LuminousAppearanceComponent::getSpecularColor)
			.def("setAttenuation", &i6engine::api::LuminousAppearanceComponent::setAttenuation)
			.def("getAttenuation", &i6engine::api::LuminousAppearanceComponent::getAttenuation)
			.def("setDirection", &i6engine::api::LuminousAppearanceComponent::setDirection)
			.def("getDirection", &i6engine::api::LuminousAppearanceComponent::getDirection),

		class_<i6engine::api::MeshAppearanceComponent, i6engine::api::Component, i6engine::utils::sharedPtr<i6engine::api::MeshAppearanceComponent, i6engine::api::Component>>("MeshAppearanceComponent")
			.def(constructor<int64_t, const i6engine::api::attributeMap &>())
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
			.def("stopAnimation", &i6engine::api::MeshAppearanceComponent::stopAnimation),

		class_<i6engine::api::MovableTextComponent, i6engine::api::Component, i6engine::utils::sharedPtr<i6engine::api::MovableTextComponent, i6engine::api::Component>>("MovableTextComponent")
			.def(constructor<int64_t, const i6engine::api::attributeMap &>())
			.def("synchronize", &i6engine::api::MovableTextComponent::synchronize)
			.def("getTemplateName", &i6engine::api::MovableTextComponent::getTemplateName)
			.def("setText", &i6engine::api::MovableTextComponent::setText),

		class_<i6engine::api::MoveComponent, i6engine::api::Component, i6engine::utils::sharedPtr<i6engine::api::MoveComponent, i6engine::api::Component>>("MoveComponent")
			.def(constructor<int64_t, const i6engine::api::attributeMap &>())
			.def("synchronize", &i6engine::api::MoveComponent::synchronize)
			.def("getTemplateName", &i6engine::api::MoveComponent::getTemplateName)
			.def("navigate", (void(i6engine::api::MoveComponent::*)(const Vec3 &)) &i6engine::api::MoveComponent::navigate)
			.def("navigate", (void(i6engine::api::MoveComponent::*)(const std::string &)) &i6engine::api::MoveComponent::navigate),

		class_<i6engine::api::MovementComponent, i6engine::lua::object::MovementComponentWrapper, i6engine::utils::sharedPtr<i6engine::api::MovementComponent, i6engine::api::Component>>("MovementComponent")
			.def(constructor<int64_t, const i6engine::api::attributeMap &>())
			.def("Tick", &i6engine::api::MovementComponent::Tick, &i6engine::lua::object::MovementComponentWrapper::default_Tick)
			.def("News", &i6engine::api::MovementComponent::News, &i6engine::lua::object::MovementComponentWrapper::default_News)
			.def("Init", &i6engine::lua::object::MovementComponentWrapper::Init)
			.def("Finalize", &i6engine::api::MovementComponent::Finalize, &i6engine::lua::object::MovementComponentWrapper::default_Finalize)
			.def("synchronize", &i6engine::lua::object::MovementComponentWrapper::synchronize)
			.def("howToAdd", &i6engine::api::Component::howToAdd, &i6engine::lua::object::MovementComponentWrapper::default_howToAdd)
			.def("getTemplateName", &i6engine::lua::object::MovementComponentWrapper::getTemplateName)
			.def("forward", &i6engine::lua::object::MovementComponentWrapper::forward)
			.def("backward", &i6engine::lua::object::MovementComponentWrapper::backward)
			.def("left", &i6engine::lua::object::MovementComponentWrapper::left)
			.def("right", &i6engine::lua::object::MovementComponentWrapper::right),

		class_<i6engine::api::MoverCircleComponent, i6engine::api::Component, i6engine::utils::sharedPtr<i6engine::api::MoverCircleComponent, i6engine::api::Component>>("MoverCircleComponent")
			.def(constructor<int64_t, const i6engine::api::attributeMap &>())
			.def("synchronize", &i6engine::api::MoverCircleComponent::synchronize)
			.def("getTemplateName", &i6engine::api::MoverCircleComponent::getTemplateName)
			.def("setPositioning", &i6engine::api::MoverCircleComponent::setPositioning)
			.def("setCircleParameters", &i6engine::api::MoverCircleComponent::setCircleParameters)
			.def("start", &i6engine::api::MoverCircleComponent::start)
			.def("getCircleAxis", &i6engine::api::MoverCircleComponent::getCircleAxis)
			.def("getCircleRadius", &i6engine::api::MoverCircleComponent::getCircleRadius)
			.def("reset", &i6engine::api::MoverCircleComponent::reset),

		class_<i6engine::api::MoverComponent, i6engine::lua::object::MoverComponentWrapper, i6engine::utils::sharedPtr<i6engine::api::MoverComponent, i6engine::api::Component>>("MoverComponent")
			.def(constructor<int64_t, const i6engine::api::attributeMap &>())
			.def("Tick", &i6engine::api::MoverComponent::Tick, &i6engine::lua::object::MoverComponentWrapper::default_Tick)
			.def("News", &i6engine::lua::object::MoverComponentWrapper::News)
			.def("Init", &i6engine::lua::object::MoverComponentWrapper::Init)
			.def("Finalize", &i6engine::api::MoverComponent::Finalize, &i6engine::lua::object::MoverComponentWrapper::default_Finalize)
			.def("synchronize", &i6engine::api::MoverComponent::synchronize, &i6engine::lua::object::MoverComponentWrapper::synchronize)
			.def("howToAdd", &i6engine::api::Component::howToAdd, &i6engine::lua::object::MoverComponentWrapper::default_howToAdd)
			.def("getTemplateName", &i6engine::lua::object::MoverComponentWrapper::getTemplateName)
			.def("start", &i6engine::lua::object::MoverComponentWrapper::start)
			.def("reset", &i6engine::lua::object::MoverComponentWrapper::reset)
			.enum_("Positioning")
			[
				value("POSITIONING_ABSOLUTE", int(i6engine::api::MoverComponent::Positioning::POSITIONING_ABSOLUTE)),
				value("POSITIONING_RELATIVE", int(i6engine::api::MoverComponent::Positioning::POSITIONING_RELATIVE))
			],

		class_<i6engine::api::MoverInterpolateComponent, i6engine::api::Component, i6engine::utils::sharedPtr<i6engine::api::MoverInterpolateComponent, i6engine::api::Component>>("MoverInterpolateComponent")
			.def(constructor<int64_t, const i6engine::api::attributeMap &>())
			.def("synchronize", &i6engine::api::MoverInterpolateComponent::synchronize)
			.def("getTemplateName", &i6engine::api::MoverInterpolateComponent::getTemplateName)
			.def("addKeyFrame", &i6engine::api::MoverInterpolateComponent::addKeyFrame)
			.def("removeKeyFrame", &i6engine::api::MoverInterpolateComponent::removeKeyFrame)
			.def("getKeyframe", &i6engine::api::MoverInterpolateComponent::getKeyframe)
			.def("setMode", &i6engine::api::MoverInterpolateComponent::setMode)
			.def("setOpenTime", &i6engine::api::MoverInterpolateComponent::setOpenTime)
			.def("setWay", &i6engine::api::MoverInterpolateComponent::setWay)
			.def("getWay", &i6engine::lua::object::getWay)
			.def("start", &i6engine::api::MoverInterpolateComponent::start)
			.def("reset", &i6engine::api::MoverInterpolateComponent::reset)
			.enum_("Mode")
			[
				value("TWOSTATE_TOGGLE", int(i6engine::api::MoverInterpolateComponent::Mode::TWOSTATE_TOGGLE)),
				value("TWOSTATE_OPENTIME", int(i6engine::api::MoverInterpolateComponent::Mode::TWOSTATE_OPENTIME)),
				value("NSTATE_LOOP", int(i6engine::api::MoverInterpolateComponent::Mode::NSTATE_LOOP)),
				value("ONCE", int(i6engine::api::MoverInterpolateComponent::Mode::ONCE))
			]
			.enum_("Way")
			[
				value("LINEAR", int(i6engine::api::MoverInterpolateComponent::Way::LINEAR)),
				value("BEZIER", int(i6engine::api::MoverInterpolateComponent::Way::BEZIER))
			],

		class_<i6engine::api::MovingCameraComponent, i6engine::api::Component, i6engine::utils::sharedPtr<i6engine::api::MovingCameraComponent, i6engine::api::Component>>("MovingCameraComponent")
			.def(constructor<int64_t, const i6engine::api::attributeMap &>())
			.def("getTemplateName", &i6engine::api::MoverCircleComponent::getTemplateName),

		class_<i6engine::api::NavigationComponent, i6engine::lua::object::NavigationComponentWrapper, i6engine::utils::sharedPtr<i6engine::api::NavigationComponent, i6engine::api::Component>>("NavigationComponent")
			.def(constructor<int64_t, const i6engine::api::attributeMap &>())
			.def("Tick", &i6engine::api::Component::Tick, &i6engine::lua::object::NavigationComponentWrapper::default_Tick)
			.def("News", &i6engine::api::Component::News, &i6engine::lua::object::NavigationComponentWrapper::default_News)
			.def("Init", &i6engine::lua::object::NavigationComponentWrapper::Init)
			.def("Finalize", &i6engine::api::Component::Finalize, &i6engine::lua::object::NavigationComponentWrapper::default_Finalize)
			.def("synchronize", &i6engine::lua::object::NavigationComponentWrapper::synchronize)
			.def("howToAdd", &i6engine::api::Component::howToAdd, &i6engine::lua::object::NavigationComponentWrapper::default_howToAdd)
			.def("getTemplateName", &i6engine::lua::object::NavigationComponentWrapper::getTemplateName)
			.def("getPathPos", (std::vector<Vec3>(i6engine::lua::object::NavigationComponentWrapper::*)(const Vec3 &, const Vec3 &) const) &i6engine::lua::object::NavigationComponentWrapper::getPath)
			.def("getPathWP", (std::vector<Vec3>(i6engine::lua::object::NavigationComponentWrapper::*)(const Vec3 &, const std::string &) const) &i6engine::lua::object::NavigationComponentWrapper::getPath),

		class_<i6engine::api::NetworkSenderComponent, i6engine::api::Component, i6engine::utils::sharedPtr<i6engine::api::NetworkSenderComponent, i6engine::api::Component>>("NetworkSenderComponent")
			.def(constructor<>())
			.def("getTemplateName", &i6engine::api::NetworkSenderComponent::getTemplateName),

		class_<i6engine::api::PhysicalStateComponent, i6engine::api::Component, i6engine::utils::sharedPtr<i6engine::api::PhysicalStateComponent, i6engine::api::Component>>("PhysicalStateComponent")
			.def("getPosition", &i6engine::api::PhysicalStateComponent::getPosition)
			.def("setPosition", &i6engine::api::PhysicalStateComponent::setPosition)
			.def("getRotation", &i6engine::api::PhysicalStateComponent::getRotation)
			.def("setRotation", &i6engine::api::PhysicalStateComponent::setRotation)
			.def("applyRotation", &i6engine::api::PhysicalStateComponent::applyRotation)
			.def("applyCentralForce", &i6engine::api::PhysicalStateComponent::applyCentralForce)
			.def("applyForce", &i6engine::api::PhysicalStateComponent::applyForce)
			.def("getLinearVelocity", &i6engine::api::PhysicalStateComponent::getLinearVelocity)
			.def("reset", &i6engine::api::PhysicalStateComponent::reset)
			.enum_("RayTestRepetition")
			[
				value("STOP", int(i6engine::api::PhysicalStateComponent::RayTestRepetition::STOP)),
				value("ONCE", int(i6engine::api::PhysicalStateComponent::RayTestRepetition::ONCE)),
				value("PERIODIC", int(i6engine::api::PhysicalStateComponent::RayTestRepetition::PERIODIC))
			]
			.enum_("RayTestNotify")
			[
				value("ALWAYS", int(i6engine::api::PhysicalStateComponent::RayTestNotify::ALWAYS)),
				value("CHANGE", int(i6engine::api::PhysicalStateComponent::RayTestNotify::CHANGE)),
				value("FOUND", int(i6engine::api::PhysicalStateComponent::RayTestNotify::FOUND)),
				value("NOTFOUND", int(i6engine::api::PhysicalStateComponent::RayTestNotify::NOTFOUND)),
				value("OBJECTCHANGE", int(i6engine::api::PhysicalStateComponent::RayTestNotify::OBJECTCHANGE))
			],

		class_<i6engine::api::RayTestResult>("RayTestResult")
			.def(constructor<>())
			.def_readonly("objID", &i6engine::api::RayTestResult::objID)
			.def_readonly("sourceID", &i6engine::api::RayTestResult::sourceID),

		def("getObject", &i6engine::lua::object::getObject),
		def("getAllObjectsOfType", &i6engine::lua::object::getAllObjectsOfType),
		def("getGOList", &i6engine::lua::object::getGOList),
		def("deleteAllObjectsOfType", &i6engine::lua::object::deleteAllObjectsOfType),
		def("createObject", &i6engine::lua::object::createObject),
		def("cleanUpAll", &i6engine::lua::object::cleanUpAll),
		def("loadLevel", &i6engine::lua::object::loadLevel),
		def("getFrameTime", &i6engine::lua::object::getFrameTime),
		def("registerCTemplate", &i6engine::lua::object::registerCTemplate),
		def("createGO", &i6engine::lua::object::createGO),
		def("createComponent", &i6engine::lua::object::createComponent),
		def("resetObjectSubSystem", &i6engine::lua::object::resetObjectSubSystem),
		def("pauseObject", &i6engine::lua::object::pauseObject),
		def("unpauseObject", &i6engine::lua::object::unpauseObject),
		def("rayTest", &i6engine::lua::object::rayTest),

		class_<i6engine::api::objects::GOTemplateComponent>("GOTemplateComponent")
			.def(constructor<const std::string &, const i6engine::api::attributeMap &, const std::string &, bool, bool>())
			.def_readwrite("template", &i6engine::api::objects::GOTemplateComponent::_template)
			.def_readwrite("id", &i6engine::api::objects::GOTemplateComponent::_id)
			.def_readwrite("params", &i6engine::api::objects::GOTemplateComponent::_params)
			.def_readwrite("owner", &i6engine::api::objects::GOTemplateComponent::_owner)
			.def_readwrite("identifier", &i6engine::api::objects::GOTemplateComponent::_identifier)
			.def_readwrite("deleted", &i6engine::api::objects::GOTemplateComponent::_deleted),

		class_<i6engine::api::objects::GOTemplate>("GOTemplate")
			.def(constructor<>())
			.def_readwrite("type", &i6engine::api::objects::GOTemplate::_type)
			.def_readwrite("components", &i6engine::api::objects::GOTemplate::_components),

		class_<i6engine::api::attributeMap>("attributeMap")
			.def(constructor<>())
			.def("insert", &i6engine::lua::object::insertPairInAttributeMap),

		class_<std::vector<i6engine::api::objects::GOTemplateComponent>>("GOTemplateComponentVector")
			.def(constructor<>())
			.def("push_back", (void(std::vector<i6engine::api::objects::GOTemplateComponent>::*)(const i6engine::api::objects::GOTemplateComponent &)) &std::vector<i6engine::api::objects::GOTemplateComponent>::push_back),

		class_<i6engine::api::CollisionGroup>("CollisionGroup")
			.def(constructor<uint32_t, uint32_t, uint32_t>()),

		class_<i6engine::api::components::ComponentTypes>("ComponentTypes")
			.def(constructor<>())
			.enum_("Types")
			[
				value("CameraComponent", i6engine::api::components::ComponentTypes::CameraComponent),
				value("LifetimeComponent", i6engine::api::components::ComponentTypes::LifetimeComponent),
				value("LuminousAppearanceComponent", i6engine::api::components::ComponentTypes::LuminousAppearanceComponent),
				value("MeshAppearanceComponent", i6engine::api::components::ComponentTypes::MeshAppearanceComponent),
				value("MoverCircleComponent", i6engine::api::components::ComponentTypes::MoverCircleComponent),
				value("MoverComponent", i6engine::api::components::ComponentTypes::MoverComponent),
				value("MoverInterpolateComponent", i6engine::api::components::ComponentTypes::MoverInterpolateComponent),
				value("MovingCameraComponent", i6engine::api::components::ComponentTypes::MovingCameraComponent),
				value("NetworkSenderComponent", i6engine::api::components::ComponentTypes::NetworkSenderComponent),
				value("ParticleEmitterComponent", i6engine::api::components::ComponentTypes::ParticleEmitterComponent),
				value("PhysicalStateComponent", i6engine::api::components::ComponentTypes::PhysicalStateComponent),
				value("ShatterComponent", i6engine::api::components::ComponentTypes::ShatterComponent),
				value("SpawnpointComponent", i6engine::api::components::ComponentTypes::SpawnpointComponent),
				value("StaticStateComponent", i6engine::api::components::ComponentTypes::StaticStateComponent),
				value("TerrainAppearanceComponent", i6engine::api::components::ComponentTypes::TerrainAppearanceComponent),
				value("SoundComponent", i6engine::api::components::ComponentTypes::SoundComponent),
				value("SoundListenerComponent", i6engine::api::components::ComponentTypes::SoundListenerComponent),
				value("BillboardComponent", i6engine::api::components::ComponentTypes::BillboardComponent),
				value("FollowComponent", i6engine::api::components::ComponentTypes::FollowComponent),
				value("MovableTextComponent", i6engine::api::components::ComponentTypes::MovableTextComponent),
				value("WaypointComponent", i6engine::api::components::ComponentTypes::WaypointComponent),
				value("NavigationComponent", i6engine::api::components::ComponentTypes::NavigationComponent),
				value("WaynetNavigationComponent", i6engine::api::components::ComponentTypes::WaynetNavigationComponent),
				value("MoveComponent", i6engine::api::components::ComponentTypes::MoveComponent),
				value("MovementComponent", i6engine::api::components::ComponentTypes::MovementComponent),
				value("ToggleWaynetComponent", i6engine::api::components::ComponentTypes::ToggleWaynetComponent),
				value("Point2PointConstraintComponent", i6engine::api::components::ComponentTypes::Point2PointConstraintComponent),
				value("ComponentTypesCount", i6engine::api::components::ComponentTypes::ComponentTypesCount)
			]
		;
}
