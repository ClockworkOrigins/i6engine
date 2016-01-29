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
 * \addtogroup Object
 * @{
 */

#ifndef __I6ENGINE_API_OBJECTCONFIG_H__
#define __I6ENGINE_API_OBJECTCONFIG_H__

#include "i6engine/utils/sharedPtr.h"

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/GameMessage.h"
#include "i6engine/api/GameMessageStruct.h"

#include "boost/function.hpp"

namespace i6engine {
namespace api {

	class Component;
	class GameObject;

	typedef std::map<std::string, std::string> attributeMap;
	typedef utils::sharedPtr<Component, Component> ComPtr;
	typedef boost::function<ComPtr(const int64_t, const attributeMap &)> createGOCCallback;
	typedef utils::sharedPtr<GameObject, GameObject> GOPtr;
	typedef void (*CreateGO)(GOPtr, const attributeMap &);

namespace objects {
	enum ObjectMessageTypes {
		ObjReset = 0,
		ObjCreate,
		ObjCreateAndCall,
		ObjLevel,
		ObjType,
		ObjRegisterCTemplate,
		ObjTicker,
		ObjInsert,
		ObjState,
		ObjKillMe,
		ObjConditionalMessage,
		Position,
		ObjClean,
		ObjPause
	};

	/**
	 * \brief internal representation of a Component within a GameObject template
	 */
	typedef struct ISIXE_MODULES_API GOTemplateComponent {
		std::string _template;
		int64_t _id;
		attributeMap _params;
		bool _owner;
		std::string _identifier;
		bool _deleted;

		GOTemplateComponent() : _template(), _id(-1), _params(), _owner(false), _identifier(), _deleted(false) {
		}

		GOTemplateComponent(const std::string & t, int64_t i, const attributeMap & p, bool b, const std::string & id, bool deleted) : _template(t), _id(i), _params(p), _owner(b), _identifier(id), _deleted(false) {
		}

		GOTemplateComponent(const std::string & t, const attributeMap & p, const std::string & id, bool deleted, bool owner) : _template(t), _id(-1), _params(p), _owner(owner), _identifier(id), _deleted(deleted) {
		}

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & _template;
			ar & _id;
			ar & _params;
			ar & _owner;
			ar & _identifier;
			ar & _deleted;
		}
	} GOTemplateComponent;

	/**
	 * \brief internal representation of all GameObject templates
	 */
	typedef struct ISIXE_MODULES_API GOTemplate {
		std::string _type; // Type of the GO
		std::vector<GOTemplateComponent> _components; // template name, id, params, owner
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & _type;
			ar & _components;
		}
	} GOTemplate;

	/**
	 * \brief creates a GameObject out of a template file
	 */
	typedef struct ISIXE_MODULES_API Object_Create_Create : GameMessageStruct {
		std::string tpl;
		core::IPKey owner;
		uint64_t uuid;
		GOTemplate tmpl;
		bool send;
		Object_Create_Create();
		Object_Create_Create(const int64_t i, const std::string & t, const core::IPKey & own, uint64_t u, const GOTemplate & g, const bool b);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
			ar & tpl;
			ar & owner;
			ar & uuid;
			ar & tmpl;
			ar & send;
		}
		Object_Create_Create * copy() { return new Object_Create_Create(*this); }
	} Object_Create_Create;

	/**
	 * \brief deletes a GameObject
	 */
	typedef struct ISIXE_MODULES_API Object_Create_Delete : GameMessageStruct {
		core::IPKey owner;
		Object_Create_Delete();
		Object_Create_Delete(const int64_t i, const core::IPKey & o);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
			ar & owner;
		}
		Object_Create_Delete * copy() { return new Object_Create_Delete(*this); }
	} Object_Create_Delete;

	/**
	 * \brief loads a level
	 */
	typedef struct ISIXE_MODULES_API Object_Level_Create : GameMessageStruct {
		std::string file;
		std::string flags;
		std::function<void(uint16_t)> callback;
		std::function<void(void)> func;
		Object_Level_Create();
		Object_Level_Create(const std::string & fi, const std::string & fl, const std::function<void(uint16_t)> & cb, const std::function<void(void)> & fu);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
			ar & file;
			ar & flags;
		}
		Object_Level_Create * copy() { return new Object_Level_Create(*this); }
	} Object_Level_Create;

	/**
	 * \brief deletes all objects of given type
	 */
	typedef struct ISIXE_MODULES_API Object_Type_Delete : GameMessageStruct {
		std::string type;
		Object_Type_Delete();
		Object_Type_Delete(const std::string & t);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
			ar & type;
		}
		Object_Type_Delete * copy() { return new Object_Type_Delete(*this); }
	} Object_Type_Delete;

	/**
	 * \brief struct for registering component templates
	 * never send through network
	 */
	typedef struct ISIXE_MODULES_API Object_RegisterCTemplate_Create : GameMessageStruct {
		std::string name;
		createGOCCallback func;
		Object_RegisterCTemplate_Create();
		Object_RegisterCTemplate_Create(const std::string & n, const createGOCCallback & f);
		Object_RegisterCTemplate_Create * copy() { return new Object_RegisterCTemplate_Create(*this); }
	} Object_RegisterCTemplate_Create;

	/**
	 * \brief struct for saying the Object subsystem to send its state through network
	 * never send through network
	 */
	typedef struct ISIXE_MODULES_API Object_State_Update : GameMessageStruct {
		core::IPKey receiver;
		Object_State_Update();
		Object_State_Update(const core::IPKey & r);
		Object_State_Update * copy() { return new Object_State_Update(*this); }
	} Object_State_Update;

	/**
	 * \brief struct for creation of a GameObject and calling a callback with it
	 * never send through network
	 */
	typedef struct ISIXE_MODULES_API Object_CreateAndCall_Create : GameMessageStruct {
		std::string tpl;
		core::IPKey owner;
		uint64_t uuid;
		GOTemplate tmpl;
		bool send;
		boost::function<void(const GOPtr &)> func;
		Object_CreateAndCall_Create();
		Object_CreateAndCall_Create(const int64_t i, const std::string & t, const core::IPKey & own, uint64_t u, const GOTemplate & g, const bool b, const boost::function<void(const GOPtr &)> & f);
		Object_CreateAndCall_Create * copy() { return new Object_CreateAndCall_Create(*this); }
	} Object_CreateAndCall_Create;

	/**
	 * \brief struct for sending messages to special GameObjects fullfilling the given method
	 * never send through network
	 */
	typedef struct ISIXE_MODULES_API Object_ConditionalMessage_Update : GameMessageStruct {
		GameMessage::Ptr msg;
		boost::function<bool(const GOPtr &)> func;
		bool sync;
		uint32_t comFamID;
		Object_ConditionalMessage_Update();
		Object_ConditionalMessage_Update(const GameMessage::Ptr & m, const boost::function<bool(const GOPtr &)> & f, bool s, uint32_t c);
		Object_ConditionalMessage_Update * copy() { return new Object_ConditionalMessage_Update(*this); }
	} Object_ConditionalMessage_Update;

	/**
	 * \brief struct for deletion of GameObject using ConditionalMessage
	 * never send through network
	 */
	typedef struct ISIXE_MODULES_API Object_KillMe_Update : GameMessageStruct {
		Object_KillMe_Update();
		Object_KillMe_Update * copy() { return new Object_KillMe_Update(*this); }
	} Object_KillMe_Update;

	/**
	 * \brief message for creation of new Position
	 */
	typedef struct ISIXE_MODULES_API Position_Update : GameMessageStruct {
		Vec3 pos;
		Quaternion rot;
		Vec3 scale;

		Position_Update();
		Position_Update(const int64_t i, const int64_t ci, const Vec3 & p, const Quaternion & r, const Vec3 & s);
		Position_Update * copy() { return new Position_Update(*this); }
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
			ar & pos;
			ar & rot;
			ar & scale;
		}
	} Position_Update;

	/**
	 * \brief struct for deletion of all GameObjects
	 * never send through network
	 */
	typedef struct ISIXE_MODULES_API Object_Clean_Delete : GameMessageStruct {
		Object_Clean_Delete();
		Object_Clean_Delete * copy() {
			return new Object_Clean_Delete(*this);
		}
	} Object_Clean_Delete;

	/**
	 * \brief pauses the subsystem
	 * never send through network
	 */
	typedef struct ISIXE_MODULES_API Object_Pause_Update : GameMessageStruct {
		bool pause;
		Object_Pause_Update(bool p) : pause(p) {
		}
		Object_Pause_Update * copy() {
			return new Object_Pause_Update(*this);
		}
	} Object_Pause_Update;

} /* namespace objects */
} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_OBJECTCONFIG_H__ */

/**
 * @}
 */
