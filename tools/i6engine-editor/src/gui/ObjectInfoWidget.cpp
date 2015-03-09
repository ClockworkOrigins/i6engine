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

#include "i6engine/editor/gui/ObjectInfoWidget.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"

#include "i6engine/editor/EditorMessageTypes.h"

#include "CEGUI/CEGUI.h"

namespace i6engine {
namespace editor {
namespace gui {

	ObjectInfoWidget::ObjectInfoWidget(const std::string & name, const std::string & style) : GUIWidget(name), _callbacks(), _count(1), _lastWindow(nullptr) {
		CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();

		_window = wmgr.createWindow("Editor/Listbox", name);
		_window->setRiseOnClickEnabled(false);

		CEGUI::PushButton * button = dynamic_cast<CEGUI::PushButton *>(wmgr.createWindow("Editor/Button"));
		button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f, 0.0f), CEGUI::UDim(0.0f, 0.0f)));
		button->setSize(CEGUI::USize(CEGUI::UDim(0.5f, 0.0f), CEGUI::UDim(0.05f, 0.0f)));
		button->setText("Apply Changes");

		button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ObjectInfoWidget::applyChanges, this));

		_window->addChild(button);
	}

	ObjectInfoWidget::~ObjectInfoWidget() {
	}

	void ObjectInfoWidget::update(uint16_t type, i6engine::api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiSetVisible) {
			bool vis = static_cast<i6engine::api::gui::GUI_Visibility *>(data)->visible;
			_window->setVisible(vis);
		} else if (type == api::gui::GuiSetPosition) {
			api::gui::GUI_Position * g = static_cast<api::gui::GUI_Position *>(data);
			setPosition(g->x, g->y);
		} else if (type == api::gui::GuiSetSize) {
			api::gui::GUI_Size * g = static_cast<api::gui::GUI_Size *>(data);
			setSize(g->width, g->height);
		} else if (type == api::gui::GuiSetText) {
			api::gui::GUI_Text * g = static_cast<api::gui::GUI_Text *>(data);
			dynamic_cast<CEGUI::Listbox *>(_window)->addItem(new CEGUI::ListboxTextItem(g->text));
			_lastWindow = _window;
		} else if (type == messages::GUIMessageTypes::AddComponent) {
			messages::GUI_AddComponent * g = static_cast<messages::GUI_AddComponent *>(data);
			addComponent(g->component, g->identifier);
		} else if (type == messages::GUIMessageTypes::AddComponentOption) {
			messages::GUI_AddComponentOption * g = static_cast<messages::GUI_AddComponentOption *>(data);
			addComponentOption(g->writeable, g->optionName, g->getter, g->setter);
		} else {
			ISIXE_THROW_API("GUI", "Don't know what to do with " << type);
		}
	}

	void ObjectInfoWidget::setPosition(double x, double y) {
		_window->setPosition(CEGUI::UVector2(CEGUI::UDim(float(x), 0.0f), CEGUI::UDim(float(y), 0.0f)));
	}

	void ObjectInfoWidget::setSize(double w, double h) {
		_window->setSize(CEGUI::USize(CEGUI::UDim(float(w), 0.0f), CEGUI::UDim(float(h), 0.0f)));
	}

	void ObjectInfoWidget::addComponent(const std::string & component, const std::string & identifier) {
		CEGUI::Listbox * comName = dynamic_cast<CEGUI::Listbox *>(CEGUI::WindowManager::getSingleton().createWindow("Editor/Listbox", component + "/" + identifier));

		comName->addItem(new CEGUI::ListboxTextItem(component + "/" + identifier));

		_window->addChild(comName);

		comName->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0.0f), CEGUI::UDim(0.045f * _count, 0.0f)));
		comName->setSize(CEGUI::USize(CEGUI::UDim(1.0f, 0.0f), CEGUI::UDim(0.05f, 0.0f)));

		_count++;

		_lastWindow = comName;
	}

	void ObjectInfoWidget::addComponentOption(bool writeable, const std::string & optionName, const boost::function<std::string(void)> & getter, const boost::function<bool(std::string)> & setter) {
		CEGUI::Listbox * optionLabel = dynamic_cast<CEGUI::Listbox *>(CEGUI::WindowManager::getSingleton().createWindow("Editor/Listbox", _lastWindow->getName() + "/" + optionName));

		optionLabel->addItem(new CEGUI::ListboxTextItem(optionName));

		_window->addChild(optionLabel);

		optionLabel->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0.0f), CEGUI::UDim(0.045f * _count, 0.0f)));
		optionLabel->setSize(CEGUI::USize(CEGUI::UDim(0.5f, 0.0f), CEGUI::UDim(0.05f, 0.0f)));

		CEGUI::Editbox * optionEdit = dynamic_cast<CEGUI::Editbox *>(CEGUI::WindowManager::getSingleton().createWindow("Editor/Editbox", _lastWindow->getName() + "/" + optionName + "/Edit"));

		optionEdit->setReadOnly(!writeable);
		optionEdit->setText(getter());

		_window->addChild(optionEdit);

		optionEdit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f, 0.0f), CEGUI::UDim(0.045f * _count, 0.0f)));
		optionEdit->setSize(CEGUI::USize(CEGUI::UDim(0.5f, 0.0f), CEGUI::UDim(0.05f, 0.0f)));

		_count++;

		_callbacks.insert(std::make_pair(optionEdit, std::make_tuple(writeable, getter, setter)));
	}

	bool ObjectInfoWidget::applyChanges(const CEGUI::EventArgs & e) {
		for (auto p : _callbacks) {
			std::get<SETTER>(p.second)(std::string(p.first->getText().c_str()));
		}
		for (auto p : _callbacks) {
			p.first->setText(std::get<GETTER>(p.second)());
		}
		return true;
	}

} /* namespace gui */
} /* namespace editor */
} /* namespace i6engine */
