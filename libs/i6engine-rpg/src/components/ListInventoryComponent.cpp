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

#include "i6engine/rpg/components/ListInventoryComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/ItemComponent.h"
#include "i6engine/rpg/components/NameComponent.h"

namespace i6engine {
namespace rpg {
namespace components {

	ListInventoryComponent::ListInventoryComponent(int64_t id, const api::attributeMap & params) : InventoryComponent(id, params), api::MessageSubscriberFacade(), _columns(5), _slotCount(0), _currentIndex(0), _widgets(), _itemTypeCount(), _maxSlot() {
		_objComponentID = config::ComponentTypes::ListInventoryComponent;
	}

	api::ComPtr ListInventoryComponent::createC(int64_t id, const api::attributeMap & params) {
		return utils::make_shared<ListInventoryComponent, api::Component>(id, params);
	}

	void ListInventoryComponent::Init() {
		InventoryComponent::Init();
		addTicker();
	}

	void ListInventoryComponent::Finalize() {
		removeTicker();
	}

	api::attributeMap ListInventoryComponent::synchronize() {
		api::attributeMap params;
		return params;
	}

	bool ListInventoryComponent::addItem(const api::GOPtr & item) {
		auto nc = item->getGOC<NameComponent>(config::ComponentTypes::NameComponent);
		auto it = _items.find(item->getGOC(config::ComponentTypes::ItemComponent)->getComponentID());
		if (it != _items.end()) {
			auto it2 = it->second.find(nc->getName());
			if (it2 != it->second.end()) {
				it2->second.push_back(item);
			} else {
				it->second.insert(std::make_pair(nc->getName(), std::vector<api::GOPtr>({ item })));
			}
		} else {
			_items.insert(std::make_pair(item->getGOC(config::ComponentTypes::ItemComponent)->getComponentID(), std::map<std::string, std::vector<api::GOPtr>>({ std::make_pair(nc->getName(), std::vector<api::GOPtr>({ item })) })));
		}
		return true;
	}

	void ListInventoryComponent::show() {
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, ListInventoryComponent::News);
		_shown = true;
		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();

		api::graphics::Resolution res = api::EngineController::GetSingleton().getGraphicsFacade()->getCurrentResolution();

		double width = 0.35 / _columns;
		double height = (width * res.width) / res.height;

		_slotCount = 0;
		for (uint32_t i = 0; 0.1 + height * (i / _columns + 1) < 0.8; i++) {
			std::string namePrefix = "Inventory_" + std::to_string(_id) + "_" + std::to_string(i) + "_";
			gf->addImage(namePrefix + "Back", "RPG/StaticImage", "RPG_Inventory_Back", "Back", 0.6 + width * (i % _columns), 0.1 + height * (i / _columns), width, height);
			_slotCount++;
		}
		if (_maxSlot == 0) {
			_maxSlot = _slotCount / _columns;
		}
		showItems();
		if (_currentIndex >= _widgets.size() / 2) {
			_currentIndex = _widgets.size() / 2 - 1;
		}
		for (uint32_t i = 0; i < _slotCount; i++) {
			std::string namePrefix = "Inventory_" + std::to_string(_id) + "_" + std::to_string(i) + "_";
			gf->addImage(namePrefix + "Slot", "RPG/StaticImage", "RPG_Inventory_Slot", "Slot", 0.6 + width * (i % _columns), 0.1 + height * (i / _columns), width, height);
			if (i == _currentIndex) {
				gf->setImage(namePrefix + "Slot", "RPG_Inventory_Highlighted", "Highlighted");
			}
		}
	}

	void ListInventoryComponent::hide() {
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::InputMessageType);
		_shown = false;
		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();

		api::graphics::Resolution res = api::EngineController::GetSingleton().getGraphicsFacade()->getCurrentResolution();

		double width = 0.35 / _columns;
		double height = (width * res.width) / res.height;

		for (uint32_t i = 0; i < _slotCount; i++) {
			std::string namePrefix = "Inventory_" + std::to_string(_id) + "_" + std::to_string(i) + "_";
			gf->deleteWidget(namePrefix + "Back");
			gf->deleteWidget(namePrefix + "Slot");
		}
		for (auto & s : _widgets) {
			gf->deleteWidget(s);
		}
		_widgets.clear();
	}

	void ListInventoryComponent::showItems() {
		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();

		api::graphics::Resolution res = api::EngineController::GetSingleton().getGraphicsFacade()->getCurrentResolution();

		double width = 0.35 / _columns;
		double height = (width * res.width) / res.height;

		uint32_t counter = 0;
		for (auto & p : _items) {
			for (auto & p2 : p.second) {
				if (counter < _slotCount) {
					std::string namePrefix = "Inventory_" + std::to_string(_id) + "_" + std::to_string(counter) + "_";
					gf->addImage(namePrefix + "Image", "RPG/StaticImage", p2.second[0]->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getImageset(), p2.second[0]->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getImage(), 0.6 + width * (counter % _columns), 0.1 + height * (counter / _columns), width, height);
					gf->addPrint(namePrefix + "Amount", "RPG/Blanko", 0.6 + width * (counter % _columns) + width / 2, 0.1 + height * (counter / _columns) + 0.7 * height, std::to_string(p2.second.size()), api::gui::Alignment::Center, -1);
					gf->setSize(namePrefix + "Amount", width, 0.05);
					gf->setColour(namePrefix + "Amount", 1.0, 1.0, 1.0, 1.0);
					if (p2.second.size() == 1) {
						gf->setVisibility(namePrefix + "Amount", false);
					}
					_widgets.push_back(namePrefix + "Image");
					_widgets.push_back(namePrefix + "Amount");
				}
				counter++;
			}
		}
		_itemTypeCount = counter;
	}

	void ListInventoryComponent::News(const api::GameMessage::Ptr & msg) {
		if (!_shown) {
			return;
		}
		if (msg->getMessageType() == api::messages::InputMessageType) {
			if (msg->getSubtype() == api::keyboard::KeyboardMessageTypes::KeyKeyboard) {
				api::KeyCode kc = dynamic_cast<api::input::Input_Keyboard_Update *>(msg->getContent())->code;
				api::KeyState ks = dynamic_cast<api::input::Input_Keyboard_Update *>(msg->getContent())->pressed;

				if (ks == api::KeyState::KEY_PRESSED || ks == api::KeyState::KEY_HOLD) {
					if (kc == api::KeyCode::KC_W) {
						if (_currentIndex >= _columns) {
							_currentIndex -= _columns;
							if (_currentIndex / _columns < _maxSlot - _slotCount / _columns) {
								_maxSlot--;
							}
							hide();
							show();
						}
					} else if (kc == api::KeyCode::KC_S) {
						if (_currentIndex + _columns < _itemTypeCount) {
							_currentIndex += _columns;
							if (_currentIndex / _columns > _maxSlot) {
								_maxSlot++;
							}
							hide();
							show();
						}
					} else if (kc == api::KeyCode::KC_A) {
						if (_currentIndex > 0) {
							_currentIndex--;
							if (_currentIndex / _columns < _maxSlot - _slotCount / _columns) {
								_maxSlot--;
							}
							hide();
							show();
						}
					} else if (kc == api::KeyCode::KC_S) {
						if (_currentIndex + 1 < _itemTypeCount) {
							_currentIndex++;
							if (_currentIndex / _columns > _maxSlot) {
								_maxSlot++;
							}
							hide();
							show();
						}
					}
				}
			}
		}
	}

	void ListInventoryComponent::Tick() {
		processMessages();
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
