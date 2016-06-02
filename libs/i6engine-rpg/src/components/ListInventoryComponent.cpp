/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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

namespace i6e {
namespace rpg {
namespace components {

	ListInventoryComponent::ListInventoryComponent(int64_t id, const api::attributeMap & params) : InventoryComponent(id, params), api::MessageSubscriberFacade(), _items(), _columns(5), _slotCount(0), _currentIndex(0), _widgets(), _itemTypeCount(), _maxSlot(), _active(true), _infoScreen(false) {
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
				item->synchronize(msgs, true);
				for (size_t i = 0; i < dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components.size(); i++) {
					if (dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components[i]._template == "MovableText") {
						dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components.erase(dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components.begin() + int(i));
						break;
					}
				}
				it->second.insert(std::make_pair(nc->getName(), std::make_tuple(msgs[0], 1, item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getImageset(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getImage(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getIdentifier(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getValue())));
			}
		} else {
			std::vector<api::GameMessage::Ptr> msgs;
			item->synchronize(msgs, true);
			for (size_t i = 0; i < dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components.size(); i++) {
				if (dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components[i]._template == "MovableText") {
					dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components.erase(dynamic_cast<api::objects::Object_Create_Create *>(msgs[0]->getContent())->tmpl._components.begin() + int(i));
					break;
				}
			}
			_items.insert(std::make_pair(item->getGOC(config::ComponentTypes::ItemComponent)->getComponentID(), std::map<std::string, std::tuple<api::GameMessage::Ptr, uint32_t, std::string, std::string, std::string, uint32_t>>({ std::make_pair(nc->getName(), std::make_tuple(msgs[0], 1, item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getImageset(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getImage(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getIdentifier(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getValue())) })));
		}
		for (auto & cb : _callbacks) {
			cb(item->getGOC(config::ComponentTypes::ItemComponent)->getComponentID(), nc->getName(), std::get<ItemEntry::Amount>(_items[item->getGOC(config::ComponentTypes::ItemComponent)->getComponentID()][nc->getName()]));
		}
		return true;
	}

	void ListInventoryComponent::show() {
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, ListInventoryComponent::News, this);
		_shown = true;
		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();

		api::graphics::Resolution res = api::EngineController::GetSingleton().getGraphicsFacade()->getCurrentResolution();

		double startPos = 0.6;

		if (!_isSelfInventory) {
			startPos = 0.05;
		}

		double width = 0.35 / _columns;
		double height = (width * res.width) / res.height;

		_slotCount = 0;
		for (uint32_t i = 0; 0.1 + height * (i / _columns + 1) < 0.6; i++) {
			std::string namePrefix = "Inventory_" + std::to_string(_id) + "_" + std::to_string(i) + "_";
			gf->addImage(namePrefix + "Back", "RPG/StaticImage", "RPG_Inventory_Back", "Back", startPos + width * (i % _columns), 0.1 + height * (i / _columns), width, height);
			_slotCount++;
		}
		if (_maxSlot == 0) {
			_maxSlot = _slotCount / _columns;
		}

		// info screen in the middle of the bottom
		gf->addImage("Inventory_InfoScreen_" + std::to_string(_id), "RPG/StaticImage", "RPG", "TbM_Filling", 0.25, 0.6, 0.5, 0.15);
		if (!_active) {
			gf->setVisibility("Inventory_InfoScreen_" + std::to_string(_id), false);
		}
		showItems();

		if (_currentIndex >= _itemTypeCount && _currentIndex != 0) {
			_currentIndex = _itemTypeCount - 1;
		}
		for (uint32_t i = 0; i < _slotCount; i++) {
			std::string namePrefix = "Inventory_" + std::to_string(_id) + "_" + std::to_string(i) + "_";
			gf->addImage(namePrefix + "Slot", "RPG/StaticImage", "RPG_Inventory_Slot", "Slot", startPos + width * (i % _columns), 0.1 + height * (i / _columns), width, height);
			if (i == _currentIndex && _active) {
				gf->setImage(namePrefix + "Slot", "RPG_Inventory_Highlighted", "Highlighted");
			}
		}
	}

	void ListInventoryComponent::hide() {
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::InputMessageType);
		_shown = false;
		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
		for (uint32_t i = 0; i < _slotCount; i++) {
			std::string namePrefix = "Inventory_" + std::to_string(_id) + "_" + std::to_string(i) + "_";
			gf->deleteWidget(namePrefix + "Back");
			gf->deleteWidget(namePrefix + "Slot");
		}
		gf->deleteWidget("Inventory_InfoScreen_" + std::to_string(_id));
		for (auto & s : _widgets) {
			gf->deleteWidget(s);
		}
		_widgets.clear();
		_infoScreen = false;
	}

	void ListInventoryComponent::showItems() {
		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();

		api::graphics::Resolution res = api::EngineController::GetSingleton().getGraphicsFacade()->getCurrentResolution();

		double width = 0.35 / _columns;
		double height = (width * res.width) / res.height;

		double startPos = 0.6;

		if (!_isSelfInventory) {
			startPos = 0.05;
		}
		
		uint32_t counter = 0;
		uint32_t placed = 0;

		for (auto & p : _items) {
			for (auto & p2 : p.second) {
				if (counter >= (_maxSlot - (_slotCount / _columns)) * _columns && counter < (_maxSlot - (_slotCount / _columns)) * _columns + _slotCount) {
					std::string namePrefix = "Inventory_" + std::to_string(_id) + "_" + std::to_string(counter) + "_";
					gf->addImage(namePrefix + "Image", "RPG/StaticImage", std::get<ItemEntry::Imageset>(p2.second), std::get<ItemEntry::Image>(p2.second), startPos + width * (placed % _columns), 0.1 + height * (placed / _columns), width, height);
					gf->addPrint(namePrefix + "Amount", "RPG/Blanko", startPos + width * (placed % _columns) + width / 2, 0.1 + height * (placed / _columns) + 0.7 * height, std::to_string(std::get<ItemEntry::Amount>(p2.second)), api::gui::Alignment::Center, -1);
					gf->setSize(namePrefix + "Amount", width, 0.05);
					gf->setColour(namePrefix + "Amount", 1.0, 1.0, 1.0, 1.0);
					if (std::get<ItemEntry::Amount>(p2.second) == 1) {
						gf->setVisibility(namePrefix + "Amount", false);
					}
					gf->subscribeEvent(namePrefix + "Image", "Clicked", std::bind(&ListInventoryComponent::useItem, this, p.first, p2.first, [this]() {
						hide();
						show();
					}));
					_widgets.push_back(namePrefix + "Image");
					_widgets.push_back(namePrefix + "Amount");
					placed++;
					if (counter == _currentIndex && _active && !_infoScreen) {
						gf->addPrint("Inventory_InfoScreen_Name_" + std::to_string(_id), "RPG/Blanko", 0.5, 0.82, api::EngineController::GetSingleton().getTextManager()->getText(p2.first), api::gui::Alignment::Center, -1);
						gf->setSize("Inventory_InfoScreen_Name_" + std::to_string(_id), 0.5, 0.05);
						_widgets.push_back("Inventory_InfoScreen_Name_" + std::to_string(_id));
						api::GameMessage::Ptr msg = std::get<ItemEntry::Message>(p2.second);
						api::objects::Object_Create_Create * occ = dynamic_cast<api::objects::Object_Create_Create *>(msg->getContent());
						api::EngineController::GetSingleton().getObjectFacade()->createGO(occ->tpl, occ->tmpl, api::EngineController::GetSingleton().getUUID(), occ->send, [this, gf](api::GOPtr go) {
							if (!_infoScreen) {
								_infoScreen = true;
								auto ic = go->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent);
								gf->addPrint("Inventory_InfoScreen_ValueText_" + std::to_string(_id), "RPG/Blanko", 0.26, 0.92, api::EngineController::GetSingleton().getTextManager()->getText("Value_Key"), api::gui::Alignment::Left, -1);
								gf->setSize("Inventory_InfoScreen_ValueText_" + std::to_string(_id), 0.24, 0.05);
								gf->addPrint("Inventory_InfoScreen_Value_" + std::to_string(_id), "RPG/Blanko", 0.5, 0.92, std::to_string(ic->getValue()), api::gui::Alignment::Right, -1);
								gf->setSize("Inventory_InfoScreen_Value_" + std::to_string(_id), 0.24, 0.05);
								_widgets.push_back("Inventory_InfoScreen_ValueText_" + std::to_string(_id));
								_widgets.push_back("Inventory_InfoScreen_Value_" + std::to_string(_id));
								if (ic->getComponentID() == config::ComponentTypes::UsableItemComponent) {
									auto attributeChanges = utils::dynamic_pointer_cast<UsableItemComponent>(ic)->getAttributeChanges();
									uint32_t counter2 = 0;
									for (auto & p3 : attributeChanges) {
										gf->addPrint("Inventory_InfoScreen_AttributeText_" + std::to_string(counter2) + "_" + std::to_string(_id), "RPG/Blanko", 0.26, 0.85 + 0.03 * counter2, api::EngineController::GetSingleton().getTextManager()->getText("Attribute_" + std::to_string(uint32_t(p3.first)) + "_Key"), api::gui::Alignment::Left, -1);
										gf->addPrint("Inventory_InfoScreen_AttributeValue_" + std::to_string(counter2) + "_" + std::to_string(_id), "RPG/Blanko", 0.5, 0.85 + 0.03 * counter2, std::to_string(p3.second), api::gui::Alignment::Right, -1);
										gf->setSize("Inventory_InfoScreen_AttributeValue_" + std::to_string(counter2) + "_" + std::to_string(_id), 0.24, 0.05);
										_widgets.push_back("Inventory_InfoScreen_AttributeText_" + std::to_string(counter2) + "_" + std::to_string(_id));
										_widgets.push_back("Inventory_InfoScreen_AttributeValue_" + std::to_string(counter2) + "_" + std::to_string(_id));
										counter2++;
									}
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
					std::string keyMapping = api::EngineController::GetSingleton().getInputFacade()->getKeyMapping(kc);
					if (keyMapping == "forward") {
						if (_currentIndex >= _columns && _active) {
							_currentIndex -= _columns;
							if (_currentIndex / _columns < _maxSlot - _slotCount / _columns) {
								_maxSlot--;
							}
							hide();
							show();
						}
					} else if (keyMapping == "backward") {
						if (_currentIndex + _columns < _itemTypeCount && _active) {
							_currentIndex += _columns;
							if (_currentIndex / _columns > _maxSlot) {
								_maxSlot++;
							}
							hide();
							show();
						}
					} else if (keyMapping == "left") {
						if (_active) {
							if (_trading && _isSelfInventory && _currentIndex % _columns == 0) {
								_active = false;
								auto inventory = utils::dynamic_pointer_cast<ListInventoryComponent>(_otherTradeInventory.get());
								inventory->_active = true;
								inventory->_currentIndex = 0;
								inventory->hide();
								inventory->show();
								hide();
								show();
							} else if (_currentIndex > 0) {
								_currentIndex--;
								if (_currentIndex / _columns < _maxSlot - _slotCount / _columns) {
									_maxSlot--;
								}
								hide();
								show();
							}
						}
					} else if (keyMapping == "right") {
						if (_active) {
							if (_trading && !_isSelfInventory && (_currentIndex % _columns == _columns - 1 || _currentIndex + 1 == _itemTypeCount)) {
								_active = false;
								auto inventory = utils::dynamic_pointer_cast<ListInventoryComponent>(_otherTradeInventory.get());
								inventory->_active = true;
								inventory->_currentIndex = 0;
								inventory->hide();
								inventory->show();
								hide();
								show();
							} else if (_currentIndex + 1 < _itemTypeCount) {
								_currentIndex++;
								if (_currentIndex / _columns > _maxSlot) {
									_maxSlot++;
								}
								hide();
								show();
							}
						}
					} else if (keyMapping == "action" && ks == api::KeyState::KEY_PRESSED) {
						// use item
						uint32_t counter = 0;
						if (!_active) {
							return;
						}
						for (auto & p : _items) {
							for (auto & p2 : p.second) {
								if (counter == _currentIndex) {
									if (_trading) {
										tradeItem(std::get<ItemEntry::Identifier>(p2.second), std::get<ItemEntry::Value>(p2.second));
									} else {
										useItem(p.first, p2.first, [this]() {
											hide();
											show();
										});
									}
									return;
								}
								counter++;
							}
						}
					} else if (kc == api::KeyCode::KC_ESCAPE && ks == api::KeyState::KEY_PRESSED) {
						hide();
						_trading = false;
						_multiplier = 1.0;
					}
				}
			} else if (msg->getSubtype() == api::mouse::MouseMessageTypes::MouWheel) {
				api::input::Input_MouseWheel_Update * imwu = dynamic_cast<api::input::Input_MouseWheel_Update *>(msg->getContent());
				for (int32_t i = 0; i < imwu->diff && _active && _currentIndex + _columns < _itemTypeCount; i++) {
					_currentIndex += _columns;
					if (_currentIndex / _columns > _maxSlot) {
						_maxSlot++;
					}
				}
				for (int32_t i = imwu->diff; i < 0 && _active && _currentIndex >= _columns; i++) {
					_currentIndex -= _columns;
					if (_currentIndex / _columns < _maxSlot - _slotCount / _columns) {
						_maxSlot--;
					}
				}
				hide();
				show();
			}
		}
	}

	void ListInventoryComponent::useItem(uint32_t item, const std::string & name, const std::function<void(void)> & callback) {
		api::GameMessage::Ptr msg = std::get<ItemEntry::Message>(_items[item][name]);
		api::objects::Object_Create_Create * occ = dynamic_cast<api::objects::Object_Create_Create *>(msg->getContent());
		api::EngineController::GetSingleton().getObjectFacade()->createGO(occ->tpl, occ->tmpl, api::EngineController::GetSingleton().getUUID(), occ->send, [this, callback](api::GOPtr go) {
			auto ic = go->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent);
			if (ic->use(getOwnerGO())) {
				std::string n = go->getGOC<NameComponent>(config::ComponentTypes::NameComponent)->getName();
				std::get<ItemEntry::Amount>(_items[ic->getComponentID()][n])--;
				bool erased = false;
				if (std::get<ItemEntry::Amount>(_items[ic->getComponentID()][n]) == 0) {
					_items[ic->getComponentID()].erase(n);
					erased = true;
				}
				for (auto & cb : _callbacks) {
					cb(ic->getComponentID(), n, (erased) ? 0 : std::get<ItemEntry::Amount>(_items[ic->getComponentID()][n]));
				}
				callback();
			}
			go->setDie();
		});
	}

	std::tuple<uint32_t, std::string, std::string, std::string> ListInventoryComponent::getSelectedItem() const {
		uint32_t counter = 0;
		for (auto & p : _items) {
			for (auto & p2 : p.second) {
				if (counter == _currentIndex) {
					return std::make_tuple(p.first, p2.first, std::get<ItemEntry::Imageset>(p2.second), std::get<ItemEntry::Image>(p2.second));
				}
				counter++;
			}
		}
		return std::make_tuple(UINT32_MAX, "", "", "");
	}

	uint32_t ListInventoryComponent::getItemCount(const std::string & identifier) const {
		for (auto it = _items.begin(); it != _items.end(); it++) {
			for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
				if (std::get<ItemEntry::Identifier>(it2->second) == identifier) {
					return std::get<ItemEntry::Amount>(it2->second);
				}
			}
		}
		return 0;
	}

	uint32_t ListInventoryComponent::getItemCount(uint32_t item, const std::string & name) const {
		auto it = _items.find(item);
		if (it != _items.end()) {
			auto it2 = it->second.find(name);
			if (it2 != it->second.end()) {
				return std::get<ItemEntry::Amount>(it2->second);
			}
		}
		return 0;
	}

	void ListInventoryComponent::removeItems(const std::string & identifier, uint32_t amount) {
		for (uint32_t i = 0; i < amount; i++) {
			for (auto it = _items.begin(); it != _items.end(); it++) {
				for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
					if (std::get<ItemEntry::Identifier>(it2->second) == identifier) {
						if (--std::get<ItemEntry::Amount>(it2->second) == 0) {
							it->second.erase(it2);
						}
						break;
					}
				}
			}
		}
	}

	void ListInventoryComponent::Tick() {
		processMessages();
	}

	void ListInventoryComponent::showTradeView(const utils::sharedPtr<InventoryComponent, api::Component> &) {
		_active = _isSelfInventory;
		show();
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */
