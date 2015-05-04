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
#include "i6engine/rpg/components/HealthbarComponent.h"
#include "i6engine/rpg/components/HumanMovementComponent.h"
#include "i6engine/rpg/components/ListInventoryComponent.h"
#include "i6engine/rpg/components/NameComponent.h"
#include "i6engine/rpg/components/QuickslotComponent.h"
#include "i6engine/rpg/components/SlotComponent.h"
#include "i6engine/rpg/components/SlotInventoryComponent.h"
#include "i6engine/rpg/components/ThirdPersonControlComponent.h"
#include "i6engine/rpg/components/UsableItemComponent.h"
#include "i6engine/rpg/components/WeightInventoryComponent.h"

#include "boost/bind.hpp"

namespace sample {

	WaynetApplication::WaynetApplication() : CommonApplication(true, false), _iniParser() {
		_iniParser.load("RPG.ini");
	}

	WaynetApplication::~WaynetApplication() {
	}

	void WaynetApplication::Initialize() {
		ISIXE_REGISTERMESSAGETYPE(i6engine::api::messages::InputMessageType, WaynetApplication::News, this);
	}

	void WaynetApplication::AfterInitialize() {
		CommonApplication::AfterInitialize();

		i6engine::api::GUIFacade * gf = i6engine::api::EngineController::GetSingleton().getGUIFacade();

		// sets gravity for the game... here like on earth
		std::string gravityString;
		_iniParser.getValue("PHYSIC", "gravity", gravityString);
		i6engine::api::EngineController::GetSingletonPtr()->getPhysicsFacade()->setGravity(Vec3(gravityString));

		// ambient light for the scene
		std::string ambientLightString;
		_iniParser.getValue("GRAPHIC", "ambientLight", ambientLightString);
		auto vec = i6engine::utils::split(ambientLightString, " ");
		i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setAmbientLight(std::stod(vec[0]), std::stod(vec[1]), std::stod(vec[2]));

		// setting shadow technique... currently only additive stencil possible
		uint16_t shadowTechnique;
		_iniParser.getValue("GRAPHIC", "shadowTechnique", shadowTechnique);
		i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setShadowTechnique(i6engine::api::graphics::ShadowTechnique(shadowTechnique));

		// setting distance fog
		i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setExponentialFog(Vec3(0.9, 0.9, 0.9), 0.005);

		// register rpg components we want to use
		// do this befor loading the level
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Attribute", boost::bind(&i6engine::rpg::components::AttributeComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Healthbar", boost::bind(&i6engine::rpg::components::HealthbarComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("HumanMovement", boost::bind(&i6engine::rpg::components::HumanMovementComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("ListInventory", boost::bind(&i6engine::rpg::components::ListInventoryComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Name", boost::bind(&i6engine::rpg::components::NameComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Quickslot", boost::bind(&i6engine::rpg::components::QuickslotComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Slot", boost::bind(&i6engine::rpg::components::SlotComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("SlotInventory", boost::bind(&i6engine::rpg::components::SlotInventoryComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("ThirdPersonControl", boost::bind(&i6engine::rpg::components::ThirdPersonControlComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("UsableItem", boost::bind(&i6engine::rpg::components::UsableItemComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("WeightInventory", boost::bind(&i6engine::rpg::components::WeightInventoryComponent::createC, _1, _2));

		// loads the RPG demo level
		i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->loadLevel("../media/maps/SampleLevel.xml", "Singleplayer");

		i6engine::api::ObjectFacade * of = i6engine::api::EngineController::GetSingleton().getObjectFacade();
		{
			i6engine::api::objects::GOTemplate tmpl;
			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("ToggleWaynet", i6engine::api::attributeMap(), "", false, false));
			of->createGO("SpectatorCam", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false, [this](i6engine::api::GOPtr go) {
				_camera = go;
				go->getGOC<i6engine::api::ToggleWaynetComponent>(i6engine::api::components::ComponentTypes::ToggleWaynetComponent)->enable(true);
			});
		}
		{
			i6engine::api::objects::GOTemplate tmpl;
			of->createObject("Sun", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;
			i6engine::api::attributeMap params;
			params.insert(std::make_pair("name", "NPCName01")); // size of the terrain in world units
			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Name", params, "", false, false));
			of->createObject("NPC", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
	}

	void WaynetApplication::News(const i6engine::api::GameMessage::Ptr & msg) {
		if (msg->getMessageType() == i6engine::api::messages::InputMessageType) {
			if (msg->getSubtype() == i6engine::api::mouse::MouseMessageTypes::MouButton) {
				i6engine::api::input::Input_Button_Update * ibu = dynamic_cast<i6engine::api::input::Input_Button_Update *>(msg->getContent());
				if (ibu->code == i6engine::api::MouseButtonID::MB_Left && ibu->pressed) {
					auto vec = i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->getSelectables();
					if (vec.size() > 0) {
						auto go = i6engine::api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("NPC").front();
						go->getGOC<i6engine::api::MoveComponent>(i6engine::api::components::ComponentTypes::MoveComponent)->navigate(vec.front().second);
					}
				}
			} else {
				CommonApplication::InputMailbox(msg);
			}
		}
	}

} /* namespace sample */
