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
 * \addtogroup RPG
 * @{
 */

#ifndef __I6ENGINE_RPG_DIALOG_DIALOGPARSER_H__
#define __I6ENGINE_RPG_DIALOG_DIALOGPARSER_H__

#include <map>
#include <string>

#include "i6engine/utils/i6eSystemParameters.h"

namespace i6e {
namespace rpg {
namespace dialog {

	struct Dialog;

	/**
	 * \brief loads and stores all dialog files
	 */
	class ISIXE_RPG_API DialogParser {
		friend class DialogManager;

	public:
		DialogParser();
		~DialogParser();

		/**
		 * \brief load all dialogs from given directory and subdirectories
		 */
		void loadDialogs(const std::string & directory);

	private:
		std::map<std::string, Dialog *> _dialogs;

		void loadDialog(const std::string & file);
	};

} /* namespace dialog */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_DIALOG_DIALOGPARSER_H__ */

/**
 * @}
 */
