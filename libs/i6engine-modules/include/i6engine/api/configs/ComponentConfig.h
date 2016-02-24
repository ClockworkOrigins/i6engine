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

#ifndef __I6ENGINE_API_COMPONENTCONFIG_H__
#define __I6ENGINE_API_COMPONENTCONFIG_H__

#include "i6engine/utils/sharedPtr.h"

#include "i6engine/api/GameMessageStruct.h"
#include "i6engine/api/configs/attributeMap.h"

#include "boost/function.hpp"

namespace i6engine {
namespace api {

	class Component;

	typedef utils::sharedPtr<Component, Component> ComPtr;

	enum ShatterInterest : uint16_t;

namespace components {
	enum ComponentMessageTypes {
		ComReset = 0,
		ComCreate,			//! Creates a new Component
		ComCreateCallback,	//! Creates a new Component and calls callback afterwards
		ComSpawnpoint,		//! Reactivates a spawnpoint
		ComShatter,			//! Indicates, that the Object shattered
		ComMoverResync,		//! Mover Component needs to be resynced
		COUNT
	};
	enum ComponentTypes {
		CameraComponent,
		LifetimeComponent,
		LuminousAppearanceComponent,
		MeshAppearanceComponent,
		MoverCircleComponent,
		MoverComponent,
		MoverInterpolateComponent,
		MovingCameraComponent,
		NetworkSenderComponent,
		ParticleEmitterComponent,
		PhysicalStateComponent,
		ShatterComponent,
		SpawnpointComponent,
		StaticStateComponent,
		TerrainAppearanceComponent,
		SoundComponent,
		SoundListenerComponent,
		BillboardComponent,
		FollowComponent,
		MovableTextComponent,
		WaypointComponent,
		NavigationComponent,
		WaynetNavigationComponent,
		MoveComponent,
		MovementComponent,
		ToggleWaynetComponent,
		Point2PointConstraintComponent,
		VelocityComponent,
		LineComponent,
		AnimatedLuminousAppearanceComponent,
		AnimatedDirectionalLightComponent,
		AnimatedPointLightComponent,
		AnimatedSpotLightComponent,
		AnimationControllerComponent,
		ComponentTypesCount
	};

	/**
	 * \brief creates a Component on Object with id goid and Component id coid with registered template name tpl and attributes of params
	 */
	typedef struct ISIXE_MODULES_API Component_Create_Create : GameMessageStruct {
		std::string tpl;
		attributeMap params;
		core::IPKey receiver;
		Component_Create_Create();
		Component_Create_Create(const int64_t goid, const int64_t coid, const core::IPKey & r, const std::string & t, const attributeMap & p);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
			ar & tpl;
			ar & params;
		}
		Component_Create_Create * copy() { return new Component_Create_Create(*this); }
	} Component_Create_Create;

	/**
	 * \brief creates a Component on Object with id goid and Component id coid with registered template name tpl and attributes of params and calls callback afterwards
	 * \note not for multiplayer!
	 */
	typedef struct ISIXE_MODULES_API Component_CreateCallback_Create : GameMessageStruct {
		std::string tpl;
		attributeMap params;
		core::IPKey receiver;
		boost::function<void(ComPtr)> callback;
		Component_CreateCallback_Create(const int64_t goid, const int64_t coid, const core::IPKey & r, const std::string & t, const attributeMap & p, const boost::function<void(ComPtr)> & cb);
		Component_CreateCallback_Create * copy() {
			return new Component_CreateCallback_Create(*this);
		}
	} Component_CreateCallback_Create;

	/**
	 * \brief deletes Component with familyID famID on GameObject with id goid
	 */
	typedef struct ISIXE_MODULES_API Component_Create_Delete : GameMessageStruct {
		uint32_t famID;
		Component_Create_Delete();
		Component_Create_Delete(const int64_t goid, const int64_t coid, uint32_t fam);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
			ar & famID;
		}
		Component_Create_Delete * copy() { return new Component_Create_Delete(*this); }
	} Component_Create_Delete;

	/**
	 * \brief reactivates a Spawnpoint
	 */
	typedef struct ISIXE_MODULES_API Component_Spawnpoint_Update : GameMessageStruct {
		Component_Spawnpoint_Update();
		Component_Spawnpoint_Update(const int64_t goid, const int64_t coid);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
		}
		Component_Spawnpoint_Update * copy() { return new Component_Spawnpoint_Update(*this); }
	} Component_Spawnpoint_Update;

	/**
	 * \brief calls shatter method of given object with id of the other object detected during collision
	 */
	typedef struct ISIXE_MODULES_API Component_Shatter_Update : GameMessageStruct {
		int64_t other;
		ShatterInterest shatterInterest;
		Component_Shatter_Update();
		Component_Shatter_Update(const int64_t goid, const int64_t coid, const int64_t o, ShatterInterest si);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
			ar & other;
			ar & shatterInterest;
		}
		Component_Shatter_Update * copy() { return new Component_Shatter_Update(*this); }
	} Component_Shatter_Update;

	/**
	 * \brief used to resynchronize MoverComponents
	 */
	typedef struct ISIXE_MODULES_API Component_MoverResync_Update : GameMessageStruct {
		std::map<std::string, std::string> attMap;
		Component_MoverResync_Update();
		Component_MoverResync_Update(const int64_t goid, const int64_t coid, std::map<std::string, std::string> am);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
			ar & attMap;
		}
		Component_MoverResync_Update * copy() { return new Component_MoverResync_Update(*this); }
	} Component_MoverResync_Update;

} /* namespace components */
} /* namespace api */
} /* namespace i6engine */

BOOST_CLASS_EXPORT_KEY(i6engine::api::components::Component_Create_Create)
BOOST_CLASS_EXPORT_KEY(i6engine::api::components::Component_Create_Delete)
BOOST_CLASS_EXPORT_KEY(i6engine::api::components::Component_Spawnpoint_Update)
BOOST_CLASS_EXPORT_KEY(i6engine::api::components::Component_Shatter_Update)
BOOST_CLASS_EXPORT_KEY(i6engine::api::components::Component_MoverResync_Update)

#endif /* __I6ENGINE_API_COMPONENTCONFIG_H__ */

/**
 * @}
 */
