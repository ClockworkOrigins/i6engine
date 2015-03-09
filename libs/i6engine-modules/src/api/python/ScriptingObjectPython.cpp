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
#include "i6engine/api/objects/GameObject.h"

#include "boost/python.hpp"

boost::python::list getAllObjectsOfType(const std::string & types) {
	boost::python::list l;
	std::list<i6engine::api::GOPtr>::const_iterator it;
	std::list<i6engine::api::GOPtr> v = i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->getAllObjectsOfType(types);
	for (it = v.begin(); it != v.end(); ++it) {
		l.append(*it);
	}
	return l;
}

i6engine::api::GOPtr getObject(const int64_t id) {
	return i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(id);
}

i6engine::api::GameObject * getObjectPtr(const i6engine::api::GOPtr & p) {
	return p.get();
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

BOOST_PYTHON_MODULE(ScriptingObjectPython) {
	boost::python::class_<i6engine::api::GOPtr>("GOPtr")
		.def(boost::python::init<i6engine::api::GOPtr>());

	boost::python::class_<i6engine::api::ComPtr>("ComPtr")
		.def(boost::python::init<i6engine::api::ComPtr>());

	boost::python::class_<i6engine::api::Component, boost::noncopyable>("Component", boost::python::no_init)
		.def("setDie", &i6engine::api::Component::setDie);

	boost::python::class_<i6engine::api::MeshAppearanceComponent, boost::noncopyable, boost::python::bases<i6engine::api::Component> >("MeshAppearanceComponent", boost::python::no_init)
		.def("getVisibility", &i6engine::api::MeshAppearanceComponent::getVisibility);

	boost::python::class_<i6engine::api::PhysicalStateComponent, boost::noncopyable, boost::python::bases<i6engine::api::Component> >("PhysicalStateComponent", boost::python::no_init)
			.def("getPosition", &i6engine::api::PhysicalStateComponent::getPosition)
			.def("setPosition", &i6engine::api::PhysicalStateComponent::setPosition)
			.def("getRotation", &i6engine::api::PhysicalStateComponent::getRotation)
			.def("setRotation", &i6engine::api::PhysicalStateComponent::setRotation)
			.def("applyRotation", &i6engine::api::PhysicalStateComponent::applyRotation)
			.def("applyCentralForce", &i6engine::api::PhysicalStateComponent::applyCentralForce)
			.def("applyForce", &i6engine::api::PhysicalStateComponent::applyForce)
			.def("getLinearVelocity", &i6engine::api::PhysicalStateComponent::getLinearVelocity)
			.def("reset", &i6engine::api::PhysicalStateComponent::reset);

	boost::python::class_<i6engine::api::RayTestResult>("RayTestResult")
			.def_readonly("objID", &i6engine::api::RayTestResult::objID)
			.def_readonly("sourceID", &i6engine::api::RayTestResult::sourceID);

	boost::python::enum_<i6engine::api::PhysicalStateComponent::RayTestRepetition>("RayTestRepetition")
			.value("STOP", i6engine::api::PhysicalStateComponent::RayTestRepetition::STOP)
			.value("ONCE", i6engine::api::PhysicalStateComponent::RayTestRepetition::ONCE)
			.value("PERIODIC", i6engine::api::PhysicalStateComponent::RayTestRepetition::PERIODIC)
			.export_values();

	boost::python::enum_<i6engine::api::PhysicalStateComponent::RayTestNotify>("RayTestNotify")
			.value("ALWAYS", i6engine::api::PhysicalStateComponent::RayTestNotify::ALWAYS)
			.value("CHANGE", i6engine::api::PhysicalStateComponent::RayTestNotify::CHANGE)
			.value("FOUND", i6engine::api::PhysicalStateComponent::RayTestNotify::FOUND)
			.value("NOTFOUND", i6engine::api::PhysicalStateComponent::RayTestNotify::NOTFOUND)
			.value("OBJECTCHANGE", i6engine::api::PhysicalStateComponent::RayTestNotify::OBJECTCHANGE)
			.export_values();

	boost::python::def("getMeshAppearanceComponent", &getMesh, boost::python::return_internal_reference<>());
	boost::python::def("getPhysicalStateComponent", &getPSC, boost::python::return_internal_reference<>());

	i6engine::api::ComPtr(i6engine::api::GameObject::*getGOC1)(uint32_t) const = &i6engine::api::GameObject::getGOC;
	i6engine::api::ComPtr(i6engine::api::GameObject::*getGOC2)(uint32_t, const std::string &) const = &i6engine::api::GameObject::getGOC;

	boost::python::class_<i6engine::api::GameObject, boost::noncopyable>("GameObject", boost::python::no_init)
		.def("getGOC", getGOC1)
		.def("getGOC", getGOC2)
		.def("getType", &i6engine::api::GameObject::getType)
		.def("setDie", &i6engine::api::GameObject::setDie);

	boost::python::def("getObject", &getObject);
	boost::python::def("getAllObjectsOfType", &getAllObjectsOfType);
	boost::python::def("getObjectPtr", &getObjectPtr, boost::python::return_internal_reference<>());
	boost::python::def("rayTest", &rayTest);

	boost::python::enum_<i6engine::api::components::ComponentTypes>("ComponentTypes")
		.value("MeshAppearanceComponent", i6engine::api::components::ComponentTypes::MeshAppearanceComponent)
		.value("PhysicalStateComponent", i6engine::api::components::ComponentTypes::PhysicalStateComponent)
		.export_values();

	boost::python::class_<i6engine::api::CollisionGroup>("CollisionGroup")
		.def(boost::python::init<uint32_t, uint32_t, uint32_t>());
}
