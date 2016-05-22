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

#ifndef __I6ENGINE_RPG_QUEST_QUEST_H__
#define __I6ENGINE_RPG_QUEST_QUEST_H__

#include <cstdint>
#include <string>
#include <vector>

#include "i6engine/utils/i6eSystemParameters.h"

namespace i6e {
namespace rpg {
namespace quest {

	enum class QuestStatus {
		LOCKED,
		RUNNING,
		COMPLETED,
		FAILED
	};

	/**
	 * \brief represents a Quest
	 */
	struct ISIXE_RPG_API Quest {
		std::string name;
		std::string category;
		int32_t level = -1;
		QuestStatus status = QuestStatus::LOCKED;
		std::vector<std::string> entries;
	};

} /* namespace quest */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_QUEST_QUEST_H__ */

/**
 * @}
 */
