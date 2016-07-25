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

#ifndef __I6ENGINE_API_COMPONENTCONFIG_H__
#define __I6ENGINE_API_COMPONENTCONFIG_H__

#include "i6engine/utils/sharedPtr.h"

#include "i6engine/api/GameMessageStruct.h"
#include "i6engine/api/configs/attributeMap.h"

#include "boost/function.hpp"

namespace i6e {
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
		ComEnableTicking,	//! Enables or disables ticking of a Component
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
		void serialize(Archive & ar, const unsigned int) {
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
		std::function<void(ComPtr)> callback;
		Component_CreateCallback_Create(const int64_t goid, const int64_t coid, const core::IPKey & r, const std::string & t, const attributeMap & p, const std::function<void(ComPtr)> & cb);
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
		void serialize(Archive & ar, const unsigned int) {
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
		void serialize(Archive & ar, const unsigned int) {
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
		void serialize(Archive & ar, const unsigned int) {
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
		void serialize(Archive & ar, const unsigned int) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
			ar & attMap;
		}
		Component_MoverResync_Update * copy() { return new Component_MoverResync_Update(*this); }
	} Component_MoverResync_Update;

	/**
	 * \brief enables or disables ticking of a Component
	 */
	typedef struct Component_EnableTicking_Update : GameMessageStruct {
		bool allowTicking;
		Component_EnableTicking_Update(const int64_t goid, const int64_t coid, bool at);
		Component_EnableTicking_Update * copy() {
			return new Component_EnableTicking_Update(*this);
		}
	} Component_EnableTicking_Update;

} /* namespace components */
} /* namespace api */
} /* namespace i6e */

BOOST_CLASS_EXPORT_KEY(i6e::api::components::Component_Create_Create)
BOOST_CLASS_EXPORT_KEY(i6e::api::components::Component_Create_Delete)
BOOST_CLASS_EXPORT_KEY(i6e::api::components::Component_Spawnpoint_Update)
BOOST_CLASS_EXPORT_KEY(i6e::api::components::Component_Shatter_Update)
BOOST_CLASS_EXPORT_KEY(i6e::api::components::Component_MoverResync_Update)

#endif /* __I6ENGINE_API_COMPONENTCONFIG_H__ */

/**
 * @}
 */
