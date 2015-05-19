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

#ifndef __I6ENGINE_RPG_NPC_NPCMANAGER_H__
#define __I6ENGINE_RPG_NPC_NPCMANAGER_H__

#include <thread>

#include "i6engine/utils/i6eSystemParameters.h"
#include "i6engine/utils/Singleton.h"

#include "i6engine/math/i6eVector.h"

namespace i6engine {
namespace rpg {
namespace npc {

	class NPC;

	class ISIXE_RPG_API NPCManager : public utils::Singleton<NPCManager> {
		friend class utils::Singleton<NPCManager>;

	public:
		~NPCManager();

		void createNPC(const std::string & identifier, const Vec3 & pos, bool player);

		NPC * getNPC(const std::string & identifier) const;

	private:
		std::map<std::string, NPC *> _npcs;

		// TODO: (Daniel) exchange this with scheduler job implementing ticket ISIXE-1457
		std::thread _worker;
		bool _running;

		NPCManager();

		void checkNPCs();
	};

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_NPC_NPCPARSER_H__ */
