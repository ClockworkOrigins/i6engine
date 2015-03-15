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

#ifndef __I6ENGINE_API_MOVERCOMPONENT_H__
#define __I6ENGINE_API_MOVERCOMPONENT_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/ShatterComponent.h"

#include "boost/thread/mutex.hpp"

namespace i6engine {
namespace api {

	class PhysicalStateComponent; // forward declaration

	typedef std::pair<Vec3, Quaternion> keyFrame;

	/**
	 * \brief This component let's the Object move through the world automatically on fixed paths
	 * Use \ref addKeyFrame() to add new keyframes (specific positions in the world)
	 * Than call start to start the moving.
	 * This component will move the object during each tick.
	 * You can control the behaviour by defining a mode, a way and the way of positioning. Consult the enum definitions
	 * for a detailed description
	 * \info This Component will soon be split into several subclasses for different Ways.
	 * For creating a Mover Component, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | duration | yes | long | Duration of one movement cycle | yes |
	 * | mode | yes | short | How are the frames interpolated, ignored in CIRCLE way | yes |
	 * | way | yes | short | In which order the frames will be processed | yes |
	 * | positioning | yes | short | Absolute or relative positioning | yes |
	 * | continue | no | "true" | continues movement in stead of starting | no |
	 * | startTime | no | time | required when continue set to synchronize time | no |
	 * | keyframes | *) | int | number of keyframesto follow, *) required when LINEAR or BEZIER | yes |
	 * | keyframe_<i>_pos | *) | vector | position of i-th keyframe, *) required for all keyframes | yes |
	 * | keyframe_<i>_rot | *) | quaternion | rotation of i-th keyframe, *) required for all keyframes | yes |
	 * | pos | *) | vector | centerposition of the circle, *) required if CIRCLE | yes |
	 * | axis | *) | vector | axis of the circle, *) required if CIRCLE | yes |
	 * | radius | *) | double | radius of the circle, *) required if CIRCLE | yes |
	 */
	class ISIXE_MODULES_API MoverComponent : public ShatterComponent {
	public:
		enum class Positioning : uint16_t {
			POSITIONING_ABSOLUTE, 	// ! the GameObject is moved to exactly this position (you won't be able to move it manually with a lasting effect)
			POSITIONING_RELATIVE	// ! the positions are calculated like with absolute, but the GameObject is just moved by the difference vector (lastPos - curPos)
		};

		MoverComponent(const int64_t id, const attributeMap & params);
		virtual ~MoverComponent();

		/**
		 * \brief
		 * \param[in] duration time in microseconds
		 */
		void setDuration(const uint64_t duration) {
			_duration = duration;
		}

		void setPositioning(const Positioning posing) { _positioning = posing; }

		/**
		 * \brief starts the moving
		 * the startPos position should be the starting position of the movement
		 * the object will be moved to this position, or in case of a circle automatically in the circle path
		 */
		virtual void start(Vec3 & startPos) = 0;

		/**
		 * \brief stops movement
		 */
		void stop();

		virtual void Tick() override;

		void Init() override;

		/**
		 * \brief resets progress to initial status
		 */
		virtual void reset() = 0;

		/**
		 * \brief synchronizes the Components state
		 */
		virtual attributeMap synchronize() override;

		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;

		void shatter(const GOPtr & go) override;

		std::vector<componentOptions> getComponentOptions() override;

	protected:
		/**
		 * \brief duration for one path
		 */
		uint64_t _duration;

		/**
		 * \brief type of positioning
		 */
		Positioning _positioning;

		/**
		 * \brief PSC of the GameObject
		 */
		utils::weakPtr<PhysicalStateComponent, Component> _psc;

		/**
		 * \brief time the moving started
		 */
		uint64_t _startTime;

		/**
		 * \brief last position value calculated in imaginary path
		 * used for RELATIVE positioning
		 */
		Vec3 _lastPos;

		/**
		 * \brief real starting position
		 * needed for synchronizing
		 */
		Vec3 _realStartPos;

		/**
		 * \brief last rotation value calculated in imaginary path
		 * used for RELATIVE positioning
		 */
		Quaternion _lastRot;

		/**
		 * \brief current status
		 */
		bool _moving;

		/**
		 * \brief whether the call to start() will start or continue the movement
		 */
		bool _initial;

		/**
		 * \brief whether the Mover should be started on creation or not
		 */
		bool _started;

		/**
		 * \brief whether the object on the Mover should be linked and moved with the Mover
		 */
		bool _linkable;

		/**
		 * \brief vector of all linked GameObject IDs
		 */
		std::vector<int64_t> _linked;

		/**
		 * \brief handles incoming messages.
		 * Currently only one message is supported:
		 * MoverResync - Synchronizes the state of the component after a non-predictable change
		 */
		virtual void News(const GameMessage::Ptr & msg) override = 0;

		/**
		 * \brief loads all parameters from a message
		 */
		void loadParams(const attributeMap & params);

		/**
		 * \brief retrieves the new postion/rotation values
		 * \param[in] t time since starting
		 * \param[out] newPos new position to be used
		 * \param[out] newRot new Rotation to be used
		 */
		virtual void getNewPosition(const uint64_t t, Vec3 & newPos, Quaternion & newRot) = 0;

		MoverComponent(const MoverComponent &) = delete;

		const MoverComponent & operator=(const MoverComponent &) = delete;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_MOVERCOMPONENT_H__ */

/**
 * @}
 */
