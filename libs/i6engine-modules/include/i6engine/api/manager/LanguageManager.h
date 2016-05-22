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
 * \addtogroup API
 * @{
 */

#ifndef __I6ENGINE_API_LANGUAGEMANAGER_H__
#define __I6ENGINE_API_LANGUAGEMANAGER_H__

#include <functional>
#include <string>
#include <vector>

#include "i6engine/utils/i6eSystemParameters.h"

namespace i6e {
namespace api {

	/**
	 * \brief handles the current language of the Game
	 * a change of the language notifies all registered callbacks
	 */
	class ISIXE_MODULES_API LanguageManager {
	public:
		LanguageManager();

		/**
		 * \brief adds a callback to be called on language change
		 */
		void addCallback(const std::function<void(const std::string &)> & callback) {
			_callbacks.push_back(callback);
		}

		/**
		 * \brief changes language
		 */
		void setLanguage(const std::string & language);

		/**
		 * \brief updates all subscribers
		 * might be necessary if your system depends on setting the value through language manager but is initialized afterwards as most things will be
		 */
		void updateSubscribers();

	private:
		std::string _language;
		std::vector<std::function<void(const std::string &)>> _callbacks;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_LANGUAGEMANAGER_H__ */

/**
 * @}
 */
