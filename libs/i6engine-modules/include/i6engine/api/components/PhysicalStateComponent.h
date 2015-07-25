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
 * \addtogroup Physics
 * @{
 */

/**
 * \page PHY_BULLET_VARS Explanation of bullet rigid body parameters
 * | name | type | values (forced) | values (reasonable) | Explanation |
 * | ---- | :--: | :-------------: | :----------------: | -------------|
 * | linearDamping | double | 0 - 1 | 0 - 1 | Speed will slowly decrease by factor (1 - x) ^ timeStep; also threshold for \ref Additional_Damping "additional damping" |
 * | angularDamping | double | 0 - 1 | 0 - 1 | Rotation will slowly decrease by factor (1 - x) ^ timeStep; also theshold for \ref Additional_Damping "additional damping" |
 * | \e additionalDamping | bool | T / F | T / F | Toggles additional dampings on for slow objects |
 * | \e additionalDampingFactor | double | * | 0 - 1 | fixed factor to slow down speed AND rotation if Threshold is not exceeded |
 * | \e additionalAngularDampingThresholdSqr | double | * | > 0 | Angular threshold to trigger additional damping |
 * | \e additionalLinearDampingThresholdSqr | double | * | > 0 | Linear threshold to trigger additional damping |
 * | | | | | |
 * | linearFactor | double | * | > 0 | all linear forces applied will be scaled by this value |
 * | angularFactor | double | * | > 0 | all angular forces applied will be scaled by this value |
 *
 * All \e italic parameters are not yet available within the engine</br>
 * </br>
 * \anchor Additional_Damping
 * If <em>additional damping</em> is turned on, slow objects will slow down even more</br>
 * First, additionalDumpingFactor is used, if BOTH additional dumping thresholds are triggered</br>
 * Second, if the speed is slower than linearDamping, it will slow down by fixed value 0.005</br>
 * Analogous for angular speed
 */

/**
 * @}
 */

/**
 * \addtogroup Components
 * @{
 */

#ifndef __I6ENGINE_API_PHYSICALSTATECOMPONENT_H__
#define __I6ENGINE_API_PHYSICALSTATECOMPONENT_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

#include "boost/thread/mutex.hpp"

namespace i6engine {
namespace api {

	namespace ResponseType {
		/**
		 * \enum CollisionFlags defining basic physical properties
		 * This has nothing to do with collision detection.
		 * \info You can combine these flags
		 */
		enum ResponseType {
			NONE = 0,		// !< Simple objects being pushable
			STATIC = 1<<0,	// !< Static object (don't move, can't be pushed away, ...)
			GHOST = 1<<1,	// !< Objects that don't interact physically with other objects (it's more like an area)
			TRIGGER = 1<<2	// !< collision with this object will trigger the internal callbacks (use this if you need shatter info)
		};
	}

	/**
	 * \brief struct containing the collision info for an object
	 */
	struct CollisionGroup {
		/**
		 * \brief defines the physical properties
		 */
		uint32_t responseType;

		/**
		 * \brief type of the object
		 */
		uint32_t crashType;

		/**
		 * \brief mask of all objects the given object gets collision feedback
		 */
		uint32_t crashMask;

		/**
		 * \brief Constructor. Initializes members with 0 (no Collisions)
		 */
		CollisionGroup() : responseType(0), crashType(0), crashMask(0) {
		}

		/**
		 * \brief Constructor. Initializes members with given values
		 * \param[in] response value indicating how this object interacts in the world
		 * \param[in] type value defining the type of the object (user definable)
		 * \param[in] mask a collision msg will be sent if this:mask & other:type != 0
		 */
		CollisionGroup(uint32_t response, uint32_t type, uint32_t mask) : responseType(response), crashType(type), crashMask(mask) {
		}

		/**
		 * \brief Constructor. Initializes members with given values
		 * \param[in] str string containing responseType, crashType and crashMask
		 */
		explicit CollisionGroup(const std::string & str) : responseType(), crashType(), crashMask() {
			std::stringstream ss(str);
			ss >> responseType;
			ss >> crashType;
			ss >> crashMask;
		}

		CollisionGroup & operator=(const CollisionGroup & cg) {
			responseType = cg.responseType;
			crashType = cg.crashType;
			crashMask = cg.crashMask;
			return *this;
		}

		template<class Archive>
		void serialize(Archive & ar, const unsigned int /*version*/) {
			ar & responseType;
			ar & crashType;
			ar & crashMask;
		}
	};

	/**
	 * \brief struct containing information from raytest
	 * objID: id of GO hit first by the ray
	 * sourceID: id of the soure GO
	 * collisionPoint: hit point
	 */
	typedef struct RayTestResult {
		int64_t objID;
		int64_t sourceID;
		Vec3 collisionPoint;

		RayTestResult() : objID(-1), sourceID(-1), collisionPoint() {}
	} RayTestResult;

	/**
	 * \brief Different flags for collision interest
	 */
	enum ShatterInterest : uint16_t {
		NONE = 0,		// !< shouldn't be used with responseType TRIGGER
		START = 1 << 0,	// !< start of a collision
		END = 1 << 1,		// !< end of a collision
		ALWAYS = 1 << 2	// !< every collision
	};

	/**
	 * \class PhysicalStateComponent
	 * \brief Represents an object's physical state and synchronizes with the Physics subsystem.
	 * For creating a PhysicalStateComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | syncPrio | no | int | priority from which upwards updates should be sent through network | yes |
	 * | gravity | no | Vec3 | gravity for this object. Replaces default | yes |
	 * | pos | yes | Vec3 | position of the SceneNode | yes |
	 * | rot | yes | Vec3 | rotation of the SceneNode | yes |
	 * | scale | yes | Vec3 | scale of the SceneNode | yes |
	 * | collisionGroup | yes | int int int | ResponseType CrashType CrashMask | yes |
	 * | shapeType | yes | int | one of the shapeTypes from ShapeType enum | yes |
	 * | shatterInterest | yes | int | one of the shatterInterests from ShatterInterest enum | yes |
	 * | compound | yes | bool | should this PSC be used as a compound shape together with other ones of this GameObject | yes |
	 */
	class ISIXE_MODULES_API PhysicalStateComponent : public Component {
	public:
		/**
		 * \brief Different shape types supported by i6engine
		 */
		enum class ShapeType : uint16_t {
			PLANE,	// !< an infinite large plane
			BOX,	// !< a cuboid
			SPHERE,	// !< a sphere
			FILE	// !< load from file
		};

		/**
		 * \brief Different flags for kind of raytest repetition
		 */
		enum class RayTestRepetition : uint16_t {
			STOP = 0,	// !< stops raytest, if started with periodic
			ONCE,		// !< sends one ray
			PERIODIC	// !< sends a ray every tick
		};

		/**
		 * \brief Different flags for the kind of notify mechanism of raytest
		 */
		enum class RayTestNotify : uint16_t {
			ALWAYS,			// !< notifies always
			CHANGE,			// !< notifies only on change whether something was hit or not
			FOUND,			// !< notifies only if raytest was successful
			NOTFOUND,		// !< notifies only if raytest was unsuccessful
			OBJECTCHANGE	// !< notifies only if found object is changed
		};

		/**
		 * \brief constructor
		 */
		PhysicalStateComponent(const int64_t id, const attributeMap & params);

		/**
		 * \brief destructor
		 */
		~PhysicalStateComponent();

		/**
		 * \brief creates the Component with given attributeMap
		 */
		static ComPtr createC(const int64_t id, const attributeMap & params);

		/**
		 * \brief Returns the position of the object.
		 * \return Position of the object
		 */
		Vec3 getPosition() const;

		/**
		 * \brief Changes the position of the object.
		 * \param[in] position new position for the object
		 * \param[in] prio priority of the change
		 * \details Also sets the dirty flag to true.
		 * During each Tick, only the change with highest priority will be
		 * propagated towards the physic. A value of 0 is reserved for the physic
		 * itself indicating the lowest priority
		 */
		void setPosition(const Vec3 & position, uint32_t prio);

		/**
		 * \brief Returns the rotation of the object.
		 * \return Rotation of the object.
		 */
		inline Quaternion getRotation() const { return _rotation; }

		/**
		 * \brief Changes the rotation of the object.
		 * \param[in] rotation new rotation for the object
		 * \details Also sets the dirty flag to true.
		 * During each Tick, only the change with highest priority will be
		 * propagated towards the physic. A value of 0 is reserved for the physic
		 * itself indicating the lowest priority
		 */
		void setRotation(const Quaternion & rotation, uint32_t prio);

		/**
		 * \brief returns the current scale value
		 */
		inline Vec3 getScale() const {
			return _scale;
		}

		/**
		 * \brief sets the current scale value
		 */
		void setScale(const Vec3 & scale, uint32_t prio);

		/**
		 * \brief sets the collision flags of this GameObject
		 */
		void setCollisionFlags(const CollisionGroup & col);

		/**
		 * \brief returns a reference to the Collision Flags of this GameObject
		 */
		CollisionGroup getCollisionFlags() const { return _collisionGroup; }

		/**
		 * \brief Clears all forces and sets the velocity to 0
		 * doesn't still move after resetting
		 */
		void reset();

		/**
		 * \brief sets type of the collision shape for this entity
		 */
		void setCollisionShape(ShapeType st, const attributeMap & params);

		/**
		 * \brief rotates the Object and the velocity
		 */
		void applyRotation(const Quaternion & rotation);

		/**
		 * \brief returns linear velocity
		 */
		Vec3 getLinearVelocity() const {
			if (_speedDirty > 0) {
				return _speedNew;
			} else {
				return _linearVelocity;
			}
		}

		/**
		 * \brief sets linear velocity
		 * During each Tick, only the change with highest priority will be
		 * propagated towards the physic. A value of 0 is reserved for the physic
		 * itself indicating the lowest priority
		 */
		void setLinearVelocity(const Vec3 & linVel, uint32_t prio);

		/**
		 * \brief applies a force to an object
		 * all forces applied during one frame will be applied at the same time
		 */
		void applyCentralForce(const Vec3 & cForce, bool forceIsLocalSpace);
		void applyForce(const Vec3 & force, const Vec3 & offset, bool forceIsLocalSpace);

		/**
		 * \brief sets type of the collision interest for this entity
		 */
		void setShatterInterest(ShatterInterest si);

		/**
		 * \brief sets the gravity
		 */
		void setGravity(const Vec3 & gravity);

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		/**
		 * \brief performs raytest with given parameters, only one periodic ray for each direction possible // FIXME: (Daniel) not really nice
		 *
		 * \param[in] to vector, the ray should be send to, relative to current position
		 * \param[in] rtr repetition behavior for this ray
		 * \param[in] rtn notify behavior for this ray
		 * \param[in] msg message to be send on notify
		 */
		void rayTest(const Vec3 & from, const Vec3 & to, RayTestRepetition rtr, RayTestNotify rtn, const GameMessage::Ptr & msg);

		std::string getTemplateName() const override {
			return "PhysicalState";
		}

		/**
		 * \brief adds the given position to the real position
		 */
		void addPosition(const Vec3 & pos);

		std::vector<componentOptions> getComponentOptions() override;

	private:
		/**
		 * \brief Stores the position of the object.
		 */
		Vec3 _position;
		Vec3 _positionNew;
		uint32_t _posDirty;
		uint32_t _rotDirty;
		uint32_t _speedDirty;

		/**
		 * \brief Stores the rotation of the object.
		 */
		Quaternion _rotation;
		Quaternion _rotationNew;

		/**
		 * \brief Stores the scale of the object.
		 */
		Vec3 _scale;
		Vec3 _scaleNew;
		uint32_t _scaleDirty;

		/**
		 * \brief linear velocity of the object
		 */
		Vec3 _linearVelocity;
		Vec3 _speedNew;

		/**
		 * \brief forces manipulating the object
		 * each tuple represents one force being applied in the next frame:
		 * <Force, Offset, isLocalSpace>
		 */
		std::vector<std::tuple<Vec3, Vec3, bool>> _forces;

		/**
		 * \brief Stores the gravity for this object
		 */
		Vec3 _gravity;

		/**
		 * \brief stores the collision information
		 */
		CollisionGroup _collisionGroup;

		/**
		 * \brief form of the CollisionShape
		 */
		ShapeType _shapeType;

		/**
		 * \brief paramaters for the collision shape
		 */
		attributeMap _shapeParams;

		/**
		 * \brief whether this component was already initialized
		 */
		// TODO (Michael) maybe move this to Component base class?
		bool _initialized;

		/**
		 * \brief flag containing shatter interests
		 */
		ShatterInterest _shatterInterest;

		/**
		 * \brief for locking the API
		 */
		mutable boost::mutex _lock;

		uint32_t _syncPrio;

		/**
		 * \brief sends initialising messages
		 * call this after you set all starting values
		 */
		void Init() override;

		void Finalize() override;

		/**
		 * \brief processes a msg
		 */
		void News(const GameMessage::Ptr & msg) override;

		/**
		 * \brief ticks the component
		 */
		void Tick() override;

		virtual std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_PHYSICALSTATECOMPONENT_H__ */

/**
 * @}
 */
