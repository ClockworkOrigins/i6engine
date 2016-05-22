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

#ifndef __I6ENGINE_RPG_NPC_NPCPARSER_H__
#define __I6ENGINE_RPG_NPC_NPCPARSER_H__

#include "i6engine/utils/Singleton.h"

#include "i6engine/api/configs/ObjectConfig.h"

namespace i6e {
namespace rpg {
namespace npc {

	/**
	 * \brief parses and stores all NPC templates
	 */
	class ISIXE_RPG_API NPCParser : public utils::Singleton<NPCParser> {
		friend class utils::Singleton<NPCParser>;

	public:
		~NPCParser();

		/**
		 * \brief loads all NPC templates from given directory and all subdirectories
		 */
		void loadNPCs(const std::string & directory);

		api::objects::GOTemplate getTemplate(const std::string & identifier) const;

		std::vector<std::pair<std::string, std::string>> getNPCList() const {
			return _npcFiles;
		}

	private:
		std::map<std::string, api::objects::GOTemplate> _npcs;
		std::vector<std::pair<std::string, std::string>> _npcFiles;

		NPCParser();

		void loadNPC(const std::string & file);
	};

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_NPC_NPCPARSER_H__ */

/**
 * @}
 */
