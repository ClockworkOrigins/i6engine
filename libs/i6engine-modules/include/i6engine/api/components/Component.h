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

#ifndef __I6ENGINE_API_COMPONENT_H__
#define __I6ENGINE_API_COMPONENT_H__

#include <map>
#include <vector>

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/i6eThreadSafety.h"
#include "i6engine/utils/Singleton.h"
#include "i6engine/utils/weakPtr.h"
#include "i6engine/utils/EnumClassStream.h"

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"
#include "i6engine/math/i6eVector4.h"

#include "i6engine/api/GameMessage.h"
#include "i6engine/api/configs/attributeMap.h"

#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/lexical_cast.hpp"

namespace i6e {
namespace api {

	class Component;
	class GameObject;

	typedef utils::sharedPtr<Component, Component> ComPtr;
	typedef utils::sharedPtr<GameObject, GameObject> GOPtr;
	typedef utils::weakPtr<Component> WeakComPtr;
	typedef utils::weakPtr<GameObject> WeakGOPtr;
	typedef boost::function<ComPtr(const int64_t, const attributeMap &)> createGOCCallback;

	/**
	 * \brief defines how new components should be handled
	 * \note REPLACE(-1) and REPLACE_DIS both replace the dispatcher component:
	 * use REPLACE_DIS if you only want to allow a single component. REPLACE(-1) assumes there can be some subcomponents in will handle this correctly
	 */
	enum class AddStrategy {
		ADD,			// !< simply add this component
		REPLACE,		// !< replace the component with the specified one (-1 is the dispatcher, 0 is the first additional comp, etc.)
		REPLACE_DIS,	// !< replaces the Dispachter component
		REJECT			// !< don't add this component at all
	};

	/**
	 * \brief defines the access state of the components objects
	 */
	enum class AccessState {
		READONLY,	// !< option is only for information of the user
		READWRITE	// !< option can be changed in editor
	};

	/**
	 * \brief enum for easy access of the parameters in the std::tuple
	 */
	enum ComponentOptionsParameter {
		ACCESSSTATE = 0,
		NAME,
		READFUNC,
		WRITEFUNC,
		WIDGETTYPE
	};

	typedef std::tuple<AccessState, std::string, boost::function<std::string(void)>, boost::function<bool(std::string)>, std::string> componentOptions;

	/**
	 * \class Component
	 * \brief Component Base Class. All Components must derive from Component.
	 */
	class ISIXE_MODULES_API Component {
		friend class GameObject;

	public:
		/**
		 * \brief Constructor for Component.
		 */
		Component();

		/**
		 * \brief Constructor for Component.
		 */
		Component(const int64_t id, const attributeMap & params);

		/**
		 * \brief Destructor of Component. Must be implemented in derived class.
		 */
		virtual ~Component();

		template<typename T>
		static ComPtr createC(const int64_t id, const attributeMap & params) {
			return utils::make_shared<T, Component>(id, params);
		}

		/**
		 * \brief Sets the GameObject that owns this Component
		 * \param[in] objGO type of GameObject*
		 */
		void setOwnerGO(const WeakGOPtr & objGo);

		/**
		 * \brief Get the GameObject that owns this Component.
		 * \return A pointer to the GameObject
		 */
		GOPtr getOwnerGO();

		/**
		 * \brief Returns the component identification string
		 * \return name of the component
		 */
		uint32_t getComponentID() const {
			return _objComponentID;
		}

		/**
		 * \brief Returns the family identification string
		 * \return name of the family
		 */
		uint32_t getFamilyID() const {
			return _objFamilyID;
		}

		/**
		 * \brief Returns the family identification string
		 * \return name of the family
		 */
		std::string getIdentifier() const { return _identifier; }

		/**
		 * \brief Components can have Tick method like normal Subsystems
		 * this method is called by the ObjectController during every Tick if the component registered itself at the ObjectFacade
		 */
		virtual void Tick() {}

		/**
		 * \brief component will be deleted soon
		 */
		void setDie() const;

		/**
		 * \brief returns the id of the component
		 */
		inline int64_t getID() const {
			return _id;
		}

		/**
		 * \brief Receives messages for this component
		 * override this function, if you expect messages
		 */
		virtual void News(const GameMessage::Ptr & msg);

		/**
		 * \brief initializes the Component
		 */
		virtual void Init() = 0;

		/**
		 * \brief finalizes the component
		 */
		virtual void Finalize() {}

		/**
		 * \brief synchronizes the Components state
		 */
		virtual attributeMap synchronize() const = 0;

		/**
		 * \brief sets whether this object has to be synchronized or not
		 */
		void setSync(bool b) { _sync = b; }
		bool getSync() const { return _sync; }

		/**
		 * \brief triggers doEnableTicking threadsafe
		 */
		void enableTicking(bool allowTicking) const;

		/**
		 * \brief asks, how to handle a new component with the same type
		 * \param[in] comp component that shall be added
		 * \returns pair containing the strategy to use as well as an additional value
		 */
		virtual std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const;

		/**
		 * \brief sets weak_ptr on self for registration of ticker
		 */
		void setSelf(const WeakComPtr & self);

		/**
		 * \brief returns the name this template was registered with
		 */
		virtual std::string getTemplateName() const = 0;

		/**
		 * \brief returns a vector containing all options of the Component being readable and writeable in the editor
		 */
		virtual std::vector<componentOptions> getComponentOptions() = 0;

		/**
		 * \brief returns all SubComponents attached to this Component
		 */
		std::vector<ComPtr> getSubComponents() const {
			return _subComps;
		}

	protected:
		/**
		 * \brief ID of the GameObject that owns this Component
		 */
		int64_t _objOwnerID;

		/**
		 * \brief Owning GameObject
		 */
		WeakGOPtr _objOwnerGO;

		/**
		 * \brief Component ID of the Component
		 */
		uint32_t _objComponentID;

		/**
		 * \brief Family ID of the Component
		 */
		uint32_t _objFamilyID;

		/**
		 * \brief vector containing the other componetns of the same type if this is the dispatcher component
		 */
		std::vector<ComPtr> _subComps;

		/**
		 * \brief identifies the subcomponent
		 */
		std::string _identifier;

		/**
		 * \brief id of this component
		 */
		int64_t _id;

		bool _sync;

		/**
		 * \brief a weak_ptr on the own shared_ptr
		 */
		WeakComPtr _self;

		/**
		 * \brief status if this object is allowed to tick or not
		 */
		bool _tickingAllowed;

		/**
		 * \brief stores if a Component which isn't allowed to tick want to tick, so it can start ticking when ticking will be allowed
		 */
		bool _wantsToTick;

		/**
		 * \brief stores whether this Component is actually ticking
		 */
		bool _isTicking;

		/**
		 * \brief adds this component to ticklist
		 */
		void addTicker();

		/**
		 * \brief removes this component from ticklist
		 */
		void removeTicker();
		
		/**
		 * \brief sets ticking status for this Component
		 * per default all Components are allowed to tick and calling addTicker will start ticking, but in some cases, e.g. for level editor, ticking has to be disabled and just a few components are allowed to tick
		 */
		void doEnableTicking(bool allowTicking);

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_COMPONENT_H__ */

/**
 * @}
 */
