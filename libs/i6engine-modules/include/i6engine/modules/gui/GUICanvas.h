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

#ifndef __I6ENGINE_MODULES_GUICANVAS_H__
#define __I6ENGINE_MODULES_GUICANVAS_H__

#include <map>
#include <string>

namespace tinyxml2 {
	class XMLElement;
} /* namespace tinyxml2 */

namespace i6e {
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
		 * \brief loads canvas file
		 */
		static void load(const std::string & name, const std::string & file, GUIManager * manager, const GUIFactory & factory, std::map<std::string, api::GUIWidget *> & widgets);

		/**
		 * \brief internal helper walking down the XML tree
		 */
		static void load(const std::string & name, const std::string & file, GUIManager * manager, const GUIFactory & factory, std::map<std::string, api::GUIWidget *> & widgets, tinyxml2::XMLElement * node);
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_GUICANVAS_H__ */

/**
 * @}
 */
