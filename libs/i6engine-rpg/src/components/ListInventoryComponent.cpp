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
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/manager/IDManager.h"
#include "i6engine/api/manager/TextManager.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/NameComponent.h"
#include "i6engine/rpg/components/UsableItemComponent.h"

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

	api::attributeMap ListInventoryComponent::synchronize() const {
		api::attributeMap params;
		return params;
	}

	bool ListInventoryComponent::addItem(const api::GOPtr & item) {
		auto nc = item->getGOC<NameComponent>(config::ComponentTypes::NameComponent);
		auto it = _items.find(item->getGOC(config::ComponentTypes::ItemComponent)->getComponentID());
		if (it != _items.end()) {
			auto it2 = it->second.find(nc->getName());
			if (it2 != it->second.end()) {
				std::get<ItemEntry::Amount>(it2->second)++;
			} else {
				std::vector<api::GameMessage::Ptr> msgs;
				item->synchronize(msgs);
				for (size_t i = 0; i < dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components.size(); i++) {
					if (dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components[i]._template == "MovableText") {
						dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components.erase(dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components.begin() + int(i));
						break;
					}
				}
				it->second.insert(std::make_pair(nc->getName(), std::make_tuple(msgs[0], 1, item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getImageset(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getImage())));
			}
		} else {
			std::vector<api::GameMessage::Ptr> msgs;
			item->synchronize(msgs);
			for (size_t i = 0; i < dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components.size(); i++) {
				if (dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components[i]._template == "MovableText") {
					dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components.erase(dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components.begin() + int(i));
					break;
				}
			}
			_items.insert(std::make_pair(item->getGOC(config::ComponentTypes::ItemComponent)->getComponentID(), std::map<std::string, std::tuple<api::GameMessage::Ptr, uint32_t, std::string, std::string>>({ std::make_pair(nc->getName(), std::make_tuple(msgs[0], 1, item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getImageset(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getImage())) })));
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

		// info screen in the middle of the bottom
		gf->addImage("Inventory_InfoScreen", "RPG/StaticImage", "RPG", "TbM_Filling", 0.25, 0.8, 0.5, 0.15);
		showItems();

		if (_currentIndex >= _itemTypeCount) {
			_currentIndex = _itemTypeCount - 1;
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
		gf->deleteWidget("Inventory_InfoScreen");
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
		uint32_t placed = 0;
		for (auto & p : _items) {
			for (auto & p2 : p.second) {
				if (counter >= (_maxSlot - (_slotCount / _columns)) * _columns && counter < (_maxSlot - (_slotCount / _columns)) * _columns + _slotCount) {
					std::string namePrefix = "Inventory_" + std::to_string(_id) + "_" + std::to_string(counter) + "_";
					gf->addImage(namePrefix + "Image", "RPG/StaticImage", std::get<ItemEntry::Imageset>(p2.second), std::get<ItemEntry::Image>(p2.second), 0.6 + width * (placed % _columns), 0.1 + height * (placed / _columns), width, height);
					gf->addPrint(namePrefix + "Amount", "RPG/Blanko", 0.6 + width * (placed % _columns) + width / 2, 0.1 + height * (placed / _columns) + 0.7 * height, std::to_string(std::get<ItemEntry::Amount>(p2.second)), api::gui::Alignment::Center, -1);
					gf->setSize(namePrefix + "Amount", width, 0.05);
					gf->setColour(namePrefix + "Amount", 1.0, 1.0, 1.0, 1.0);
					if (std::get<ItemEntry::Amount>(p2.second) == 1) {
						gf->setVisibility(namePrefix + "Amount", false);
					}
					_widgets.push_back(namePrefix + "Image");
					_widgets.push_back(namePrefix + "Amount");
					placed++;
					if (counter == _currentIndex) {
						gf->addPrint("Inventory_InfoScreen_Name", "RPG/Blanko", 0.5, 0.82, api::EngineController::GetSingleton().getTextManager()->getText(p2.first), api::gui::Alignment::Center, -1);
						gf->setSize("Inventory_InfoScreen_Name", 0.5, 0.05);
						_widgets.push_back("Inventory_InfoScreen_Name");
						api::GameMessage::Ptr msg = std::get<ItemEntry::Message>(p2.second);
						api::objects::Object_Create_Create * occ = dynamic_cast<api::objects::Object_Create_Create *>(msg->getContent());
						api::EngineController::GetSingleton().getObjectFacade()->createGO(occ->tpl, occ->tmpl, api::EngineController::GetSingleton().getUUID(), occ->send, [this, gf](api::GOPtr go) {
							auto ic = go->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent);
							gf->addPrint("Inventory_InfoScreen_ValueText", "RPG/Blanko", 0.26, 0.92, api::EngineController::GetSingleton().getTextManager()->getText("Value_Key"), api::gui::Alignment::Left, -1);
							gf->setSize("Inventory_InfoScreen_ValueText", 0.24, 0.05);
							gf->addPrint("Inventory_InfoScreen_Value", "RPG/Blanko", 0.5, 0.92, std::to_string(ic->getValue()), api::gui::Alignment::Right, -1);
							gf->setSize("Inventory_InfoScreen_Value", 0.24, 0.05);
							_widgets.push_back("Inventory_InfoScreen_ValueText");
							_widgets.push_back("Inventory_InfoScreen_Value");
							if (ic->getComponentID() == config::ComponentTypes::UsableItemComponent) {
								auto attributeChanges = utils::dynamic_pointer_cast<UsableItemComponent>(ic)->getAttributeChanges();
								uint32_t counter = 0;
								for (auto & p : attributeChanges) {
									gf->addPrint("Inventory_InfoScreen_AttributeText_" + std::to_string(counter), "RPG/Blanko", 0.26, 0.85 + 0.03 * counter, api::EngineController::GetSingleton().getTextManager()->getText("Attribute_" + std::to_string(uint32_t(p.first)) + "_Key"), api::gui::Alignment::Left, -1);
									gf->addPrint("Inventory_InfoScreen_AttributeValue_" + std::to_string(counter), "RPG/Blanko", 0.5, 0.85 + 0.03 * counter, std::to_string(p.second), api::gui::Alignment::Right, -1);
									_widgets.push_back("Inventory_InfoScreen_AttributeText_" + std::to_string(counter));
									_widgets.push_back("Inventory_InfoScreen_AttributeValue_" + std::to_string(counter));
									counter++;
								}
							}
							go->setDie();
						});
					}
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
					} else if (kc == api::KeyCode::KC_E && ks == api::KeyState::KEY_PRESSED) {
						// use item
						uint32_t counter = 0;
						for (auto & p : _items) {
							for (auto & p2 : p.second) {
								if (counter == _currentIndex) {
									api::GameMessage::Ptr msg = std::get<ItemEntry::Message>(p2.second);
									api::objects::Object_Create_Create * occ = dynamic_cast<api::objects::Object_Create_Create *>(msg->getContent());
									api::EngineController::GetSingleton().getObjectFacade()->createGO(occ->tpl, occ->tmpl, api::EngineController::GetSingleton().getUUID(), occ->send, [this](api::GOPtr go) {
										auto ic = go->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent);
										if (ic->use(getOwnerGO())) {
											std::string name = go->getGOC<NameComponent>(config::ComponentTypes::NameComponent)->getName();
											std::get<ItemEntry::Amount>(_items[ic->getComponentID()][name])--;
											if (std::get<ItemEntry::Amount>(_items[ic->getComponentID()][name]) == 0) {
												_items[ic->getComponentID()].erase(name);
											}
											hide();
											show();
										}
										go->setDie();
									});
									return;
								}
								counter++;
							}
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
