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

/**
 * \addtogroup RPG
 * @{
 */

#ifndef __I6ENGINE_RPG_NPC_NPCPARSER_H__
#define __I6ENGINE_RPG_NPC_NPCPARSER_H__

#include "i6engine/utils/Singleton.h"

#include "i6engine/api/configs/ObjectConfig.h"

namespace i6engine {
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
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_NPC_NPCPARSER_H__ */

/**
 * @}
 */
