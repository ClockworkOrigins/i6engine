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

#ifndef __I6ENGINE_API_MOVERCOMPONENT_H__
#define __I6ENGINE_API_MOVERCOMPONENT_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/ShatterComponent.h"

#include "boost/thread/mutex.hpp"

namespace i6e {
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
	 * For creating a MoverComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | duration | yes | uint64_t | Duration of one movement cycle | yes |
	 * | positioning | yes | short | Absolute or relative positioning | yes |
	 * | continue | no | "true" | continues movement instead of starting | no |
	 * | startTime | *) | uint64_t | *) required when continue set to synchronize time | no |
	 * | lastPos | no | Vec3 | the last position of the circle | no |
	 * | linkable | no | bool | should colliding objects (with correct flags) be linked to this mover and moved with it | yes |
	 * | started | yes | bool | should this mover start directly | yes |
	 */
	class ISIXE_MODULES_API MoverComponent : public ShatterComponent {
	public:
		enum class Positioning : uint16_t {
			POSITIONING_ABSOLUTE, 	// ! the GameObject is moved to exactly this position (you won't be able to move it manually with a lasting effect)
			POSITIONING_RELATIVE	// ! the positions are calculated like with absolute, but the GameObject is just moved by the difference vector (lastPos - curPos)
		};

		MoverComponent(const int64_t id, const attributeMap & params);
		virtual ~MoverComponent();

		virtual void Tick() override;

		virtual void Init() override;

		virtual void Finalize() override;

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

		/**
		 * \brief resets progress to initial status
		 */
		virtual void reset() = 0;

		/**
		 * \brief synchronizes the Components state
		 */
		virtual attributeMap synchronize() const override;

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
		virtual void loadParams(const attributeMap & params);

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
} /* namespace i6e */

#endif /* __I6ENGINE_API_MOVERCOMPONENT_H__ */

/**
 * @}
 */
