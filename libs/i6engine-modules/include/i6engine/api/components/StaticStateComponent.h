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

#ifndef __I6ENGINE_API_STATICSTATECOMPONENT_H__
#define __I6ENGINE_API_STATICSTATECOMPONENT_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	/**
	 * \class StaticStateComponent
	 * \brief Makes it possible to place a static GameObject
	 * For creating a StaticStateComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | pos | yes | Vec3 | position of the SceneNode | yes |
	 * | rot | yes | Quaternion | rotation of the SceneNode | yes |
	 * | scale | yes | Vec3 | scale of the SceneNode | yes |
	 */
	class ISIXE_MODULES_API StaticStateComponent : public Component {
	public:
		/**
		 * \brief Constructor
		 */
		StaticStateComponent(const int64_t id, const attributeMap & params);

		~StaticStateComponent();

		/**
		 * \brief Sets position
		 */
		inline void setPosition(const Vec3 & position) {
			_position = position;
			sendUpdateMessage();
		}

		/**
		 * \brief Sets rotation
		 */
		inline void setRotation(const Quaternion & rotation) {
			_rotation = rotation;
			sendUpdateMessage();
		}

		/**
		 * \brief Sets scale
		 */
		inline void setScale(const Vec3 & scale) {
			_scale = scale;
			sendUpdateMessage();
		}

		/**
		 * \brief Gets position
		 */
		inline Vec3 getPosition() const {
			return _position;
		}

		/**
		 * \brief Gets rotation
		 */
		inline Quaternion getRotation() const { return _rotation; }

		/**
		 * \brief Gets scale
		 */
		inline Vec3 getScale() const {
			return _scale;
		}

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "StaticState";
		}

		std::vector<componentOptions> getComponentOptions() override;

	private:
		/**
		 * \brief Stores the position of the object.
		 */
		Vec3 _position;

		/**
		 * \brief Stores the rotation of the object.
		 */
		Quaternion _rotation;

		/**
		 * \brief Stores the scale of the object.
		 */
		Vec3 _scale;

		/**
		 * \brief Initialized the Component
		 */
		void Init() override;

		void sendUpdateMessage();
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_STATICSTATECOMPONENT_H__ */

/**
 * @}
 */
