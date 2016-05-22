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

/**
 * \addtogroup RPG
 * @{
 */

#ifndef __I6ENGINE_RPG_RPGAPPLICATION_H__
#define __I6ENGINE_RPG_RPGAPPLICATION_H__

#include "i6engine/api/Application.h"

#include "clockUtils/iniParser/iniParser.h"

namespace i6e {
namespace rpg {

	/**
	 * \brief base class for RPG module
	 * derive from this class to get basic RPG setup
	 */
	class ISIXE_RPG_API RPGApplication : virtual public i6e::api::Application {
	public:
		RPGApplication();

		virtual ~RPGApplication();

		/**
		 * \brief reads RPG.ini and loads items, NPCs and dialogs
		 */
		virtual void Initialize();
		
		/**
		 * \brief registers GUI widgets, Components, loads level and sets basic settings
		 */
		virtual void AfterInitialize();

		void loadLevel(const std::string & level) const;

		virtual void Tick();

		virtual bool ShutdownRequest();

		virtual void Finalize();

		virtual void ShutDown();

	protected:
		clockUtils::iniParser::IniParser _iniParser;
	};

} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_RPGAPPLICATION_H__ */

/**
 * @}
 */
