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

#ifndef __I6ENGINE_API_MOVERCIRCLECOMPONENT_H__
#define __I6ENGINE_API_MOVERCIRCLECOMPONENT_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/MoverComponent.h"

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/thread/mutex.hpp"

namespace i6e {
namespace api {

	/**
	 * \brief moves an object around a point in a circular way
	 * For creating a MoverCircleComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | pos | yes | Vec3 | center of the circle | yes |
	 * | axis | yes | Vec3 | axis around which the mover should circle | yes |
	 * | radius | yes | double | radius of the circle | yes |
	 * | realCenterPos | no | Vec3 | the real start pos of the circle | no |
	 */
	class ISIXE_MODULES_API MoverCircleComponent : public MoverComponent {
	public:
		MoverCircleComponent(const int64_t id, const attributeMap & params);
		~MoverCircleComponent();

		void setPositioning(const Positioning posing) { _positioning = posing; }

		void setCircleParameters(const Vec3 & cC, const Vec3 & cA, const double & cR) {
			_circleCenter = cC;
			_circleAxis = cA;
			_circleRadius = cR;
		}

		/**
		 * \brief starts the moving
		 * the startPos position should be the starting position of the movement
		 * the object will be moved to this position, or in case of a circle automatically in the circle path
		 */
		void start(Vec3 & startPos) override;

		Vec3 getCircleAxis() const { return _circleAxis; }
		double getCircleRadius() const { return _circleRadius; }

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "MoverCircle";
		}

		void reset() override;

		std::vector<componentOptions> getComponentOptions() override;

	private:
		/**
		 * \brief stores parameters for circle mode
		 */
		Vec3 _circleCenter;

		/**
		 * \brief normal axis of the circle
		 * used in CYCLE mode
		 */
		Vec3 _circleAxis;

		/**
		 * \brief radius of the circle
		 * used in CYCLE mode
		 */
		double _circleRadius;

		/**
		 * \brief loads all parameters from a message
		 */
		void loadParams(const attributeMap & params) override;

		void News(const GameMessage::Ptr & msg) override;

		void getNewPosition(const uint64_t t, Vec3 & newPos, Quaternion & newRot) override;

		MoverCircleComponent(const MoverCircleComponent &) = delete;

		const MoverCircleComponent & operator=(const MoverCircleComponent &) = delete;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_MOVERCIRCLECOMPONENT_H__ */

/**
 * @}
 */
