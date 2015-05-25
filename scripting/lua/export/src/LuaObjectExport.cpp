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
#include "i6engine/api/components/MeshAppearanceComponent.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
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

	i6engine::api::PhysicalStateComponent * getPSC(const i6engine::api::ComPtr & c) {
		return dynamic_cast<i6engine::api::PhysicalStateComponent *>(c.get());
	}

	i6engine::api::MeshAppearanceComponent * getMesh(const i6engine::api::ComPtr & c) {
		return dynamic_cast<i6engine::api::MeshAppearanceComponent *>(c.get());
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
			.def("getUUID", &i6engine::api::GameObject::getUUID),

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

		class_<i6engine::api::MeshAppearanceComponent, i6engine::api::ComPtr>("MeshAppearanceComponent")
			.def("getVisibility", &i6engine::api::MeshAppearanceComponent::getVisibility),

		class_<i6engine::api::PhysicalStateComponent, i6engine::api::ComPtr>("PhysicalStateComponent")
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

		def("getMeshAppearanceComponent", &i6engine::lua::object::getMesh),
		def("getPhysicalStateComponent", &i6engine::lua::object::getPSC),

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
