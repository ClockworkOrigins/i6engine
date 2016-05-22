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
 * \addtogroup Object
 * @{
 */

#ifndef __I6ENGINE_API_GAMEOBJECT_H__
#define __I6ENGINE_API_GAMEOBJECT_H__

#include <map>

#include "i6engine/utils/i6eThreadSafety.h"
#include "i6engine/utils/sequence_map.h"
#include "i6engine/utils/weakPtr.h"

#include "i6engine/core/messaging/IPKey.h"

#include "i6engine/api/GameMessage.h"
#include "i6engine/api/components/Component.h"
#include "i6engine/api/configs/attributeMap.h"

#include "boost/function.hpp"

namespace i6e {
namespace modules {
	class ObjectMailbox;
} /* namespace modules */
namespace api {

	class Component;
	class GameObject;

	typedef utils::weakPtr<GameObject> WeakGOPtr;
	typedef utils::sharedPtr<Component, Component> ComPtr;
	typedef utils::sequence_map<int64_t, ComPtr> component_table_t;
	typedef std::pair<int64_t, ComPtr> component_table_pair_t;

	/**
	 * \class GameObject
	 * \brief Class representing a GameObject in the i6engine.
	 *
	 * Every GameObject holds a list of Components characterizing this GameObject. These Components
	 * also can receive messages as they derive from MessageSubsriber.
	 *
	 * \note
	 * GameObject.h defines the types:
	 * \li \c component_table_t is an alias for std::map<const goc_id_type, GOComponent*>.
	 * \li \c component_table_pair_t is an alias for std::pair<const goc_id_type, GOComponent*>.
	 *
	 * Have a look at \link i6engine here \endlink for an overview of typedef's in namespace %i6engine.
	 */
	class ISIXE_MODULES_API GameObject {
	public:
		/**
		 * \brief Constructor for GameObject. Sets the _objGoid with an ID given by GuidServer.
		 * The GameObject registers itself in the ObjectController. So deleting the Object shouldn't be necessary.
		 * \param type Type of the created object
		 */
		GameObject() {
		}
		GameObject(const int64_t goid, const core::IPKey & ownerNode, uint64_t uuid, const std::string & tpl, const boost::function<ComPtr(const int64_t, const std::string &, const attributeMap &, const WeakGOPtr &)> & f);

		/**
		 * \brief Remove all GOCs held by the component table.
		 */
		~GameObject();

		/**
		 * \brief Returns the GUID of the GameObject
		 * \return _objGoid
		 */
		inline int64_t getID() const {
			return _objGoid;
		}

		/**
		 * \brief Returns a Pointer to the searched GOComponent, if it is in the component table
		 * otherwise NULL
		 * \param[in] objFamilyID of type const goc_id_type&
		 * \return the wanted Component's dispatcher
		 */
		ComPtr getGOC(uint32_t famID) const;

		/**
		 * \brief Returns a Pointer to the searched GOComponent, if it is in the component table
		 * otherwise NULL
		 * \param[in] objFamilyID of type const goc_id_type&
		 * \param[in] identifier string identifier of the component
		 * \return the wanted Component
		 */
		ComPtr getGOC(uint32_t famID, const std::string & identifier) const;

		template<typename T>
		utils::sharedPtr<T, api::Component> getGOC(uint32_t famID) const;

		template<typename T>
		utils::sharedPtr<T, api::Component> getGOC(uint32_t famID, const std::string & identifier) const;

		template<typename T>
		utils::sharedPtr<T, api::Component> getGOC() const;

		template<typename T>
		utils::sharedPtr<T, api::Component> getGOC(const std::string & identifier) const;

		/**
		 * \brief Returns a Pointer to the searched GOComponent, if it is in the component table
		 * otherwise nullptr
		 * \param[in] objFamilyID of type const
		 * \return the wanted GOComponent
		 */
		ComPtr getGOCID(const int64_t id) const;

		/**
		 * \brief returns a list of all Components on this GameObject
		 */
		std::vector<ComPtr> getGOCList() const;

		/**
		 * \brief Inserts objNewGOC into the component table.
		 * If a GOComponent of the same type already exists, it will be replaced.
		 * \param[in] objNewGOC a GOComponent* argument
		 * \throws ISIXE_THROW_FAILURE when objNewGOC is NULL
		 */
		void setGOC(const ComPtr & objNewGOC);

		/**
		 * \brief Remove all GOCs held by the component table.
		 */
		void clearGOCs();

		/**
		 * \brief Sends a message to delete this GameObject on the next Tick of the ObjectController.
		 */
		void setDie() const;

		/**
		 * \brief Returns the type of the GameObject
		 */
		inline std::string getType() const { return _type; }

		/**
		 * \brief Sets the type of the GameObject
		 */
		void setType(const std::string & type);

		/**
		 * \brief Deletes a Component of the Game Object
		 * \param[in] famID FamilyID of the component, which you want to delete
		 */
		void deleteGOC(uint32_t famID);

		/**
		 * \brief Deletes a Component of the Game Object
		 * \param[in] famID FamilyID of the component, which you want to delete
		 * \param[in] identifier the string identifier of the special component
		 */
		void deleteGOC(uint32_t famID, const std::string & identifier);

		/**
		 * \brief
		 */
		void sendCreateMessage() const;

		/**
		 * \brief adds synchronize messages for GameObject and all Components of this GO
		 * set param all false if you just want Components with sync flag to be added to message
		 */
		void synchronize(std::vector<GameMessage::Ptr> & messages, bool all) const;

		/**
		 * \brief sends a message to delete the GameObject
		 */
		void sendDeleteMessage() const;

		/**
		 * \brief returns the owner of this gameObject
		 */
		inline core::IPKey getOwner() const { return _owner; }

		/**
		 * \brief processes a message
		 * process messages for GameObjects or forwards messages concerning Components
		 * \params[in] msg Message to be processed
		 */
		void News(const GameMessage::Ptr & msg);

		/**
		 * \brief calls Init for every Component on the GO
		 */
		void initializeComponents();

		/**
		 * \brief returns the uuid of this GO
		 */
		uint64_t getUUID() const {
			return _uuid;
		}

		void setSelf(const WeakGOPtr & self) {
			_self = self;
		}

		/**
		 * \brief stores the flags for which this GO is visible
		 */
		void setFlags(const std::vector<std::string> & flags) {
			_flags = flags;
		}

		std::vector<std::string> getFlags() const {
			return _flags;
		}

		/**
		 * \brief stores whether this object has to be synchronized or not
		 */
		void setSend(bool send) {
			_send = send;
		}

		bool getSend() const {
			return _send;
		}

	private:
		std::string _type;

		/**
		 * \brief Unique identifier for this GameObject.
		 */
		int64_t _objGoid;

		/**
		 * \brief Map of all components held by this GameObject.
		 */
		component_table_t _objComponents;

		std::string _template;

		/**
		 * \brief map containing the latest messages for synchronising on new clients
		 */
		std::map<std::string, GameMessage::Ptr> _messages;

		core::IPKey _owner;

		uint64_t _uuid;

		/**
		 * \brief callback for creation of Components
		 */
		boost::function<ComPtr(const int64_t, const std::string &, const attributeMap &, const WeakGOPtr &)> _componentFunc;

		WeakGOPtr _self;

		std::vector<std::string> _flags;
		bool _send;

		ASSERT_THREAD_SAFETY_HEADER
	};

	template<typename T>
	utils::sharedPtr<T, Component> GameObject::getGOC(uint32_t famID) const {
		component_table_t::const_iterator it = _objComponents.find(famID);
		// GOComponent not found
		if (it == _objComponents.end()) {
			return utils::sharedPtr<T, Component>();
		}

		// Return a pointer to the found GOComponent
		return utils::dynamic_pointer_cast<T>(it->second);
	}

	template<typename T>
	utils::sharedPtr<T, Component> GameObject::getGOC(uint32_t famID, const std::string & identifier) const {
		component_table_t::const_iterator it = _objComponents.find(famID);
		// GOComponent not found
		if (it == _objComponents.end()) {
			return utils::sharedPtr<T, Component>();
		}

		if (it->second->getIdentifier() != identifier) {
			for (ComPtr & c : it->second->_subComps) {
				if (c->getIdentifier() == identifier) {
					return utils::dynamic_pointer_cast<T>(c);
				}
			}

			return utils::sharedPtr<T, Component>();
		}

		// Return a pointer to the found GOComponent
		return utils::dynamic_pointer_cast<T>(it->second);
	}

	template<typename T>
	utils::sharedPtr<T, Component> GameObject::getGOC() const {
		for (const std::pair<int64_t, ComPtr> & com : _objComponents) {
			auto c = utils::dynamic_pointer_cast<T>(com.second);
			if (c != nullptr) {
				return c;
			}
		}

		// Return a pointer to the found GOComponent
		return utils::sharedPtr<T, Component>();
	}

	template<typename T>
	utils::sharedPtr<T, Component> GameObject::getGOC(const std::string & identifier) const {
		for (const std::pair<int64_t, ComPtr> & com : _objComponents) {
			auto co = utils::dynamic_pointer_cast<T>(com.second);
			if (co != nullptr) {
				for (ComPtr & c : co->_subComps) {
					if (c->getIdentifier() == identifier) {
						return utils::dynamic_pointer_cast<T>(c);
					}
				}
			}
		}

		// Return a pointer to the found GOComponent
		return utils::sharedPtr<T, Component>();
	}

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_GAMEOBJECT_H__ */

/**
 * @}
 */
