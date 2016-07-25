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

#include "i6engine/api/configs/ObjectConfig.h"

namespace i6e {
namespace api {
namespace objects {

	Object_Create_Create::Object_Create_Create() {}
	Object_Create_Create::Object_Create_Create(const int64_t i, const std::string & t, const core::IPKey & own, uint64_t u, const GOTemplate & g, const bool b) : GameMessageStruct(i, -1), tpl(t), owner(own), uuid(u), tmpl(g), send(b) {
	}

	Object_Create_Delete::Object_Create_Delete() {}
	Object_Create_Delete::Object_Create_Delete(const int64_t i, const core::IPKey & o) : GameMessageStruct(i, -1), owner(o) {
	}

	Object_Level_Create::Object_Level_Create() {}
	Object_Level_Create::Object_Level_Create(const std::string & fi, const std::string & fl, const std::function<void(uint16_t)> & cb, const std::function<void(void)> & fu) : GameMessageStruct(), file(fi), flags(fl), callback(cb), func(fu) {
	}

	Object_Type_Delete::Object_Type_Delete() {}
	Object_Type_Delete::Object_Type_Delete(const std::string & t) : GameMessageStruct(), type(t) {}

	Object_RegisterCTemplate_Create::Object_RegisterCTemplate_Create() {}
	Object_RegisterCTemplate_Create::Object_RegisterCTemplate_Create(const std::string & n, const createGOCCallback & f) : GameMessageStruct(), name(n), func(f) {}

	Object_State_Update::Object_State_Update() {}
	Object_State_Update::Object_State_Update(const core::IPKey & r) : GameMessageStruct(), receiver(r) {
	}

	Object_CreateAndCall_Create::Object_CreateAndCall_Create() {}
	Object_CreateAndCall_Create::Object_CreateAndCall_Create(const int64_t i, const std::string & t, const core::IPKey & own, uint64_t u, const GOTemplate & g, const bool b, const std::function<void(const GOPtr &)> & f) : GameMessageStruct(i, -1), tpl(t), owner(own), uuid(u), tmpl(g), send(b), func(f) {
	}

	Object_ConditionalMessage_Update::Object_ConditionalMessage_Update() {}
	Object_ConditionalMessage_Update::Object_ConditionalMessage_Update(const GameMessage::Ptr & m, const std::function<bool(const GOPtr &)> & f, bool s, uint32_t c) : GameMessageStruct(), msg(m), func(f), sync(s), comFamID(c) {}

	Object_KillMe_Update::Object_KillMe_Update() {}

	Position_Update::Position_Update() : GameMessageStruct(), pos(), rot(), scale() {}
	Position_Update::Position_Update(const int64_t i, const int64_t ci, const Vec3 & p, const Quaternion & r, const Vec3 & s) : GameMessageStruct(ci, i), pos(p), rot(r), scale(s) {
	}

	Object_Clean_Delete::Object_Clean_Delete() {
	}

} /* namespace objects */
} /* namespace api */
} /* namespace i6e */

BOOST_CLASS_EXPORT_GUID(i6e::api::objects::Object_Create_Create, "O1")
BOOST_CLASS_IMPLEMENTATION(i6e::api::objects::Object_Create_Create, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT_GUID(i6e::api::objects::Object_Create_Delete, "O2")
BOOST_CLASS_IMPLEMENTATION(i6e::api::objects::Object_Create_Delete, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT_GUID(i6e::api::objects::Object_Level_Create, "O3")
BOOST_CLASS_IMPLEMENTATION(i6e::api::objects::Object_Level_Create, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT_GUID(i6e::api::objects::Object_Type_Delete, "O4")
BOOST_CLASS_IMPLEMENTATION(i6e::api::objects::Object_Type_Delete, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT_GUID(i6e::api::objects::Position_Update, "O5")
BOOST_CLASS_IMPLEMENTATION(i6e::api::objects::Position_Update, boost::serialization::object_serializable)
