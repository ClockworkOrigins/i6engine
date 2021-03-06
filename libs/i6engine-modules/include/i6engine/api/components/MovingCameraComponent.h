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

#ifndef __I6ENGINE_API_MOVINGCAMERACOMPONENT_H__
#define __I6ENGINE_API_MOVINGCAMERACOMPONENT_H__

#include "i6engine/api/components/CameraComponent.h"

namespace i6e {
namespace api {

	/**
	 * \class MovingCameraComponent
	 * \brief Attaches a moving camera to an object.
	 * This camera will follow the object in a more sophisticated way.
	 */
	class ISIXE_MODULES_API MovingCameraComponent : public CameraComponent {
	public:
		MovingCameraComponent(const int64_t id, const attributeMap & params);
		~MovingCameraComponent();

		std::string getTemplateName() const override {
			return "MovingCamera";
		}

	private:
		Vec3 _oldPos;

		void Init() override;

		void Finalize() override;

		void Tick() override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_MOVINGCAMERACOMPONENT_H__ */

/**
 * @}
 */
