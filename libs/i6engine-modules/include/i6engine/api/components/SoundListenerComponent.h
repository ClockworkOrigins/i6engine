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

#ifndef __I6ENGINE_API_SOUNDLISTENERCOMPONENT_H__
#define __I6ENGINE_API_SOUNDLISTENERCOMPONENT_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	class PhysicalStateComponent;

	/**
	 * \class SoundListenerComponent
	 * \brief Attaches a sound listener to an object. The Listener will follow the object
	 */
	class ISIXE_MODULES_API SoundListenerComponent : public Component {
	public:
		/**
		 * \brief Constructor with attribute map
		 */
		explicit SoundListenerComponent(const int64_t id, const attributeMap & params);

		/**
		 * \brief Destructor
		 */
		~SoundListenerComponent();

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "SoundListener";
		}

		std::vector<componentOptions> getComponentOptions() override {
			return {};
		}

	private:
		/**
		 * \brief Position of the camera
		 */
		Vec3 _position;

		/**
		 * \brief Point the Camera looks at
		 */
		Quaternion _rotation;

		/**
		 * \brief weakPtr on PhysicalStateComponent for faster access
		 */
		utils::weakPtr<PhysicalStateComponent, Component> _psc;

		/**
		 * \brief initializes the component
		 */
		void Init() override;

		void Finalize() override;

		void Tick() override;

		virtual std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_SOUNDLISTENERCOMPONENT_H__ */

/**
 * @}
 */
