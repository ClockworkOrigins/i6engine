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

#ifndef __I6ENGINE_API_TEXTMANAGER_H__
#define __I6ENGINE_API_TEXTMANAGER_H__

#include <map>
#include <string>

#include "i6engine/utils/i6eSystemParameters.h"

namespace i6e {
namespace api {

	class EngineController;

	/**
	 * \brief handles Texts for the Game
	 */
	class ISIXE_MODULES_API TextManager {
		friend class EngineController;

	public:
		TextManager();

		/**
		 * \brief returns the text in the current language for the given key
		 */
		std::string getText(const std::string & key) const;

	private:
		std::string _language;
		std::map<std::string, std::string> _texts;
		std::string _textDir;
		
		void setLanguage(const std::string & language) {
			_language = language;
			_texts.clear();
			initialize(_textDir);
		}

		void initialize(const std::string & textDir);
		void loadFile(const std::string & file);
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_TEXTMANAGER_H__ */

/**
 * @}
 */
