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
 * \addtogroup Components
 * @{
 */

#ifndef __I6ENGINE_API_LIFETIMECOMPONENT_H__
#define __I6ENGINE_API_LIFETIMECOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	/**
	 * \class LifetimeComponent
	 * \brief destroys the owning game object after given lifetime
	 * For creating a LifetimeComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | lifetime | yes | uint64_t | time until this GameObject will be destroyed | yes |
	 */
	class ISIXE_MODULES_API LifetimeComponent : public Component {
	public:
		/**
		 * \brief This GameObject will be deleted after lifetime milliseconds after creation.
		 * \param[in] lifetime milliseconds until deletion
		 */
		LifetimeComponent(const int64_t id, const attributeMap & params);

		~LifetimeComponent();

		/**
		 * \brief Called when the time for this GameObject is over.
		 */
		void dead();

		/**
		 * \brief Instant kill
		 */
		void instantKill();

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "Lifetime";
		}

		std::vector<componentOptions> getComponentOptions() override;

	private:
		uint64_t _startTime;

		/**
		 * ticks left until the object will be removed
		 */
		uint64_t _lifetime;

		void Init() override;

		void Finalize() override;

		/**
		 * \brief ticking the component
		 */
		void Tick() override;

		LifetimeComponent(const LifetimeComponent &) = delete;
		const LifetimeComponent & operator=(const LifetimeComponent &) = delete;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_LIFETIMECOMPONENT_H__ */

/**
 * @}
 */
