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

#include "MultilanguageApplication.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/manager/LanguageManager.h"
#include "i6engine/api/manager/TextManager.h"
#include "i6engine/api/objects/GameObject.h"

#include "boost/bind.hpp"

namespace sample {

	MultilanguageApplication::MultilanguageApplication() : i6e::api::Application() {
	}

	MultilanguageApplication::~MultilanguageApplication() {
	}

	void MultilanguageApplication::Initialize() {
	}

	void MultilanguageApplication::AfterInitialize() {
		i6e::api::GUIFacade * gf = i6e::api::EngineController::GetSingleton().getGUIFacade();

		// register GUI scheme
		gf->startGUI("RPG.scheme", "", "", "RPG", "MouseArrow");

		// enter buttons for language switching
		gf->addImage("Background", "RPG/StaticImage", "RPG", "MainMenu", 0.0, 0.0, 1.0, 1.0);
		gf->addTextButton("GermanButton", "RPG/Button", 0.05, 0.05, 0.1, 0.05, i6e::api::EngineController::GetSingleton().getTextManager()->getText("German_Key"));
		gf->addTextButton("EnglishButton", "RPG/Button", 0.05, 0.11, 0.1, 0.05, i6e::api::EngineController::GetSingleton().getTextManager()->getText("English_Key"));

		// onClick events setting new language
		gf->subscribeEvent("GermanButton", i6e::api::gui::SubscribeEvent::Clicked, boost::bind(&i6e::api::LanguageManager::setLanguage, i6e::api::EngineController::GetSingleton().getLanguageManager(), "Deutsch"));
		gf->subscribeEvent("EnglishButton", i6e::api::gui::SubscribeEvent::Clicked, boost::bind(&i6e::api::LanguageManager::setLanguage, i6e::api::EngineController::GetSingleton().getLanguageManager(), "English"));

		// description how sample works
		gf->addPrint("ChangeLanguagePrint", "RPG/Blanko", 0.5, 0.48, i6e::api::EngineController::GetSingleton().getTextManager()->getText("ChangeLanguageDescription_Key"), i6e::api::gui::Alignment::Center, -1);
		gf->setSize("ChangeLanguagePrint", 1.0, 0.05);

		// this callback handles changing language
		i6e::api::EngineController::GetSingleton().getLanguageManager()->addCallback([gf](const std::string &) {
			gf->setText("GermanButton", i6e::api::EngineController::GetSingleton().getTextManager()->getText("German_Key"));
			gf->setText("EnglishButton", i6e::api::EngineController::GetSingleton().getTextManager()->getText("English_Key"));
			gf->setText("ChangeLanguagePrint", i6e::api::EngineController::GetSingleton().getTextManager()->getText("ChangeLanguageDescription_Key"));
		});

		// register ESC to close the application
		i6e::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6e::api::KeyCode::KC_ESCAPE, i6e::api::KeyState::KEY_PRESSED, boost::bind(&i6e::api::EngineController::stop, i6e::api::EngineController::GetSingletonPtr()));
	}

	void MultilanguageApplication::Tick() {
	}

	bool MultilanguageApplication::ShutdownRequest() {
		return true;
	}

	void MultilanguageApplication::Finalize() {
	}

	void MultilanguageApplication::ShutDown() {
	}

} /* namespace sample */
