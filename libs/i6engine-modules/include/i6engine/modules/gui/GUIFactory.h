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
 * \addtogroup GUI
 * @{
 */

#ifndef __I6ENGINE_MODULES_GUIFACTORY_H__
#define __I6ENGINE_MODULES_GUIFACTORY_H__

#include <map>
#include <string>

#include "boost/function.hpp"

namespace i6e {
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
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_GUIFACTORY_H__ */

/**
 * @}
 */
