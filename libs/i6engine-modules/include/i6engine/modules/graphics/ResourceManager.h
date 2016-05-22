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
 * \addtogroup Graphics
 * @{
 */

#ifndef __I6ENGINE_MODULES_RESOURCEMANAGER_H__
#define __I6ENGINE_MODULES_RESOURCEMANAGER_H__

#include <string>

#include "i6engine/utils/i6eThreadSafety.h"

namespace i6e {
namespace modules {

	class GraphicsManager;
	class PhysicsManager;

	/**
	 * \class ResourceManager
	 * \brief This class is responsible for loading all required resources into memory.
	 */
	class ISIXE_MODULES_API ResourceManager {
		friend class GraphicsManager;
		friend class PhysicsManager;

	public:
		/**
		 * \brief Loads all resources defined in the configuration file "resources.cfg".
		 *
		 *     loads Ogre's resources by telling it to read the configuration file and then add the entries (addResourceLocation).
		 * Loads all resources defined in the configuration file "resources.cfg".
		 */
		ResourceManager(const std::string & ogrePath);

		/**
		 * \brief removes resource group i6engine
		 */
		~ResourceManager();

	private:
		/**
		 * \brief forbidden
		 */
		ResourceManager(const ResourceManager &) = delete;

		/**
		 * \brief forbidden
		 */
		ResourceManager & operator=(const ResourceManager &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_RESOURCEMANAGER_H__ */

/**
 * @}
 */
