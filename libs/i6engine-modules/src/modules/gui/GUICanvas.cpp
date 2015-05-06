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

#include "i6engine/modules/gui/GUICanvas.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/math/i6eVector4.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/gui/GUIWidget.h"

#include "i6engine/modules/gui/GUIFactory.h"
#include "i6engine/modules/gui/GUIManager.h"

#include "CEGUI/Window.h"

#include "tinyxml2.h"

namespace i6engine {
namespace modules {

	void GUICanvas::load(const std::string & name, const std::string & file, GUIManager * manager, const GUIFactory & factory, std::map<std::string, api::GUIWidget *> & widgets) {
		tinyxml2::XMLDocument doc;

		if (doc.LoadFile(file.c_str())) {
			ISIXE_THROW_FAILURE("GUICanvas", "Couldn't open canvas file: '" << file << "'");
		}

		tinyxml2::XMLElement * root = doc.FirstChildElement("GUICanvas");

		for (tinyxml2::XMLElement * widget = root->FirstChildElement("Widget"); widget != nullptr; widget = widget->NextSiblingElement("Widget")) {
			if (!widget->Attribute("type")) {
				ISIXE_THROW_FAILURE("GUICanvas", "A Widget in file " << file << " has no type!");
			}
			std::string t = widget->Attribute("type");
			if (!widget->Attribute("style")) {
				ISIXE_THROW_FAILURE("GUICanvas", "A Widget in file " << file << " has no style!");
			}
			std::string s = widget->Attribute("style");
			if (!widget->Attribute("name")) {
				ISIXE_THROW_FAILURE("GUICanvas", "A Widget in file " << file << " has no name!");
			}
			std::string n = widget->Attribute("name");
			api::GUIWidget * w = factory.createGUIWidget(name + "/" + n, t, s);
			w->setMouseOverCallback(std::bind(&GUIManager::getMouseoverWidgets, manager));
			widgets[name + "/" + n] = w;
			manager->addToRootWindow(w->_window);

			load(name + "/" + n, file, manager, factory, widgets, widget);
		}
	}

	void GUICanvas::load(const std::string & name, const std::string & file, GUIManager * manager, const GUIFactory & factory, std::map<std::string, api::GUIWidget *> & widgets, tinyxml2::XMLElement * node) {
		api::GUIWidget * gw = widgets[name];
		for (tinyxml2::XMLElement * prop = node->FirstChildElement("Property"); prop != nullptr; prop = prop->NextSiblingElement("Property")) {
			if (!prop->Attribute("type") || !prop->GetText()) {
				ISIXE_THROW_FAILURE("GUICanvas", "Property of widget " << name << " has no type!");
			}
			std::string t = prop->Attribute("type");
			if (t == "position") {
				Vec2 pos(prop->GetText());
				api::EngineController::GetSingleton().getGUIFacade()->setPosition(name, pos.getX(), pos.getY());
			} else if (t == "size") {
				Vec2 size(prop->GetText());
				api::EngineController::GetSingleton().getGUIFacade()->setSize(name, size.getX(), size.getY());
			} else if (t == "image") {
				std::string image = prop->GetText();
				api::EngineController::GetSingleton().getGUIFacade()->setImage(name, utils::split(image, "/").front(), utils::split(image, "/").back());
			} else if (t == "text") {
				std::string text = prop->GetText();
				api::EngineController::GetSingleton().getGUIFacade()->setText(name, text);
			} else if (t == "visibility") {
				std::string visibility = prop->GetText();
				api::EngineController::GetSingleton().getGUIFacade()->setVisibility(name, bool(std::stoi(visibility)));
			} else if (t == "amount") {
				std::string amount = prop->GetText();
				api::EngineController::GetSingleton().getGUIFacade()->setAmount(name, std::stoul(amount));
			} else if (t == "colour") {
				Vec4 colour(prop->GetText());
				api::EngineController::GetSingleton().getGUIFacade()->setColour(name, colour.getX(), colour.getY(), colour.getZ(), colour.getW());
			} else if (t == "font") {
				std::string font = prop->GetText();
				api::EngineController::GetSingleton().getGUIFacade()->setFont(name, font);
			} else if (t == "lifetime") {
				std::string lifetime = prop->GetText();
				api::EngineController::GetSingleton().getGUIFacade()->setLifetime(name, std::stol(lifetime));
			} else if (t == "alignment") {
				std::string alignment = prop->GetText();
				api::gui::Alignment align;
				if (alignment == "left") {
					align = api::gui::Alignment::Left;
				} else if (alignment == "center") {
					align = api::gui::Alignment::Center;
				} else if (alignment == "right") {
					align = api::gui::Alignment::Right;
				} else {
					ISIXE_THROW_FAILURE("GUICanvas", "'" << alignment << "' is not a known alignment type. Used in canvas " << file << " unkown!");
				}

				api::EngineController::GetSingleton().getGUIFacade()->setAlignment(name, align);
			} else if (t == "progress") {
				std::string progress = prop->GetText();
				api::EngineController::GetSingleton().getGUIFacade()->setProgress(name, std::stod(progress));
			} else if (t == "property") {
				std::string propertyValue = prop->GetText();
				if (!prop->Attribute("name")) {
					ISIXE_THROW_FAILURE("GUICanvas", "Type 'property' requires attribute 'name' in canvas " << file);
				}
				std::string n = prop->Attribute("name");
				api::EngineController::GetSingleton().getGUIFacade()->setProperty(name, n, propertyValue);
			} else if (t == "tooltip") {
				std::string tooltip = prop->GetText();
				api::EngineController::GetSingleton().getGUIFacade()->setTooltip(name, tooltip);
			} else if (t == "animation") {
				std::string animation = prop->GetText();
				api::EngineController::GetSingleton().getGUIFacade()->playAnimation(name, animation);
			} else {
				ISIXE_THROW_FAILURE("GUICanvas", "Type '" << t << "' in canvas " << file << " unkown!");
			}
		}
		for (tinyxml2::XMLElement * widget = node->FirstChildElement("Widget"); widget != nullptr; widget = widget->NextSiblingElement("Widget")) {
			if (!widget->Attribute("type")) {
				ISIXE_THROW_FAILURE("GUICanvas", "A Widget in file " << file << " has no type!");
			}
			std::string t = widget->Attribute("type");
			if (!widget->Attribute("style")) {
				ISIXE_THROW_FAILURE("GUICanvas", "A Widget in file " << file << " has no style!");
			}
			std::string s = widget->Attribute("style");
			if (!widget->Attribute("name")) {
				ISIXE_THROW_FAILURE("GUICanvas", "A Widget in file " << file << " has no name!");
			}
			std::string n = widget->Attribute("name");
			api::GUIWidget * w = factory.createGUIWidget(name + "/" + n, t, s);
			w->setMouseOverCallback(std::bind(&GUIManager::getMouseoverWidgets, manager));
			widgets[name + "/" + n] = w;
			widgets[name]->_window->addChild(w->_window);
			widgets[name]->_childs.push_back(w);
			w->_parent = widgets[name];

			load(name + "/" + n, file, manager, factory, widgets, widget);
		}
	}

} /* namespace modules */
} /* namespace i6engine */
