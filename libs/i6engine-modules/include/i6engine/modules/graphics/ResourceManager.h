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
