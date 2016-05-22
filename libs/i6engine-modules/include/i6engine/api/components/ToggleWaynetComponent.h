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

#ifndef __I6ENGINE_API_COMPONENTS_TOGGLEWAYNETCOMPONENT_H__
#define __I6ENGINE_API_COMPONENTS_TOGGLEWAYNETCOMPONENT_H__

#include "i6engine/api/components/Component.h"
#include "i6engine/api/facades/MessageSubscriberFacade.h"

namespace i6e {
namespace api {

	/**
	 * \brief Component used to show the waynet
	 */
	class ISIXE_MODULES_API ToggleWaynetComponent : public Component, public MessageSubscriberFacade {
	public:
		ToggleWaynetComponent(int64_t id, const attributeMap & params);

		attributeMap synchronize() const  override {
			return {};
		}

		std::string getTemplateName() const  override {
			return "ToggleWaynet";
		}

		std::vector<componentOptions> getComponentOptions() override {
			return {};
		}

		/**
		 * \brief enables or disables the showing of the waynet
		 */
		void enable(bool enabled) {
			_active = enabled;
		}

	private:
		bool _active;

		void Init() override;

		void Tick() override;

		void Finalize() override;

		void News(const GameMessage::Ptr & msg) override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_COMPONENTS_TOGGLEWAYNETCOMPONENT_H__ */

/**
 * @}
 */
