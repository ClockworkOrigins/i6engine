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

#ifndef __I6ENGINE_RPG_NPC_NPCMANAGER_H__
#define __I6ENGINE_RPG_NPC_NPCMANAGER_H__

#include "i6engine/utils/i6eSystemParameters.h"
#include "i6engine/utils/Singleton.h"

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

namespace i6e {
namespace rpg {
namespace npc {

	class NPC;

	/**
	 * \brief handles NPCs
	 */
	class ISIXE_RPG_API NPCManager : public utils::Singleton<NPCManager> {
		friend class utils::Singleton<NPCManager>;

	public:
		~NPCManager();

		/**
		 * \brief creates an NPC with given identifier at position and with rotation
		 */
		void createNPC(const std::string & identifier, const Vec3 & pos, const Quaternion & rot, bool player);

		/**
		 * \brief returns active NPC for given identifier
		 */
		NPC * getNPC(const std::string & identifier) const;

	private:
		std::map<std::string, NPC *> _npcs;

		bool _running;
		uint64_t _jobID;

		NPCManager();

		bool checkNPCs();
	};

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_NPC_NPCPARSER_H__ */

/**
 * @}
 */
