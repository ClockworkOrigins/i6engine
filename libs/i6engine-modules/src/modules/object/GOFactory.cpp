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

#include "i6engine/modules/object/GOFactory.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"

#include "i6engine/api/components/Component.h"

#ifdef ISIXE_WITH_NETWORK
	#include "i6engine/api/components/NetworkSenderComponent.h"
#endif

#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/facades/PhysicsFacade.h"
#include "i6engine/api/manager/WaynetManager.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/modules/object/ComponentFactory.h"
#include "i6engine/modules/object/ObjectManager.h"

#include "boost/bind.hpp"
#include "boost/filesystem.hpp"

#include "tinyxml2.h"

namespace i6e {
namespace modules {

	GOFactory::GOFactory(ObjectManager * m, ComponentFactory * c) : _templateList(), _manager(m), _compFactory(c) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR

		std::string goTemplatePath;
		if (clockUtils::ClockError::SUCCESS != api::EngineController::GetSingletonPtr()->getIniParser().getValue("OBJECT", "GOTemplatePath", goTemplatePath)) {
			ISIXE_LOG_ERROR("Object", "An exception has occurred: value GOTemplatePath in section OBJECT not found!");
			return;
		}
		boost::filesystem::directory_iterator iter(goTemplatePath), dirEnd;
		while (iter != dirEnd) {
			if (boost::filesystem::is_regular_file(*iter)) {
				std::string file = iter->path().string();
				registerGOTemplate(file);
			}
			iter++;
		}

		api::EngineController::GetSingleton().getObjectFacade()->setGOTemplates(_templateList);
	}

	void GOFactory::registerGOTemplate(const std::string & file) {
		ASSERT_THREAD_SAFETY_FUNCTION
		tinyxml2::XMLDocument doc;

		if (doc.LoadFile(file.c_str())) {
			ISIXE_THROW_FAILURE("GOFactory", "Couldn't open template file (" << file << ")");
			return;
		}

		tinyxml2::XMLElement * root = doc.FirstChildElement("GOTemplate");

		if (root->Attribute("type") == nullptr) {
			ISIXE_THROW_FAILURE("GOFactory", "No type set for template " << file);
			return;
		}

		api::objects::GOTemplate tmp;
		tmp._type = root->Attribute("type");

		if (_templateList.find(tmp._type) != _templateList.end()) {
			ISIXE_THROW_FAILURE("GOFactory", "Template type " << tmp._type << " already registered! Second found in file " << file);
		}

		for (tinyxml2::XMLElement * component = root->FirstChildElement("Component"); component != nullptr; component = component->NextSiblingElement("Component")) {
			api::attributeMap params;

			if (component->Attribute("template") == nullptr) {
				ISIXE_THROW_FAILURE("GOFactory", "Component has no template in template " << file);
				return;
			}

			std::string identifier = "";

			if (component->Attribute("identifier") != nullptr) {
				identifier = component->Attribute("identifier");
			}

			for (tinyxml2::XMLElement * attribute = component->FirstChildElement("Attribute"); attribute != nullptr; attribute = attribute->NextSiblingElement("Attribute")) {
				if (attribute->Attribute("name") == nullptr) {
					ISIXE_THROW_FAILURE("GOFactory", "Attribute has no name in template " << file << " for component " << component->Attribute("template"));
					return;
				}

				params[attribute->Attribute("name")] = attribute->GetText();
			}

			bool owner = false;

			if (component->Attribute("owner") != nullptr) {
				owner = true;
			}

			tmp._components.push_back(api::objects::GOTemplateComponent(component->Attribute("template"), -1, params, owner, identifier, false));
		}

		_templateList[tmp._type] = tmp;
	}

	GOPtr GOFactory::createGO(const std::string & tmpl, const int64_t goid, const core::IPKey & owner, uint64_t uuid, const api::objects::GOTemplate & templ, const bool sender) {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (owner == api::EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
			if (_templateList.find(tmpl) == _templateList.end()) {
				ISIXE_THROW_FAILURE("GOFactory", "Template with name " << tmpl << " not found");
			}
			api::objects::GOTemplate defaultTmpl = _templateList[tmpl];

			for (size_t i = 0; i < templ._components.size(); i++) {
				if (templ._components[i]._deleted) {
					for (size_t j = 0; j < defaultTmpl._components.size(); j++) {
						if (templ._components[i]._template == defaultTmpl._components[j]._template && templ._components[i]._identifier == defaultTmpl._components[j]._identifier) {
							defaultTmpl._components.erase(defaultTmpl._components.begin() + int(j));
							break;
						}
					}
				} else {
					bool found = false;
					for (size_t j = 0; j < defaultTmpl._components.size(); j++) {
						if (templ._components[i]._template == defaultTmpl._components[j]._template && templ._components[i]._identifier == defaultTmpl._components[j]._identifier) {
							for (const std::pair<std::string, std::string> & p : templ._components[i]._params) {
								defaultTmpl._components[j]._params[p.first] = p.second;
							}
							found = true;
							break;
						}
					}
					if (!found) {
						defaultTmpl._components.push_back(templ._components[i]);
					}
				}
			}

			return createGO(goid, owner, uuid, tmpl, defaultTmpl, sender);
		} else {
			return createGO(goid, owner, uuid, tmpl, templ, sender);
		}
	}

	GOPtr GOFactory::createGO(const int64_t goid, const core::IPKey & owner, uint64_t uuid, const std::string & tpl, const api::objects::GOTemplate & templ, const bool sender) {
		ASSERT_THREAD_SAFETY_FUNCTION
		GOPtr go = utils::make_shared<api::GameObject, api::GameObject>(goid, owner, uuid, tpl, boost::bind(&ComponentFactory::createGOC, _compFactory, _1, _2, _3, _4));

		go->setSelf(go);
		go->setType(templ._type);

		for (api::objects::GOTemplateComponent t : templ._components) {
			if (!t._owner || owner == api::EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
				if (t._params.find("identifier") != t._params.end()) {
					ISIXE_THROW_API("GOFactory", "identifier found in Components params!");
				}

				t._params["identifier"] = t._identifier;

				api::ComPtr c = _compFactory->createGOC(t._id, t._template, t._params, go);
				c->setSync(!t._owner);

				if (c == nullptr) {
					ISIXE_THROW_FAILURE("GOFactory", "couldn't create Component!");
				}
			}
		}

		go->initializeComponents();

#ifdef ISIXE_WITH_NETWORK
		// add NetworkSenderComponent if sending object and Multiplayer
		if (sender && api::EngineController::GetSingletonPtr()->getType() != api::GameType::SINGLEPLAYER) {
			api::ComPtr nsc = utils::make_shared<api::NetworkSenderComponent, api::Component>();
			go->setGOC(nsc);
			nsc->setOwnerGO(go);
			nsc->Init();
		}
#endif

		_manager->insertObject(go);

		api::EngineController::GetSingletonPtr()->getObjectFacade()->notifyNewID(go->getID());
		api::EngineController::GetSingletonPtr()->getPhysicsFacade()->notifyNewID(go->getID());

		return go;
	}

	void GOFactory::loadLevel(const std::string & file, const std::string & flags, const std::function<void(uint16_t)> & callback) {
		ASSERT_THREAD_SAFETY_FUNCTION
		tinyxml2::XMLDocument doc;

		callback(50);

		if (doc.LoadFile(file.c_str())) {
			ISIXE_THROW_FAILURE("GOFactory", "Couldn't open levelfile: '" << file << "'");
		}

		std::string temp;
		std::string fFlags;
		bool ignore = false;
		bool send;

		std::vector<std::string> flagV;

		size_t n = 0;

		do {
			size_t n2 = flags.find("|", n);

			if (n2 == flags.npos) {
				flagV.push_back(flags.substr(n, flags.size() - n));
				break;
			} else {
				flagV.push_back(flags.substr(n, n2 - n));
				n = n2 + 1;
			}
		} while (true);

		tinyxml2::XMLElement * root = doc.FirstChildElement("Map");

		std::vector<std::tuple<std::string, api::objects::GOTemplate, bool, std::vector<std::string>>> objects;

		for (tinyxml2::XMLElement * section = root->FirstChildElement("Section"); section != nullptr; section = section->NextSiblingElement("Section")) {
			std::string tmp = section->Attribute("flags");

			std::vector<std::string> flagL;

			fFlags = tmp;

			n = 0;

			do {
				size_t n2 = fFlags.find("|", n);

				if (n2 == fFlags.npos) {
					flagL.push_back(fFlags.substr(n, fFlags.size() - n));
					break;
				} else {
					flagL.push_back(fFlags.substr(n, n2 - n));
					n = n2 + 1;
				}
			} while (true);

			ignore = true;

			for (size_t i = 0; i < flagV.size(); ++i) {
				for (size_t j = 0; j < flagL.size(); ++j) {
					if (flagV[i] == flagL[j]) {
						ignore = false;
						i = flagV.size();
						break;
					}
				}
			}

			if (ignore) {
				continue;
			}

			for (tinyxml2::XMLElement * object = section->FirstChildElement("GameObject"); object != nullptr; object = object->NextSiblingElement("GameObject")) {
				temp = object->Attribute("name");
				tmp = object->Attribute("send");

				if (tmp == "false") {
					send = false;
				} else {
					send = true;
				}

				api::objects::GOTemplate tmpl;

				for (tinyxml2::XMLElement * component = object->FirstChildElement("Component"); component != nullptr; component = component->NextSiblingElement("Component")) {
					api::attributeMap params;
					for (tinyxml2::XMLElement * attribute = component->FirstChildElement("Attribute"); attribute != nullptr; attribute = attribute->NextSiblingElement("Attribute")) {
						params[attribute->Attribute("name")] = attribute->GetText();
					}
					std::string identifier = "";

					if (component->Attribute("identifier") != nullptr) {
						identifier = component->Attribute("identifier");
					}

					bool deleted = false;

					if (component->Attribute("deleted") != nullptr) {
						if (std::string(component->Attribute("deleted")) == "true") {
							deleted = true;
						}
					}

					tmpl._components.push_back(api::objects::GOTemplateComponent(component->Attribute("template"), -1, params, component->Attribute("owner") != nullptr, identifier, deleted));
				}

				objects.push_back(std::make_tuple(temp, tmpl, send, flagL));
			}
		}

		uint32_t amount = uint32_t(objects.size());
		uint32_t counter = 0;
		core::IPKey owner = api::EngineController::GetSingletonPtr()->getNetworkFacade()->getIP();

		for (const std::tuple<std::string, api::objects::GOTemplate, bool, std::vector<std::string>> & t : objects) {
			auto go = createGO(std::get<0>(t), -1, owner, api::EngineController::GetSingleton().getUUID(), std::get<1>(t), std::get<2>(t));
			go->setFlags(std::get<3>(t));
			go->setSend(std::get<2>(t));
			callback(uint16_t(50 + (counter++ / double(amount)) * 50));
		}

		api::EngineController::GetSingleton().getWaynetManager()->createWaynet();
	}

} /* namespace modules */
} /* namespace i6e */
