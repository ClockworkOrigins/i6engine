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

#ifndef __I6ENGINE_RPG_COMPONENTS_EXPERIENCECOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_EXPERIENCECOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace rpg {
namespace components {

	/**
	 * \brief handles experience points and level of characters
	 */
	class ISIXE_RPG_API ExperienceComponent : public api::Component {
	public:
		ExperienceComponent(int64_t id, const api::attributeMap & params);

		static api::ComPtr createC(int64_t id, const api::attributeMap & params);

		api::attributeMap synchronize() const override;

		std::vector<api::componentOptions> getComponentOptions() override {
			return {};
		}

		std::string getTemplateName() const override {
			return "Experience";
		}

		/**
		 * \brief returns the current XP
		 */
		uint32_t getCurrentXP() const {
			return _currentXP;
		}

		/**
		 * \brief sets the current xp
		 */
		void setCurrentXP(uint32_t xp) {
			_currentXP = xp;
		}

		/**
		 * \brief returns xp for next level
		 */
		uint32_t getNextXP() const {
			return _nextXP;
		}

		/**
		 * \brief sets xp for next level
		 */
		void setNextXP(uint32_t xp) {
			_nextXP = xp;
		}

		/**
		 * \brief returns current level
		 */
		uint32_t getLevel() const {
			return _level;
		}

		/**
		 * \brief sets new level
		 */
		void setLevel(uint32_t level) {
			_level = level;
		}

	private:
		uint32_t _currentXP;
		uint32_t _nextXP;
		uint32_t _level;

		void Init() override;

		std::pair<api::AddStrategy, int64_t> howToAdd(const api::ComPtr &) const override {
			return std::make_pair(api::AddStrategy::REJECT, -1);
		}
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_COMPONENTS_EXPERIENCECOMPONENT_H__ */

/**
 * @}
 */
