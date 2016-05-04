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

#ifndef __I6ENGINE_API_MOVERINTERPOLATECOMPONENT_H__
#define __I6ENGINE_API_MOVERINTERPOLATECOMPONENT_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/MoverComponent.h"

namespace i6e {
namespace api {

	/**
	 * \brief moving an object interpolating in some way
	 * For creating a MoverInterpolateComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | way | yes | short | How are the frames interpolated, one of enum entries of Way | yes |
	 * | mode | yes | short | In which order the frames will be processed | yes |
	 * | direction | yes | bool | Direction of this mover, true is forward, false is backward | yes |
	 * | openTime | *) | uint64_t | time mover stays at end position before moving backwards, *) required when mode is TWOSTATE_OPENTIME | yes |
	 * | keyframes | yes | int | number of keyframes to follow | yes |
	 * | keyframe_<i>_pos | *) | vector | position of i-th keyframe, *) required for all keyframes | yes |
	 * | keyframe_<i>_rot | *) | quaternion | rotation of i-th keyframe, *) required for all keyframes | yes |
	 */
	class ISIXE_MODULES_API MoverInterpolateComponent : public MoverComponent {
	public:
		enum class Mode : uint16_t {
			TWOSTATE_TOGGLE, 	// ! loops forward through all keyframes and afterwards backwards. This is repeated endlessly.
			TWOSTATE_OPENTIME, 	// ! same as TWOSTATE_TOGGLE but waits opentime before returning
			NSTATE_LOOP,		// ! loops through all keyframes (moving from last to first keyframe is interpolated just like all other segments)
			ONCE				// ! moves from start to end and stops there
		};

		enum class Way : uint16_t {
			LINEAR, 		// ! linear interpolation
			BEZIER 			// ! approximation using bezier-curves
		};

		MoverInterpolateComponent(const int64_t id, const attributeMap & params);
		~MoverInterpolateComponent();

		/**
		 * \brief add a new keyframe at the end of the current path
		 */
		void addKeyFrame(const Vec3 & position, const Quaternion & rotation);

		/**
		 * \brief removes the keyframe with given id
		 * id is 0 indexed
		 */
		void removeKeyFrame(const uint32_t id);

		/**
		 * \brief gets the nth keyFrame
		 * param[in] nth which keyFrame should be returned
		 * nth can be an arbitary number. The function will return the
		 * keyFrame that will be the nth one being passed
		 * Thus the frame is also dependend on the mode being used
		 * returns the appropriate keyFrame
		 * If mode is TWOSTATE_TOGGLE, a number > number of available
		 * frames will return the last one
		 */
		inline keyFrame getKeyframe(const uint32_t nth) const {
			return _keyFrames[nth];
		}

		/**
		 * \brief
		 * \param[in] duration time in microseconds
		 */
		inline void setMode(const Mode m) { _mode = m; }
		inline void setOpenTime(const uint64_t opentime) {
			_openTime = opentime;
		}
		inline void setWay(const Way w) { _way = w; }

		inline Way getWay() const { return _way; }

		void start(Vec3 & startPos) override;

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "MoverInterpolate";
		}

		void reset() override;

		std::vector<componentOptions> getComponentOptions() override;

	private:
		/**
		 * \brief list of all keyframes defining a path
		 * used in all path modes
		 */
		std::vector<keyFrame> _keyFrames;

		/**
		 * \brief movement mode
		 */
		Mode _mode;

		/**
		 * \brief time to wait after on direction
		 * used in path modes
		 */
		uint64_t _openTime;

		/**
		 * \brief way mode to be used
		 */
		Way _way;

		/**
		 * \brief total distance the Object has to move
		 */
		double _totalDistance;

		/**
		 * \brief current means: the last passed keyframe
		 */
		double _currentDist;

		/**
		 * \brief current Frame for LINEAR
		 */
		size_t _currentFrame;

		/**
		 * \brief direction of the Mover (default is forward = true)
		 */
		bool _direction;

		/**
		 * \brief used to lock the internal objects
		 */
		boost::mutex _lock;

		void News(const GameMessage::Ptr & msg) override;

		void getNewPosition(const uint64_t t, Vec3 & newPos, Quaternion & newRot) override;

		/**
		 * \brief loads all parameters from a message
		 */
		void loadParams(const attributeMap & params) override;

		MoverInterpolateComponent(const MoverInterpolateComponent &) = delete;

		const MoverInterpolateComponent & operator=(const MoverInterpolateComponent &) = delete;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_MOVERLINEARCOMPONENT_H__ */

/**
 * @}
 */
