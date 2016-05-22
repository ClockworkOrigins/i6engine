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

#ifndef __I6ENGINE_EDITOR_PLUGINS_FLAGPLUGININTERFACE_H__
#define __I6ENGINE_EDITOR_PLUGINS_FLAGPLUGININTERFACE_H__

#include <string>
#include <vector>

#include <qplugin.h>

namespace i6e {
namespace editor {
namespace plugins {

	/**
	 * \brief interface for plugins used to offer flags for maps
	 */
	class FlagPluginInterface {
	public:
		virtual ~FlagPluginInterface() {
		}

		virtual std::vector<std::string> getFlags() const = 0;
	};

} /* namespace plugins */
} /* namespace editor */
} /* namespace i6e */

Q_DECLARE_INTERFACE(i6e::editor::plugins::FlagPluginInterface, "i6e.editor.initialization.FlagPluginInterface")

#endif /* __I6ENGINE_EDITOR_PLUGINS_FLAGPLUGININTERFACE_H__ */
