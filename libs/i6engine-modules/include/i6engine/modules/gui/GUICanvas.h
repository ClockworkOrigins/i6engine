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
 * \addtogroup GUI
 * @{
 */

#ifndef __I6ENGINE_MODULES_GUICANVAS_H__
#define __I6ENGINE_MODULES_GUICANVAS_H__

#include <map>
#include <string>

namespace tinyxml2 {
	class XMLElement;
} /* namespace tinyxml2 */

namespace i6engine {
namespace api {
	class GUIWidget;
} /* namespace api */
namespace modules {

	class GUIFactory;
	class GUIManager;

	/**
	 * \class GUICanvas
	 * \brief This class provides method to load a canvas file
	 */
	class GUICanvas {
		friend class GUIManager;

	private:
		/**
		 * \brief constructor
		 */
		static void load(const std::string & name, const std::string & file, GUIManager * manager, const GUIFactory & factory, std::map<std::string, api::GUIWidget *> & widgets);

		static void load(const std::string & name, const std::string & file, GUIManager * manager, const GUIFactory & factory, std::map<std::string, api::GUIWidget *> & widgets, tinyxml2::XMLElement * node);
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_GUICANVAS_H__ */

/**
 * @}
 */
