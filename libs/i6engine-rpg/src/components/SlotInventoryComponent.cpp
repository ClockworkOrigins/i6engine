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

#include "i6engine/rpg/components/SlotInventoryComponent.h"

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/i6eString.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/manager/IDManager.h"
#include "i6engine/api/manager/TextManager.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/NameComponent.h"
#include "i6engine/rpg/components/SlotComponent.h"
#include "i6engine/rpg/components/UsableItemComponent.h"

namespace i6engine {
namespace rpg {
namespace components {

	SlotInventoryComponent::SlotInventoryComponent(int64_t id, const api::attributeMap & params) : InventoryComponent(id, params), api::MessageSubscriberFacade(), _rows(), _columns(), _widgetList(), _slots(), _items(), _slotMarker(false), _currentIndex(UINT16_MAX) {
		_objComponentID = config::ComponentTypes::SlotInventoryComponent;
		_rows = uint16_t(std::stoul(params.find("rows")->second));
		_columns = uint16_t(std::stoul(params.find("columns")->second));
		_slots = std::vector<std::vector<uint16_t>>(_rows, std::vector<uint16_t>(_columns, UINT16_MAX));
	}

	api::ComPtr SlotInventoryComponent::createC(int64_t id, const api::attributeMap & params) {
		ISIXE_THROW_API_COND("SlotInventoryComponent", "rows not set!", params.find("rows") != params.end());
		ISIXE_THROW_API_COND("SlotInventoryComponent", "columns not set!", params.find("columns") != params.end());
		return utils::make_shared<SlotInventoryComponent, api::Component>(id, params);
	}

	void SlotInventoryComponent::Init() {
		InventoryComponent::Init();
		addTicker();
	}

	void SlotInventoryComponent::Finalize() {
		removeTicker();
	}

	api::attributeMap SlotInventoryComponent::synchronize() const {
		api::attributeMap params;
		params.insert(std::make_pair("rows", std::to_string(_rows)));
		params.insert(std::make_pair("columns", std::to_string(_columns)));
		return params;
	}

	bool SlotInventoryComponent::addItem(const api::GOPtr & item) {
		auto sc = item->getGOC<SlotComponent>(config::ComponentTypes::SlotComponent);
		// look for a place this item has enough place
		for (size_t i = 0; i < _rows; i++) {
			for (size_t j = 0; j < _columns; j++) {
				if (_slots[i][j] == UINT16_MAX) {
					uint16_t counter = 0;
					for (size_t k = i; k < _rows && k - i < sc->getHeight(); k++) {
						for (size_t l = j; l < _columns && l - j < sc->getWidth(); l++) {
							if (_slots[k][l] == UINT16_MAX) {
								counter++;
							}
						}
					}
					if (counter == sc->getHeight() * sc->getWidth()) {
						// enough place, so reserve those slots and store item
						for (size_t k = i; k < _rows && k - i < sc->getHeight(); k++) {
							for (size_t l = j; l < _columns && l - j < sc->getWidth(); l++) {
								_slots[k][l] = _items.size();
							}
						}
						std::vector<api::GameMessage::Ptr> msgs;
						item->synchronize(msgs);
						for (size_t i = 0; i < dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components.size(); i++) {
							if (dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components[i]._template == "MovableText") {
								dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components.erase(dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components.begin() + int(i));
								break;
							}
						}
						_items.push_back(std::make_tuple(item->getGOC(config::ComponentTypes::ItemComponent)->getComponentID(), item->getGOC<NameComponent>(config::ComponentTypes::NameComponent)->getName(), msgs[0], item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getImageset(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getImage(), sc->getWidth(), sc->getHeight()));
						for (auto & cb : _callbacks) {
							cb(item->getGOC(config::ComponentTypes::ItemComponent)->getComponentID(), item->getGOC<NameComponent>(config::ComponentTypes::NameComponent)->getName(), getItemCount(item->getGOC(config::ComponentTypes::ItemComponent)->getComponentID(), item->getGOC<NameComponent>(config::ComponentTypes::NameComponent)->getName()));
						}
						return true;
					}
				}
			}
		}
		return false;
	}

	void SlotInventoryComponent::show() {
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, SlotInventoryComponent::News, this);
		_shown = true;
		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
		api::graphics::Resolution res = api::EngineController::GetSingleton().getGraphicsFacade()->getCurrentResolution();
		double height = 0.75 / _rows;
		double width = (height * res.height) / res.width;

		for (uint16_t i = 0; i < _rows; i++) {
			for (uint16_t j = 0; j < _columns; j++) {
				gf->addImage("SlotInventory_BackgroundSlot_" + std::to_string(i) + "_" + std::to_string(j), "RPG/StaticImage", "RPG_Inventory_Slot", "Slot", 0.5 - width * 0.5 * _columns + j * width, 0.025 + i * height, width, height);
			}
		}

		for (size_t i = 0; i < _items.size(); i++) {
			for (uint16_t j = 0; j < _rows; j++) {
				bool found = false;
				for (uint16_t k = 0; k < _columns; k++) {
					if (_slots[j][k] == i) {
						found = true;
						gf->addImage("SlotInventory_Item_" + std::to_string(i), "RPG/StaticImage", std::get<ItemEntry::Imageset>(_items[i]), std::get<ItemEntry::Image>(_items[i]), 0.5 - width * 0.5 * _columns + k * width, 0.025 + j * height, width * std::get<ItemEntry::Width>(_items[i]), height * std::get<ItemEntry::Height>(_items[i]));
						_widgetList.push_back("SlotInventory_Item_" + std::to_string(i));
						break;
					}
				}
				if (found) {
					break;
				}
			}
		}
	}

	void SlotInventoryComponent::hide() {
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::InputMessageType);
		_shown = false;
		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
		for (uint16_t i = 0; i < _rows; i++) {
			for (uint16_t j = 0; j < _columns; j++) {
				gf->deleteWidget("SlotInventory_BackgroundSlot_" + std::to_string(i) + "_" + std::to_string(j));
			}
		}
		for (std::string s : _widgetList) {
			gf->deleteWidget(s);
		}
		_widgetList.clear();
		_slotMarker = false;
		_currentIndex = UINT16_MAX;
	}

	void SlotInventoryComponent::News(const api::GameMessage::Ptr & msg) {
		if (!_shown) {
			return;
		}
		if (msg->getMessageType() == api::messages::InputMessageType) {
			if (msg->getSubtype() == api::mouse::MouseMessageTypes::MouMouse) {
				int32_t x = dynamic_cast<api::input::Input_Mouse_Update *>(msg->getContent())->intNewX;
				int32_t y = dynamic_cast<api::input::Input_Mouse_Update *>(msg->getContent())->intNewY;

				api::graphics::Resolution res = api::EngineController::GetSingleton().getGraphicsFacade()->getCurrentResolution();
				double height = 0.75 / _rows;
				double width = (height * res.height) / res.width;

				int32_t j = int32_t((x / double(res.width) - (0.5 - width * 0.5 * _columns)) / width);
				int32_t i = int32_t((y / double(res.height) - 0.025) / height);

				if (i < 0 || i >= _rows || j < 0 || j >= _columns) {
					return;
				}

				if (_slots[i][j] != UINT16_MAX) {
					if (_currentIndex == _slots[i][j]) {
						return;
					}
					_currentIndex = _slots[i][j];
					for (uint16_t k = 0; k < _rows; k++) {
						bool found = false;
						for (uint16_t l = 0; l < _columns; l++) {
							if (_slots[k][l] == _slots[i][j]) {
								found = true;
								api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
								if (_slotMarker) {
									gf->setPosition("SlotInventory_ItemMarker", 0.5 - width * 0.5 * _columns + l * width, 0.025 + k * height);
									gf->setSize("SlotInventory_ItemMarker", width * std::get<ItemEntry::Width>(_items[_slots[i][j]]), height * std::get<ItemEntry::Height>(_items[_slots[i][j]]));
								} else {
									gf->addImage("SlotInventory_ItemMarker", "RPG/StaticImage", "RPG_Inventory_Highlighted", "Highlighted", 0.5 - width * 0.5 * _columns + l * width, 0.025 + k * height, width * std::get<ItemEntry::Width>(_items[_slots[i][j]]), height * std::get<ItemEntry::Height>(_items[_slots[i][j]]));
									_widgetList.push_back("SlotInventory_ItemMarker");
									_slotMarker = true;
								}
								break;
							}
						}
						if (found) {
							break;
						}
					}
				} else if (_slotMarker) {
					api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
					gf->deleteWidget("SlotInventory_ItemMarker");
					for (size_t k = 0; k < _widgetList.size(); k++) {
						if (_widgetList[k] == "SlotInventory_ItemMarker") {
							_widgetList.erase(_widgetList.begin() + int(k));
							break;
						}
					}
					_slotMarker = false;
					_currentIndex = UINT16_MAX;
				}
			} if (msg->getSubtype() == api::mouse::MouseMessageTypes::MouButton) {
				api::MouseButtonID mbi = dynamic_cast<api::input::Input_Button_Update *>(msg->getContent())->code;
				bool pressed = dynamic_cast<api::input::Input_Button_Update *>(msg->getContent())->pressed;

				if (pressed && mbi == api::MouseButtonID::MB_Right) {
					if (_currentIndex != UINT16_MAX) {
						useItem(std::get<ItemEntry::Type>(_items[_currentIndex]), std::get<ItemEntry::Name>(_items[_currentIndex]), [this]() {
							hide();
							show();
						}, _currentIndex);
					}
				}
			}
		}
	}

	void SlotInventoryComponent::useItem(uint32_t item, const std::string & name, const std::function<void(void)> & callback) {
		for (size_t i = 0; i < _items.size(); i++) {
			if (std::get<ItemEntry::Type>(_items[i]) == item && std::get<ItemEntry::Name>(_items[i]) == name) {
				useItem(item, name, callback, i);
				return;
			}
		}
	}

	void SlotInventoryComponent::useItem(uint32_t item, const std::string & name, const std::function<void(void)> & callback, uint16_t index) {
		api::GameMessage::Ptr msg = std::get<ItemEntry::Message>(_items[index]);
		api::objects::Object_Create_Create * occ = dynamic_cast<api::objects::Object_Create_Create *>(msg->getContent());
		api::EngineController::GetSingleton().getObjectFacade()->createGO(occ->tpl, occ->tmpl, api::EngineController::GetSingleton().getUUID(), occ->send, [this, callback, index](api::GOPtr go) {
			auto ic = go->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent);
			if (ic->use(getOwnerGO())) {
				std::string n = go->getGOC<NameComponent>(config::ComponentTypes::NameComponent)->getName();
				for (uint16_t i = 0; i < _rows; i++) {
					for (uint16_t j = 0; j < _columns; j++) {
						if (_slots[i][j] == index) {
							_slots[i][j] = UINT16_MAX;
						} else if (_slots[i][j] > index && _slots[i][j] != UINT16_MAX) {
							_slots[i][j]--;
						}
					}
				}
				_items.erase(_items.begin() + int(index));
				for (auto & cb : _callbacks) {
					cb(ic->getComponentID(), n, getItemCount(ic->getComponentID(), n));
				}
				callback();
			}
			go->setDie();
		});
	}

	std::tuple<uint32_t, std::string, std::string, std::string> SlotInventoryComponent::getSelectedItem() const {
		if (_currentIndex != UINT16_MAX) {
			return std::make_tuple(std::get<ItemEntry::Type>(_items[_currentIndex]), std::get<ItemEntry::Name>(_items[_currentIndex]), std::get<ItemEntry::Imageset>(_items[_currentIndex]), std::get<ItemEntry::Image>(_items[_currentIndex]));
		}
		return std::make_tuple(UINT32_MAX, "", "", "");
	}

	uint32_t SlotInventoryComponent::getItemCount(uint32_t item, const std::string & name) const {
		uint32_t counter = 0;
		for (auto & t : _items) {
			if (std::get<ItemEntry::Type>(t) == item && std::get<ItemEntry::Name>(t) == name) {
				counter++;
			}
		}
		return counter;
	}

	void SlotInventoryComponent::Tick() {
		processMessages();
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
