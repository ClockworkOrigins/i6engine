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

#ifndef __I6ENGINE_MODULES_GUIFACTORY_H__
#define __I6ENGINE_MODULES_GUIFACTORY_H__

#include <map>
#include <string>

#include "boost/function.hpp"

namespace i6engine {
namespace api {
	class GUIWidget;
} /* namespace api */
namespace modules {

	class GUICanvas;
	class GUIManager;

	typedef boost::function<api::GUIWidget * (const std::string & name, const std::string & style)> GUIWidget_factory;

	/**
	 * \class GUIFactory
	 * \brief This class provides methods for creating different kinds of GUI elements
	 */
	class GUIFactory {
		friend class GUICanvas;
		friend class GUIManager;

	private:
		std::map<std::string, GUIWidget_factory> _templates;

		/**
		 * \brief constructor
		 */
		GUIFactory();

		/**
		 * \brief destructor
		 */
		~GUIFactory();

		/**
		 * \brief creates a new GUIWidget.
		 * \param[in] name The name of the Widget
		 * \param[in] type The typename registered via registerGUITemplate
		 * \param[in] style Image
		 */
		api::GUIWidget * createGUIWidget(const std::string & name, const std::string & type, const std::string & style) const;

		/**
		 * \brief Registers a template to create new Widgets
		 * \param[in] name The name of the template (i.e. the type parameter in createGUIWidget)
		 * \param[in] createFunc A boost::function, which takes a name and a style to create a new GUIWidget.
		 *  Tip: You can use boost::factory to register a GUIWidget constructor as a template
		 */
		void registerGUIWidgetTemplate(const std::string & name, const GUIWidget_factory & createFunc);

		/**
		 * \brief forbidden
		 */
		GUIFactory(const GUIFactory &) = delete;

		/**
		 * \brief forbidden
		 */
		GUIFactory & operator=(const GUIFactory &) = delete;
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_GUIFACTORY_H__ */

/**
 * @}
 */
