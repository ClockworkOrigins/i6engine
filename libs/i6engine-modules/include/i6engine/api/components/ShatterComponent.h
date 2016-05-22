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

#ifndef __I6ENGINE_API_SHATTERCOMPONENT_H__
#define __I6ENGINE_API_SHATTERCOMPONENT_H__

#include "i6engine/utils/sharedPtr.h"

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	typedef utils::sharedPtr<GameObject, GameObject> GOPtr;

	/**
	 * \class ShatterComponent
	 * \brief This class is needed on every GameObject with: TRIGGER
	 * The \ref shatter() method is called for detected collisions
	 */
	class ISIXE_MODULES_API ShatterComponent : public Component {
	public:
		/**
		 * \brief Constructor of the component with a given id
		 */
		ShatterComponent(int64_t id, const attributeMap & params);

		/**
		 * \brief Destructor of the component
		 */
		virtual ~ShatterComponent();

		/**
		 * \brief Initializes the Component
		 */
		virtual void Init() override;

		inline void resetRespawn() { _respawn = 0; }

		/**
		 * \brief Is called after a collision is detected
		 * \param[in] other GameObject you collided with
		 */
		virtual void shatter(const GOPtr & other) = 0;

		/**
		 * \brief Receives messages for this component
		 */
		virtual void News(const GameMessage::Ptr & msg) override;

		/**
		 * \brief synchronizes the Components state
		 */
		virtual attributeMap synchronize() const override;

	private:
		uint32_t _respawn;

		/**
		 * \brief This Method resets the respawn counter
		 */
		ShatterComponent(const ShatterComponent &) = delete;
		const ShatterComponent & operator=(const ShatterComponent &) = delete;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_SHATTERCOMPONENT_H__ */

/**
 * @}
 */
