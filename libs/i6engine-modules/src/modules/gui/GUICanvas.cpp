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

namespace i6e {
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
		for (tinyxml2::XMLElement * prop = node->FirstChildElement("Property"); prop != nullptr; prop = prop->NextSiblingElement("Property")) {
			if (!prop->Attribute("type") || !prop->GetText()) {
				ISIXE_THROW_FAILURE("GUICanvas", "Property of widget " << name << " has no type!");
			}
			std::string t = prop->Attribute("type");
			if (t == "position") {
				Vec2 pos(prop->GetText());
				auto data = new api::gui::GUI_Position(name, pos.getX(), pos.getY());
				widgets[name]->update(api::gui::GuiSetPosition, data);
				delete data;
			} else if (t == "size") {
				Vec2 size(prop->GetText());
				auto data = new api::gui::GUI_Size(name, size.getX(), size.getY());
				widgets[name]->update(api::gui::GuiSetSize, data);
				delete data;
			} else if (t == "image") {
				std::string image = prop->GetText();
				auto data = new api::gui::GUI_Image(name, utils::split(image, "/").front(), utils::split(image, "/").back());
				widgets[name]->update(api::gui::GuiSetImage, data);
				delete data;
			} else if (t == "text") {
				std::string text = prop->GetText();
				auto data = new api::gui::GUI_Text(name, text);
				widgets[name]->update(api::gui::GuiSetText, data);
				delete data;
			} else if (t == "visibility") {
				std::string visibility = prop->GetText();
				auto data = new api::gui::GUI_Visibility(name, std::stoi(visibility) != 0);
				widgets[name]->update(api::gui::GuiSetVisible, data);
				delete data;
			} else if (t == "amount") {
				std::string amount = prop->GetText();
				auto data = new api::gui::GUI_Amount(name, std::stoul(amount));
				widgets[name]->update(api::gui::GuiSetAmount, data);
				delete data;
			} else if (t == "colour") {
				Vec4 colour(prop->GetText());
				auto data = new api::gui::GUI_Colour(name, colour.getX(), colour.getY(), colour.getZ(), colour.getW());
				widgets[name]->update(api::gui::GuiSetColour, data);
				delete data;
			} else if (t == "font") {
				std::string font = prop->GetText();
				auto data = new api::gui::GUI_Text(name, font);
				widgets[name]->update(api::gui::GuiSetFont, data);
				delete data;
			} else if (t == "lifetime") {
				std::string lifetime = prop->GetText();
				auto data = new api::gui::GUI_Lifetime(name, std::stol(lifetime));
				widgets[name]->update(api::gui::GuiSetLifetime, data);
				delete data;
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
				auto data = new api::gui::GUI_Alignment(name, align);
				widgets[name]->update(api::gui::GuiSetAlignment, data);
				delete data;
			} else if (t == "progress") {
				std::string progress = prop->GetText();
				auto data = new api::gui::GUI_Progress(name, std::stod(progress));
				widgets[name]->update(api::gui::GuiSetProgress, data);
				delete data;
			} else if (t == "property") {
				std::string propertyValue = prop->GetText();
				if (!prop->Attribute("name")) {
					ISIXE_THROW_FAILURE("GUICanvas", "Type 'property' requires attribute 'name' in canvas " << file);
				}
				std::string n = prop->Attribute("name");
				auto data = new api::gui::GUI_SetProperty(name, n, propertyValue);
				widgets[name]->update(api::gui::GuiSetProperty, data);
				delete data;
			} else if (t == "tooltip") {
				std::string tooltip = prop->GetText();
				auto data = new api::gui::GUI_SetTooltip(name, tooltip);
				widgets[name]->update(api::gui::GuiSetTooltip, data);
				delete data;
			} else if (t == "animation") {
				std::string animation = prop->GetText();
				auto data = new api::gui::GUI_PlayAnimation(name, animation);
				widgets[name]->update(api::gui::GuiPlayAnimation, data);
				delete data;
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
} /* namespace i6e */
