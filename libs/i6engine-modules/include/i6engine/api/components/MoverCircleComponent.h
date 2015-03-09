/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

namespace i6engine {
namespace api {

	class ISIXE_MODULES_API MoverCircleComponent : public MoverComponent {
	public:
		MoverCircleComponent(const int64_t id, const attributeMap & params);
		virtual ~MoverCircleComponent();

		/**
		 * \brief creates the Component with given attributeMap
		 */
		static ComPtr createC(const int64_t id, const attributeMap & params);

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
		void start(Vec3 & startPos);

		Vec3 getCircleAxis() const { return _circleAxis; }
		double getCircleRadius() const { return _circleRadius; }

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() override;

		std::string getTemplateName() const override {
			return "MoverCircle";
		}

		void reset();

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
		void loadParams(const attributeMap & params);

		void News(const GameMessage::Ptr & msg) override;

		void getNewPosition(const uint64_t t, Vec3 & newPos, Quaternion & newRot);

		MoverCircleComponent(const MoverCircleComponent &) = delete;

		const MoverCircleComponent & operator=(const MoverCircleComponent &) = delete;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_MOVERCIRCLECOMPONENT_H__ */

/**
 * @}
 */
