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
#include "i6engine/api/components/MeshAppearanceComponent.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
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
		.def("getUUID", &i6engine::api::GameObject::getUUID);
	
	enum_<i6engine::api::AddStrategy>("AddStrategy")
		.value("ADD", i6engine::api::AddStrategy::ADD)
		.value("REPLACE", i6engine::api::AddStrategy::REPLACE)
		.value("REPLACE_DIS", i6engine::api::AddStrategy::REPLACE_DIS)
		.value("REJECT", i6engine::api::AddStrategy::REJECT)
		.export_values();

	class_<i6engine::api::Component, i6engine::python::object::ComponentWrapper, i6engine::api::ComPtr, boost::noncopyable>("Component", no_init)
		.def(init<int64_t, const i6engine::api::attributeMap &>())
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
		.def("howToAdd", &i6engine::api::Component::howToAdd, &i6engine::python::object::ComponentWrapper::default_howToAdd)
		.def("getTemplateName", pure_virtual(&i6engine::python::object::ComponentWrapper::getTemplateName))
		.def("addTicker", &i6engine::python::object::ComponentWrapper::addTicker)
		.def("removeTicker", &i6engine::python::object::ComponentWrapper::removeTicker);

	class_<i6engine::api::MeshAppearanceComponent, boost::noncopyable, bases<i6engine::api::Component>>("MeshAppearanceComponent", no_init)
		.def("getVisibility", &i6engine::api::MeshAppearanceComponent::getVisibility);

	class_<i6engine::api::PhysicalStateComponent, boost::noncopyable, bases<i6engine::api::Component>>("PhysicalStateComponent", no_init)
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

	def("getMeshAppearanceComponent", &i6engine::python::object::getMesh, return_internal_reference<>());
	def("getPhysicalStateComponent", &i6engine::python::object::getPSC, return_internal_reference<>());

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
