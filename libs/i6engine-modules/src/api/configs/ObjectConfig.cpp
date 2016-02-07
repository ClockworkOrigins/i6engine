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

#include "i6engine/api/configs/ObjectConfig.h"

namespace i6engine {
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
	Object_CreateAndCall_Create::Object_CreateAndCall_Create(const int64_t i, const std::string & t, const core::IPKey & own, uint64_t u, const GOTemplate & g, const bool b, const boost::function<void(const GOPtr &)> & f) : GameMessageStruct(i, -1), tpl(t), owner(own), uuid(u), tmpl(g), send(b), func(f) {
	}

	Object_ConditionalMessage_Update::Object_ConditionalMessage_Update() {}
	Object_ConditionalMessage_Update::Object_ConditionalMessage_Update(const GameMessage::Ptr & m, const boost::function<bool(const GOPtr &)> & f, bool s, uint32_t c) : GameMessageStruct(), msg(m), func(f), sync(s), comFamID(c) {}

	Object_KillMe_Update::Object_KillMe_Update() {}

	Position_Update::Position_Update() : GameMessageStruct(), pos(), rot(), scale() {}
	Position_Update::Position_Update(const int64_t i, const int64_t ci, const Vec3 & p, const Quaternion & r, const Vec3 & s) : GameMessageStruct(ci, i), pos(p), rot(r), scale(s) {
	}

	Object_Clean_Delete::Object_Clean_Delete() {
	}

} /* namespace objects */
} /* namespace api */
} /* namespace i6engine */

BOOST_CLASS_EXPORT_GUID(i6engine::api::objects::Object_Create_Create, "O1")
BOOST_CLASS_IMPLEMENTATION(i6engine::api::objects::Object_Create_Create, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT_GUID(i6engine::api::objects::Object_Create_Delete, "O2")
BOOST_CLASS_IMPLEMENTATION(i6engine::api::objects::Object_Create_Delete, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT_GUID(i6engine::api::objects::Object_Level_Create, "O3")
BOOST_CLASS_IMPLEMENTATION(i6engine::api::objects::Object_Level_Create, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT_GUID(i6engine::api::objects::Object_Type_Delete, "O4")
BOOST_CLASS_IMPLEMENTATION(i6engine::api::objects::Object_Type_Delete, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT_GUID(i6engine::api::objects::Position_Update, "O5")
BOOST_CLASS_IMPLEMENTATION(i6engine::api::objects::Position_Update, boost::serialization::object_serializable)
