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

#include "i6engine/api/manager/LanguageManager.h"

namespace i6e {
namespace api {

	LanguageManager::LanguageManager() : _language("Deutsch"), _callbacks() {
	}

	void LanguageManager::setLanguage(const std::string & language) {
		if (language != _language) {
			_language = language;

			for (auto & cb : _callbacks) {
				cb(_language);
			}
		}
	}

	void LanguageManager::updateSubscribers() {
		for (auto & cb : _callbacks) {
			cb(_language);
		}
	}

} /* namespace api */
} /* namespace i6e */
