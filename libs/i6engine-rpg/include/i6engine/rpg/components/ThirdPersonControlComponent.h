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

#ifndef __I6ENGINE_RPG_COMPONENTS_THIRDPERSONCONTROLCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_THIRDPERSONCONTROLCOMPONENT_H__

#include "i6engine/api/components/Component.h"
#include "i6engine/api/facades/MessageSubscriberFacade.h"

namespace i6e {
namespace api {
	class PhysicalStateComponent;
} /* namespace api */
namespace rpg {
namespace components {

	/**
	 * \brief handles input for player character
	 */
	class ISIXE_RPG_API ThirdPersonControlComponent : public api::Component, public api::MessageSubscriberFacade {
	public:
		ThirdPersonControlComponent(const int64_t id, const api::attributeMap & params);
		virtual ~ThirdPersonControlComponent();

		static api::ComPtr createC(const int64_t id, const api::attributeMap & params);

		api::attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "ThirdPersonControl";
		}

		std::vector<api::componentOptions> getComponentOptions() override {
			return {};
		}

		std::string getNPCIdentifier() const {
			return _identifier;
		}

	private:
		utils::weakPtr<api::PhysicalStateComponent, api::Component> _psc;
		int64_t _highlightTargetID;
		std::string _identifier;

		void Init() override;

		void Tick() override;

		void Finalize() override;

		void News(const api::GameMessage::Ptr & msg) override;
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_COMPONENTS_THIRDPERSONCONTROLCOMPONENT_H__ */

/**
 * @}
 */
