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

#include "i6engine/rpg/components/WeightInventoryComponent.h"

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
#include "i6engine/rpg/components/UsableItemComponent.h"

namespace i6e {
namespace rpg {
namespace components {

	WeightInventoryComponent::WeightInventoryComponent(int64_t id, const api::attributeMap & params) : InventoryComponent(id, params), api::MessageSubscriberFacade(), _items(), _maxWeight(), _currentWeight(0), _currentIndex(), _maxShowIndex(), _currentFilter("NONE"), _slotsPerView(), _widgetList(), _otherInventory(), _filter() {
		_objComponentID = config::ComponentTypes::WeightInventoryComponent;
		api::parseAttribute<true>(params, "maxWeight", _maxWeight);
		uint32_t filters = 0;
		api::parseAttribute<true>(params, "filters", filters);

		for (uint32_t i = 0; i < filters; i++) {
			ISIXE_THROW_API_COND("WeightInventoryComponent", "'filter_" + std::to_string(i) + "_type' not set!", params.find("filter_" + std::to_string(i) + "_type") != params.end());
			ISIXE_THROW_API_COND("WeightInventoryComponent", "'filter_" + std::to_string(i) + "_normalImage' not set!", params.find("filter_" + std::to_string(i) + "_normalImage") != params.end());
			ISIXE_THROW_API_COND("WeightInventoryComponent", "'filter_" + std::to_string(i) + "_hoverImage' not set!", params.find("filter_" + std::to_string(i) + "_hoverImage") != params.end());
			ISIXE_THROW_API_COND("WeightInventoryComponent", "'filter_" + std::to_string(i) + "_pushedImage' not set!", params.find("filter_" + std::to_string(i) + "_pushedImage") != params.end());
			_filter.push_back(std::make_tuple(params.find("filter_" + std::to_string(i) + "_type")->second, params.find("filter_" + std::to_string(i) + "_normalImage")->second, params.find("filter_" + std::to_string(i) + "_hoverImage")->second, params.find("filter_" + std::to_string(i) + "_pushedImage")->second));
		}
	}

	api::ComPtr WeightInventoryComponent::createC(int64_t id, const api::attributeMap & params) {
		return utils::make_shared<WeightInventoryComponent, api::Component>(id, params);
	}

	void WeightInventoryComponent::Init() {
		InventoryComponent::Init();
		addTicker();
	}

	void WeightInventoryComponent::Finalize() {
		removeTicker();
	}

	api::attributeMap WeightInventoryComponent::synchronize() const {
		api::attributeMap params;
		api::writeAttribute(params, "maxWeight", _maxWeight);
		api::writeAttribute(params, "filters", _filter.size());
		for (size_t i = 0; i < _filter.size(); i++) {
			api::writeAttribute(params, "filter_" + std::to_string(i) + "_type", std::get<FilterEntry::Type>(_filter[i]));
			api::writeAttribute(params, "filter_" + std::to_string(i) + "_normalImage", std::get<FilterEntry::NormalImage>(_filter[i]));
			api::writeAttribute(params, "filter_" + std::to_string(i) + "_hoverImage", std::get<FilterEntry::HoverImage>(_filter[i]));
			api::writeAttribute(params, "filter_" + std::to_string(i) + "_pushedImage", std::get<FilterEntry::PushedImage>(_filter[i]));
		}
		return params;
	}

	bool WeightInventoryComponent::addItem(const api::GOPtr & item) {
		if (std::abs(_maxWeight) > 1e-15 && item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getWeight() > _maxWeight - _currentWeight) { // if weight is limited AND new item would exceed maximum reject collection
			return false;
		}
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
				it->second.insert(std::make_pair(nc->getName(), std::make_tuple(msgs[0], 1, item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getImageset(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getImage(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getIdentifier(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getValue(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getWeight(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getTemplateName())));
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
			_items.insert(std::make_pair(item->getGOC(config::ComponentTypes::ItemComponent)->getComponentID(), std::map<std::string, std::tuple<api::GameMessage::Ptr, uint32_t, std::string, std::string, std::string, uint32_t, double, std::string>>({ std::make_pair(nc->getName(), std::make_tuple(msgs[0], 1, item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getImageset(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getImage(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getIdentifier(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getValue(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getWeight(), item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getTemplateName())) })));
		}
		for (auto & cb : _callbacks) {
			cb(item->getGOC(config::ComponentTypes::ItemComponent)->getComponentID(), nc->getName(), std::get<ItemEntry::Amount>(_items[item->getGOC(config::ComponentTypes::ItemComponent)->getComponentID()][nc->getName()]));
		}
		_currentWeight += item->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent)->getWeight();
		return true;
	}

	void WeightInventoryComponent::show() {
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, WeightInventoryComponent::News, this);
		_shown = true;
		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
		gf->addImage("WeightInventory_Background", "RPG/StaticImage", "RPG", "TbM_Filling", 0.0, 0.0, 1.0, 1.0);
		gf->addImageButton("WeightInventory_Category_All", "RPG/ImageButton", 0.05, 0.05, 0.1, 0.05, "RPG/Drag01", "RPG/Drag02", "RPG/Drag03");
		gf->subscribeEvent("WeightInventory_Category_All", "Clicked", [this]() {
			if (_currentFilter != "NONE") {
				_currentFilter = "NONE";
				_currentIndex = 0;
				_maxShowIndex = 0;
				hide();
				show();
			}
		});
		for (size_t i = 0; i < _filter.size(); i++) {
			std::string name = std::get<FilterEntry::Type>(_filter[i]);
			gf->addImageButton("WeightInventory_Category_" + name, "RPG/ImageButton", 0.15 + i * 0.1, 0.05, 0.1, 0.05, std::get<FilterEntry::NormalImage>(_filter[i]), std::get<FilterEntry::HoverImage>(_filter[i]), std::get<FilterEntry::PushedImage>(_filter[i]));
			gf->subscribeEvent("WeightInventory_Category_" + name, "Clicked", [this, name]() {
				if (_currentFilter != name) {
					_currentFilter = name;
					_currentIndex = 0;
					_maxShowIndex = 0;
					hide();
					show();
				}
			});
		}
		if (_trading) {
			if (!_isSelfInventory) {
				gf->addImageButton("WeightInventory_Sell", "RPG/ImageButton", 0.35, 0.05, 0.1, 0.05, "RPG/Drag01", "RPG/Drag02", "RPG/Drag03");
				gf->subscribeEvent("WeightInventory_Sell", "Clicked", [this]() {
					hide();
					_otherInventory.get()->show();
				});
			} else {
				gf->addImageButton("WeightInventory_Buy", "RPG/ImageButton", 0.35, 0.05, 0.1, 0.05, "RPG/Drag01", "RPG/Drag02", "RPG/Drag03");
				gf->subscribeEvent("WeightInventory_Buy", "Clicked", [this]() {
					hide();
					_otherInventory.get()->show();
				});
			}
		}
		if (_isSelfInventory) {
			gf->addPrint("WeightInventory_Weight", "RPG/Blanko", 0.8, 0.05, utils::to_string_with_precision(_currentWeight, 3) + " / " + utils::to_string_with_precision(_maxWeight, 3), api::gui::Alignment::Left, -1);
			if (std::abs(_maxWeight) < 1e-15) {
				gf->setVisibility("WeightInventory_Weight", false);
			}
		} else {
			gf->addPrint("WeightInventory_Weight", "RPG/Blanko", 0.8, 0.05, utils::to_string_with_precision(_otherInventory.get()->_currentWeight, 3) + " / " + utils::to_string_with_precision(_otherInventory.get()->_maxWeight, 3), api::gui::Alignment::Left, -1);
			if (std::abs(_otherInventory.get()->_maxWeight) < 1e-15) {
				gf->setVisibility("WeightInventory_Weight", false);
			}
		}
		// calculate list slots
		_slotsPerView = uint32_t(0.8 / 0.05);

		if (_maxShowIndex == 0) {
			_maxShowIndex = _slotsPerView;
		}

		uint32_t counter = 0;
		for (auto & p : _items) {
			for (auto & p2 : p.second) {
				if (_currentFilter == "NONE" || _currentFilter == std::get<ItemEntry::Template>(p2.second)) {
					if (counter >= _maxShowIndex - _slotsPerView && counter < _maxShowIndex) {
						// create view
						gf->addPrint("WeightInventory_Item_" + std::to_string(p.first) + "_" + p2.first, "RPG/Blanko", 0.05, 0.1 + 0.05 * (_maxShowIndex - _slotsPerView), api::EngineController::GetSingleton().getTextManager()->getText(p2.first) + ((std::get<ItemEntry::Amount>(p2.second) > 1) ? (" (" + std::to_string(std::get<ItemEntry::Amount>(p2.second)) + ")") : ""), api::gui::Alignment::Left, -1);
						gf->setSize("WeightInventory_Item_" + std::to_string(p.first) + "_" + p2.first, 0.3, 0.05);
						gf->setFont("WeightInventory_Item_" + std::to_string(p.first) + "_" + p2.first, "DejaVuSansBig");
						uint32_t first = p.first;
						std::string second = p2.first;
						gf->subscribeEvent("WeightInventory_Item_" + std::to_string(p.first) + "_" + p2.first, "Clicked", [this, counter, first, second]() {
							if (_currentIndex == counter) {
								useItem(first, second, [this]() {
									hide();
									show();
								});
							} else {
								_currentIndex = counter;
								hide();
								show();
							}
						});
						_widgetList.push_back("WeightInventory_Item_" + std::to_string(p.first) + "_" + p2.first);

						// current selection
						if (_currentIndex == counter) {
							api::graphics::Resolution res = api::EngineController::GetSingleton().getGraphicsFacade()->getCurrentResolution();
							double width = 0.1;
							double height = (width * res.width) / res.height;
							gf->addImage("WeightInventory_CurrentItem_Thumbnail", "RPG/StaticImage", std::get<ItemEntry::Imageset>(p2.second), std::get<ItemEntry::Image>(p2.second), 0.675 - width / 2.0, 0.4 - height - 0.02, width, height);
							gf->addPrint("WeightInventory_CurrentItem_Name", "RPG/Blanko", 0.675, 0.4, api::EngineController::GetSingleton().getTextManager()->getText(p2.first) + ((std::get<ItemEntry::Amount>(p2.second) > 1) ? (" (" + std::to_string(std::get<ItemEntry::Amount>(p2.second)) + ")") : ""), api::gui::Alignment::Center, -1);
							gf->setSize("WeightInventory_CurrentItem_Name", 0.55, 0.05);
							gf->setFont("WeightInventory_CurrentItem_Name", "DejaVuSansBig");
							gf->addImage("WeightInventory_CurrentItem_Frame", "RPG/StaticImage", "RPG", "TbM_Filling", 0.4, 0.5, 0.55, 0.3);
							_widgetList.push_back("WeightInventory_CurrentItem_Thumbnail");
							_widgetList.push_back("WeightInventory_CurrentItem_Name");
							_widgetList.push_back("WeightInventory_CurrentItem_Frame");
							api::GameMessage::Ptr msg = std::get<ItemEntry::Message>(p2.second);
							api::objects::Object_Create_Create * occ = dynamic_cast<api::objects::Object_Create_Create *>(msg->getContent());
							api::EngineController::GetSingleton().getObjectFacade()->createGO(occ->tpl, occ->tmpl, api::EngineController::GetSingleton().getUUID(), occ->send, [this, gf](api::GOPtr go) {
								auto ic = go->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent);
								gf->addPrint("WeightInventory_CurrentItem_WeightText", "RPG/Blanko", 0.41, 0.72, api::EngineController::GetSingleton().getTextManager()->getText("Weight_Key"), api::gui::Alignment::Left, -1);
								gf->setSize("WeightInventory_CurrentItem_WeightText", 0.24, 0.05);
								gf->addPrint("WeightInventory_CurrentItem_Weight", "RPG/Blanko", 0.675, 0.72, utils::to_string_with_precision(ic->getWeight(), 3), api::gui::Alignment::Right, -1);
								gf->setSize("WeightInventory_CurrentItem_Weight", 0.274, 0.05);
								gf->addPrint("WeightInventory_CurrentItem_ValueText", "RPG/Blanko", 0.41, 0.75, api::EngineController::GetSingleton().getTextManager()->getText("Value_Key"), api::gui::Alignment::Left, -1);
								gf->setSize("WeightInventory_CurrentItem_ValueText", 0.24, 0.05);
								gf->addPrint("WeightInventory_CurrentItem_Value", "RPG/Blanko", 0.675, 0.75, std::to_string(ic->getValue()), api::gui::Alignment::Right, -1);
								gf->setSize("WeightInventory_CurrentItem_Value", 0.274, 0.05);
								_widgetList.push_back("WeightInventory_CurrentItem_WeightText");
								_widgetList.push_back("WeightInventory_CurrentItem_Weight");
								_widgetList.push_back("WeightInventory_CurrentItem_ValueText");
								_widgetList.push_back("WeightInventory_CurrentItem_Value");
								if (ic->getComponentID() == config::ComponentTypes::UsableItemComponent) {
									auto attributeChanges = utils::dynamic_pointer_cast<UsableItemComponent>(ic)->getAttributeChanges();
									uint32_t counter2 = 0;
									for (auto & p3 : attributeChanges) {
										gf->addPrint("WeightInventory_CurrentItem_AttributeText_" + std::to_string(counter2), "RPG/Blanko", 0.41, 0.51 + 0.03 * counter2, api::EngineController::GetSingleton().getTextManager()->getText("Attribute_" + std::to_string(uint32_t(p3.first)) + "_Key"), api::gui::Alignment::Left, -1);
										gf->addPrint("WeightInventory_CurrentItem_AttributeValue_" + std::to_string(counter2), "RPG/Blanko", 0.675, 0.51 + 0.03 * counter2, std::to_string(p3.second), api::gui::Alignment::Right, -1);
										gf->setSize("WeightInventory_CurrentItem_AttributeValue_" + std::to_string(counter2), 0.274, 0.05);
										_widgetList.push_back("WeightInventory_CurrentItem_AttributeText_" + std::to_string(counter2));
										_widgetList.push_back("WeightInventory_CurrentItem_AttributeValue_" + std::to_string(counter2));
										counter2++;
									}
								}
								go->setDie();
							});
						}
					}
					counter++;
				}
			}
		}
	}

	void WeightInventoryComponent::hide() {
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::InputMessageType);
		_shown = false;
		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
		gf->deleteWidget("WeightInventory_Background");
		gf->deleteWidget("WeightInventory_Category_All");
		for (size_t i = 0; i < _filter.size(); i++) {
			std::string name = std::get<FilterEntry::Type>(_filter[i]);
			gf->deleteWidget("WeightInventory_Category_" + name);
		}
		gf->deleteWidget("WeightInventory_Weight");
		if (_trading) {
			if (_isSelfInventory) {
				gf->deleteWidget("WeightInventory_Buy");
			} else {
				gf->deleteWidget("WeightInventory_Sell");
			}
		}
		for (std::string s : _widgetList) {
			gf->deleteWidget(s);
		}
		_widgetList.clear();

		_currentIndex = 0;
		_maxShowIndex = 0;
	}

	void WeightInventoryComponent::News(const api::GameMessage::Ptr & msg) {
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
						if (_currentIndex > 0) {
							_currentIndex--;
							if (_maxShowIndex - _currentIndex >= _slotsPerView) {
								_maxShowIndex--;
							}
							hide();
							show();
						}
					} else if (keyMapping == "backward") {
						uint32_t maxIndex = 0;
						for (auto & p : _items) {
							for (auto & p2 : p.second) {
								if (_currentFilter == "NONE" || _currentFilter == std::get<ItemEntry::Template>(p2.second)) {
									maxIndex++;
								}
							}
						}
						if (_currentIndex < maxIndex - 1) {
							_currentIndex++;
							if (_currentIndex > _maxShowIndex) {
								_maxShowIndex++;
							}
							hide();
							show();
						}
					} else if (keyMapping == "action" && ks == api::KeyState::KEY_PRESSED) {
						uint32_t counter = 0;
						for (auto & p : _items) {
							for (auto & p2 : p.second) {
								if (_currentFilter == "NONE" || _currentFilter == std::get<ItemEntry::Template>(p2.second)) {
									if (counter == _currentIndex) {
										if (_trading) {
											tradeItem(std::get<ItemEntry::Identifier>(p2.second), std::get<ItemEntry::Value>(p2.second));
										} else {
											useItem(p.first, p2.first, [this]() {
												hide();
												show();
											});
										}
										break;
									}
									counter++;
								}
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
				uint32_t maxIndex = 0;
				for (auto & p : _items) {
					for (auto & p2 : p.second) {
						if (_currentFilter == "NONE" || _currentFilter == std::get<ItemEntry::Template>(p2.second)) {
							maxIndex++;
						}
					}
				}
				for (int32_t i = 0; i < imwu->diff && _currentIndex < maxIndex - 1; i++) {
					_currentIndex++;
					if (_currentIndex > _maxShowIndex) {
						_maxShowIndex++;
					}
				}
				for (int32_t i = imwu->diff; i < 0 && _currentIndex > 0; i++) {
					_currentIndex--;
					if (_maxShowIndex - _currentIndex >= _slotsPerView) {
						_maxShowIndex--;
					}
				}
				hide();
				show();
			}
		}
	}

	void WeightInventoryComponent::useItem(uint32_t item, const std::string & name, const std::function<void(void)> & callback) {
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
				_currentWeight -= ic->getWeight();
				for (auto & cb : _callbacks) {
					cb(ic->getComponentID(), n, (erased) ? 0 : std::get<ItemEntry::Amount>(_items[ic->getComponentID()][n]));
				}
				callback();
			}
			go->setDie();
		});
	}

	std::tuple<uint32_t, std::string, std::string, std::string> WeightInventoryComponent::getSelectedItem() const {
		uint32_t counter = 0;
		for (auto & p : _items) {
			for (auto & p2 : p.second) {
				if (_currentFilter == "NONE" || _currentFilter == std::get<ItemEntry::Template>(p2.second)) {
					if (counter == _currentIndex) {
						return std::make_tuple(p.first, p2.first, std::get<ItemEntry::Imageset>(p2.second), std::get<ItemEntry::Image>(p2.second));
					}
					counter++;
				}
			}
		}
		return std::make_tuple(UINT32_MAX, "", "", "");
	}

	uint32_t WeightInventoryComponent::getItemCount(const std::string & identifier) const {
		for (auto it = _items.begin(); it != _items.end(); it++) {
			for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
				if (std::get<ItemEntry::Identifier>(it2->second) == identifier) {
					return std::get<ItemEntry::Amount>(it2->second);
				}
			}
		}
		return 0;
	}

	uint32_t WeightInventoryComponent::getItemCount(uint32_t item, const std::string & name) const {
		auto it = _items.find(item);
		if (it != _items.end()) {
			auto it2 = it->second.find(name);
			if (it2 != it->second.end()) {
				return std::get<ItemEntry::Amount>(it2->second);
			}
		}
		return 0;
	}

	void WeightInventoryComponent::removeItems(const std::string & identifier, uint32_t amount) {
		for (uint32_t i = 0; i < amount; i++) {
			for (auto it = _items.begin(); it != _items.end(); it++) {
				for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
					if (std::get<ItemEntry::Identifier>(it2->second) == identifier) {
						if (--std::get<ItemEntry::Amount>(it2->second) == 0) {
							it->second.erase(it2);
							_currentWeight -= std::get<ItemEntry::Weight>(it2->second);
						}
						break;
					}
				}
			}
		}
	}

	void WeightInventoryComponent::Tick() {
		processMessages();
	}

	void WeightInventoryComponent::showTradeView(const utils::sharedPtr<InventoryComponent, api::Component> & otherInventory) {
		_otherInventory = utils::dynamic_pointer_cast<WeightInventoryComponent>(otherInventory);
		if (!_isSelfInventory) {
			show();
		}
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */
