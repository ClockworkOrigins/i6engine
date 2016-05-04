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

#ifndef __I6ENGINE_MODULES_GRAPHICSMAILBOX_H__
#define __I6ENGINE_MODULES_GRAPHICSMAILBOX_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

namespace i6e {
namespace modules {

	class GraphicsManager;

	/**
	 * \class GraphicsMailbox
	 * \brief Extracts the contents of incoming messages on the "Graphics" channel and calls the corresponding methods of the graphics subsystem
	 * \details Please consult the messaging documentation found in the "Related Pages" on top of this page for in-depth usage details.
	 * 	The supported methods are declared in the enum method of GraphicsConfig: \ref i6e::modules::GraphicsConfig::method
	 */
	class GraphicsMailbox {
	public:
		/**
		 * \brief constructor taking pointer to GraphicsManager
		 */
		explicit GraphicsMailbox(GraphicsManager * manager);

		/**
		 * \brief destructor
		 */
		~GraphicsMailbox();

		/**
		 *\brief See Class-Description
		 *\param objMessage Any message on the channel
		 */
		void News(const api::GameMessage::Ptr & msg);

	private:
		GraphicsManager * _manager;

		/**
		 * \brief forbidden
		 */
		GraphicsMailbox(const GraphicsMailbox &) = delete;

		/**
		 * \brief forbidden
		 */
		const GraphicsMailbox & operator=(const GraphicsMailbox &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_GRAPHICSMAILBOX_H__ */

/**
 * @}
 */
