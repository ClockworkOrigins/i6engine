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

#ifndef __I6ENGINE_RPG_NPC_NPCPARSER_H__
#define __I6ENGINE_RPG_NPC_NPCPARSER_H__

#include "i6engine/utils/Singleton.h"

#include "i6engine/api/configs/ObjectConfig.h"

namespace i6engine {
namespace rpg {
namespace npc {

	class ISIXE_RPG_API NPCParser : public i6engine::utils::Singleton<NPCParser> {
		friend class i6engine::utils::Singleton<NPCParser>;

	public:
		~NPCParser();

		void loadNPCs(const std::string & directory);

		i6engine::api::objects::GOTemplate getTemplate(const std::string & identifier) const;

	private:
		std::map<std::string, i6engine::api::objects::GOTemplate> _npcs;

		NPCParser();

		void loadNPC(const std::string & file);
	};

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_NPC_NPCPARSER_H__ */
