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
