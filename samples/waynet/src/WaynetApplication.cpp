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

#include "WaynetApplication.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/MoveComponent.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/components/ToggleWaynetComponent.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/facades/PhysicsFacade.h"
#include "i6engine/api/manager/WaynetManager.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/AttributeComponent.h"
#include "i6engine/rpg/components/DialogCheckerComponent.h"
#include "i6engine/rpg/components/ExperienceComponent.h"
#include "i6engine/rpg/components/HealthbarComponent.h"
#include "i6engine/rpg/components/HumanMovementComponent.h"
#include "i6engine/rpg/components/ListInventoryComponent.h"
#include "i6engine/rpg/components/MiscItemComponent.h"
#include "i6engine/rpg/components/NameComponent.h"
#include "i6engine/rpg/components/QuickslotComponent.h"
#include "i6engine/rpg/components/SlotComponent.h"
#include "i6engine/rpg/components/SlotInventoryComponent.h"
#include "i6engine/rpg/components/ThirdPersonControlComponent.h"
#include "i6engine/rpg/components/UsableItemComponent.h"
#include "i6engine/rpg/components/WeightInventoryComponent.h"

namespace sample {

	WaynetApplication::WaynetApplication() : CommonApplication(true, false), _iniParser() {
		_iniParser.load("RPG.ini");
	}

	WaynetApplication::~WaynetApplication() {
	}

	void WaynetApplication::Initialize() {
		ISIXE_REGISTERMESSAGETYPE(i6e::api::messages::InputMessageType, WaynetApplication::News, this);
	}

	void WaynetApplication::AfterInitialize() {
		CommonApplication::AfterInitialize();

		// sets gravity for the game... here like on earth
		std::string gravityString;
		_iniParser.getValue("PHYSIC", "gravity", gravityString);
		i6e::api::EngineController::GetSingletonPtr()->getPhysicsFacade()->setGravity(Vec3(gravityString));

		// ambient light for the scene
		std::string ambientLightString;
		_iniParser.getValue("GRAPHIC", "ambientLight", ambientLightString);
		auto vec = i6e::utils::split(ambientLightString, " ");
		i6e::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setAmbientLight(std::stod(vec[0]), std::stod(vec[1]), std::stod(vec[2]));

		// setting shadow technique... currently only additive stencil possible
		uint16_t shadowTechnique;
		_iniParser.getValue("GRAPHIC", "shadowTechnique", shadowTechnique);
		i6e::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setShadowTechnique(i6e::api::graphics::ShadowTechnique(shadowTechnique));

		// setting distance fog
		i6e::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setExponentialFog(Vec3(0.9, 0.9, 0.9), 0.005);

		// register rpg components we want to use
		// do this befor loading the level
		i6e::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Attribute", std::bind(&i6e::rpg::components::AttributeComponent::createC, std::placeholders::_1, std::placeholders::_2));
		i6e::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("DialogChecker", std::bind(&i6e::rpg::components::DialogCheckerComponent::createC, std::placeholders::_1, std::placeholders::_2));
		i6e::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Experience", std::bind(&i6e::rpg::components::ExperienceComponent::createC, std::placeholders::_1, std::placeholders::_2));
		i6e::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Healthbar", std::bind(&i6e::rpg::components::HealthbarComponent::createC, std::placeholders::_1, std::placeholders::_2));
		i6e::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("HumanMovement", std::bind(&i6e::rpg::components::HumanMovementComponent::createC, std::placeholders::_1, std::placeholders::_2));
		i6e::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("ListInventory", std::bind(&i6e::rpg::components::ListInventoryComponent::createC, std::placeholders::_1, std::placeholders::_2));
		i6e::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("MiscItem", std::bind(&i6e::rpg::components::MiscItemComponent::createC, std::placeholders::_1, std::placeholders::_2));
		i6e::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Name", std::bind(&i6e::rpg::components::NameComponent::createC, std::placeholders::_1, std::placeholders::_2));
		i6e::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Quickslot", std::bind(&i6e::rpg::components::QuickslotComponent::createC, std::placeholders::_1, std::placeholders::_2));
		i6e::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Slot", std::bind(&i6e::rpg::components::SlotComponent::createC, std::placeholders::_1, std::placeholders::_2));
		i6e::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("SlotInventory", std::bind(&i6e::rpg::components::SlotInventoryComponent::createC, std::placeholders::_1, std::placeholders::_2));
		i6e::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("ThirdPersonControl", std::bind(&i6e::rpg::components::ThirdPersonControlComponent::createC, std::placeholders::_1, std::placeholders::_2));
		i6e::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("UsableItem", std::bind(&i6e::rpg::components::UsableItemComponent::createC, std::placeholders::_1, std::placeholders::_2));
		i6e::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("WeightInventory", std::bind(&i6e::rpg::components::WeightInventoryComponent::createC, std::placeholders::_1, std::placeholders::_2));

		// loads the RPG demo level
		i6e::api::EngineController::GetSingletonPtr()->getObjectFacade()->loadLevel("../media/maps/SampleLevel.xml", "Singleplayer");

		i6e::api::ObjectFacade * of = i6e::api::EngineController::GetSingleton().getObjectFacade();
		{
			i6e::api::objects::GOTemplate tmpl;
			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("ToggleWaynet", i6e::api::attributeMap(), "", false, false));
			of->createGO("SpectatorCam", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false, [this](i6e::api::GOPtr go) {
				_camera = go;
				go->getGOC<i6e::api::ToggleWaynetComponent>(i6e::api::components::ComponentTypes::ToggleWaynetComponent)->enable(true);
			});
		}
		{
			i6e::api::objects::GOTemplate tmpl;
			of->createObject("Sun", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6e::api::objects::GOTemplate tmpl;
			i6e::api::attributeMap params;
			params.insert(std::make_pair("name", "NPCName01")); // size of the terrain in world units
			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Name", params, "", false, false));
			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("DialogChecker", i6e::api::attributeMap(), "", true, false));
			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Experience", i6e::api::attributeMap(), "", true, false));
			of->createObject("NPC", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
	}

	void WaynetApplication::News(const i6e::api::GameMessage::Ptr & msg) {
		if (msg->getMessageType() == i6e::api::messages::InputMessageType) {
			if (msg->getSubtype() == i6e::api::keyboard::KeyboardMessageTypes::KeyKeyboard) {
				i6e::api::input::Input_Keyboard_Update * iku = dynamic_cast<i6e::api::input::Input_Keyboard_Update *>(msg->getContent());
				if (iku->code == i6e::api::KeyCode::KC_MBLeft && iku->pressed == i6e::api::KeyState::KEY_PRESSED) {
					auto vec = i6e::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->getSelectables();
					if (vec.size() > 0) {
						auto go = i6e::api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("NPC").front();
						go->getGOC<i6e::api::MoveComponent>(i6e::api::components::ComponentTypes::MoveComponent)->navigate(vec.front().second);
					}
				} else {
					CommonApplication::InputMailbox(msg);
				}
			} else {
				CommonApplication::InputMailbox(msg);
			}
		}
	}

} /* namespace sample */
