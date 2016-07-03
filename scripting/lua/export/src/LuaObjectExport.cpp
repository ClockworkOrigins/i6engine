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

#include "LuaObjectExport.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/math/i6eMath.h"

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

#include "i6engine/luabind/operator.hpp"

namespace i6e {
namespace lua {
namespace object {

	std::vector<i6e::api::GOPtr> getAllObjectsOfType(const std::string & types) {
		return i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->getAllObjectsOfType(types);
	}

	i6e::api::GOPtr getObject(const int64_t id) {
		return i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(id);
	}

	std::vector<i6e::api::GOPtr> getGOList() {
		std::vector<i6e::api::GOPtr> l;
		std::unordered_map<int64_t, i6e::api::GOPtr> v = i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->getGOMap();
		for (std::unordered_map<int64_t, i6e::api::GOPtr>::const_iterator it = v.begin(); it != v.end(); ++it) {
			l.push_back(it->second);
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

	void registerCTemplate(const std::string & GOCType, const std::string & func) {
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->registerCTemplate(GOCType, [func](int64_t id, const i6e::api::attributeMap & params) {
			return utils::sharedPtr<i6e::api::Component, i6e::api::Component>(i6e::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<i6e::api::Component *>(func, id, params)->get());
		});
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

	struct ComponentWrapper : public i6e::api::Component, public luabind::wrap_base {
		ComponentWrapper(const int64_t id, const i6e::api::attributeMap & params) : Component(id, params), luabind::wrap_base() {
		}

		virtual void Tick() override {
			luabind::call_member<void>(this, "Tick");
		}

		static void default_Tick(i6e::api::Component * ptr) {
			ptr->Component::Tick();
		}

		virtual void News(const i6e::api::GameMessage::Ptr & msg) override {
			luabind::call_member<void>(this, "News", msg);
		}

		static void default_News(i6e::api::Component * ptr, const i6e::api::GameMessage::Ptr & msg) {
			ptr->Component::News(msg);
		}

		virtual void Init() override {
			luabind::call_member<void>(this, "Init");
		}

		virtual void Finalize() override {
			luabind::call_member<void>(this, "Finalize");
		}

		static void default_Finalize(i6e::api::Component * ptr) {
			ptr->Component::Finalize();
		}

		virtual i6e::api::attributeMap synchronize() const {
			return luabind::call_member<i6e::api::attributeMap>(this, "synchronize");
		}

		virtual std::pair<i6e::api::AddStrategy, int64_t> howToAdd(const i6e::api::ComPtr & comp) const override {
			return luabind::call_member<std::pair<i6e::api::AddStrategy, int64_t>>(this, "howToAdd", comp);
		}

		static std::pair<i6e::api::AddStrategy, int64_t> default_howToAdd(i6e::api::Component * ptr, const i6e::api::ComPtr & comp) {
			return ptr->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return luabind::call_member<std::string>(this, "getTemplateName");
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

	struct CameraComponentWrapper : public i6e::api::CameraComponent, public luabind::wrap_base {
		CameraComponentWrapper(const int64_t id, const i6e::api::attributeMap & params) : CameraComponent(id, params), luabind::wrap_base() {
		}

		virtual void Tick() override {
			luabind::call_member<void>(this, "Tick");
		}

		static void default_Tick(i6e::api::CameraComponent * ptr) {
			ptr->Component::Tick();
		}

		virtual void News(const i6e::api::GameMessage::Ptr & msg) override {
			luabind::call_member<void>(this, "News", msg);
		}

		static void default_News(i6e::api::CameraComponent * ptr, const i6e::api::GameMessage::Ptr & msg) {
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

		static void default_Finalize(i6e::api::CameraComponent * ptr) {
			ptr->CameraComponent::Finalize();
		}

		virtual i6e::api::attributeMap synchronize() const {
			i6e::api::attributeMap params = CameraComponent::synchronize();
			i6e::api::attributeMap params2 = luabind::call_member<i6e::api::attributeMap>(this, "synchronize");
			params.insert(params2.begin(), params2.end());
			return params;
		}

		virtual std::pair<i6e::api::AddStrategy, int64_t> howToAdd(const i6e::api::ComPtr & comp) const override {
			return luabind::call_member<std::pair<i6e::api::AddStrategy, int64_t>>(this, "howToAdd", comp);
		}

		static std::pair<i6e::api::AddStrategy, int64_t> default_howToAdd(i6e::api::CameraComponent * ptr, const i6e::api::ComPtr & comp) {
			return ptr->CameraComponent::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return luabind::call_member<std::string>(this, "getTemplateName");
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

	int getLightType(i6e::api::LuminousAppearanceComponent * c) {
		return int(c->getLightType());
	}

	struct MovementComponentWrapper : public i6e::api::MovementComponent, public luabind::wrap_base {
		MovementComponentWrapper(const int64_t id, const i6e::api::attributeMap & params) : MovementComponent(id, params), luabind::wrap_base() {
		}

		virtual void Tick() override {
			MovementComponent::Tick();
			luabind::call_member<void>(this, "Tick");
		}

		static void default_Tick(i6e::api::MovementComponent * ptr) {
			ptr->MovementComponent::Tick();
		}

		virtual void News(const i6e::api::GameMessage::Ptr & msg) override {
			luabind::call_member<void>(this, "News", msg);
		}

		static void default_News(i6e::api::MovementComponent * ptr, const i6e::api::GameMessage::Ptr & msg) {
			ptr->Component::News(msg);
		}

		virtual void Init() override {
			MovementComponent::Init();
			luabind::call_member<void>(this, "Init");
		}

		static void default_Init(i6e::api::MovementComponent * ptr) {
			ptr->MovementComponent::Init();
		}

		virtual void Finalize() override {
			MovementComponent::Finalize();
			luabind::call_member<void>(this, "Finalize");
		}

		static void default_Finalize(i6e::api::MovementComponent * ptr) {
			ptr->MovementComponent::Finalize();
		}

		virtual i6e::api::attributeMap synchronize() const {
			return luabind::call_member<i6e::api::attributeMap>(this, "synchronize");
		}

		virtual std::pair<i6e::api::AddStrategy, int64_t> howToAdd(const i6e::api::ComPtr & comp) const override {
			return luabind::call_member<std::pair<i6e::api::AddStrategy, int64_t>>(this, "howToAdd", comp);
		}

		static std::pair<i6e::api::AddStrategy, int64_t> default_howToAdd(i6e::api::MovementComponent * ptr, const i6e::api::ComPtr & comp) {
			return ptr->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return luabind::call_member<std::string>(this, "getTemplateName");
		}

		std::vector<i6e::api::componentOptions> getComponentOptions() {
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

		virtual void stop() override {
			luabind::call_member<void>(this, "stop");
		}
	};

	struct MoverComponentWrapper : public i6e::api::MoverComponent, public luabind::wrap_base {
		MoverComponentWrapper(const int64_t id, const i6e::api::attributeMap & params) : MoverComponent(id, params), luabind::wrap_base() {
		}

		virtual void Tick() override {
			MoverComponent::Tick();
			luabind::call_member<void>(this, "Tick");
		}

		static void default_Tick(i6e::api::MoverComponent * ptr) {
			ptr->MoverComponent::Tick();
		}

		virtual void News(const i6e::api::GameMessage::Ptr & msg) override {
			luabind::call_member<void>(this, "News", msg);
		}

		virtual void Init() override {
			MoverComponent::Init();
			luabind::call_member<void>(this, "Init");
		}

		static void default_Init(i6e::api::MoverComponent * ptr) {
			ptr->MoverComponent::Init();
		}

		virtual void Finalize() override {
			MoverComponent::Finalize();
			luabind::call_member<void>(this, "Finalize");
		}

		static void default_Finalize(i6e::api::MoverComponent * ptr) {
			ptr->MoverComponent::Finalize();
		}

		virtual i6e::api::attributeMap synchronize() const {
			i6e::api::attributeMap params = MoverComponent::synchronize();
			i6e::api::attributeMap params2 = luabind::call_member<i6e::api::attributeMap>(this, "synchronize");
			params.insert(params2.begin(), params2.end());
			return params;
		}

		static i6e::api::attributeMap default_synchronize(i6e::api::MoverComponent * ptr) {
			return ptr->MoverComponent::synchronize();
		}

		virtual std::pair<i6e::api::AddStrategy, int64_t> howToAdd(const i6e::api::ComPtr & comp) const override {
			return luabind::call_member<std::pair<i6e::api::AddStrategy, int64_t>>(this, "howToAdd", comp);
		}

		static std::pair<i6e::api::AddStrategy, int64_t> default_howToAdd(i6e::api::MoverComponent * ptr, const i6e::api::ComPtr & comp) {
			return ptr->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return luabind::call_member<std::string>(this, "getTemplateName");
		}

		std::vector<i6e::api::componentOptions> getComponentOptions() {
			return {};
		}

		void start(Vec3 & startPos) override {
			luabind::call_member<void>(this, "start", startPos);
		}

		virtual void reset() override {
			luabind::call_member<void>(this, "reset");
		}

		void getNewPosition(const uint64_t time, Vec3 & pos, Quaternion & rot) override {
			luabind::call_member<void>(this, "getNewPosition", time, pos, rot);
		}
	};

	int getWay(i6e::api::MoverInterpolateComponent * c) {
		return int(c->getWay());
	}

	struct NavigationComponentWrapper : public i6e::api::NavigationComponent, public luabind::wrap_base {
		NavigationComponentWrapper(const int64_t id, const i6e::api::attributeMap & params) : NavigationComponent(id, params), luabind::wrap_base() {
		}

		virtual void Tick() override {
			luabind::call_member<void>(this, "Tick");
		}

		static void default_Tick(i6e::api::NavigationComponent * ptr) {
			ptr->Component::Tick();
		}

		virtual void News(const i6e::api::GameMessage::Ptr & msg) override {
			luabind::call_member<void>(this, "News", msg);
		}

		static void default_News(i6e::api::NavigationComponent * ptr, const i6e::api::GameMessage::Ptr & msg) {
			ptr->Component::News(msg);
		}

		virtual void Init() override {
			luabind::call_member<void>(this, "Init");
		}

		virtual void Finalize() override {
			luabind::call_member<void>(this, "Finalize");
		}

		static void default_Finalize(i6e::api::NavigationComponent * ptr) {
			ptr->NavigationComponent::Finalize();
		}

		virtual i6e::api::attributeMap synchronize() const {
			return luabind::call_member<i6e::api::attributeMap>(this, "synchronize");
		}

		virtual std::pair<i6e::api::AddStrategy, int64_t> howToAdd(const i6e::api::ComPtr & comp) const override {
			return luabind::call_member<std::pair<i6e::api::AddStrategy, int64_t>>(this, "howToAdd", comp);
		}

		static std::pair<i6e::api::AddStrategy, int64_t> default_howToAdd(i6e::api::NavigationComponent * ptr, const i6e::api::ComPtr & comp) {
			return ptr->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return luabind::call_member<std::string>(this, "getTemplateName");
		}

		std::vector<i6e::api::componentOptions> getComponentOptions() {
			return {};
		}

		std::vector<Vec3> getPath(const Vec3 & from, const Vec3 & to) const override {
			return luabind::call_member<std::vector<Vec3>>(this, "getPathPos", from, to);
		}

		std::vector<Vec3> getPath(const Vec3 & from, const std::string & to) const override {
			return luabind::call_member<std::vector<Vec3>>(this, "getPathWP", from, to);
		}
	};

	struct ShatterComponentWrapper : public i6e::api::ShatterComponent, public luabind::wrap_base {
		ShatterComponentWrapper(const int64_t id, const i6e::api::attributeMap & params) : ShatterComponent(id, params), luabind::wrap_base() {
		}

		virtual void Tick() override {
			luabind::call_member<void>(this, "Tick");
		}

		static void default_Tick(i6e::api::ShatterComponent * ptr) {
			ptr->Component::Tick();
		}

		virtual void News(const i6e::api::GameMessage::Ptr & msg) override {
			luabind::call_member<void>(this, "News", msg);
		}

		static void default_News(i6e::api::ShatterComponent * ptr, const i6e::api::GameMessage::Ptr & msg) {
			ptr->ShatterComponent::News(msg);
		}

		virtual void Init() override {
			luabind::call_member<void>(this, "Init");
		}

		static void default_Init(i6e::api::ShatterComponent * ptr) {
			ptr->ShatterComponent::Init();
		}

		virtual void Finalize() override {
			luabind::call_member<void>(this, "Finalize");
		}

		static void default_Finalize(i6e::api::ShatterComponent * ptr) {
			ptr->Component::Finalize();
		}

		virtual i6e::api::attributeMap synchronize() const {
			return luabind::call_member<i6e::api::attributeMap>(this, "synchronize");
		}

		virtual std::pair<i6e::api::AddStrategy, int64_t> howToAdd(const i6e::api::ComPtr & comp) const override {
			return luabind::call_member<std::pair<i6e::api::AddStrategy, int64_t>>(this, "howToAdd", comp);
		}

		static std::pair<i6e::api::AddStrategy, int64_t> default_howToAdd(i6e::api::ShatterComponent * ptr, const i6e::api::ComPtr & comp) {
			return ptr->Component::howToAdd(comp);
		}

		virtual std::string getTemplateName() const {
			return luabind::call_member<std::string>(this, "getTemplateName");
		}

		std::vector<i6e::api::componentOptions> getComponentOptions() {
			return {};
		}

		void shatter(const i6e::api::GOPtr & other) override {
			luabind::call_member<void>(this, "shatter", other);
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
} /* namespace lua */
} /* namespace i6e */

using namespace luabind;

scope registerObject() {
	return
		class_<i6e::api::GameObject, i6e::api::GOPtr>("GameObject")
			.def(constructor<>())
			.def("getID", &i6e::api::GameObject::getID)
			.def("getGOC", (i6e::api::ComPtr(i6e::api::GameObject::*)(uint32_t)const) &i6e::api::GameObject::getGOC)
			.def("getGOC", (i6e::api::ComPtr(i6e::api::GameObject::*)(uint32_t, const std::string &)const) &i6e::api::GameObject::getGOC)
			.def("getGOCID", &i6e::api::GameObject::getGOCID)
			.def("getGOCList", &i6e::api::GameObject::getGOCList)
			.def("getType", &i6e::api::GameObject::getType)
			.def("setDie", &i6e::api::GameObject::setDie)
			.def("getOwner", &i6e::api::GameObject::getOwner)
			.def("getUUID", &i6e::api::GameObject::getUUID)
			.def("getAnimatedDirectionalLightComponent", (i6e::utils::sharedPtr<i6e::api::AnimatedDirectionalLightComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getAnimatedDirectionalLightComponent)
			.def("getAnimatedDirectionalLightComponent", (i6e::utils::sharedPtr<i6e::api::AnimatedDirectionalLightComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getAnimatedDirectionalLightComponent)
			.def("getAnimatedLuminousAppearanceComponent", (i6e::utils::sharedPtr<i6e::api::AnimatedLuminousAppearanceComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getAnimatedLuminousAppearanceComponent)
			.def("getAnimatedLuminousAppearanceComponent", (i6e::utils::sharedPtr<i6e::api::AnimatedLuminousAppearanceComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getAnimatedLuminousAppearanceComponent)
			.def("getAnimatedSpotLightComponent", (i6e::utils::sharedPtr<i6e::api::AnimatedSpotLightComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getAnimatedSpotLightComponent)
			.def("getAnimatedSpotLightComponent", (i6e::utils::sharedPtr<i6e::api::AnimatedSpotLightComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getAnimatedSpotLightComponent)
			.def("getAnimationControllerComponent", (i6e::utils::sharedPtr<i6e::api::AnimationControllerComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getAnimationControllerComponent)
			.def("getAnimationControllerComponent", (i6e::utils::sharedPtr<i6e::api::AnimationControllerComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getAnimationControllerComponent)
			.def("getCameraComponent", (i6e::utils::sharedPtr<i6e::api::CameraComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getCameraComponent)
			.def("getCameraComponent", (i6e::utils::sharedPtr<i6e::api::CameraComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getCameraComponent)
			.def("getLifetimeComponent", (i6e::utils::sharedPtr<i6e::api::LifetimeComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getLifetimeComponent)
			.def("getLifetimeComponent", (i6e::utils::sharedPtr<i6e::api::LifetimeComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getLifetimeComponent)
			.def("getLuminousAppearanceComponent", (i6e::utils::sharedPtr<i6e::api::LuminousAppearanceComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getLuminousAppearanceComponent)
			.def("getLuminousAppearanceComponent", (i6e::utils::sharedPtr<i6e::api::LuminousAppearanceComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getLuminousAppearanceComponent)
			.def("getMeshAppearanceComponent", (i6e::utils::sharedPtr<i6e::api::MeshAppearanceComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getMeshAppearanceComponent)
			.def("getMeshAppearanceComponent", (i6e::utils::sharedPtr<i6e::api::MeshAppearanceComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getMeshAppearanceComponent)
			.def("getMoverCircleComponent", (i6e::utils::sharedPtr<i6e::api::MoverCircleComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getMoverCircleComponent)
			.def("getMoverCircleComponent", (i6e::utils::sharedPtr<i6e::api::MoverCircleComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getMoverCircleComponent)
			.def("getMoverComponent", (i6e::utils::sharedPtr<i6e::api::MoverComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getMoverComponent)
			.def("getMoverComponent", (i6e::utils::sharedPtr<i6e::api::MoverComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getMoverComponent)
			.def("getMoverInterpolateComponent", (i6e::utils::sharedPtr<i6e::api::MoverInterpolateComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getMoverInterpolateComponent)
			.def("getMoverInterpolateComponent", (i6e::utils::sharedPtr<i6e::api::MoverInterpolateComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getMoverInterpolateComponent)
			.def("getMovingCameraComponent", (i6e::utils::sharedPtr<i6e::api::MovingCameraComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getMovingCameraComponent)
			.def("getMovingCameraComponent", (i6e::utils::sharedPtr<i6e::api::MovingCameraComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getMovingCameraComponent)
#ifdef ISIXE_WITH_NETWORK
			.def("getNetworkSenderComponent", (i6e::utils::sharedPtr<i6e::api::NetworkSenderComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getNetworkSenderComponent)
			.def("getNetworkSenderComponent", (i6e::utils::sharedPtr<i6e::api::NetworkSenderComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getNetworkSenderComponent)
#endif
			.def("getParticleEmitterComponent", (i6e::utils::sharedPtr<i6e::api::ParticleEmitterComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getParticleEmitterComponent)
			.def("getParticleEmitterComponent", (i6e::utils::sharedPtr<i6e::api::ParticleEmitterComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getParticleEmitterComponent)
			.def("getPhysicalStateComponent", (i6e::utils::sharedPtr<i6e::api::PhysicalStateComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getPhysicalStateComponent)
			.def("getPhysicalStateComponent", (i6e::utils::sharedPtr<i6e::api::PhysicalStateComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getPhysicalStateComponent)
			.def("getShatterComponent", (i6e::utils::sharedPtr<i6e::api::ShatterComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getShatterComponent)
			.def("getShatterComponent", (i6e::utils::sharedPtr<i6e::api::ShatterComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getShatterComponent)
			.def("getSpawnpointComponent", (i6e::utils::sharedPtr<i6e::api::SpawnpointComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getSpawnpointComponent)
			.def("getSpawnpointComponent", (i6e::utils::sharedPtr<i6e::api::SpawnpointComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getSpawnpointComponent)
			.def("getStaticStateComponent", (i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getStaticStateComponent)
			.def("getStaticStateComponent", (i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getStaticStateComponent)
			.def("getTerrainAppearanceComponent", (i6e::utils::sharedPtr<i6e::api::TerrainAppearanceComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getTerrainAppearanceComponent)
			.def("getTerrainAppearanceComponent", (i6e::utils::sharedPtr<i6e::api::TerrainAppearanceComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getTerrainAppearanceComponent)
			.def("getSoundComponent", (i6e::utils::sharedPtr<i6e::api::SoundComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getSoundComponent)
			.def("getSoundComponent", (i6e::utils::sharedPtr<i6e::api::SoundComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getSoundComponent)
			.def("getSoundListenerComponent", (i6e::utils::sharedPtr<i6e::api::SoundListenerComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getSoundListenerComponent)
			.def("getSoundListenerComponent", (i6e::utils::sharedPtr<i6e::api::SoundListenerComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getSoundListenerComponent)
			.def("getBillboardComponent", (i6e::utils::sharedPtr<i6e::api::BillboardComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getBillboardComponent)
			.def("getBillboardComponent", (i6e::utils::sharedPtr<i6e::api::BillboardComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getBillboardComponent)
			.def("getFollowComponent", (i6e::utils::sharedPtr<i6e::api::FollowComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getFollowComponent)
			.def("getFollowComponent", (i6e::utils::sharedPtr<i6e::api::FollowComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getFollowComponent)
			.def("getMovableTextComponent", (i6e::utils::sharedPtr<i6e::api::MovableTextComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getMovableTextComponent)
			.def("getMovableTextComponent", (i6e::utils::sharedPtr<i6e::api::MovableTextComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getMovableTextComponent)
			.def("getWaypointComponent", (i6e::utils::sharedPtr<i6e::api::WaypointComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getWaypointComponent)
			.def("getWaypointComponent", (i6e::utils::sharedPtr<i6e::api::WaypointComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getWaypointComponent)
			.def("getNavigationComponent", (i6e::utils::sharedPtr<i6e::api::NavigationComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getNavigationComponent)
			.def("getNavigationComponent", (i6e::utils::sharedPtr<i6e::api::NavigationComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getNavigationComponent)
			.def("getWaynetNavigationComponent", (i6e::utils::sharedPtr<i6e::api::WaynetNavigationComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getWaynetNavigationComponent)
			.def("getWaynetNavigationComponent", (i6e::utils::sharedPtr<i6e::api::WaynetNavigationComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getWaynetNavigationComponent)
			.def("getMoveComponent", (i6e::utils::sharedPtr<i6e::api::MoveComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getMoveComponent)
			.def("getMoveComponent", (i6e::utils::sharedPtr<i6e::api::MoveComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getMoveComponent)
			.def("getMovementComponent", (i6e::utils::sharedPtr<i6e::api::MovementComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getMovementComponent)
			.def("getMovementComponent", (i6e::utils::sharedPtr<i6e::api::MovementComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getMovementComponent)
			.def("getToggleWaynetComponent", (i6e::utils::sharedPtr<i6e::api::ToggleWaynetComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getToggleWaynetComponent)
			.def("getToggleWaynetComponent", (i6e::utils::sharedPtr<i6e::api::ToggleWaynetComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getToggleWaynetComponent)
			.def("getPoint2PointConstraintComponent", (i6e::utils::sharedPtr<i6e::api::Point2PointConstraintComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getPoint2PointConstraintComponent)
			.def("getPoint2PointConstraintComponent", (i6e::utils::sharedPtr<i6e::api::Point2PointConstraintComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getPoint2PointConstraintComponent)
			.def("getVelocityComponent", (i6e::utils::sharedPtr<i6e::api::VelocityComponent, i6e::api::Component>(*)(i6e::api::GameObject *)) &i6e::lua::object::getVelocityComponent)
			.def("getVelocityComponent", (i6e::utils::sharedPtr<i6e::api::VelocityComponent, i6e::api::Component>(*)(i6e::api::GameObject *, const std::string &)) &i6e::lua::object::getVelocityComponent),

		class_<i6e::api::AddStrategy>("AddStrategy")
			.def(constructor<>())
			.enum_("AddStrategy")
			[
				value("ADD", int(i6e::api::AddStrategy::ADD)),
				value("REPLACE", int(i6e::api::AddStrategy::REPLACE)),
				value("REPLACE_DIS", int(i6e::api::AddStrategy::REPLACE_DIS)),
				value("REJECT", int(i6e::api::AddStrategy::REJECT))
			],

		class_<i6e::api::Component, i6e::lua::object::ComponentWrapper, i6e::api::ComPtr>("Component")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("getOwnerGO", &i6e::api::Component::getOwnerGO)
			.def("getComponentID", &i6e::api::Component::getComponentID)
			.def("getFamilyID", &i6e::api::Component::getFamilyID)
			.def("getIdentifier", &i6e::api::Component::getIdentifier)
			.def("Tick", &i6e::api::Component::Tick, &i6e::lua::object::ComponentWrapper::default_Tick)
			.def("setDie", &i6e::api::Component::setDie)
			.def("getID", &i6e::api::Component::getID)
			.def("News", &i6e::api::Component::News, &i6e::lua::object::ComponentWrapper::default_News)
			.def("Init", &i6e::lua::object::ComponentWrapper::Init)
			.def("Finalize", &i6e::api::Component::Finalize, &i6e::lua::object::ComponentWrapper::default_Finalize)
			.def("synchronize", &i6e::lua::object::ComponentWrapper::synchronize)
			.def("setSync", &i6e::api::Component::setSync)
			.def("getSync", &i6e::api::Component::getSync)
			.def("howToAdd", &i6e::api::Component::howToAdd, &i6e::lua::object::ComponentWrapper::default_howToAdd)
			.def("getTemplateName", &i6e::lua::object::ComponentWrapper::getTemplateName)
			.def("addTicker", &i6e::lua::object::ComponentWrapper::addTicker)
			.def("removeTicker", &i6e::lua::object::ComponentWrapper::removeTicker),

		class_<i6e::api::AnimatedDirectionalLightComponent, i6e::utils::sharedPtr<i6e::api::AnimatedDirectionalLightComponent, i6e::api::Component>>("AnimatedDirectionalLightComponent")
			.def("synchronize", &i6e::api::AnimatedDirectionalLightComponent::synchronize)
			.def("getTemplateName", &i6e::api::AnimatedDirectionalLightComponent::getTemplateName),

		class_<i6e::api::AnimatedLuminousAppearanceComponent, i6e::utils::sharedPtr<i6e::api::AnimatedLuminousAppearanceComponent, i6e::api::Component>>("AnimatedLuminousAppearanceComponent")
			.def("synchronize", &i6e::api::LuminousAppearanceComponent::synchronize)
			.def("getTemplateName", &i6e::api::LuminousAppearanceComponent::getTemplateName),

		class_<i6e::api::AnimatedSpotLightComponent, i6e::utils::sharedPtr<i6e::api::AnimatedSpotLightComponent, i6e::api::Component>>("AnimatedSpotLightComponent")
			.def("synchronize", &i6e::api::AnimatedDirectionalLightComponent::synchronize)
			.def("getTemplateName", &i6e::api::AnimatedDirectionalLightComponent::getTemplateName),

		class_<i6e::api::AnimationControllerComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::AnimationControllerComponent, i6e::api::Component>>("AnimationControllerComponent")
			.def("addAnimationFrameEvent", (void(*)(i6e::api::AnimationControllerComponent*, const std::string &, uint64_t, const std::string &)) &i6e::lua::object::addAnimationFrameEvent)
			.def("addAnimationFrameEvent", (void(*)(i6e::api::AnimationControllerComponent*, const std::string &, uint64_t, const std::string &, const std::string &)) &i6e::lua::object::addAnimationFrameEvent)
			.def("playAnimation", &i6e::api::AnimationControllerComponent::playAnimation)
			.def("setAnimationSpeed", &i6e::api::AnimationControllerComponent::setAnimationSpeed)
			.def("stopAnimation", &i6e::api::AnimationControllerComponent::stopAnimation)
			.def("synchronize", &i6e::api::AnimationControllerComponent::synchronize)
			.def("getTemplateName", &i6e::api::AnimationControllerComponent::getTemplateName),

		class_<i6e::api::BillboardComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::BillboardComponent, i6e::api::Component>>("BillboardComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("createOrUpdateBillboard", &i6e::api::BillboardComponent::createOrUpdateBillboard)
			.def("deleteBillboard", &i6e::api::BillboardComponent::deleteBillboard)
			.def("synchronize", &i6e::api::BillboardComponent::synchronize)
			.def("getTemplateName", &i6e::api::BillboardComponent::getTemplateName),

		class_<i6e::api::CameraComponent, i6e::lua::object::CameraComponentWrapper, i6e::utils::sharedPtr<i6e::api::CameraComponent, i6e::api::Component>>("CameraComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("Tick", &i6e::api::Component::Tick, &i6e::lua::object::CameraComponentWrapper::default_Tick)
			.def("News", &i6e::api::Component::News, &i6e::lua::object::CameraComponentWrapper::default_News)
			.def("Init", &i6e::lua::object::CameraComponentWrapper::Init)
			.def("Finalize", &i6e::api::Component::Finalize, &i6e::lua::object::CameraComponentWrapper::default_Finalize)
			.def("synchronize", &i6e::lua::object::CameraComponentWrapper::synchronize)
			.def("howToAdd", &i6e::api::Component::howToAdd, &i6e::lua::object::CameraComponentWrapper::default_howToAdd)
			.def("getTemplateName", &i6e::lua::object::CameraComponentWrapper::getTemplateName)
			.def("addTicker", &i6e::lua::object::CameraComponentWrapper::addTicker)
			.def("removeTicker", &i6e::lua::object::CameraComponentWrapper::removeTicker)
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
			.def("enableCompositor", &i6e::api::CameraComponent::enableCompositor),

		class_<i6e::api::FollowComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::FollowComponent, i6e::api::Component>>("FollowComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::FollowComponent::synchronize)
			.def("getTemplateName", &i6e::api::FollowComponent::getTemplateName),

		class_<i6e::api::LifetimeComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::LifetimeComponent, i6e::api::Component>>("LifetimeComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::LifetimeComponent::synchronize)
			.def("getTemplateName", &i6e::api::LifetimeComponent::getTemplateName)
			.def("instantKill", &i6e::api::LifetimeComponent::instantKill),

		class_<i6e::api::LuminousAppearanceComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::LuminousAppearanceComponent, i6e::api::Component>>("LuminousAppearanceComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::LuminousAppearanceComponent::synchronize)
			.def("getTemplateName", &i6e::api::LuminousAppearanceComponent::getTemplateName)
			.enum_("LightType")
			[
				value("POINT", int(i6e::api::LuminousAppearanceComponent::LightType::POINT)),
				value("DIRECTIONAL", int(i6e::api::LuminousAppearanceComponent::LightType::DIRECTIONAL)),
				value("SPOT", int(i6e::api::LuminousAppearanceComponent::LightType::SPOT))
			]
			.def("setLightType", &i6e::api::LuminousAppearanceComponent::setLightType)
			.def("getLightType", &i6e::lua::object::getLightType)
			.def("setDiffuseColor", &i6e::api::LuminousAppearanceComponent::setDiffuseColor)
			.def("getDiffuseColor", &i6e::api::LuminousAppearanceComponent::getDiffuseColor)
			.def("setSpecularColor", &i6e::api::LuminousAppearanceComponent::setSpecularColor)
			.def("getSpecularColor", &i6e::api::LuminousAppearanceComponent::getSpecularColor)
			.def("setAttenuation", &i6e::api::LuminousAppearanceComponent::setAttenuation)
			.def("getAttenuation", &i6e::api::LuminousAppearanceComponent::getAttenuation)
			.def("setDirection", &i6e::api::LuminousAppearanceComponent::setDirection)
			.def("getDirection", &i6e::api::LuminousAppearanceComponent::getDirection),

		class_<i6e::api::MeshAppearanceComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::MeshAppearanceComponent, i6e::api::Component>>("MeshAppearanceComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
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
			.def("addAnimationFrameEvent", (void(*)(i6e::api::MeshAppearanceComponent*, uint64_t, const std::string &)) &i6e::lua::object::addAnimationFrameEvent)
			.def("addAnimationFrameEvent", (void(*)(i6e::api::MeshAppearanceComponent*, uint64_t, const std::string &, const std::string &)) &i6e::lua::object::addAnimationFrameEvent)
			.def("getBoneTransform", &i6e::api::MeshAppearanceComponent::getBoneTransform)
			.def("setShadowCasting", &i6e::api::MeshAppearanceComponent::setShadowCasting),

		class_<i6e::api::MovableTextComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::MovableTextComponent, i6e::api::Component>>("MovableTextComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::MovableTextComponent::synchronize)
			.def("getTemplateName", &i6e::api::MovableTextComponent::getTemplateName)
			.def("setText", &i6e::api::MovableTextComponent::setText),

		class_<i6e::api::MoveComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::MoveComponent, i6e::api::Component>>("MoveComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::MoveComponent::synchronize)
			.def("getTemplateName", &i6e::api::MoveComponent::getTemplateName)
			.def("navigate", (void(i6e::api::MoveComponent::*)(const Vec3 &)) &i6e::api::MoveComponent::navigate)
			.def("navigate", (void(i6e::api::MoveComponent::*)(const std::string &)) &i6e::api::MoveComponent::navigate),

		class_<i6e::api::MovementComponent, i6e::lua::object::MovementComponentWrapper, i6e::utils::sharedPtr<i6e::api::MovementComponent, i6e::api::Component>>("MovementComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("Tick", &i6e::api::MovementComponent::Tick, &i6e::lua::object::MovementComponentWrapper::default_Tick)
			.def("News", &i6e::api::MovementComponent::News, &i6e::lua::object::MovementComponentWrapper::default_News)
			.def("Init", &i6e::lua::object::MovementComponentWrapper::Init)
			.def("Finalize", &i6e::api::MovementComponent::Finalize, &i6e::lua::object::MovementComponentWrapper::default_Finalize)
			.def("synchronize", &i6e::lua::object::MovementComponentWrapper::synchronize)
			.def("howToAdd", &i6e::api::Component::howToAdd, &i6e::lua::object::MovementComponentWrapper::default_howToAdd)
			.def("getTemplateName", &i6e::lua::object::MovementComponentWrapper::getTemplateName)
			.def("forward", &i6e::lua::object::MovementComponentWrapper::forward)
			.def("backward", &i6e::lua::object::MovementComponentWrapper::backward)
			.def("left", &i6e::lua::object::MovementComponentWrapper::left)
			.def("right", &i6e::lua::object::MovementComponentWrapper::right)
			.def("stop", &i6e::lua::object::MovementComponentWrapper::stop),

		class_<i6e::api::MoverCircleComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::MoverCircleComponent, i6e::api::Component>>("MoverCircleComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::MoverCircleComponent::synchronize)
			.def("getTemplateName", &i6e::api::MoverCircleComponent::getTemplateName)
			.def("setPositioning", &i6e::api::MoverCircleComponent::setPositioning)
			.def("setCircleParameters", &i6e::api::MoverCircleComponent::setCircleParameters)
			.def("start", &i6e::api::MoverCircleComponent::start)
			.def("getCircleAxis", &i6e::api::MoverCircleComponent::getCircleAxis)
			.def("getCircleRadius", &i6e::api::MoverCircleComponent::getCircleRadius)
			.def("reset", &i6e::api::MoverCircleComponent::reset),

		class_<i6e::api::MoverComponent, i6e::lua::object::MoverComponentWrapper, i6e::utils::sharedPtr<i6e::api::MoverComponent, i6e::api::Component>>("MoverComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("Tick", &i6e::api::MoverComponent::Tick, &i6e::lua::object::MoverComponentWrapper::default_Tick)
			.def("News", &i6e::lua::object::MoverComponentWrapper::News)
			.def("Init", &i6e::lua::object::MoverComponentWrapper::Init)
			.def("Finalize", &i6e::api::MoverComponent::Finalize, &i6e::lua::object::MoverComponentWrapper::default_Finalize)
			.def("synchronize", &i6e::api::MoverComponent::synchronize, &i6e::lua::object::MoverComponentWrapper::synchronize)
			.def("howToAdd", &i6e::api::Component::howToAdd, &i6e::lua::object::MoverComponentWrapper::default_howToAdd)
			.def("getTemplateName", &i6e::lua::object::MoverComponentWrapper::getTemplateName)
			.def("start", &i6e::lua::object::MoverComponentWrapper::start)
			.def("reset", &i6e::lua::object::MoverComponentWrapper::reset)
			.enum_("Positioning")
			[
				value("POSITIONING_ABSOLUTE", int(i6e::api::MoverComponent::Positioning::POSITIONING_ABSOLUTE)),
				value("POSITIONING_RELATIVE", int(i6e::api::MoverComponent::Positioning::POSITIONING_RELATIVE))
			],

		class_<i6e::api::MoverInterpolateComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::MoverInterpolateComponent, i6e::api::Component>>("MoverInterpolateComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::MoverInterpolateComponent::synchronize)
			.def("getTemplateName", &i6e::api::MoverInterpolateComponent::getTemplateName)
			.def("addKeyFrame", &i6e::api::MoverInterpolateComponent::addKeyFrame)
			.def("removeKeyFrame", &i6e::api::MoverInterpolateComponent::removeKeyFrame)
			.def("getKeyframe", &i6e::api::MoverInterpolateComponent::getKeyframe)
			.def("setMode", &i6e::api::MoverInterpolateComponent::setMode)
			.def("setOpenTime", &i6e::api::MoverInterpolateComponent::setOpenTime)
			.def("setWay", &i6e::api::MoverInterpolateComponent::setWay)
			.def("getWay", &i6e::lua::object::getWay)
			.def("start", &i6e::api::MoverInterpolateComponent::start)
			.def("reset", &i6e::api::MoverInterpolateComponent::reset)
			.enum_("Mode")
			[
				value("TWOSTATE_TOGGLE", int(i6e::api::MoverInterpolateComponent::Mode::TWOSTATE_TOGGLE)),
				value("TWOSTATE_OPENTIME", int(i6e::api::MoverInterpolateComponent::Mode::TWOSTATE_OPENTIME)),
				value("NSTATE_LOOP", int(i6e::api::MoverInterpolateComponent::Mode::NSTATE_LOOP)),
				value("ONCE", int(i6e::api::MoverInterpolateComponent::Mode::ONCE))
			]
			.enum_("Way")
			[
				value("LINEAR", int(i6e::api::MoverInterpolateComponent::Way::LINEAR)),
				value("BEZIER", int(i6e::api::MoverInterpolateComponent::Way::BEZIER))
			],

		class_<i6e::api::MovingCameraComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::MovingCameraComponent, i6e::api::Component>>("MovingCameraComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("getTemplateName", &i6e::api::MoverCircleComponent::getTemplateName),

		class_<i6e::api::NavigationComponent, i6e::lua::object::NavigationComponentWrapper, i6e::utils::sharedPtr<i6e::api::NavigationComponent, i6e::api::Component>>("NavigationComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("Tick", &i6e::api::Component::Tick, &i6e::lua::object::NavigationComponentWrapper::default_Tick)
			.def("News", &i6e::api::Component::News, &i6e::lua::object::NavigationComponentWrapper::default_News)
			.def("Init", &i6e::lua::object::NavigationComponentWrapper::Init)
			.def("Finalize", &i6e::api::Component::Finalize, &i6e::lua::object::NavigationComponentWrapper::default_Finalize)
			.def("synchronize", &i6e::lua::object::NavigationComponentWrapper::synchronize)
			.def("howToAdd", &i6e::api::Component::howToAdd, &i6e::lua::object::NavigationComponentWrapper::default_howToAdd)
			.def("getTemplateName", &i6e::lua::object::NavigationComponentWrapper::getTemplateName)
			.def("getPathPos", (std::vector<Vec3>(i6e::lua::object::NavigationComponentWrapper::*)(const Vec3 &, const Vec3 &) const) &i6e::lua::object::NavigationComponentWrapper::getPath)
			.def("getPathWP", (std::vector<Vec3>(i6e::lua::object::NavigationComponentWrapper::*)(const Vec3 &, const std::string &) const) &i6e::lua::object::NavigationComponentWrapper::getPath),

#ifdef ISIXE_WITH_NETWORK
		class_<i6e::api::NetworkSenderComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::NetworkSenderComponent, i6e::api::Component>>("NetworkSenderComponent")
			.def(constructor<>())
			.def("getTemplateName", &i6e::api::NetworkSenderComponent::getTemplateName),
#endif

		class_<i6e::api::ParticleEmitterComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::ParticleEmitterComponent, i6e::api::Component>>("ParticleEmitterComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::ParticleEmitterComponent::synchronize)
			.def("getTemplateName", &i6e::api::ParticleEmitterComponent::getTemplateName)
			.def("setScale", &i6e::api::ParticleEmitterComponent::setScale),

		class_<i6e::api::ResponseType::ResponseType>("ResponseType")
			.def(constructor<>())
			.enum_("ResponseType")
			[
				value("NONE", i6e::api::ResponseType::ResponseType::NONE),
				value("STATIC", i6e::api::ResponseType::ResponseType::STATIC),
				value("GHOST", i6e::api::ResponseType::ResponseType::GHOST),
				value("TRIGGER", i6e::api::ResponseType::ResponseType::TRIGGER)
			],

		class_<i6e::api::ShatterInterest>("ShatterInterest")
			.def(constructor<>())
			.enum_("ShatterInterest")
			[
				value("NONE", i6e::api::ShatterInterest::NONE),
				value("START", i6e::api::ShatterInterest::START),
				value("END", i6e::api::ShatterInterest::END),
				value("ALWAYS", i6e::api::ShatterInterest::ALWAYS)
			],

		class_<i6e::api::PhysicalStateComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::PhysicalStateComponent, i6e::api::Component>>("PhysicalStateComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
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
			.def("addPosition", &i6e::api::PhysicalStateComponent::addPosition)
			.enum_("ShapeType")
			[
				value("PLANE", int(i6e::api::PhysicalStateComponent::ShapeType::PLANE)),
				value("BOX", int(i6e::api::PhysicalStateComponent::ShapeType::BOX)),
				value("SPHERE", int(i6e::api::PhysicalStateComponent::ShapeType::SPHERE)),
				value("FILE", int(i6e::api::PhysicalStateComponent::ShapeType::FILE))
			]
			.enum_("RayTestRepetition")
			[
				value("STOP", int(i6e::api::PhysicalStateComponent::RayTestRepetition::STOP)),
				value("ONCE", int(i6e::api::PhysicalStateComponent::RayTestRepetition::ONCE)),
				value("PERIODIC", int(i6e::api::PhysicalStateComponent::RayTestRepetition::PERIODIC))
			]
			.enum_("RayTestNotify")
			[
				value("ALWAYS", int(i6e::api::PhysicalStateComponent::RayTestNotify::ALWAYS)),
				value("CHANGE", int(i6e::api::PhysicalStateComponent::RayTestNotify::CHANGE)),
				value("FOUND", int(i6e::api::PhysicalStateComponent::RayTestNotify::FOUND)),
				value("NOTFOUND", int(i6e::api::PhysicalStateComponent::RayTestNotify::NOTFOUND)),
				value("OBJECTCHANGE", int(i6e::api::PhysicalStateComponent::RayTestNotify::OBJECTCHANGE))
			],

			class_<i6e::api::RayTestResult>("RayTestResult")
			.def(constructor<>())
			.def_readonly("objID", &i6e::api::RayTestResult::objID)
			.def_readonly("sourceID", &i6e::api::RayTestResult::sourceID)
			.def_readonly("collisionPoint", &i6e::api::RayTestResult::collisionPoint),

			class_<i6e::api::Point2PointConstraintComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::Point2PointConstraintComponent, i6e::api::Component>>("Point2PointConstraintComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::Point2PointConstraintComponent::synchronize)
			.def("getTemplateName", &i6e::api::Point2PointConstraintComponent::getTemplateName),

			class_<i6e::api::ShatterComponent, i6e::lua::object::ShatterComponentWrapper, i6e::utils::sharedPtr<i6e::api::ShatterComponent, i6e::api::Component>>("ShatterComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("Tick", &i6e::api::Component::Tick, &i6e::lua::object::ShatterComponentWrapper::default_Tick)
			.def("News", &i6e::api::ShatterComponent::News, &i6e::lua::object::ShatterComponentWrapper::default_News)
			.def("Init", &i6e::api::ShatterComponent::Init, &i6e::lua::object::ShatterComponentWrapper::Init)
			.def("Finalize", &i6e::api::Component::Finalize, &i6e::lua::object::ShatterComponentWrapper::default_Finalize)
			.def("synchronize", &i6e::lua::object::CameraComponentWrapper::synchronize)
			.def("howToAdd", &i6e::api::Component::howToAdd, &i6e::lua::object::ShatterComponentWrapper::default_howToAdd)
			.def("getTemplateName", &i6e::lua::object::ShatterComponentWrapper::getTemplateName)
			.def("shatter", &i6e::lua::object::ShatterComponentWrapper::shatter)
			.def("resetRespawn", &i6e::api::ShatterComponent::resetRespawn),

			class_<i6e::api::SoundComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::SoundComponent, i6e::api::Component>>("SoundComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::SoundComponent::synchronize)
			.def("getTemplateName", &i6e::api::SoundComponent::getTemplateName),

			class_<i6e::api::SoundListenerComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::SoundListenerComponent, i6e::api::Component>>("SoundListenerComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::SoundListenerComponent::synchronize)
			.def("getTemplateName", &i6e::api::SoundListenerComponent::getTemplateName),

			class_<i6e::api::SpawnpointComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::SpawnpointComponent, i6e::api::Component>>("SpawnpointComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::SpawnpointComponent::synchronize)
			.def("getTemplateName", &i6e::api::SpawnpointComponent::getTemplateName)
			.def("addSpawntype", &i6e::api::SpawnpointComponent::addSpawntype)
			.def("addSpawntypes", &i6e::api::SpawnpointComponent::addSpawntypes)
			.def("removeSpawntype", &i6e::api::SpawnpointComponent::removeSpawntype)
			.def("containsSpawntype", &i6e::api::SpawnpointComponent::containsSpawntype)
			.def("available", &i6e::api::SpawnpointComponent::available)
			.def("setState", &i6e::api::SpawnpointComponent::setState),

			class_<i6e::api::StaticStateComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component>>("StaticStateComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::StaticStateComponent::synchronize)
			.def("getTemplateName", &i6e::api::StaticStateComponent::getTemplateName)
			.def("setPosition", &i6e::api::StaticStateComponent::setPosition)
			.def("setRotation", &i6e::api::StaticStateComponent::setRotation)
			.def("setScale", &i6e::api::StaticStateComponent::setScale)
			.def("getPosition", &i6e::api::StaticStateComponent::getPosition)
			.def("getRotation", &i6e::api::StaticStateComponent::getRotation)
			.def("getScale", &i6e::api::StaticStateComponent::getScale),

			class_<i6e::api::TerrainAppearanceComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::TerrainAppearanceComponent, i6e::api::Component>>("TerrainAppearanceComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::TerrainAppearanceComponent::synchronize)
			.def("getTemplateName", &i6e::api::TerrainAppearanceComponent::getTemplateName)
			.def("getHeightmap", &i6e::api::TerrainAppearanceComponent::getHeightmap)
			.def("getSize", &i6e::api::TerrainAppearanceComponent::getSize),

			class_<i6e::api::ToggleWaynetComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::ToggleWaynetComponent, i6e::api::Component>>("ToggleWaynetComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::ToggleWaynetComponent::synchronize)
			.def("getTemplateName", &i6e::api::ToggleWaynetComponent::getTemplateName)
			.def("enable", &i6e::api::ToggleWaynetComponent::enable),

			class_<i6e::api::VelocityComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::VelocityComponent, i6e::api::Component>>("VelocityComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::VelocityComponent::synchronize)
			.def("getTemplateName", &i6e::api::VelocityComponent::getTemplateName)
			.def("accelerate", (void(*)(i6e::api::VelocityComponent *, const Vec3 &, i6e::api::VelocityComponent::MaxSpeedHandling, const std::string &)) &i6e::lua::object::accelerate)
			.def("accelerate", (void(*)(i6e::api::VelocityComponent *, const std::string &)) &i6e::lua::object::accelerate)
			.def("decelerate", (void(*)(i6e::api::VelocityComponent *, const Vec3 &, i6e::api::VelocityComponent::DecelerationHandling, const std::string &)) &i6e::lua::object::decelerate)
			.def("decelerate", (void(*)(i6e::api::VelocityComponent *, const std::string &)) &i6e::lua::object::decelerate)
			.def("setMaxSpeed", &i6e::api::VelocityComponent::setMaxSpeed)
			.def("setResistanceCoefficient", &i6e::api::VelocityComponent::setResistanceCoefficient)
			.def("setWindage", &i6e::api::VelocityComponent::setWindage)
			.enum_("MaxSpeedHandling")
			[
				value("KeepSpeed", int(i6e::api::VelocityComponent::MaxSpeedHandling::KeepSpeed)),
				value("StopAcceleration", int(i6e::api::VelocityComponent::MaxSpeedHandling::StopAcceleration))
			]
			.enum_("DecelerationHandling")
			[
				value("Backward", int(i6e::api::VelocityComponent::DecelerationHandling::Backward)),
				value("StopDeceleration", int(i6e::api::VelocityComponent::DecelerationHandling::StopDeceleration))
			],

		class_<i6e::api::WaynetNavigationComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::WaynetNavigationComponent, i6e::api::Component>>("WaynetNavigationComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::WaynetNavigationComponent::synchronize)
			.def("getTemplateName", &i6e::api::WaynetNavigationComponent::getTemplateName)
			.def("getPathPos", (std::vector<Vec3>(i6e::api::WaynetNavigationComponent::*)(const Vec3 &, const Vec3 &) const) &i6e::api::WaynetNavigationComponent::getPath)
			.def("getPathWP", (std::vector<Vec3>(i6e::api::WaynetNavigationComponent::*)(const Vec3 &, const std::string &) const) &i6e::api::WaynetNavigationComponent::getPath),

		class_<i6e::api::WaypointComponent, i6e::api::Component, i6e::utils::sharedPtr<i6e::api::WaypointComponent, i6e::api::Component>>("WaypointComponent")
			.def(constructor<int64_t, const i6e::api::attributeMap &>())
			.def("synchronize", &i6e::api::WaypointComponent::synchronize)
			.def("getTemplateName", &i6e::api::WaypointComponent::getTemplateName)
			.def("getName", &i6e::api::WaypointComponent::getName)
			.def("getConnections", &i6e::api::WaypointComponent::getConnections)
			.def("isConnected", &i6e::api::WaypointComponent::isConnected)
			.def("addConnection", &i6e::api::WaypointComponent::addConnection)
			.def("removeConnection", &i6e::api::WaypointComponent::removeConnection),

		def("getObject", &i6e::lua::object::getObject),
		def("getAllObjectsOfType", &i6e::lua::object::getAllObjectsOfType),
		def("getGOList", &i6e::lua::object::getGOList),
		def("deleteAllObjectsOfType", &i6e::lua::object::deleteAllObjectsOfType),
		def("createObject", &i6e::lua::object::createObject),
		def("cleanUpAll", &i6e::lua::object::cleanUpAll),
		def("loadLevel", (void(*)(const std::string &, const std::string &)) &i6e::lua::object::loadLevel),
		def("loadLevel", (void(*)(const std::string &, const std::string &, const std::string &)) &i6e::lua::object::loadLevel),
		def("loadLevelCallbackFunc", (void(*)(const std::string &, const std::string &, const std::string &)) &i6e::lua::object::loadLevelCallbackFunc),
		def("loadLevelCallbackScript", (void(*)(const std::string &, const std::string &, const std::string &, const std::string &)) &i6e::lua::object::loadLevelCallbackScript),
		def("loadLevelCallbackFunc", (void(*)(const std::string &, const std::string &, const std::string &, const std::string &)) &i6e::lua::object::loadLevelCallbackFunc),
		def("loadLevelCallbackScript", (void(*)(const std::string &, const std::string &, const std::string &, const std::string &, const std::string &)) &i6e::lua::object::loadLevelCallbackScript),
		def("getFrameTime", &i6e::lua::object::getFrameTime),
		def("registerCTemplate", &i6e::lua::object::registerCTemplate),
		def("createGO", &i6e::lua::object::createGO),
		def("createComponent", &i6e::lua::object::createComponent),
		def("createComponentCallback", (void(*)(int64_t, int64_t, const std::string &, const i6e::api::attributeMap &, const std::string &)) &i6e::lua::object::createComponentCallback),
		def("createComponentCallback", (void(*)(int64_t, int64_t, const std::string &, const i6e::api::attributeMap &, const std::string &, const std::string &)) &i6e::lua::object::createComponentCallback),
		def("resetObjectSubSystem", &i6e::lua::object::resetObjectSubSystem),
		def("pauseObject", &i6e::lua::object::pauseObject),
		def("unpauseObject", &i6e::lua::object::unpauseObject),
		def("rayTest", &i6e::lua::object::rayTest),

		class_<i6e::api::objects::GOTemplateComponent>("GOTemplateComponent")
			.def(constructor<const std::string &, const i6e::api::attributeMap &, const std::string &, bool, bool>())
			.def_readwrite("template", &i6e::api::objects::GOTemplateComponent::_template)
			.def_readwrite("id", &i6e::api::objects::GOTemplateComponent::_id)
			.def_readwrite("params", &i6e::api::objects::GOTemplateComponent::_params)
			.def_readwrite("owner", &i6e::api::objects::GOTemplateComponent::_owner)
			.def_readwrite("identifier", &i6e::api::objects::GOTemplateComponent::_identifier)
			.def_readwrite("deleted", &i6e::api::objects::GOTemplateComponent::_deleted),

		class_<i6e::api::objects::GOTemplate>("GOTemplate")
			.def(constructor<>())
			.def_readwrite("type", &i6e::api::objects::GOTemplate::_type)
			.def_readwrite("components", &i6e::api::objects::GOTemplate::_components),

		class_<i6e::api::attributeMap>("attributeMap")
			.def(constructor<>())
			.def("insert", &i6e::lua::object::insertPairInAttributeMap),

		class_<std::vector<i6e::api::objects::GOTemplateComponent>>("GOTemplateComponentVector")
			.def(constructor<>())
			.def("push_back", (void(std::vector<i6e::api::objects::GOTemplateComponent>::*)(const i6e::api::objects::GOTemplateComponent &)) &std::vector<i6e::api::objects::GOTemplateComponent>::push_back),

		class_<i6e::api::CollisionGroup>("CollisionGroup")
			.def(constructor<>())
			.def(constructor<uint32_t, uint32_t, uint32_t>())
			.def(constructor<const std::string &>())
			.def_readwrite("responseType", &i6e::api::CollisionGroup::responseType)
			.def_readwrite("crashType", &i6e::api::CollisionGroup::crashType)
			.def_readwrite("crashMask", &i6e::api::CollisionGroup::crashMask),

		class_<i6e::api::components::ComponentTypes>("ComponentTypes")
			.def(constructor<>())
			.enum_("Types")
			[
				value("CameraComponent", i6e::api::components::ComponentTypes::CameraComponent),
				value("LifetimeComponent", i6e::api::components::ComponentTypes::LifetimeComponent),
				value("LuminousAppearanceComponent", i6e::api::components::ComponentTypes::LuminousAppearanceComponent),
				value("MeshAppearanceComponent", i6e::api::components::ComponentTypes::MeshAppearanceComponent),
				value("MoverCircleComponent", i6e::api::components::ComponentTypes::MoverCircleComponent),
				value("MoverComponent", i6e::api::components::ComponentTypes::MoverComponent),
				value("MoverInterpolateComponent", i6e::api::components::ComponentTypes::MoverInterpolateComponent),
				value("MovingCameraComponent", i6e::api::components::ComponentTypes::MovingCameraComponent),
				value("NetworkSenderComponent", i6e::api::components::ComponentTypes::NetworkSenderComponent),
				value("ParticleEmitterComponent", i6e::api::components::ComponentTypes::ParticleEmitterComponent),
				value("PhysicalStateComponent", i6e::api::components::ComponentTypes::PhysicalStateComponent),
				value("ShatterComponent", i6e::api::components::ComponentTypes::ShatterComponent),
				value("SpawnpointComponent", i6e::api::components::ComponentTypes::SpawnpointComponent),
				value("StaticStateComponent", i6e::api::components::ComponentTypes::StaticStateComponent),
				value("TerrainAppearanceComponent", i6e::api::components::ComponentTypes::TerrainAppearanceComponent),
				value("SoundComponent", i6e::api::components::ComponentTypes::SoundComponent),
				value("SoundListenerComponent", i6e::api::components::ComponentTypes::SoundListenerComponent),
				value("BillboardComponent", i6e::api::components::ComponentTypes::BillboardComponent),
				value("FollowComponent", i6e::api::components::ComponentTypes::FollowComponent),
				value("MovableTextComponent", i6e::api::components::ComponentTypes::MovableTextComponent),
				value("WaypointComponent", i6e::api::components::ComponentTypes::WaypointComponent),
				value("NavigationComponent", i6e::api::components::ComponentTypes::NavigationComponent),
				value("WaynetNavigationComponent", i6e::api::components::ComponentTypes::WaynetNavigationComponent),
				value("MoveComponent", i6e::api::components::ComponentTypes::MoveComponent),
				value("MovementComponent", i6e::api::components::ComponentTypes::MovementComponent),
				value("ToggleWaynetComponent", i6e::api::components::ComponentTypes::ToggleWaynetComponent),
				value("Point2PointConstraintComponent", i6e::api::components::ComponentTypes::Point2PointConstraintComponent),
				value("VelocityComponent", i6e::api::components::ComponentTypes::VelocityComponent),
				value("ComponentTypesCount", i6e::api::components::ComponentTypes::ComponentTypesCount)
			]
		;
}
