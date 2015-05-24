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

	class ComponentTypes {
	public:
		enum Types {
			CameraComponent,
			LifetimeComponent,
			LuminousAppearanceComponent,
			MeshAppearanceComponent,
			MoverCircleComponent,
			MoverComponent,
			MoverInterpolateComponent,
			MovingCameraComponent,
			NetworkSenderComponent,
			ParticleEmitterComponent,
			PhysicalStateComponent,
			ShatterComponent,
			SpawnpointComponent,
			StaticStateComponent,
			TerrainAppearanceComponent,
			SoundComponent,
			SoundListenerComponent,
			BillboardComponent,
			FollowComponent,
			MovableTextComponent,
			WaypointComponent,
			NavigationComponent,
			WaynetNavigationComponent,
			MoveComponent,
			MovementComponent,
			ToggleWaynetComponent,
			Point2PointConstraintComponent,
			ComponentTypesCount
		};
	};

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

		class_<i6engine::api::Component, i6engine::api::ComPtr>("ComPtr")
			.def("setDie", &i6engine::api::Component::setDie),

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

		class_<i6engine::lua::object::ComponentTypes>("ComponentTypes")
			.def(constructor<>())
			.enum_("Types")
			[
				value("MeshAppearanceComponent", i6engine::lua::object::ComponentTypes::Types::MeshAppearanceComponent),
				value("PhysicalStateComponent", i6engine::lua::object::ComponentTypes::Types::PhysicalStateComponent)
			]
		;
}
