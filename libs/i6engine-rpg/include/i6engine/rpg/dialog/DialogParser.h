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
