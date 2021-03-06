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

#ifndef __I6ENGINE_API_VELOCITYCOMPONENT_H__
#define __I6ENGINE_API_VELOCITYCOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	/**
	 * \brief Component for objects with more complex velocity handling than just setting an impulse, e.g. a car
	 * For creating a VelocityComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | acceleration | yes | double | acceleration of this GameObject, in m / s� | yes |
	 * | deceleration | no | double | breaking acceleration of this GameObject, in m / s� | yes |
	 * | maxSpeed | yes | double | maximum speed this GameObject can reach, in m / s | yes |
	 * | resistanceCoefficient | no | double | resistance coefficient, default: 0.6 | yes |
	 * | windage | no | double | windage of this GameObject, default: 0.8 m� | yes |
	 * | handling | no | int | handling for reaching maximum speed, keep maximum speed or stop acceleration to roll out, default: 0 (= KeepSpeed) | yes |
	 */
	class ISIXE_MODULES_API VelocityComponent : public Component {
	public:
		enum class MaxSpeedHandling {
			KeepSpeed,
			StopAcceleration
		};
		enum class DecelerationHandling {
			Backward,
			StopDeceleration
		};

		VelocityComponent(const int64_t id, const attributeMap & params);

		~VelocityComponent();

		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "Velocity";
		}

		std::vector<componentOptions> getComponentOptions() override {
			return {};
		}

		/**
		 * \brief accelerates the GameObject using acceleration
		 * reaching maxSpeed, it will perform as defined in handling parameter and callback will be called
		 */
		void accelerate(const Vec3 & acceleration, MaxSpeedHandling handling, const std::function<void(void)> & callback);
		void accelerate(const std::function<void(void)> & callback) {
			accelerate(_acceleration, _handling, callback);
		}
		
		/**
		 * \brief decelerates the GameObject using deceleration
		 * reaching 0 speed, it will perform as defined in handling parameter and callback will be called
		 */
		void decelerate(const Vec3 & deceleration, DecelerationHandling handling, const std::function<void(void)> & callback);
		void decelerate(const std::function<void(void)> & callback) {
			decelerate(_deceleration, _decelerationHandling, callback);
		}

		/**
		 * \brief stops current acceleration
		 */
		void stopAcceleration() const;
		
		/**
		 * \brief sets new maximum speed
		 */
		void setMaxSpeed(double maxSpeed);
		
		/**
		 * \brief sets new resistance coefficient
		 */
		void setResistanceCoefficient(double coeff);
		
		/**
		 * \brief sets new windage
		 */
		void setWindage(double windage);

		/**
		 * \brief sets the deceleration handling
		 */
		void setDecelerationHandling(DecelerationHandling handling) {
			_decelerationHandling = handling;
		}

	private:
		Vec3 _acceleration;
		Vec3 _deceleration;
		double _maxSpeed;
		double _resistanceCoefficient;
		double _windage;
		MaxSpeedHandling _handling;
		DecelerationHandling _decelerationHandling;

		void Init() override;

		void Finalize() override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_VELOCITYCOMPONENT_H__ */

/**
 * @}
 */
